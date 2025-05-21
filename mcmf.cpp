#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Edge
{
    int to, rev;
    int cap;
    int cost;
};

class MinCostMaxFlow
{
public:
    MinCostMaxFlow(int N) : N(N), graph(N), h(N), dist(N), prevv(N), preve(N) {}

    void addEdge(int from, int to, int cap, int cost)
    {
        graph[from].push_back({to, (int)graph[to].size(), cap, cost});
        graph[to].push_back({from, (int)graph[from].size() - 1, 0, -cost});
    }

    // Returns {max_flow, min_cost}
    pair<int, int> flow(int s, int t)
    {
        int total_flow = 0, total_cost = 0;
        fill(h.begin(), h.end(), 0); // potential init

        while (true)
        {
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
            fill(dist.begin(), dist.end(), INF);
            dist[s] = 0;
            pq.push({0, s});

            while (!pq.empty())
            {
                auto [d, v] = pq.top();
                pq.pop();
                if (dist[v] < d)
                    continue;
                for (int i = 0; i < graph[v].size(); ++i)
                {
                    Edge &e = graph[v][i];
                    int cost = e.cost + h[v] - h[e.to];
                    if (e.cap > 0 && dist[e.to] > dist[v] + cost)
                    {
                        dist[e.to] = dist[v] + cost;
                        prevv[e.to] = v;
                        preve[e.to] = i;
                        pq.push({dist[e.to], e.to});
                    }
                }
            }

            if (dist[t] == INF)
                break; // no more augmenting path

            for (int v = 0; v < N; ++v)
                h[v] += dist[v]; // update potentials

            // Determine bottleneck capacity
            int d = INF;
            for (int v = t; v != s; v = prevv[v])
            {
                d = min(d, graph[prevv[v]][preve[v]].cap);
            }

            total_flow += d;
            total_cost += d * h[t];

            // Push flow
            for (int v = t; v != s; v = prevv[v])
            {
                Edge &e = graph[prevv[v]][preve[v]];
                e.cap -= d;
                graph[v][e.rev].cap += d;
            }
        }

        return {total_flow, total_cost};
    }

private:
    int N;
    vector<vector<Edge>> graph;
    vector<int> h, dist, prevv, preve;
};

int main()
{
    int N, M;
    cin >> N >> M;
    MinCostMaxFlow mcf(N);

    for (int i = 0; i < M; ++i)
    {
        int u, v, cap, cost;
        cin >> u >> v >> cap >> cost;
        mcf.addEdge(u, v, cap, cost);
    }

    int s, t;
    cin >> s >> t;

    auto [flow, cost] = mcf.flow(s, t);
    cout << "Max flow: " << flow << '\n';
    cout << "Min cost: " << cost << '\n';

    return 0;
}
