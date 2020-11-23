#ifndef SEARCH_LAZY_BEST_FIRST_SEARCH_H
#define SEARCH_LAZY_BEST_FIRST_SEARCH_H


#include "search.h"
#include "search_space.h"
#include "../open_lists/greedy_open_list.h"

template <class PackedStateT>
class LazySearch : public SearchBase {
protected:
    SearchSpace<PackedStateT> space;

    int heuristic_layer{};
    bool keep_relaxed_useless_operators;
public:
    explicit LazySearch(bool b) : keep_relaxed_useless_operators(b) {}

    using StatePackerT = typename PackedStateT::StatePackerT;

    utils::ExitCode search(const Task &task, SuccessorGenerator &generator, Heuristic &heuristic) override;

    void print_statistics() const override;
    bool is_useful_operator(const DBState &state,
                            const std::map<int, std::vector<GroundAtom>> &useful_atoms,
                            const std::vector<bool> &useful_nullary_atoms);

    StateID get_top_node(GreedyOpenList &preferred, GreedyOpenList &other) {
        if (not preferred.empty()) return preferred.remove_min();
        else return other.remove_min();
    }
};


#endif  // SEARCH_LAZY_BEST_FIRST_SEARCH_H
