#include<bits/stdc++.h>

using namespace std;

class Kosaraju {
  public:
  vector< vector<int> > graph;
  vector< vector<int> > reverse_graph;
  int n;
  stack<int> vertex_stack;
  vector< vector<int> > groups;
  vector<bool> used;
  Kosaraju() {}

  Kosaraju(int _n) : n(_n), graph(_n + 1), reverse_graph(_n + 1), used(_n + 1, false), groups(_n + 2) {}

  void addEdge(int u, int v) {
    graph[u].push_back(v);
    reverse_graph[v].push_back(u);
  }

  void dfs(int u) {
    used[u] = true;
    for(int v : graph[u]) {
      if(!used[v]) dfs(v);
    }
    vertex_stack.push(u);
  }

  void reverse_dfs(int u, int group) {
    groups[group].push_back(u);
    used[u] = true;
    for(int v : reverse_graph[u]) {
      if(!used[v]) reverse_dfs(v, group);
    }
  }

  void process() {
    for(int i = 1; i <= n; i++) {
      if(!used[i]) dfs(i);
    }
    used.assign(n + 1, false);
    int group = 0;
    while(!vertex_stack.empty()) {
      int u = vertex_stack.top();
      vertex_stack.pop();
      if(used[u]) continue;
      reverse_dfs(u, group);
      group++;
    }
    for(int i = 0; i < group; i++) {
      sort(groups[i].begin(), groups[i].end());
    }
    sort(groups.begin(), groups.begin() + group);
  }

  vector< vector<int> > getSCC() {
    process();
    vector< vector<int> > ret;
    for(int i = 1; i <= n + 1; i++) {
      if(groups[i].size() == 0) break;
      ret.push_back(groups[i]);
    }
    return ret;
  }
};

int n, m;

int main() {
  ios::sync_with_stdio(false);
  cin >> n >> m;
  Kosaraju kos(n);
  for(int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    kos.addEdge(u, v);
  }
  vector< vector<int> > scc = kos.getSCC();
  for(int i = 0; i < scc.size(); i++) {
    cout << "Group #" << i << ":\n";
    for(int j = 0; j < scc[i].size(); j++) {
      if(j != 0) cout << " ";
      cout << scc[i][j];
    }
    cout << "\n";
  }
  return 0;
}












