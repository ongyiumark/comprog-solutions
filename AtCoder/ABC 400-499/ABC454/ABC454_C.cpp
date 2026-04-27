#include <iostream>
#include <vector>
#include <cassert>
#include <functional>

using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m; cin >> n >> m;
  vector<vi> adj(n);
  for (int i = 0; i < m; i++) {
    int a, b; cin >> a >> b;
    a--; b--;
    adj[a].emplace_back(b);
  }

  vector<bool> vis(n);
  function<void(int)> dfs = [&](int u) {
    vis[u] = true;
    for (int &v : adj[u]) {
      if (vis[v]) continue;
      dfs(v);
    }
  };
  dfs(0);

  int cnt = 0;
  for (int i = 0; i < n; i++)
    cnt += vis[i];

  cout << cnt << "\n"; 
  return 0;
}