#include <iostream>
#include <bits/stdc++.h>
using namespace std;

#define ALL(x) x.begin(), x.end()
typedef pair<int , int> PII;
int intersections_num, streets_num, queries_num;
const int N = 2e5 + 7;
const int LOG = 22;
const int INF = 1e9 + 7;
vector<PII> ADJ[N];
int binaryLiftingDA[N][LOG], minHeight[N][LOG];
int parent[N];
int Rank[N];
int depth[N];
int visited[N];
multiset<int> Trucks;

struct Edge {
    int u, v, w;
};
vector<Edge> edges;

// DSU Operations
void makeSet(int x){
    parent[x] = x;
}

int findSet(int x) {
    if (x != parent[x]) {
        parent[x] = findSet(parent[x]);
    }
    return parent[x];
}

void link(int x, int y) {
    if (Rank[x] > Rank[y]) {
        parent[y] = x;
    }
    else {
        parent[x] = y;
        if (Rank[x] == Rank[y]) {
            Rank[y]++;
        }
    }
}

void unionSet(int x, int y) {
    link(findSet(x), findSet(y));
}

// Inputing the base GRAPH
void INPUT_GRAPH(){
    cin >> intersections_num >> streets_num;
    for (int roads = 0; roads < streets_num; roads++){
        int inter_in, inter_out, max_height;
        cin >> inter_in >> inter_out >> max_height;
        edges.push_back({inter_in, inter_out, max_height});
    }
}

// Doing a MaxST-Kruskal on the given graph and saving it on ADJ[N]
// to find the highest height possible for every road
void MaxST_Kruskal(){
    sort(ALL(edges), [](const Edge& a, const Edge& b) { return a.w > b.w; });
    for (int intersection = 1; intersection <= intersections_num; intersection++) {
        makeSet(intersection);
    }
    for (auto e: edges) {
        if (findSet(e.u) != findSet(e.v)) {
            ADJ[e.u].push_back({e.v, e.w});
            ADJ[e.v].push_back({e.u, e.w});
            unionSet(e.u, e.v);
        }
    }
}

// Doing a DFS-VISIT to make a forest and connect them to (intersections + 1) Node from the collected info from ADJ[N]
void DFS_VISIT(int now, int p = intersections_num + 1){
    visited[now] = 1;
    binaryLiftingDA[now][0] = p;
    for (auto [on, weight]: ADJ[now]) {
        if (on != p) {
            minHeight[on][0] = weight;
            depth[on] = depth[now] + 1;
            DFS_VISIT(on, now);
        }
    }
}

// Doing a Whole DFS
void DFS(){
    for (int i = 1; i <= intersections_num; i++) {
        if (!visited[i]) {
            DFS_VISIT(i);
        }
    }
}

// Doing a Binary Lifting for calculating the minHeight for future usage
void preCalc(){
    binaryLiftingDA[1][0] = binaryLiftingDA[intersections_num + 1][0] = intersections_num + 1;
    for (int l = 1; l < LOG; l++){
        for (int i = 1; i <= intersections_num + 1; i++) {
            int lastColLifted = binaryLiftingDA[i][l - 1];
            binaryLiftingDA[i][l] = binaryLiftingDA[lastColLifted][l - 1];
            minHeight[i][l] = min(minHeight[i][l - 1], minHeight[lastColLifted][l - 1]);
        }
    }
}

// A lift function for fast use and calcluating minHeight
int lift(int v, int k, int& minH){
	for (int i = LOG - 1; i >= 0; i--){
		if ((k >> i) & 1) {
            minH = min(minH, minHeight[v][i]);
            v = binaryLiftingDA[v][i];
        }
	}
	return v;
}

// Doing LCA to find the min between two intersections like (u, v)
int LCA(int u, int v){
    int minH = INF;
    if (depth[v] > depth[u]) swap(u, v);

    u = lift(u, depth[u] - depth[v], minH);
    if (u == v) {
        return minH;
    }

    for (int i = LOG - 1; i >= 0; i--){
        if (binaryLiftingDA[u][i] != binaryLiftingDA[v][i]) {
            minH = min({minH, minHeight[u][i], minHeight[v][i]});
            u = binaryLiftingDA[u][i];
            v = binaryLiftingDA[v][i];
        }
    }

    minH = min({minH, minHeight[u][0], minHeight[v][0]});

    return minH;
}

// Handeling the given queries for the problems
void HANDELING_QUERIES(){
    cin >> queries_num;
    while(queries_num--){
        int type;
        cin >> type;
        int truck;
        switch (type)
        {
        case 1:
            cin >> truck;
            Trucks.insert(truck);
            break;
        case 2:
            cin >> truck;
            {
                auto it = Trucks.find(truck);
                if (it != Trucks.end()) {
                    Trucks.erase(it);
                }
            }
            break;
        case 3:
            int u, v; 
            cin >> u >> v;
            if (Trucks.empty()) {
                cout << -1 << endl;
                break;
            }
            if (findSet(u) != findSet(v)) {
                cout << -1 << endl;
                break;
            }
            int limit = LCA(u, v);
            auto it = Trucks.upper_bound(limit);
            if (it == Trucks.begin()) {
                cout << -1 << endl;
            }
            else {
                cout << *prev(it) << endl;
            }
            break;
        }
    }
}

int main() {
    for(int i = 0; i < N; i++){
        for(int j = 0; j < LOG; j++) {
            minHeight[i][j] = INF;
        }
    }
    INPUT_GRAPH();
    MaxST_Kruskal();
    DFS();
    preCalc();
    HANDELING_QUERIES();
}