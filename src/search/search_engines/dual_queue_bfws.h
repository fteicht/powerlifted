#ifndef SEARCH_SEARCH_ENGINES_DUAL_QUEUE_BFWS_H_
#define SEARCH_SEARCH_ENGINES_DUAL_QUEUE_BFWS_H_

#include "search.h"
#include "search_space.h"

#include "../lifted_heuristic/lifted_heuristic.h"

#include "../novelty/atom_counter.h"
#include "../novelty/node_novelty.h"
#include "../novelty/standard_novelty.h"

#include "../open_lists/tiebreaking_open_list.h"

#include "../options.h"


template <class PackedStateT>
class DualQueueBFWS : public SearchBase {
    AtomCounter atom_counter;
    int width;
    bool only_effects_opt;
    std::string datalog_file_name;

    int priority_preferred;
    int priority_regular;

    const int BOOST_PREF_OPEN_LIST = 1000;

    void boost_priority_queue();

    StateID get_top_node(TieBreakingOpenList &preferred, TieBreakingOpenList &other) {
        /*
         * This alternated between the preferred and the regular open list.
         * The reason why we cannot use a boost is because we cannot easily check when we enter
         * a new h-layer. However, I kept the implementation using numeric priority values just
         * in case we decide to adapt it later to some specific use case.
         */
        if ((priority_preferred > priority_regular) and (not preferred.empty())) {
            priority_preferred--;
            return preferred.remove_min();
        } else {
            return other.remove_min();
        }
    }

protected:
    SearchSpace<PackedStateT> space;

    int heuristic_layer{};

    AtomCounter initialize_counter_with_gc(const Task &task);

public:
    explicit DualQueueBFWS(int width, const Options &opt) : width(width) {
        std::cout << "Using Dual-Queue BFWS" << std::endl;
        datalog_file_name = opt.get_datalog_file();
        priority_preferred = BOOST_PREF_OPEN_LIST;
        priority_regular = 0;
    }

    using StatePackerT = typename PackedStateT::StatePackerT;

    utils::ExitCode search(const Task &task, SuccessorGenerator &generator, Heuristic &heuristic) override;
    void print_statistics() const override;
    AtomCounter initialize_counter_with_useful_atoms(const Task &task, LiftedHeuristic &delete_free_h) const;
};

#endif //SEARCH_SEARCH_ENGINES_DUAL_QUEUE_BFWS_H_
