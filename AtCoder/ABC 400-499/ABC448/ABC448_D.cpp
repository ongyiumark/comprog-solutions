/* https://atcoder.jp/contests/abc448/tasks/abc448_d */
#include <iostream>
#include <vector>
#include <cassert>
#include <set>
#include <functional>

using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n; cin >> n;
  vi A(n);
  for (int &a : A) cin >> a;
  vector<vi> adj(n);
  for (int i = 0; i < n-1; i++) {
    int u, v; cin >> u >> v;
    u--; v--;
    adj[u].emplace_back(v);
    adj[v].emplace_back(u);
  }

  vector<bool> dup(n);
  multiset<int> tracker;
  function<void(int,int,bool)> dfs = [&](int u, int p, bool d) {
    if (tracker.find(A[u]) != tracker.end() || d) dup[u] = 1;
    tracker.insert(A[u]);

    for (int &v : adj[u]) {
      if (v == p) continue;
      dfs(v, u, dup[u]);
    }
    tracker.erase(tracker.find(A[u]));
  };
  dfs(0, -1, 0);

  for (int i = 0; i < n; i++) {
    cout << (dup[i] ? "Yes" : "No") << "\n";
  }
  
  return 0;
}