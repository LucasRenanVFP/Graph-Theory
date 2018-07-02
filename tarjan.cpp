#include<bits/stdc++.h>

#include "graph.h"

using namespace std;

class Tarjan {
  public:
  Graph& graph;
	int n;
  stack<int> vertex_stack;
  vector< vector<int> > groups;
  vector<bool> used;
  vector<bool> on_stack;
  vector<int> lowest;
  vector<int> level;
  int current_index;
  int current_group;
  
  Tarjan(Graph& graph) : graph(graph), n(graph.n), used(n, false), groups(n + 2), lowest(n), level(n), on_stack(n, false) {}

  void dfs(int u) {
    used[u] = true;
    lowest[u] = level[u] = current_index++;
    vertex_stack.push(u);
    on_stack[u] = true;
    for(int v : graph.children[u]) {
      if(!used[v]) {
        dfs(v);
        lowest[u] = min(lowest[u], lowest[v]);
      }
      else if(on_stack[v]) {
        lowest[u] = min(lowest[u], level[v]);
      }
    }
    if(lowest[u] == level[u]) {
      int current;
      do {
        current = vertex_stack.top();
        vertex_stack.pop();
        on_stack[current] = false;
        groups[current_group].push_back(current);
      } while(current != u);
      current_group++;
    }
  }

  void run() {
    current_index = 1;
    current_group = 0;
    for(int i = 0; i < n; i++) {
      if(!used[i]) dfs(i);
    }
    for(int i = 0; i < current_group; i++) {
      sort(groups[i].begin(), groups[i].end());
    }
    sort(groups.begin(), groups.begin() + min(current_group, (int)groups.size()));
  }

  vector< vector<int> > result() {
    vector< vector<int> > ret;
    for(int i = 0; i < current_group; i++) {
      if(groups[i].size() == 0) break;
      ret.push_back(groups[i]);
    }
		/*for(int i = 0; i < ret.size(); i++) {
			cout << "group #" << i << ": ";
			for(int j = 0; j < ret[i].size(); j++) {
				cout << ret[i][j] << " ";
			}
			cout << endl;
		}*/
    return ret;
  }
};












