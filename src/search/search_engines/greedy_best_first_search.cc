#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#include "../utils/segmented_vector.h"

#include "greedy_best_first_search.h"

using namespace std;

const int GreedyBestFirstSearch::search(const Task &task, SuccessorGenerator *generator, Heuristic &heuristic) const {
    /*
     * Greedy best first search
     *
     */
    cout << "Starting greedy best first search" << endl;
    clock_t timer_start = clock();

    int state_counter = 0;
    int generations = 1;
    priority_queue<Node, vector<Node>, NodeComparison> q; // Queue has Node structures
    segmented_vector::SegmentedVector<pair<int, Action>> cheapest_parent;
    //unordered_map<int, State> index_to_state;
    segmented_vector::SegmentedVector<State> index_to_state;
    unordered_map<State, int, boost::hash<State>> visited;

    segmented_vector::SegmentedVector<int> shortest_distance;

    index_to_state.push_back(task.initial_state);
    cheapest_parent.push_back(make_pair(-1, Action(-1, vector<int>())));

    int heuristic_layer = heuristic.compute_heuristic(task.initial_state, task)+1;
    cout << "Initial heuristic value " << heuristic_layer << endl;
    int g_layer = 0;

    q.emplace(0, heuristic.compute_heuristic(task.initial_state, task), state_counter);
    shortest_distance.push_back(0);
    visited[task.initial_state] = state_counter++;

    if (task.is_goal(task.initial_state, task.goal)) {
        cout << "Initial state is a goal" << endl;
        extract_goal(state_counter, generations, task.initial_state, cheapest_parent, visited, index_to_state, task);
        return SOLVED;
    }

    while (not q.empty()) {
        Node head = q.top();
        int next = head.id;
        int h = head.h;
        int g = head.g;
        q.pop();
        if (g > shortest_distance[next]) {
            continue;
        }
        if (h < heuristic_layer) {
            heuristic_layer = h;
            cout << "New heuristic value expanded: h=" << h <<
                 " [expansions: " << state_counter << ", generations: " << generations
                 << ", time: " << double(clock() - timer_start) / CLOCKS_PER_SEC << "]" << '\n';
        }
        /*if (g > g_layer) {
            g_layer = g;
            cout << "Expansions " << state_counter << ", generations " << generations <<
                 " states at layer g=" << g_layer
                 << " [" << double(clock() - timer_start) / CLOCKS_PER_SEC << "]" << '\n';
        }*/
        //cout << state_counter << endl;
        assert (index_to_state.size() >= next);
        State state = index_to_state[next];
        if (task.is_goal(state, task.goal)) {
            cout << "Goal found at: " << double(clock() - timer_start) / CLOCKS_PER_SEC << endl;
            cout << "Total time: " << double(clock() - timer_start) / CLOCKS_PER_SEC << endl;
            extract_goal(state_counter, generations, state, cheapest_parent, visited, index_to_state, task);
            extract_plan(cheapest_parent, state, visited, index_to_state, task);
            return SOLVED;
        }
        vector<pair<State, Action>> successors = generator->generate_successors(task.actions, state, task.static_info);
        //cout << "STATE:" << " ";
        //task.dumpState(state);
        generations += successors.size();
        int init_state_succ = 0;
        for (const pair<State, Action> &successor : successors) {
            const State &s = successor.first;
            const Action &a = successor.second;
            //cout << "SUCCESSOR (" << task.actions[a.index].getName() << "): ";
            //task.dumpState(s);
            int dist = g + task.actions[a.index].getCost();
            int new_h = heuristic.compute_heuristic(s, task);
            pair<unordered_map<State, int, boost::hash<State>>::iterator, bool> try_to_insert = visited.insert(make_pair(s, state_counter));
            if (try_to_insert.second) {
                // Inserted for the first time in the map
                init_state_succ++;
                cheapest_parent.push_back(make_pair(next, a));
                q.emplace(dist, new_h, state_counter);
                shortest_distance.push_back(dist);
                index_to_state.push_back(s);
                state_counter++;
            }
            else {
                size_t index = visited[s];
                if (dist < shortest_distance[index]) {
                    cheapest_parent[index] = make_pair(next, a);
                    q.emplace(dist, new_h, index);
                    shortest_distance[index] = dist;
                }
            }
        }
        //cout << "Init state succ: " << init_state_succ << endl;
        //exit(0);
    }

    cout << "Total time: " << double(clock() - timer_start) / CLOCKS_PER_SEC << endl;
    cerr << "No solution found!" << endl;

    return NOT_SOLVED;

}
