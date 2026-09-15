# TapNap

Five routing and analytics problems set on a freight network, each solved in C++ with attention to the complexity bound rather than to whether it runs at all. The framing is a fictional haulage company; the content is graph algorithms, state-space search and a balanced tree written from scratch.

Every module is a standalone program reading from stdin. Each ships with the problem statement, a sample input and the corresponding output.

---

## Contents

- [Building and running](#building-and-running)
- [Module I: bottleneck queries on a dynamic fleet](#module-i-bottleneck-queries-on-a-dynamic-fleet)
- [Module II: routing with one refuel](#module-ii-routing-with-one-refuel)
- [Module III: routing through traffic signals](#module-iii-routing-through-traffic-signals)
- [Module IV: both constraints at once](#module-iv-both-constraints-at-once)
- [Module V: order statistics on truck prices](#module-v-order-statistics-on-truck-prices)
- [Layout](#layout)
- [Known issues](#known-issues)

---

## Building and running

Each module is one translation unit with no dependencies beyond the standard library.

```bash
g++ -std=c++17 -O2 module-1-core/the_core.cpp -o the_core
./the_core < module-1-core/input.in
```

Compare against the `output.out` sitting next to each input.

GCC is required. The sources include `<bits/stdc++.h>`, which Clang and MSVC do not ship.

---

## Module I: bottleneck queries on a dynamic fleet

**Problem.** Roads have height limits. Trucks have heights. The fleet changes over time as vehicles are added and retired. Given two intersections, report the tallest truck currently in the fleet that can get from one to the other.

**Approach.** The tallest truck that can traverse a path is limited by the lowest bridge on it, so the useful quantity between two nodes is the maximum over all paths of the minimum edge weight. That value is the bottleneck, and it is found on the maximum spanning tree: the path between two nodes in a maximum spanning tree has the best possible minimum edge, so the whole graph collapses to a tree without losing any answer.

Kruskal builds the maximum spanning tree, sorting edges descending and unioning with a disjoint set that does both path compression and union by rank. A DFS then roots the tree and fills the first column of two lifting tables, one holding ancestors and one holding the minimum edge weight on the way to them. Binary lifting fills the rest, so the minimum along any ancestor chain is available in logarithmic time. An LCA walk over both tables returns the bottleneck between any pair.

Disconnected graphs are handled by rooting every component at a shared virtual node, so the query does not need a separate connectivity check beyond comparing DSU representatives.

The fleet itself is a `multiset` of heights. Answering a query means finding the largest element not exceeding the bottleneck, which is `upper_bound` followed by a step back.

**Complexity.** O(M log M) to build, O(log N) per query, O(log N) per fleet update.

---

## Module II: routing with one refuel

**Problem.** A truck starts with a given amount of fuel and has a tank capacity. Fuel burns in proportion to distance. It may stop at a fuel station at most once. Find the shortest route.

**Approach.** The single refuel splits any valid route into two independent legs: start to station, station to destination. So two runs of Dijkstra suffice, one from the origin and one from the destination, and the answer is the cheapest station where the first leg fits inside the fuel currently in the tank and the second fits inside a full tank. The direct route is checked separately as the no-refuel case.

The point is that no search over combinations is needed. Two shortest-path trees answer every candidate at once, and the constraint becomes a filter over stations rather than part of the search.

**Complexity.** O((N + M) log N), dominated by the two Dijkstra runs.

**Note.** Unreachable inputs print `nemisarfed`, Persian for "not worth it". That is what the judge expected, and it is preserved.

---

## Module III: routing through traffic signals

**Problem.** Every intersection has a traffic light with a green phase and a red phase. Arriving during red means waiting for the next green. Minimise total travel time.

**Approach.** Edge cost is no longer fixed; it depends on when you arrive. With a cycle length of green plus red, arriving at time t means waiting zero if t modulo the cycle falls inside the green window, and otherwise waiting out the remainder of the cycle. That waiting term is folded directly into the relaxation step.

Dijkstra still works here, and the reason is worth stating. A time-dependent shortest path problem is only safe for Dijkstra when waiting cannot help you arrive earlier, meaning departure time and arrival time move in the same direction. Traffic lights satisfy that: leaving later never gets you through sooner. Without that property the greedy argument collapses and Dijkstra stops being correct, which is why the same trick does not transfer to networks with, say, scheduled departures.

**Complexity.** O((N + M) log N).

---

## Module IV: both constraints at once

**Problem.** Module II and Module III together. Traffic lights everywhere, and one permitted refuel.

**Approach.** Layer the graph. Every city becomes two states: one before the refuel has been used, one after. Road edges connect states within the same layer, and each fuel station gets a zero-cost edge from its lower state to its upper one. The single permitted refuel becomes a single permitted layer transition, enforced by the graph's shape rather than by extra logic in the search.

Dijkstra then runs unmodified over the layered graph, with the traffic light delay applied exactly as in Module III. Start in the lower layer at the origin, read the answer from the upper layer at the destination.

This is the general move for constraints of the form "you may do X at most k times": push the counter into the node identity and let an unmodified shortest-path algorithm handle it.

**Complexity.** O((kN + kM) log(kN)) for k layers, which is two here.

---

## Module V: order statistics on truck prices

**Problem.** Prices arrive one at a time. At any point, report how many trucks cost no more than a given amount.

**Approach.** An AVL tree, written from scratch, with each node carrying its subtree size alongside its height. Height keeps the tree balanced through the usual four rotation cases; size turns it into an order statistic tree, so a count query descends once and sums subtree sizes as it goes rather than walking the matching elements.

Counting is O(log N) rather than O(k) in the number of matches, which is the entire reason for augmenting the tree instead of using a sorted container.

**Complexity.** O(log N) insert, O(log N) query.

---

## Layout

```
module-1-core/                       maximum spanning tree, DSU, binary lifting, LCA
module-2-fuel-routing/               two-source Dijkstra with a refuel constraint
module-3-temporal-routing/           Dijkstra with time-dependent node delays
module-4-waypoint-routing/           layered state-space graph
module-5-market-analytics/           size-augmented AVL tree
```

Each directory holds the source, the problem statement as a PDF, `input.in` and `output.out`.

---

## Known issues

- **`<bits/stdc++.h>` is a GCC extension.** Every module includes it, so nothing here builds under Clang or MSVC without editing the includes.
- **The DFS in Module I is recursive** and the node bound is 200,007. A deep enough tree will overflow the stack before the algorithm is at fault.
- **Module I flushes on every query.** `endl` inside the query loop forces a flush per line; `'\n'` would be faster on large inputs.
- **Module IV multiplies node indices by ten to build two layers.** Two would do. The array bounds are sized for it and it works, but it wastes memory and makes the indexing harder to read than `2 * u` and `2 * u + 1`.
- **`rebalanceTree` in Module V has no final return statement.** Every reachable path does return, but the compiler cannot prove it, and falling off the end of a non-void function is undefined behaviour rather than a warning to ignore.
- **The AVL tree supports insertion only.** Deletion was never needed by the problem and was never written.
- **Compiled `.exe` files are committed.** They are build artefacts and the `.gitignore` does not exclude them.
