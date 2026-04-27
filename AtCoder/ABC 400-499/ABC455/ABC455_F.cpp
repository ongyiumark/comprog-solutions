#include <cassert>
#include <concepts>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;
typedef int64_t ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

/* Supports range updates and range queries in $O(\log n)$. This is based on the
 * AC Library. */
template <typename T, auto op, auto e, typename U, auto mapping, auto compose,
          auto id>
  requires is_convertible_v<decltype(op), function<T(T, T)>> &&
           is_convertible_v<decltype(e), function<T()>> &&
           is_convertible_v<decltype(mapping), function<T(U, T)>> &&
           is_convertible_v<decltype(compose), function<U(U, U)>> &&
           is_convertible_v<decltype(id), function<U()>>
struct SegmentTree {
  int n, sz, log;
  vector<T> st;
  vector<U> lz;
  SegmentTree(const vector<T> &ar) : n(ar.size()) {
    sz = bit_ceil<size_t>(n);
    log = countr_zero<size_t>(sz);
    st = vector<T>(2 * sz, e());
    lz = vector<U>(sz, id());

    for (int i = 0; i < n; i++)
      st[sz + i] = ar[i];
    for (int i = sz - 1; i >= 1; i--)
      pull(i);
  }

  void pull(int p) { st[p] = op(st[p << 1], st[p << 1 | 1]); }
  void all_apply(int p, U f) {
    st[p] = mapping(f, st[p]);
    if (p < sz)
      lz[p] = compose(f, lz[p]);
  }
  void push(int p) {
    all_apply(p << 1, lz[p]);
    all_apply(p << 1 | 1, lz[p]);
    lz[p] = id();
  }
  void set(int p, T x) {
    assert(0 <= p && p < n);
    p += sz;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    st[p] = x;
    for (int i = 1; i <= log; i++)
      pull(p >> i);
  }

  T query(int p) {
    assert(0 <= p && p < n);
    p += sz;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    return st[p];
  }
  T query(int l, int r) { // [l,r)
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return e();
    l += sz;
    r += sz;
    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }

    T lhs = e(), rhs = e();
    while (l < r) {
      if (l & 1)
        lhs = op(lhs, st[l++]);
      if (r & 1)
        rhs = op(st[--r], rhs);
      l >>= 1;
      r >>= 1;
    }
    return op(lhs, rhs);
  }
  void update(int p, U f) {
    assert(0 <= p && p < n);
    p += sz;
    for (int i = log; i >= 1; i--)
      push(p >> i);
    st[p] = mapping(f, st[p]);
    for (int i = 1; i <= log; i++)
      pull(p >> i);
  }
  void update(int l, int r, U f) {
    assert(0 <= l && l <= r && r <= n);
    if (l == r)
      return;
    l += sz;
    r += sz;
    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l)
        push(l >> i);
      if (((r >> i) << i) != r)
        push((r - 1) >> i);
    }

    int l2 = l, r2 = r;
    while (l < r) {
      if (l & 1)
        all_apply(l++, f);
      if (r & 1)
        all_apply(--r, f);
      l >>= 1;
      r >>= 1;
    }
    tie(l, r) = {l2, r2};

    for (int i = 1; i <= log; i++) {
      if (((l >> i) << i) != l)
        pull(l >> i);
      if (((r >> i) << i) != r)
        pull((r - 1) >> i);
    }
  }
};

const int MOD = 998'244'353;
struct T {
  int cost, val, sz;
};

T op(T a, T b) {
  int tot = (a.cost + b.cost < MOD) ? a.cost + b.cost : a.cost + b.cost - MOD;
  int ncost = (ll)a.val * b.val % MOD;
  ncost = (ncost + tot < MOD) ? ncost + tot : ncost + tot - MOD;
  int nval = (a.val + b.val < MOD) ? a.val + b.val : a.val + b.val - MOD;
  int nsz = (a.sz + b.sz < MOD) ? a.sz + b.sz : a.sz + b.sz - MOD;
  return T{ncost, nval, nsz};
}
T e() { return T{0, 0, 0}; }

T mapping(int x, T a) {
  int ncost = a.cost;
  int t1 = (ll)(a.sz - 1) * a.val % MOD * x % MOD;
  int t2 = (ll)a.sz * (a.sz - 1) % MOD * x % MOD * x % MOD;
  t2 = (ll)t2 * 499122177 % MOD;

  ncost = (ncost + t1 < MOD) ? ncost + t1 : ncost + t1 - MOD;
  ncost = (ncost + t2 < MOD) ? ncost + t2 : ncost + t2 - MOD;

  int nval = a.val;
  int t3 = (ll)a.sz * x % MOD;
  nval = (nval + t3 < MOD) ? nval + t3 : nval + t3 - MOD;

  return T{ncost, nval, a.sz};
}

int compose(int f, int g) { return (f + g < MOD) ? f + g : f + g - MOD; }

int id() { return 0; }

using segtree = SegmentTree<T, op, e, int, mapping, compose, id>;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, q;
  cin >> n >> q;
  vector<T> ar;
  for (int i = 0; i < n; i++)
    ar.push_back(T{0, 0, 1});
  segtree st(ar);

  while (q--) {
    int l, r, a;
    cin >> l >> r >> a;
    l--;
    r--;
    st.update(l, r + 1, a);
    T res = st.query(l, r + 1);
    cout << res.cost << "\n";
  }

  return 0;
}
