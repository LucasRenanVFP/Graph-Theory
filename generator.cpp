#include <random>
#include <chrono>

#include "graph.h"
#include "kosaraju.cpp"
#include "dcsc_qs.cpp"
#include "dcsc_set.cpp"

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
    int edges = 100000;
    Graph graph = generate(vertices, edges);

    Kosaraju kos(graph);
    auto start_time = chrono::high_resolution_clock::now();
    kos.run();
    auto end_time = chrono::high_resolution_clock::now();
    long kos_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    DCSC_QS dcsc_qs(graph);
    start_time = chrono::high_resolution_clock::now();
    dcsc_qs.run();
    end_time = chrono::high_resolution_clock::now();
    long dcsc_qs_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    DCSC_Set dcsc_set(graph);
    start_time = chrono::high_resolution_clock::now();
    dcsc_set.run();
    end_time = chrono::high_resolution_clock::now();
    long dcsc_set_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    cout << vertices << " vertices / " << edges << " edges" << endl;
    // cout << "Edges: " << endl;
    // graph.print();
    cout << "Components Kosaraju: " << kos_time << "ms" << endl;
    // kos.print();
    cout << "Components DCSC (QS): " << dcsc_qs_time << "ms" << endl;
    // dcsc.print();
    cout << "Components DCSC (Set): " << dcsc_set_time << "ms" << endl;

    if (dcsc_qs.result() == kos.result() && kos.result() == dcsc_set.result()) {
        cout << "Results match (" << dcsc_qs.result().size() << " scc's)";
    } else {
        cout << "Kosaraju found " << kos.result().size() << " components" << endl;
        cout << "DCSC (QS) found " << dcsc_qs.result().size() << " components" << endl;
        cout << "DCSC (Set) found " << dcsc_set.result().size() << " components" << endl;
    }

    return 0;
}

