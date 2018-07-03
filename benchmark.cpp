#include <iostream>
#include <sstream>

#include "generator.cpp"
#include "kosaraju.cpp"
#include "dcsc_qs.cpp"
#include "dcsc_set.cpp"
#include "tarjan.cpp"

int main(int argc, char* argv[]) {
    int vertices, edges, components, bridges;

    if (argc < 5) {
        cout << "usage: " << argv[0] << " [verices per component] [edges per component]" << endl;
        cout << "             [components] [bridges]" << endl;
        exit(1);
    }

    stringstream(argv[1]) >> vertices;
    stringstream(argv[2]) >> edges;
    stringstream(argv[3]) >> components;
    stringstream(argv[4]) >> bridges;
    cout << vertices << " vertices / " << edges << " edges / "
        << components << " components / " << bridges << " bridges" << endl;

    // Graph graph = generate(vertices, edges);
    Graph graph = generate_clusters(vertices, edges, components, bridges);
    // Graph graph = generate_clusters(50000, 500000, 20, 10);

    Kosaraju kos(graph);
    auto start_time = chrono::high_resolution_clock::now();
    kos.run();
    auto end_time = chrono::high_resolution_clock::now();
    auto kos_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    DCSC_QS dcsc_qs(graph, 1);
    start_time = chrono::high_resolution_clock::now();
    dcsc_qs.run();
    end_time = chrono::high_resolution_clock::now();
    auto dcsc_qs_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    DCSC_Set dcsc_set(graph);
    start_time = chrono::high_resolution_clock::now();
    dcsc_set.run();
    end_time = chrono::high_resolution_clock::now();
    auto dcsc_set_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    Tarjan tarjan(graph);
    start_time = chrono::high_resolution_clock::now();
    tarjan.run();
    end_time = chrono::high_resolution_clock::now();
    auto tarjan_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    // cout << "Edges: " << endl;
    // graph.print();
    // kos.print();
    cout << "Components Tarjan: " << tarjan_time << "ms" << endl;
    cout << "Components Kosaraju: " << kos_time << "ms" << endl;
    cout << "Components DCSC (QS): " << dcsc_qs_time << "ms" << endl;
    cout << "Components DCSC (Set): " << dcsc_set_time << "ms" << endl;

    if (dcsc_qs.result() == kos.result() && kos.result() == dcsc_set.result() && tarjan.result() == kos.result()) {
        cout << "Results match (" << dcsc_qs.result().size() << " scc's)";
    } else {
        cout << "Tarjan found " << tarjan.result().size() << " components" << endl;
        cout << "Kosaraju found " << kos.result().size() << " components" << endl;
        cout << "DCSC (QS) found " << dcsc_qs.result().size() << " components" << endl;
        cout << "DCSC (Set) found " << dcsc_set.result().size() << " components" << endl;
    }

    return 0;
}
