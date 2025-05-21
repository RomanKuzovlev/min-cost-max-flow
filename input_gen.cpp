#include <bits/stdc++.h>
using namespace std;

int main()
{
    int N = 100000; // nodes
    int M = 500000; // edges
    int maxCap = 20, maxCost = 10;
    cout << N << ' ' << M << '\n';
    mt19937 rng(42);
    uniform_int_distribution<int> dist(0, N - 1);
    uniform_int_distribution<int> cap(1, maxCap);
    uniform_int_distribution<int> cost(1, maxCost);

    for (int i = 0; i < M; ++i)
    {
        int u = dist(rng), v = dist(rng);
        while (u == v)
            v = dist(rng);
        cout << u << ' ' << v << ' ' << cap(rng) << ' ' << cost(rng) << '\n';
    }

    cout << "0 " << N - 1 << '\n'; // source and sink
}
