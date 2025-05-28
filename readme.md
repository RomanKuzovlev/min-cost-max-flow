# Min-Cost Max-Flow (MCMF)

C++ implementation of the classic successive shortest-path algorithm.  
Goal: fully understand it and optimize performance.

Uses node potentials to keep Dijkstra valid even with negative-cost reverse edges.  
Residual graph maintains flow reversibility via back edges..

## Build & Run
```bash
g++ mcmf.cpp -O2 -std=c++23 -o mcmf
./mcmf < input.txt
```

## Notes

This section is for helping me recall the key tricky points.

Two most difficult concepts for me to understand in this algorithm were potentials and using reverse edges. Notes here are to let me easier remember it.

### Potentials

We assign a potential to each node to keep all reduced edge costs non-negative:

```cpp
reduced_cost = raw_cost + h[u] - h[v];
```

This makes Dijkstra work even with negative-cost reverse edges—without needing Bellman-Ford.

The potential formula preserves relative costs, reweights the graph, and ensures previously used paths have zero reduced cost, so we don’t waste time re-exploring them. It also prevents negative-cost cycles.


### Reverse edges

Every edge gets a reverse edge with `cap = 0` and `cost = -original`.

Pushing flow opens the reverse edge. Later, if a cheaper route appears (made possible by these reverse edges), we can "refund" the original flow and reroute it at lower cost.

The key idea: this may — and often does — undo part of a previous path. It costs us something locally, but if the reroute saves more elsewhere, it's worth it.b Think: sacrificing a bit of efficiency near the sink to unlock a much cheaper route for a larger amount of flow upstream.

We also never lose downstream flow: rerouting only happens when there’s already an alternative path to preserve flow past the refunded point.
