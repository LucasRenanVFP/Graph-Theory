#include <random>
#include <chrono>

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
    int vertices = 20000;
    int edges = 2000;
    Graph graph = generate(vertices, edges);

    Kosaraju kos(graph);
    auto start_time = chrono::high_resolution_clock::now();
    kos.run();
    auto end_time = chrono::high_resolution_clock::now();
    long kos_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    DCSC dcsc(graph);
    start_time = chrono::high_resolution_clock::now();
    dcsc.run();
    end_time = chrono::high_resolution_clock::now();
    long dcsc_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    cout << vertices << " vertices / " << edges << " edges" << endl;
    // cout << "Edges: " << endl;
    // graph.print();
    cout << "Components Kosaraju (" << kos_time << "ms)" << endl;
    // kos.print();
    cout << "Components DCSC (" << dcsc_time << "ms)" << endl;

    if (dcsc.result() == kos.result()) {
        cout << "Results match";
    }
    // dcsc.print();

    return 0;
}

