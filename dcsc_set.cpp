#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#include "graph.h"

using namespace std;

class DCSC_Set {
public:
  Graph& graph;
  vector<vector<int>> components;

  DCSC_Set(Graph& graph) : graph(graph) {}

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

  void findComponents(unordered_set<int> &starting_vertices) {
    stack<unordered_set<int>> work_stack;
    work_stack.push(starting_vertices);

    while (work_stack.size() != 0) {
      unordered_set<int>& vertices = work_stack.top();

      int v = *vertices.begin();

      vector<int> component;
      unordered_set<int> desc;
      unordered_set<int> pred;
      unordered_set<int> desc_pred;

      getDescendants(v, vertices, desc, desc_pred);
      getPredecessors(v, vertices, pred, desc_pred);

      for(int u : desc) {
        if(pred.find(u) != pred.end()) {
          component.push_back(u);
        }
      }
      components.push_back(component);

      for (int u : desc_pred) {
        vertices.erase(u);
      }

      if (vertices.size() == 0) work_stack.pop();

      for (int u : component) {
        pred.erase(u);
        desc.erase(u);
      }
      if (pred.size() != 0) work_stack.push(pred);
      if (desc.size() != 0) work_stack.push(desc);
    }
 }

  void run() {
    unordered_set<int> vertices;
    vector<int> pred_count(graph.n);
    vector<int> desc_count(graph.n);
    queue<int> trim_queue;

    for (int i = 0; i < graph.n; i++) {
        vertices.insert(i);
        pred_count[i] = graph.parents[i].size();
        desc_count[i] = graph.children[i].size();
        if (pred_count[i] == 0 || desc_count[i] == 0) trim_queue.push(i);
    }

    while (trim_queue.size() != 0) {
      int v = trim_queue.front();
      trim_queue.pop();

      vertices.erase(v);
      components.push_back({v});

      for (int u : graph.children[v]) {
        pred_count[u] -= 1;
        if (pred_count[u] == 0 && desc_count[u] != 0) {
          trim_queue.push(u);
        }
      }

      for (int u : graph.parents[v]) {
        desc_count[u] -= 1;
        if (desc_count[u] == 0 && pred_count[u] != 0) {
          trim_queue.push(u);
        }
      }
    }

    if (vertices.size()) findComponents(vertices);
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
