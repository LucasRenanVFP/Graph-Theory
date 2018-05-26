#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#include "graph.h"

using namespace std;

class DCSC {
public:
  Graph& graph;
  vector<vector<int>> components;

  DCSC(Graph& graph) : graph(graph) {}

  void getDescendants(int v,
                      unordered_set<int> &vertices,
                      unordered_set<int> &desc,
                      unordered_set<int> &desc_pred) {
    desc.insert(v);
    desc_pred.insert(v);
    for(int u : graph.children[v]) {
      if(vertices.find(u) != vertices.end() && desc.find(u) == desc.end())
        getDescendants(u, vertices, desc, desc_pred);
    }
  }

  void getPredecessors(int v,
                       unordered_set<int> &vertices,
                       unordered_set<int> &pred,
                       unordered_set<int> &desc_pred) {
    pred.insert(v);
    desc_pred.insert(v);
    for(int u : graph.parents[v]) {
      if(vertices.find(u) != vertices.end() && pred.find(u) == pred.end())
        getPredecessors(u, vertices, pred, desc_pred);
    }
  }

  void findComponent(unordered_set<int> &vertices) {
    if (vertices.size() == 0) return;

    int v = *vertices.begin();

    vector<int> component;
    unordered_set<int> desc;
    unordered_set<int> pred;
    unordered_set<int> desc_pred;

    getDescendants(v, vertices, desc, desc_pred);
    getPredecessors(v, vertices, pred, desc_pred);

    for(int v : desc) {
      if(pred.find(v) != pred.end()) {
        component.push_back(v);
      }
    }
    components.push_back(component);

    for (int i : desc_pred) {
      vertices.erase(i);
    }
    findComponent(vertices);

    for (int i : component) {
      pred.erase(i);
      desc.erase(i);
    }
    findComponent(pred);
    findComponent(desc);
  }

  void run() {
    unordered_set<int> vertices;

    for (int i = 0; i < graph.n; i++) {
      vertices.insert(i);
    }

    findComponent(vertices);

    for(int i = 0; i < components.size(); i++) {
      sort(components[i].begin(), components[i].end());
    }

    sort(components.begin(), components.end());
  }

  void print() {
    for(int i = 0; i < components.size(); i++) {
      for(int j = 0; j < components[i].size(); j++) {
        if(j != 0) cout << " ";
        cout << components[i][j];
      }
      cout << endl;
    }
  }
};
