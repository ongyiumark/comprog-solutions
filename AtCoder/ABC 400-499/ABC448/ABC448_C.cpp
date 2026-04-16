/* https://atcoder.jp/contests/abc448/tasks/abc448_c */
#include <iostream>
#include <vector>
#include <cassert>
#include <bit>
#include <functional>
#include <limits>

using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

template <integral T, auto op, auto e>
requires is_convertible_v<decltype(op), function<T(T,T)>>
&& is_convertible_v<decltype(e), function<T()>>
struct SparseTable {
  int n;
  vector<vector<T>> tbl;
  SparseTable(const vector<T> &ar) {
    n = ar.size();
    int logn = countr_zero(bit_ceil<size_t>(n));

    tbl = vector<vector<T>>(logn, vector<T>(n));
    for (int i = 0; i < n; i++) tbl[0][i] = ar[i];
    for (int k = 1; k < logn; k++) {
      for (int i = 0; i < n; i++) {
        int nxt = min(n-1, i+(1<<(k-1)));
        tbl[k][i] = op(tbl[k-1][i], tbl[k-1][nxt]);
      }
    }
  }

  T query(int l, int r) { // [l,r)
    assert(0 <= l && l <= r && r <= n);
    if (l == r) return e();
    if (l+1 == r) return tbl[0][l];
    int k = countr_zero(bit_floor<size_t>(r-l));
    return op(tbl[k][l], tbl[k][r-(1<<k)]);
  }
};


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, q; cin >> n >> q;
  vi A(n);
  for (int &a : A) cin >> a;

  auto op = [](int a, int b) { return min(a, b); };
  auto e = []() { return numeric_limits<int>::max(); };
  SparseTable<int, op, e> sparse(A);

  vi B(5);
  while (q--) {
    int k; cin >> k;
    for (int j = 0; j < k; j++) {
      cin >> B[j];
      B[j]--;
    }
    int ans = sparse.query(0, B[0]);
    for (int i = 1; i < k; i++) ans = min(ans, sparse.query(B[i-1]+1, B[i]));
    ans = min(ans, sparse.query(B[k-1]+1, n));
    cout << ans << "\n";
  }
  
  return 0;
}