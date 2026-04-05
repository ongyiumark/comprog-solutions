/* https://atcoder.jp/contests/abc452/tasks/abc452_f */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

template<typename T>
struct FenwickTree {
  vector<T> ft;
  FenwickTree(int n) : ft(n+1) {}
  T query(int i) {
    T total = 0;
    for (; i > 0; i -= (i&-i)) total += ft[i];
    return total;
  }
  T query(int l, int r) { return query(r)-query(l-1); }
  void update(int i, T v) {
    for (; i < (int)ft.size(); i += (i&-i))
      ft[i] += v;
  }

  // optional O(n) build from array
  FenwickTree(const vector<T> &ar) {
    int n = ar.size(); ft.assign(n+1, 0);
    for (int i = 1; i <= n; i++) {
      ft[i] += ar[i-1];
      if (i+(i&-i) <= n) ft[i+(i&-i)] += ft[i];
    }
  }
};


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n; ll k; cin >> n >> k;
  vi P(n);
  for (int &x : P) cin >> x;

  function<ll(ll)> solve = [&](ll kk) {
    ll ans = 0;
    FenwickTree<int> ft(n);
    ll ninv = 0;
    for (int i = 0, j = 0; i < n; i++) {
      if (P[i]+1 <= n) ninv += ft.query(P[i]+1, n);
      ft.update(P[i], 1);

      while (ninv > kk && j < i) {
        if (1 <= P[j]-1) ninv -= ft.query(1, P[j]-1);
        ft.update(P[j], -1);
        j++;
      }
      ans += (i-j+1);
    }
    return ans;
  };

  cout << solve(k)-(k ? solve(k-1) : 0) << "\n";
  
  return 0;
}