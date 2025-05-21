#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

static constexpr int INF = numeric_limits<int>::max();

struct Edge
{
    int to, rev, cap, cost;
};

class MinCostMaxFlow
{
    int N;
    vector<vector<Edge>> graph;
    vector<int> dist, potential, parentV, parentE;

public:
    MinCostMaxFlow(int n)
        : N(n),
          graph(n),
          dist(n),
          potential(n, 0),
          parentV(n),
          parentE(n)
    {
    }

    void addEdge(int u, int v, int cap, int cost)
    {
        graph[u].push_back(Edge{v, (int)graph[v].size(), cap, cost});
        graph[v].push_back(Edge{u, (int)graph[u].size() - 1, 0, -cost});
    }

    pair<int, int> run(int s, int t)
    {
        int flow = 0, cost = 0;

        while (true)
        {
            // 1) Dijkstra on reduced costs
            fill(dist.begin(), dist.end(), INF);
            dist[s] = 0;
            priority_queue<pair<int, int>,
                           vector<pair<int, int>>,
                           greater<>>
                pq;
            pq.push({0, s});

            while (!pq.empty())
            {
                auto [d, u] = pq.top();
                pq.pop();
                if (d > dist[u])
                    continue;
                for (int i = 0; i < (int)graph[u].size(); ++i)
                {
                    Edge &e = graph[u][i];
                    if (e.cap > 0)
                    {
                        int v = e.to;
                        int nd = dist[u] + e.cost + potential[u] - potential[v];
                        if (nd < dist[v])
                        {
                            dist[v] = nd;
                            parentV[v] = u;
                            parentE[v] = i;
                            pq.push({nd, v});
                        }
                    }
                }
            }

            if (dist[t] == INF)
                break;

            // 2) update potentials
            for (int i = 0; i < N; ++i)
            {
                if (dist[i] < INF)
                    potential[i] += dist[i];
            }

            // 3) find bottleneck
            int add = INF;
            for (int v = t; v != s; v = parentV[v])
            {
                Edge &e = graph[parentV[v]][parentE[v]];
                add = min(add, e.cap);
            }

            // 4) apply flow
            flow += add;
            cost += add * potential[t];
            for (int v = t; v != s; v = parentV[v])
            {
                Edge &e = graph[parentV[v]][parentE[v]];
                e.cap -= add;
                graph[v][e.rev].cap += add;
            }
        }

        return {flow, cost};
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    MinCostMaxFlow mcmf(N);

    for (int i = 0; i < M; ++i)
    {
        int u, v, cap, cost;
        cin >> u >> v >> cap >> cost;
        mcmf.addEdge(u, v, cap, cost);
    }

    int s, t;
    cin >> s >> t;

    auto [flow, minCost] = mcmf.run(s, t);
    cout << "Flow: " << flow << "\n";
    cout << "Cost: " << minCost << "\n";

    return 0;
}
