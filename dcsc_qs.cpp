#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#include "graph.h"

using namespace std;

class DCSC_QS {
public:
  Graph& graph;
  vector<vector<int>> components;
  vector<int> work_id;

  DCSC_QS(Graph& graph) : graph(graph), work_id(graph.n) {}

  void getDescendants(int v, int current_id) {
    work_id[v] |= 0b01;
    for(int u : graph.children[v]) {
      if(work_id[u] == current_id) getDescendants(u, current_id);
    }
  }

  void getPredecessors(int v, int current_id) {
    work_id[v] ^= 0b11;
    for(int u : graph.parents[v]) {
      if((work_id[u] ^ current_id) <= 1) getPredecessors(u, current_id);
    }
  }

  void findComponents(vector<int> &vertices) {
    stack<pair<int, int>> work_stack;
    if (vertices.size() > 0) work_stack.emplace(make_pair(0, vertices.size()));

    int current_id = 0;

    while (work_stack.size() != 0) {
      pair<int, int> work = work_stack.top();
      work_stack.pop();

      int v = vertices[work.first];

      for (int i = work.first; i < work.second; i++) {
        work_id[vertices[i]] = current_id;
      }

      getDescendants(v, current_id);
      getPredecessors(v, current_id);

      // vector segment is re-ordered as:
      // ... | rem | desc \ comp | comp | pred \ comp | ...
      auto comp = [this](int a, int b) {
        return work_id[a] < work_id[b];
      };

      sort(vertices.begin() + work.first, vertices.begin() + work.second, comp);

      int rem_start = work.first;
      int desc_start = work.second;
      int component_start = work.second;
      int pred_start = work.second;

      vector<int> component;

      for (int i = work.first; i < work.second; i++) {
        int vertex_id = work_id[vertices[i]];

        if (vertex_id == (current_id | 0b01)) {
          if (desc_start == work.second) desc_start = i;

        } else if (vertex_id == (current_id | 0b10)) {
          if (desc_start == work.second) desc_start = i;
          if (component_start == work.second) component_start = i;
          component.push_back(vertices[i]);

        } else if (vertex_id == (current_id | 0b11)) {
          if (desc_start == work.second) desc_start = i;
          if (component_start == work.second) component_start = i;
          if (pred_start == work.second) pred_start = i;
        }
      }

      components.push_back(component);

      if (desc_start - rem_start > 0) {
        work_stack.emplace(make_pair(rem_start, desc_start));
      }

      if (component_start - desc_start > 0) {
        work_stack.emplace(make_pair(desc_start, component_start));
      }

      if (work.second - pred_start > 0) {
        work_stack.emplace(make_pair(pred_start, work.second));
      }

      current_id += 0b100;
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

    if (vertices.size()) findComponents(vector<int>(vertices.begin(), vertices.end()));
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
