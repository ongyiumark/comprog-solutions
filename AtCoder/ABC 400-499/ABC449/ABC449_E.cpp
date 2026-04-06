/* https://atcoder.jp/contests/abc449/tasks/abc449_e */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m; cin >> n >> m;
  vi A(n);
  for (int &x : A) cin >> x;

  int q; cin >> q;
  vector<pair<ll,int>> X(q);
  for (int i = 0; i < q; i++) {
    ll x; cin >> x;
    x--;
    X[i] = {x, i};
  }
  sort(X.begin(), X.end());

  vi freq(m+1); // get frequencies
  for (int &x : A) freq[x]++;

  vector<vi> c(n+1); // c[i] = elements that appear i times
  for (int i = 1; i <= m; i++) c[freq[i]].emplace_back(i);

  vll level_sz(n+1);
  level_sz[0] = c[0].size();
  for (int i = 1; i <= n; i++) level_sz[i] = level_sz[i-1] + c[i].size(); 

  vi result(q);
  int level = 0;
  ordered_set<int> s;
  ll idx = 0, pidx = 0;

  for (int j = 0; j < q; j++) {
    ll x; int i; tie(x,i) = X[j];
    if (x < n) {
      result[i] = A[x];
      continue;
    }
    x -= n;

    while(level <= n && idx <= x) {
      for (int &v : c[level]) s.insert(v);
      pidx = idx;
      idx += level_sz[level++];
    }

    if (idx > x) result[i] = *s.find_by_order(x-pidx);
    else result[i] = *s.find_by_order((x-pidx)%level_sz[n]);
  }

  for (int i = 0; i < q; i++) cout << result[i] << "\n";

  
  return 0;
}