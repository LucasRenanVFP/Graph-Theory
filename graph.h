#pragma once

#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <set>
using namespace std;

class Graph {
public:
  vector< vector<int> > children;
  vector< vector<int> > parents;
  vector<int> degree;
	set< pair<int, int> > edges;
	int n;

  Graph(int n) : n(n), children(n), parents(n), degree(n, 0) {};

  ~Graph() {};

  void addEdge(int u, int v) {
    children[u].push_back(v);
    parents[v].push_back(u);
		degree[u]++;
  	edges.insert(make_pair(u, v));
	}

  bool hasEdge(int u, int v) {
  	return edges.count(make_pair(u, v));
	}

	int getDegree(int u) {
		return degree[u];
	}

  void print() {
    for (int i = 0; i < n; i++) {
      for (int j : children[i]) {
        cout << i << " " << j << endl;
      }
    }
  }
};
