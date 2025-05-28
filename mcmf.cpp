#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Edge
{
    int to;   // Target node
    int rev;  // Index of the reverse edge in the adjacency list
    int cap;  // Remaining capacity
    int cost; // Cost per unit flow
};

class MinCostMaxFlow
{
public:
    // Initialize N nodes
    MinCostMaxFlow(int N) : N(N), graph(N), h(N), dist(N), prevv(N), preve(N) {}

    void addEdge(int from, int to, int cap, int cost)
    {
        graph[from].push_back({to, (int)graph[to].size(), cap, cost});      // Forward edge
        graph[to].push_back({from, (int)graph[from].size() - 1, 0, -cost}); // Backward edge for residuals
    }

    // Returns a pair: (max flow, min cost) from s (starting node) to t (target node)
    pair<int, int> flow(int s, int t)
    {
        int total_flow = 0, total_cost = 0;
        fill(h.begin(), h.end(), 0); // Set all node potentials to 0

        while (true)
        {
            // 1) Find shortest augmenting path in residual graph under *reduced* costs
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
            fill(dist.begin(), dist.end(), INF);
            dist[s] = 0;
            pq.push({0, s});

            while (!pq.empty())
            {
                auto [d, v] = pq.top(); // d - distance; v - node
                pq.pop();
                if (dist[v] < d)
                    continue; // Skip if we've already found a better path here
                for (int i = 0; i < graph[v].size(); ++i)
                {
                    Edge &e = graph[v][i];
                    int cost = e.cost + h[v] - h[e.to]; // Reduced cost with potentials
                    if (e.cap > 0 && dist[e.to] > dist[v] + cost)
                    {
                        dist[e.to] = dist[v] + cost;
                        prevv[e.to] = v; // Store previous node
                        preve[e.to] = i; // Store previous edge index
                        pq.push({dist[e.to], e.to});
                    }
                }
            }

            if (dist[t] == INF)
                break; // No augmenting path found, stop

            // 2) Update potentials so that all reduced costs remain non-negative
            //    h[v] tracks the cost of the cheapest path found so far to v
            for (int v = 0; v < N; ++v)
                h[v] += dist[v];

            // 3) Compute bottleneck capacity along the found path
            int d = INF;
            for (int v = t; v != s; v = prevv[v])
            {
                d = min(d, graph[prevv[v]][preve[v]].cap);
            }

            total_flow += d;
            total_cost += d * h[t]; // h[t] is the true min-cost to send one unit

            // Update capacities along the path
            for (int v = t; v != s; v = prevv[v])
            {
                Edge &e = graph[prevv[v]][preve[v]];
                e.cap -= d;               // Reduce capacity on forward edge
                graph[v][e.rev].cap += d; // Open up back-edge for possible "refund"
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