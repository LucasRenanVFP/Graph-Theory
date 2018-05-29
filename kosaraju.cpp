#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>

#include "graph.h"

using namespace std;

class Kosaraju {
public:
  Graph& graph;
  vector<bool> visited;
  stack<int> vertex_stack;
  vector<vector<int>> components;

  Kosaraju(Graph& graph) : graph(graph), visited(graph.n) {}

  void dfs(int v) {
    visited[v] = true;
    for(int u : graph.children[v]) {
      if(!visited[u]) dfs(u);
    }
    vertex_stack.push(v);
  }

  void reverse_dfs(int v, int component) {
    components[component].push_back(v);
    visited[v] = true;
    for(int u : graph.parents[v]) {
      if(!visited[u]) reverse_dfs(u, component);
    }
  }

  void run() {
    for(int i = 0; i < graph.n; i++) {
      if(!visited[i]) dfs(i);
    }

    visited.assign(graph.n, false);
    components.resize(graph.n);

    int cur_component = 0;
    while(!vertex_stack.empty()) {
      int v = vertex_stack.top();
      vertex_stack.pop();
      if(visited[v]) continue;

      reverse_dfs(v, cur_component);
      cur_component++;
    }

    components.resize(cur_component);
  }

  vector<vector<int>> result() {
    for(size_t i = 0; i < components.size(); i++) {
      sort(components[i].begin(), components[i].end());
    }

    sort(components.begin(), components.end());

    return components;
  }

  void print() {
    for(size_t i = 0; i < components.size(); i++) {
      for(size_t j = 0; j < components[i].size(); j++) {
        if(j != 0) cout << " ";
        cout << components[i][j];
      }
      cout << endl;
    }
  }
};
