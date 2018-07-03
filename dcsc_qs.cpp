#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <thread>
#include <atomic>
#include <functional>

#include "graph.h"
#include "ThreadPool.h"
#include "blockingconcurrentqueue.h"

using namespace std;

class DCSC_QS {
public:
  Graph& graph;
  vector<vector<int>> components;
  vector<int> work_id;
  int threads;

  DCSC_QS(Graph& graph, int threads) : graph(graph), work_id(graph.n), threads(threads) {}

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
    if (vertices.size() > 0) work_stack.emplace(0, vertices.size());

    ThreadPool pool(threads);
    moodycamel::BlockingConcurrentQueue<vector<int>> result_queue;

    auto comp = [this](int a, int b) {
      return work_id[a] < work_id[b];
    };

    atomic_int current_id(0);
    atomic_int work_count(0);
    function<void(int, int)> worker = [&](int begin, int end) {
      int v = vertices[begin];
      int id = (current_id += 0b100);

      for (int i = begin; i < end; i++) {
        work_id[vertices[i]] = id;
      }

      getDescendants(v, id);
      getPredecessors(v, id);

      // vector segment is re-ordered as:
      // ... | rem | desc \ comp | comp | pred \ comp | ...
      sort(vertices.begin() + begin, vertices.begin() + end, comp);

      int rem_start = begin;
      int desc_start = end;
      int component_start = end;
      int pred_start = end;

      vector<int> component;

      for (int i = begin; i < end; i++) {
        int vertex_id = work_id[vertices[i]];

        if (vertex_id == (id | 0b01)) {
          if (desc_start == end) desc_start = i;

        } else if (vertex_id == (id | 0b10)) {
          if (desc_start == end) desc_start = i;
          if (component_start == end) component_start = i;
          component.push_back(vertices[i]);

        } else if (vertex_id == (id | 0b11)) {
          if (desc_start == end) desc_start = i;
          if (component_start == end) component_start = i;
          if (pred_start == end) pred_start = i;
        }
      }

      if (desc_start - rem_start > 0) {
        work_count++;
        pool.enqueue(worker, rem_start, desc_start);
      }

      if (component_start - desc_start > 0) {
        work_count++;
        pool.enqueue(worker, desc_start, component_start);
      }

      if (end - pred_start > 0) {
        work_count++;
        pool.enqueue(worker, pred_start, end);
      }

      result_queue.enqueue(component);
    };

    work_count++;
    pool.enqueue(worker, 0, vertices.size());

    while (work_count != 0) {
      work_count--;

      vector<int> component;
      result_queue.wait_dequeue(component);
      components.emplace_back(component);
    }
 };

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

    cout << "Trim found " << components.size() << " components" << endl;
    if (vertices.size()) {
      vector<int> vertices_vec(vertices.begin(), vertices.end());
      findComponents(vertices_vec);
    }
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
