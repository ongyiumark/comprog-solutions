#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

template <typename T, auto op, auto e>
requires is_convertible_v<decltype(op), function<T(T,T)>>
&& is_convertible_v<decltype(e), function<T()>>
struct PersSegmentTree {
  struct Node { int lc, rc; T val; };

  vector<Node> nodes; int n;
  vi roots;
  PersSegmentTree(const vector<T> &ar) : n(ar.size()) {
    nodes.reserve(n*20);
    roots.reserve(n);
    nodes.push_back({0,0,0});
    roots.push_back(build(ar, 0, n));
  }

  inline int new_node(int lc, int rc, T val) {
    nodes.push_back({lc, rc, val});
    return (int)nodes.size()-1;
  }

  int build(const vector<T> &ar, int l, int r) {
    if (l+1 == r) return new_node(0, 0, ar[l]);
    
    int k = (l+r) >> 1;
    int lc = build(ar, l, k), rc = build(ar, k, r);
    T val = op(nodes[lc].val, nodes[rc].val);
    return new_node(lc, rc, val);
  }

  int update(int root, int p, T val) {
    roots.push_back(update(root, p, val, 0, n));
    return roots.back();
  }
  int update(int idx, int p, T val, int l, int r) {
    if (l+1 == r) return new_node(0, 0, val);

    int k = (l+r) >> 1;
    int lc = nodes[idx].lc, rc = nodes[idx].rc;
    if (p < k) lc = update(lc, p, val, l, k);
    else rc = update(rc, p, val, k, r);

    T nval = op(nodes[lc].val, nodes[rc].val);
    return new_node(lc, rc, nval);
  }

  T query(int root, int ql, int qr) {
    return query(root, ql, qr, 0, n);
  }
  T query(int idx, int ql, int qr, int l, int r) {
    if (!idx || r <= ql || qr <= l) return e();
    if (ql <= l && r <= qr) return nodes[idx].val;

    int k = (l+r) >> 1;
    T lval = query(nodes[idx].lc, ql, qr, l, k);
    T rval = query(nodes[idx].rc, ql, qr, k, r); 
    return op(lval, rval);
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  int n, m, q; cin >> n >> m >> q;
  vll ar(m, 0);

  auto op = [](ll a, ll b) { return a+b; };
  auto e = []() { return 0LL; };
  PersSegmentTree<ll, op, e> pers_segtree(ar);
  vi roots(n, pers_segtree.roots[0]);

  while (q--) {
    int t; cin >> t;
    if (t == 1) {
      int x, y; cin >> x >> y;
      x--; y--;
      roots[x] = roots[y];
    }
    else if (t == 2) {
      int x, y; ll z;
      cin >> x >> y >> z;
      x--; y--;
      roots[x] = pers_segtree.update(roots[x], y, z);
    }
    else {
      int x, l, r; cin >> x >> l >> r;
      x--; l--; r--;
      cout << pers_segtree.query(roots[x], l, r+1) << "\n";
    }
  }

  return 0;
}