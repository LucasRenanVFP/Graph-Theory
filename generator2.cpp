#include<bits/stdc++.h>

#include "graph.h"
#include "kosaraju.cpp"
#include "dcsc_qs.cpp"
#include "dcsc_set.cpp"

using namespace std;


random_device rd;
mt19937 rng(rd());


Graph generate(int n, int m, int max_deg) {
    Graph graph(n);
		
    uniform_int_distribution<int> uniform(0, n - 1);

    for (int i = 0; i < m; i++) {
        while (true) {
            int u = uniform(rng);
            int v = uniform(rng);

            if (u != v and !graph.hasEdge(u, v) and (graph.getDegree(u) < max_deg)) {
                graph.addEdge(u, v);
                break;
            }
        }
    }

    return graph;
}

int main(int argc, char* argv[]) {
    int vertices, edges, max_degree;
    if (argc < 4) {
        cerr << "usage: " << argv[0] << " [vertices] [edges] [max degree]" << endl;
				exit(1);
    }

    stringstream(argv[1]) >> vertices;
    stringstream(argv[2]) >> edges;
    stringstream(argv[3]) >> max_degree;
		if(vertices * max_degree < edges or max_degree >= vertices) {
			cerr << "Incompatible number of edges and max degree" << endl;
			exit(1);
		}
    cout << vertices << " vertices / " << edges << " edges / " << max_degree << " max degree" << endl;

    Graph graph = generate(vertices, edges, max_degree);

		cerr << "Graph generated correctly" << endl;

    Kosaraju kos(graph);
    auto start_time = chrono::high_resolution_clock::now();
    kos.run();
    auto end_time = chrono::high_resolution_clock::now();
    auto kos_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    DCSC_QS dcsc_qs(graph);
    start_time = chrono::high_resolution_clock::now();
    dcsc_qs.run();
    end_time = chrono::high_resolution_clock::now();
    auto dcsc_qs_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    DCSC_Set dcsc_set(graph);
    start_time = chrono::high_resolution_clock::now();
    dcsc_set.run();
    end_time = chrono::high_resolution_clock::now();
    auto dcsc_set_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

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

