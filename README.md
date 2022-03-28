# Powerlifted Planner

Powerlifted is a domain-independent classical planner that uses only lifted
representations.

(See [References](#references) for more details.)

## Usage

The `powerlifted.py` script solves a PDDL task provided as input. It also builds
the planner if the `--build` parameter is passed. The script has the following
parameters:

```$ ./powerlifted.py [-d DOMAIN] -i INSTANCE -s SEARCH -e HEURISTIC -g GENERATOR [--state STATE REPR.] [ADDITIONAL OPTIONS] [--build]```

Use the `build.py` script to build the planner first.

### Available Options for `SEARCH`:
- `astar`: A* Search
- `bfs`: Breadth-First Search (This option was previously called `naive`. You
  can still use `naive` with the `powerlifted.py` script but the planner will internally
  use the new keyword `bfs`.)
- `bfws1` and `bfws2`: Best-First Width Search with w=1 and w=2, respectively.
- `bfws1-rx` and `bfws2-rx`: BFWS(R_x) with w=1 and w=2, respectively. (See Corrêa and Seipp 2022.)
- `dq-bfws1-rx` and `dq-bfws2-rx`: DQ(R_x) with w=1 and w=2, respectively. (See Corrêa and Seipp 2022.)
- `alt1` and `alt2`: [R_x, h] with w=1 and w=2, respectively. The choice of h is
  given the `HEURISTIC` option. (See Corrêa and Seipp 2022.)
- `gbfs`: Greedy Best-First Search
- `lazy`: Lazy Best-First Search
- `lazy-po`: Lazy Best-First Search with Boosted Dual-Queue
- `lazy-prune`: Lazy Best-First Search with pruning of states generated by
non-preferred operators

### Available Options for `HEURISTIC`:
- `add`: The additive heuristic
- `blind`: No Heuristic
- `ff`: The FF heuristic
- `goalcount`: The goal-count/STRIPS heuristic
- `hmax`: The hmax heuristic
- `rff`: The rule-based FF heuristic

### Available Options for `GENERATOR`:
- `join`: Join program using the predicate order given in the PDDL file
- `random_join`: Randomly ordered join program
- `ordered_join`: Join program ordered by the arity of the predicates
- `full_reducer`: Generate successor for acyclic schemas using the full
  reducer method; for cyclic schemas it uses a partial reducer and a join
  program.
- `yannakakis`: Same as above but replaces the final join of the full
      reducer method by the Yannakakis' project-join program.

### Available Options for `STATE REPR.`:

- `sparse`: Use the sparse state representation where a state is only
  represented by the facts that are true in this state.
- `extensional`: Use the extensional representation where a state is a bitset
  where the ith-bit is true if the fact associated to it is true in this
  state. This representation requires the grounding of facts (but not of
  actions) which, right now, is performed in the search component. *Warning*:
  this options does not support all `HEURISTIC` options.


### Available `ADDITIONAL OPTIONS`:
- `[--only-effects-novelty-check]`: Flag if the novelty evaluation of a state
  should only consider atoms in the applied action effect. *Warning*: for
  state-of-the-art performance, you must use this option when running BFWS-based
  search engines. (See Corrêa and Seipp 2022.)
- `[--novelty-early-stop]`: Flag if the novelty evaluation of a state should
  stop as soon as the return value is defined. (See Corrêa and Seipp 2022.)
- `[--seed RANDOM SEED]`: Random seed for the random number generator.
- `[--translator-output-file TRANSLATOR_FILE]`: Output of the intermediate
  representation to be parsed by the search component will be saved into
  `TRANSLATOR_FILE`. (Default: `output.lifted`)
- `[--unit-cost]`: Use unit cost  (i.e., all costs are equal to 1) instead of
  the costs specified in the domain file.
- `[--validate]`: Runs VAL after a plan is found to validate it. This requires
  [VAL](https://github.com/KCL-Planning/VAL) to be added as `validate` to the `PATH`.

## Running Powerlifted as a Singularity container

You can also build a Singularity image to run the planner. This might be useful
in the case where you are not able to compile the planner locally, for
example. To do so, first remove the `builds/` directory, in case you have any
builds already in your system. Then, you can run the following command to create
the planner image:


``` sudo singularity build powerlifted.sif Singularity```

Be aware that this might take a while. Once the image `powerlifted.sif` is
created, you can run it with the same parameters as the `powerlifted.py`
script. The only exception is that, by default, VAL is not installed in the
container, so it is not possible to use the `--validate` flag with the
Singularity image. However, you can run VAL with the `sas_plan` file created by
the planner after the execution. The following command is a usage example on
how to run the planner with the Singularity image:

```./powerlifted.sif -i /path/to/instance.pddl -s lazy-po -e add -g yannakakis --datalog-file model.lp --translator-output-file output.lifted```

## Requirements
 - A C++17-compliant compiler
 - CMake 3.9+
 - Python 3.5+
 - Boost

The planner also uses data-structures from [Abseil](https://abseil.io/). The
source code of Abseil is contained in the `src/search/abseil-cpp` directory

## Limitations
 - **Axioms**: not supported
 - **Conditional effects**: not supported
 - **Negated preconditions**: only inequality
 - **Quantifiers**: not supported

 ## References

 1. Corrêa, A. B.; Pommerening, F.; Helmert, M.; and Francès, G. 2020. Lifted Successor Generation using Query Optimization Techniques. In Proc. ICAPS 2020, pp. 80-89. [[pdf]](https://ai.dmi.unibas.ch/papers/correa-et-al-icaps2020.pdf)
 2. Corrêa, A. B.; Francès, G.; Pommerening, F.; and Helmert, M. 2021. Delete-Relaxation Heuristics for Lifted Classical Planning. In Proc. ICAPS 2021, pp. 94-102. [[pdf]](https://ai.dmi.unibas.ch/papers/correa-et-al-icaps2021.pdf)
 3. Corrêa, A. B.; Pommerening, F.; Helmert, M.; and Francès, G. 2022. The
    FF Heuristic for Lifted Classical Planning. In Proc. AAAI 2022. [[pdf]](https://ai.dmi.unibas.ch/papers/correa-et-al-aaai2022.pdf)
 4. Corrêa, A. B.; and Seipp, J. 2022. Best-First Width Search for Lifted
    Classical Planning. In Proc. ICAPS 2022. [[pdf]](https://ai.dmi.unibas.ch/papers/correa-seipp-icaps2022.pdf)
