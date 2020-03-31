#include "task.h"

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

const void Task::add_predicate(string &name, int index, int arity,
                               bool static_predicate, vector<int> &types) {
  Task::predicates.emplace_back(move(name), index, arity, static_predicate,
                                move(types));
}

const void Task::add_object(const string &name, int index,
                            const vector<int> &types) {
  Task::objects.emplace_back(name, index, types);
}

const void Task::add_type(const string &type_name) {
  type_names.push_back(type_name);
}

void Task::create_empty_initial_state(size_t number_predicates) {
  /*
   * Creates one empty relation for every predicate of the task in the initial
   * state.
   */
  vector<Relation> fluents, static_preds;
  for (size_t i = 0; i < predicates.size(); ++i) {
    Relation r;
    r.predicate_symbol = i;
    assert(r.tuples.empty());
    static_preds.push_back(r);
    fluents.push_back(r);
  }
  initial_state = State(move(fluents),
      vector<bool>(predicates.size(), false));
  static_info = StaticInformation(move(static_preds),
                                  vector<bool>(predicates.size(),
                                      false));
  initial_state.nullary_atoms.clear();
  initial_state.nullary_atoms.resize(number_predicates, false);
}

void Task::dump_state(State s) const {
  /*
   * Output initial state in a human readable way.
   */
  for (size_t j = 0; j < s.nullary_atoms.size(); ++j) {
    if (s.nullary_atoms[j])
      cout << predicates[j].getName() << ", ";
  }
  for (size_t i = 0; i < s.relations.size(); ++i) {
    string relation_name = predicates[i].getName();
    unordered_set<GroundAtom, TupleHash> tuples = s.relations[i].tuples;
    for (auto &tuple : tuples) {
      cout << relation_name << "(";
      for (auto obj : tuple) {
        cout << objects[obj].getName() << ",";
      }
      cout << "), ";
    }
  }
  cout << endl;
}

void Task::dump_goal() {
  /*
   * Output goal condition in a readable format.
   */
  for (int g : goal.positive_nullary_goals) {
    cout << predicates[g].getName() << endl;
  }
  for (int g : goal.negative_nullary_goals) {
    cout << "Not " << predicates[g].getName() << endl;
  }
  for (const auto &g : goal.goal) {
    if (g.negated) {
      cout << "Not ";
    }
    cout << predicates[g.predicate].getName() << " ";
    for (int arg : g.args) {
      cout << objects[arg].getName() << " ";
    }
    cout << endl;
  }
}


void Task::create_goal_condition(std::vector<AtomicGoal> goals,
                                 std::unordered_set<int> nullary_goals,
                                 std::unordered_set<int> negative_nullary_goals) {
  goal = GoalCondition(std::move(goals), std::move(nullary_goals),
                       std::move(negative_nullary_goals));
}

void Task::initialize_action_schemas(
    const std::vector<ActionSchema> &action_list) {
  actions = action_list;
}

bool Task::is_goal(const State &state,
                   const GoalCondition &goal_condition) const {
  for (int pred : goal_condition.positive_nullary_goals) {
    if (!state.nullary_atoms[pred])
      return false;
  }
  for (int pred : goal_condition.negative_nullary_goals) {
    if (!state.nullary_atoms[pred])
      return false;
  }
  for (const AtomicGoal &atomicGoal : goal_condition.goal) {
    int goal_predicate = atomicGoal.predicate;
    Relation relation_at_goal_predicate = state.relations[goal_predicate];

    assert (!predicates[relation_at_goal_predicate.predicate_symbol].isStaticPredicate());
    assert(goal_predicate == relation_at_goal_predicate.predicate_symbol);

    if (!atomicGoal.negated) {
      // Positive goal_condition
      if (find(relation_at_goal_predicate.tuples.begin(),
               relation_at_goal_predicate.tuples.end(),
               atomicGoal.args) == relation_at_goal_predicate.tuples.end()) {
        return false;
      }
    } else {
      // Negative goal_condition
      if (find(relation_at_goal_predicate.tuples.begin(),
               relation_at_goal_predicate.tuples.end(),
               atomicGoal.args) != relation_at_goal_predicate.tuples.end()) {
        return false;
      }
    }
  }
  return true;
}

bool Task::is_trivially_unsolvable() const {
  /*
   * Checks whether the static conditions in the goal condition are not
   * satisfied.
   *
   * This should be guaranteed by the translator. Just a safety check.
   */
  for (const AtomicGoal &atomicGoal : goal.goal) {
    int goal_predicate = atomicGoal.predicate;
    Relation relation_at_goal_predicate = static_info.relations[goal_predicate];
    if (!predicates[relation_at_goal_predicate.predicate_symbol]
             .isStaticPredicate())
      continue;
    assert(goal_predicate == relation_at_goal_predicate.predicate_symbol);
    if (!atomicGoal.negated) {
      // Positive goal_condition
      if (find(relation_at_goal_predicate.tuples.begin(),
               relation_at_goal_predicate.tuples.end(),
               atomicGoal.args) == relation_at_goal_predicate.tuples.end()) {
        return true;
      }
    } else {
      // Negative goal_condition
      if (find(relation_at_goal_predicate.tuples.begin(),
               relation_at_goal_predicate.tuples.end(),
               atomicGoal.args) != relation_at_goal_predicate.tuples.end()) {
        return true;
      }
    }
  }
  return false;
}
