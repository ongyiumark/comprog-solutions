/* https://atcoder.jp/contests/abc451/tasks/abc451_g */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;


struct Edge {
  int u, v; ll w;
  Edge(int u, int v) : u(u), v(v), w(0) {}
  Edge(int u, int v, ll w) : u(u), v(v), w(w) {}
  Edge reverse() const { return Edge(v, u, w); }
};

struct Graph {
  vector<Edge> edges;
  vector<vi> adj;
  Graph(int n) : adj(n) {}
  Edge& operator()(int e) { return edges[e]; }
  vi& operator[](int u) { return adj[u]; }
  int size() { return adj.size(); }
  void append(const Edge& e) {
    adj[e.u].emplace_back(edges.size());
    edges.emplace_back(e);
  }
  void add_edge(const Edge& e, bool directed = false) {
    append(e);
    if (!directed) append(e.reverse());
  }
};

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

struct Node {
  vector<Node*> child;
  int cnt;
  Node() : child(2, nullptr), cnt(0) {}
  ~Node() {
    for (Node* node : child)
      if (node) delete node; 
  }
};

struct Trie {
  Node* root;
  Trie() : root(new Node()) {}
  ~Trie() { delete root; }
  void insert(int num) {
    Node* curr = root;
    for (int l = 30; l >= 0; l--) {
      int bit = (num >> l) & 1;
      if (!curr->child[bit]) {
        curr->child[bit] = new Node();
      }
      curr = curr->child[bit];
      curr->cnt++;
    }
  }
};

ll count_pairs(Trie &tr, int num, int k) {
  Node* curr = tr.root;
  ll cnt = 0;
  for (int l = 30; l >= 0; l--) {
    if (!curr) break;
    int bit_num = (num >> l) & 1;
    int bit_k = (k >> l) & 1;
    if (bit_k == 1) {
      // choosing the same bit results in 0
      if (curr->child[bit_num]) {
        cnt += (ll)curr->child[bit_num]->cnt;
      }

      curr = curr->child[1-bit_num]; // move to opposite bit
    }
    else curr = curr->child[bit_num]; // force to choose path that results in 0
  }
  if (curr) cnt += (ll)curr->cnt;
  return cnt;
} 

vi xor_basis(const vi& span, int logn=30) {
  vi basis(logn+1, 0);
  
  function<void(int)> add_base = [&](int x) {
    for (int l = 30; l >= 0; l--) {
      if (x & (1 << l)) {
        if (basis[l] == 0) {
          basis[l] = x;
          return;
        }
        x ^= basis[l];
      }
    }
  };

  for (int x : span) {
    add_base(x);
  }
  return basis;
}

ll solve(vi d, ll k) {
  // count the number of pairs (i,j) such that d[i]^d[j] <= k
  Trie tr = Trie();
  ll ans = 0;
  for (int num : d) {
    ans += count_pairs(tr, num, k);
    tr.insert(num);
  }
  return ans;
}

void testcase() {
  int n, m, k;
  cin >> n >> m >> k;

  Graph g(n);
  for (int j = 0; j < m; j++) {
    int u, v, w; cin >> u >> v >> w;
    u--; v--;
    g.add_edge(Edge(u, v, w));
  }

  // find spanning tree
  UnionFind uf(n);
  vector<Edge> in_tree, not_in_tree;
  for (int e = 0; e < g.edges.size(); e += 2) { // +2 because graph is undirected
    if (uf.is_same(g(e).u, g(e).v)) not_in_tree.emplace_back(g(e));
    else {
      uf.unite(g(e).u, g(e).v);
      in_tree.emplace_back(g(e));
    }
  }

  Graph tree(n);
  for (Edge &e : in_tree) tree.add_edge(e);

  // root tree at 0 and calculate dist (0, v) for all vertices, v.
  vector<int> d(n);
  function<void(int,int)> dfs = [&](int u, int p) {
    for (int e : tree[u]) {
      int v = tree(e).v;
      if (v == p) continue;
      d[v] = d[u]^tree(e).w;
      dfs(v, u);
    }
  };
  dfs(0, -1);

  vi span; // span to get xor basis
  for (Edge &e : not_in_tree) {
    span.emplace_back(d[e.u]^d[e.v]^e.w); // traverse 0->u->v->0
  }
  vi basis = xor_basis(span);

  // minimize distances with basis vector
  for (int i = 0; i < n; i++) {
    for (int l = 30; l >= 0; l--) {
      if (d[i] & (1 << l)) {
        d[i] ^= basis[l];
      }
    } 
  }

  ll ans = solve(d, k);
  cout << ans << "\n";
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int t; cin >> t;
  while (t--) {
    testcase();
  }
  
  return 0;
}