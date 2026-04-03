/* https://atcoder.jp/contests/abc451/tasks/abc451_f */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;


struct UnionFind {
  vi p;
  UnionFind(int n) : p(n, -1) {}
  int find(int x) {
    return p[x] < 0 ? x : p[x] = find(p[x]); }
  bool is_same(int a, int b) { return find(a) == find(b); }
  int size(int a) { return -p[find(a)]; }
  bool unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return false;
    if (p[a] > p[b]) swap(a, b);
    p[a] += p[b]; p[b] = a;
    return true;
  }
}; 


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, q; cin >> n >> q;
  vector<vi> subgraphs(n);
  for (int i = 0; i < n; i++) subgraphs[i] = vi({i});
  vector<bool> is_col(n, 0);
  vi num_col(n, 0);
  UnionFind uf(n);

  int total_blacks = 0;
  while (q--) {
    int u, v; cin >> u >> v;
    u--; v--;

    if (total_blacks == -1) {
      cout << -1 << "\n";
      continue;
    }

    if (uf.is_same(u, v)) {
      if (is_col[u] == is_col[v]) total_blacks = -1;
      cout << total_blacks << "\n";
      continue;
    }

    bool flip = (is_col[u] == is_col[v]); 
    u = uf.find(u); v = uf.find(v);
    uf.unite(u, v);

    total_blacks -= min(num_col[u], (int)subgraphs[u].size() - num_col[u]);
    total_blacks -= min(num_col[v], (int)subgraphs[v].size() - num_col[v]);
    if (subgraphs[u].size() < subgraphs[v].size()) swap(u, v);
    for (int vv : subgraphs[v]) {
      subgraphs[u].emplace_back(vv);
      if (flip) is_col[vv] = !is_col[vv];
    } 
    num_col[v] = flip ? ((int)subgraphs[v].size() - num_col[v]) : num_col[v];
    num_col[u] += num_col[v];
    
    total_blacks += min(num_col[u], (int)subgraphs[u].size() - num_col[u]);

    cout << total_blacks << "\n";
  }
  
  return 0;
}