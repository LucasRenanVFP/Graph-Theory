#include<bits/stdc++.h>

using namespace std;

class DCSC {
  public:
  vector< vector<int> > graph;
  vector< vector<int> > reverse_graph;
  int n;
  vector< vector<int> > groups;
  vector<bool> used;
  int current_group;
  DCSC() {}

  DCSC(int _n) : n(_n), graph(_n + 1), reverse_graph(_n + 1), used(_n + 1, false), groups(_n + 2) {}

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

  void getDecendent(int u, vector<int> &decendent) {
    decendent.push_back(u);
    for(int v : graph[u]) {
      if(!used[v]) getDecendent(v, decendent);
    }
  }

  void getGroup(int u) {
    groups[current_group].push_back(u);
    vector<int> decendent;
    getDecendent(u, decendent);
    vector<int> predecendent;
    getPredecendent(u, predecendent);
    for(int v : decendent) {
      if(v == u) continue;
      if(is_predecendent[v]) {
        groups[current_group].push_back(v);
        used[v] = true;
      }
    }
  }

  void process() {
    current_group = 0;
    for(int i = 1; i <= n; i++) {
      if(!used[i]) {
        getGroup(i);
      }
    }
  }

  vector< vector<int> > getSCC() {
    process();
    return groups;
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












