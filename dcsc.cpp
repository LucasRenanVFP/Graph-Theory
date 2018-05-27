#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#include "graph.h"

using namespace std;

class DCSC {
public:
  Graph& graph;
  vector<vector<int>> components;
  vector<int> pred_count;
  vector<int> desc_count;

  DCSC(Graph& graph) : graph(graph), pred_count(graph.n), desc_count(graph.n) {}

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

      for(int v : desc) {
        if(pred.find(v) != pred.end()) {
          component.push_back(v);
        }
      }
      components.push_back(component);

      for (int i : desc_pred) {
        vertices.erase(i);
      }

      if (vertices.size() == 0) work_stack.pop();

      for (int i : component) {
        pred.erase(i);
        desc.erase(i);
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
    for(int i = 0; i < components.size(); i++) {
      sort(components[i].begin(), components[i].end());
    }

    sort(components.begin(), components.end());

    return components;
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
