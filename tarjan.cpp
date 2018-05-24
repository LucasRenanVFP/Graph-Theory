#include<bits/stdc++.h>

using namespace std;

class Tarjan {
  public:
  vector< vector<int> > graph;
  int n;
  stack<int> vertex_stack;
  vector< vector<int> > groups;
  vector<bool> used;
  vector<bool> on_stack;
  vector<int> lowest;
  vector<int> level;
  int current_index;
  int current_group;
  
  Tarjan() {}

  Tarjan(int _n) : n(_n), graph(_n + 1), used(_n + 1, false), groups(_n + 2), lowest(_n + 1), level(_n + 1), on_stack(_n + 1, false) {}

  void addEdge(int u, int v) {
    graph[u].push_back(v);
  }

  void dfs(int u) {
    used[u] = true;
    lowest[u] = level[u] = current_index++;
    vertex_stack.push(u);
    on_stack[u] = true;
    for(int v : graph[u]) {
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

  void process() {
    current_index = 1;
    current_group = 0;
    for(int i = 1; i <= n; i++) {
      if(!used[i]) dfs(i);
    }
    for(int i = 0; i < current_group; i++) {
      sort(groups[i].begin(), groups[i].end());
    }
    sort(groups.begin(), groups.begin() + current_group);
  }

  vector< vector<int> > getSCC() {
    process();
    vector< vector<int> > ret;
    for(int i = 0; i <= n + 1; i++) {
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
  Tarjan tarj(n);
  for(int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    tarj.addEdge(u, v);
  }
  vector< vector<int> > scc = tarj.getSCC();
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












