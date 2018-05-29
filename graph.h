#pragma once

#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

using namespace std;

class Graph {
public:
  vector<vector<int>> children;
  vector<vector<int>> parents;
  int n;

  Graph(int n) : n(n), children(n), parents(n) {};

  ~Graph() {};

  void addEdge(int u, int v) {
    children[u].push_back(v);
    parents[v].push_back(u);
  }

  bool hasEdge(int u, int v) {
    vector<int> &childs = children[u];
    return find(childs.begin(), childs.end(), v) != childs.end();
  }

  void print() {
    for (int i = 0; i < n; i++) {
      for (int j : children[i]) {
        cout << i << " " << j << endl;
      }
    }
  }
};
