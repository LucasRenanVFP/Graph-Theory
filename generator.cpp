#include <random>
#include <chrono>

#include "graph.h"

using namespace std;

random_device rd;
mt19937 rng(rd());

void connect_cluster(Graph& graph, vector<int> cluster, int m) {
    uniform_int_distribution<int> uniform(0, cluster.size() - 1);

    for (int i = 0; i < m; i++) {
        while (true) {
            int u = cluster[uniform(rng)];
            int v = cluster[uniform(rng)];

            if (u != v && !graph.hasEdge(u, v)) {
                graph.addEdge(u, v);
                break;
            }
        }
    }
}

Graph generate(int n, int m) {
    Graph graph(n);
    vector<int> vertices(n);

    for (int i = 0; i < n; i++) {
        vertices[i] = i;
    }

    connect_cluster(graph, vertices, m);
    return graph;
}

Graph generate_clusters(int n, int m, int clusters, int bridges) {
    Graph graph(n * clusters);

    for (int i = 0; i < clusters; i++) {
        vector<int> cluster(n);

        for (int j = 0; j < n; j++) {
            cluster[j] = i * n + j;
        }

        connect_cluster(graph, cluster, m);
    }

    uniform_int_distribution<int> vertex_dist(0, n - 1);
    Graph cluster_graph = generate(clusters, bridges);
    for (int v = 0; v < clusters; v++) {
        for (int u : cluster_graph.children[v]) {
            int source = v * n + vertex_dist(rng);
            int target = u * n + vertex_dist(rng);

            graph.addEdge(source, target);
        }
    }

    return graph;
}

