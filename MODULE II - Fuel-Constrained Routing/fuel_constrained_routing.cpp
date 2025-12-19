#include <bits/stdc++.h>
using namespace std;

typedef pair<long long, long long> PLL;

const long long INF = 1e18;
const int N = 1e5 + 7, M = 2e5 + 7;
int cities, roads, gas_stations_number, gas_stations[N];
long long MAXF, CURRF;
vector<PLL> ADJ[M];
long long dist_from_start[N];
long long dist_from_end[N];

// Initializing the whole array for later Dijkstra Algorithm
void INITIALIZE_SINGLE_SOURCE(long long* arr, int size, int start) {
    fill(arr + 1, arr + size + 1, INF);
    arr[start] = 0;
}

// General Relaxation Used for Dijkstra
void RELAXATION(long long u, long long v, long long w, long long* dist, priority_queue<PLL, vector<PLL>, greater<PLL>>& pq) {
    if (dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w;
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

int main() {
    cin >> cities >> roads;

    for (int r = 0; r < roads; r++) {
        long long u, v, w;
        cin >> u >> v >> w;
        ADJ[u].push_back(PLL(v, w));
        ADJ[v].push_back(PLL(u, w));
    }
    cin >> gas_stations_number >> CURRF >> MAXF;
    for (int P = 0; P < gas_stations_number; P++) {
        cin >> gas_stations[P];
    }

    DIJKSTRA(1, cities, dist_from_start);
    DIJKSTRA(cities, cities, dist_from_end);

    long long ans = INF;
    if (dist_from_start[cities] <= CURRF) ans = dist_from_start[cities];
    for (int P = 0; P < gas_stations_number; P++) {
        if (dist_from_start[gas_stations[P]] > CURRF) continue;
        if (dist_from_end[gas_stations[P]] > MAXF) continue;
        ans = (dist_from_start[gas_stations[P]] + dist_from_end[gas_stations[P]] < ans) ? dist_from_start[gas_stations[P]] + dist_from_end[gas_stations[P]]:ans;
    }
    if (ans == INF) cout << "nemisarfed";
    else cout << ans;
}