#include <random>

#include "graph.h"
#include "kosaraju.cpp"

using namespace std;

random_device rd;
mt19937 rng(rd());

Graph generate(int n, int m) {
    Graph graph(n);

    uniform_int_distribution<int> uniform(0, n - 1);

    for (int i = 0; i < m; i++) {
        while (true) {
            int u = uniform(rng);
            int v = uniform(rng);

            if (u != v && !graph.hasEdge(u, v)) {
                graph.addEdge(u, v);
                break;
            }
        }
    }

    return graph;
}

int main() {
    Graph graph = generate(5, 5);
    Kosaraju kos(graph);
    kos.run();

    cout << "Edges: " << endl;
    graph.print();
    cout << "Components: " << endl;
    kos.print();

    return 0;
}

