# TapNap Logistics Engine 🚚💨

### High-Performance Algorithmic Core for Freight Transport Optimization

## 📖 Project Overview

This repository hosts the core algorithmic engine for **TapNap**, a fictional joint venture between ride-sharing giants entering the freight and heavy transport market. The project simulates a backend system responsible for optimizing logistics across a complex, constraint-heavy road network.

The codebase demonstrates solutions to NP-hard and polynomial-time optimization problems found in real-world navigation systems, focusing on **time complexity reduction** and **memory efficiency**.

## 🚀 Key Technical Highlights

- **Graph Theory**: Implementation of Maximum Spanning Trees (MST) and Dijkstra's algorithm modified for state-space search.

- **Dynamic Queries**: Handling online updates to graph topology using Disjoint Set Union (DSU) and Lowest Common Ancestor (LCA) techniques.

- **Temporal Routing**: Modeling time-dependent edge weights to account for traffic light cycles.

- **Data Structures**: Utilization of Order Statistic Trees (or Coordinate Compressed Fenwick Trees) for $O(\log N)$ statistical queries on pricing data.

🧩 Module Breakdown

The project is divided into five distinct computational modules, each addressing a specific logistical constraint.

### 1. Network Capacity Analysis ("The Core")

**Problem Space**: Dynamic Connectivity & Bottleneck Paths
**Description**:
Calculates the maximum vehicle height allowance between any two nodes in the network. The system handles dynamic updates (trucks entering/leaving the fleet) and static road height constraints.

- **Approach**: We treat the road network as a weighted graph where weights represent height limits. The problem is reduced to finding the bottleneck edge on the path between $u$ and $v$ in a Maximum Spanning Tree.

- **Complexity**:

  - Preprocessing: $O(M \log M)$ (Kruskal's Algorithm)

  - Query Processing: $O(\log N)$ via Binary Lifting (LCA).

### 2. Fuel-Constrained Routing

**Problem Space**: Shortest Path on State-Space Graphs
**Description**:
Determines the shortest path for a vehicle with finite fuel capacity $C_{max}$ and initial fuel $C_{cur}$. The route must account for fuel consumption proportional to distance and refuel availability at specific nodes.

- **Approach**: Modeling the state as $(u, fuel)$, effectively expanding the graph nodes to $N * C_{max}$.

- **Optimization**: Pruning states where $fuel < 0$ to minimize search space.

### 3. Temporal Routing (Traffic Signals)

**Problem Space**: Time-Dependent Shortest Path
**Description**:
Minimizes travel time in a network where node traversal latency is a function of arrival time (due to red/green light cycles $t_{on}$, $t_{off}$).

- **Approach**: Modified Dijkstra's Algorithm where the cost to traverse edge (u, v) is $w_{uv} + wait\_time(t_{arrival})$.

- **Mathematical Model**:

$$\text{wait\_time}(t) = \max(0, (t \pmod{T_{cycle}}) - t_{on})$$

Where $T_{cycle} = t_{on} + t_{off}$.

### 4. Multi-Objective Waypoint Routing

**Problem Space**: Pathfinding with Mandatory Stops
**Description**:
Finds the optimal path that minimizes time (considering traffic lights) while ensuring the vehicle visits at least one refueling station.

- **Approach**:

1. Compute shortest paths from Source $(S)$ to all nodes: $d_S[i]$.

2. Compute shortest paths from all nodes to Destination $(T)$ on the transpose graph: $d_T[i]$.

3. Iterate over the set of Fuel Stations $F$ to find the minimum total cost:

$$\min_{k \in F} (d_S[k] + d_T[k])$$

### 5. Market Analytics Engine

**Problem Space**: Online Statistical Queries
Description:
A module for real-time analysis of competitor pricing. Supports inserting new price points and querying the rank of a specific price (number of offers $<= P$) in $O(log N)$ time.

- **Approach**: Implementation of a Balanced Binary Search Tree (BST) or Fenwick Tree with Coordinate Compression to handle values up to $10^9$.

## 🛠️ Build & Run

Instructions for building the project will be added here. (e.g., standard CMake or Makefile instructions).

```
Example build command:
g++ -std=c++17 -O2 main.cpp -o implementation
./implementation < input.txt
```


## 📝 Future Improvements

- Implement A* heuristics for faster average-case routing on Euclidean datasets.

- Parallelize the "Multi-Objective Waypoint" search using OpenMP.

- Visualizer for the MST and Shortest Path output.

Author: Mohsen Naderinejad