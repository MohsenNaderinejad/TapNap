#include <bits/stdc++.h>
using namespace std;

typedef pair<long long, long long> PLL;

const long long INF = 1e18;
const int N = 1e5 + 7, M = 2e5 + 7;
int cities, roads, gas_stations_number, gas_stations[N];
vector<PLL> ADJ[M * 10];
long long dist[N * 10];
PLL TLC[N];

// Initializing the whole array for later Dijkstra Algorithm
void INITIALIZE_SINGLE_SOURCE(long long* arr, int size, int start) {
    fill(arr + 1, arr + size + 1, INF);
    arr[start] = 0;
}

// General Relaxation Used for Dijkstra
void RELAXATION(long long u, long long v, long long w, long long* dist, priority_queue<PLL, vector<PLL>, greater<PLL>>& pq) {
    long long T = ((dist[u] % (TLC[u / 10].second) < TLC[u / 10].first) ? 0:TLC[u / 10].second - (dist[u] % (TLC[u / 10].second)));
    if (dist[v] > dist[u] + w + T) {
        dist[v] = dist[u] + w + T;
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
        ADJ[u * 10].push_back(PLL(v * 10, w));
        ADJ[u * 10 + 1].push_back(PLL(v * 10 + 1, w));
        ADJ[v * 10].push_back(PLL(u * 10, w));
        ADJ[v * 10 + 1].push_back(PLL(u * 10 + 1, w));
    }
    cin >> gas_stations_number;
    for (int P = 0; P < gas_stations_number; P++) {
        cin >> gas_stations[P];;
        ADJ[gas_stations[P] * 10].push_back(PLL(gas_stations[P] * 10 + 1, 0));
        ADJ[gas_stations[P] * 10 + 1].push_back(PLL(gas_stations[P] * 10, 0));
    }

    for (int city = 1; city <= cities; city++){
        long long TLG, TLR;
        cin >> TLG >> TLR;
        TLC[city] = PLL(TLG, TLG + TLR);
    }

    DIJKSTRA(1 * 10, cities * 10 + 1, dist);

    cout << dist[cities * 10 + 1];
}