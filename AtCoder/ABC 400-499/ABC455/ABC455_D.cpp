#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;
typedef int64_t ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, q;
  cin >> n >> q;
  vi nxt(n + 1);
  while (q--) {
    int c, p;
    cin >> c >> p;
    nxt[c] = p;
  }

  vector<bool> vis(n + 1);
  vi cnt(n + 1);

  function<void(int, int)> dfs = [&](int u, int tot) {
    vis[u] = true;
    if (nxt[u])
      dfs(nxt[u], tot + 1);
    else
      cnt[u] = tot;
  };
  for (int i = 1; i <= n; i++) {
    if (vis[i])
      continue;
    dfs(i, 1);
  }
  for (int i = 1; i <= n; i++)
    cout << cnt[i] << " \n"[i == n];

  return 0;
}
