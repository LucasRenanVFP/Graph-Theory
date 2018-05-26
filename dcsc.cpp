#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>

#include "graph.h"

using namespace std;

class DCSC {
  public:
  Graph& graph;
  vector<vector<int>> components;
  vector<bool> visited;
  vector<bool> processed;

  DCSC(Graph& graph) : graph(graph), processed(graph.n) {}

  void getDescendants(int v, vector<int> &desc) {
    desc.push_back(v);
    visited[v] = true;
    for(int u : graph.children[v]) {
      if(!visited[u]) getDescendants(u, desc);
    }
  }

  void getPredecessors(int v, vector<int> &pred) {
    pred.push_back(v);
    visited[v] = true;
    for(int u : graph.parents[v]) {
      if(!visited[u]) getPredecessors(u, pred);
    }
  }

  void findComponent(int v) {
    vector<int> component;
    vector<int> desc;
    vector<int> pred;

    visited = processed;
    getDescendants(v, desc);
    visited = processed;
    getPredecessors(v, pred);


    for(int u : desc) {
      if(visited[u]) {
        component.push_back(u);
        processed[u] = true;
      }
    }

    components.push_back(component);
  }

  void run() {
    for(int i = 0; i < graph.n; i++) {
      if(!processed[i]) {
        findComponent(i);
      }
    }

    for(int i = 0; i < components.size(); i++) {
      sort(components[i].begin(), components[i].end());
    }
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
