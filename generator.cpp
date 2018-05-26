#include <random>

#include "graph.h"
#include "kosaraju.cpp"
#include "dcsc.cpp"

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

    DCSC dcsc(graph);
    dcsc.run();

    cout << "Edges: " << endl;
    graph.print();
    cout << "Components (Kosaraju): " << endl;
    kos.print();
    cout << "Components (DCSC): " << endl;
    dcsc.print();

    return 0;
}

