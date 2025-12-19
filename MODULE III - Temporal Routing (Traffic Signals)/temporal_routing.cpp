#include <bits/stdc++.h>
using namespace std;

typedef pair<long long, long long> PLL;

const long long INF = 1e18;
const int N = 1e5 + 7, M = 2e5 + 7;
int cities, roads;
vector<PLL> ADJ[M];
PLL TLC[N];
long long dist[N];

// Initializing the whole array for later Dijkstra Algorithm
void INITIALIZE_SINGLE_SOURCE(long long* arr, int size, int start) {
    fill(arr + 1, arr + size + 1, INF);
    arr[start] = 0;
}

// General Relaxation Used for Dijkstra
void RELAXATION(long long u, long long v, long long w, long long* dist, priority_queue<PLL, vector<PLL>, greater<PLL>>& pq) {
    if (dist[v] > dist[u] + w + ((dist[u] % (TLC[u].second) < TLC[u].first) ? 0:TLC[u].second - (dist[u] % (TLC[u].second)))) {
        dist[v] = dist[u] + w + ((dist[u] % (TLC[u].second) < TLC[u].first) ? 0:TLC[u].second - (dist[u] % (TLC[u].second)));
        pq.push(PLL(dist[v], v));
    }
}

// Dijkstra Algorithm
void DIJKSTRA(int start, int size, long long* dist) {
    INITIALIZE_SINGLE_SOURCE(dist, size, start);
    priority_queue<PLL, vector<PLL>, greater<PLL>> paths;
    paths.push(PLL(0, start));
    while(!paths.empty()) {
        auto [length, now] = paths.top();
        paths.pop();

        if (length > dist[now]) continue;

        for (auto& [on, weight]: ADJ[now]) {
            RELAXATION(now, on, weight, dist, paths);
        }
    }
}

int main(){
    cin >> cities >> roads;

    for (int r = 0; r < roads; r++) {
        long long u, v, w;
        cin >> u >> v >> w;
        ADJ[u].push_back(PLL(v, w));
        ADJ[v].push_back(PLL(u, w));
    }

    for (int city = 1; city <= cities; city++){
        long long TLG, TLR;
        cin >> TLG >> TLR;
        TLC[city] = PLL(TLG, TLG + TLR);
    }

    DIJKSTRA(1, cities, dist);

    cout << dist[cities];
}