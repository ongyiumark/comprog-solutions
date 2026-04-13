#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;


template <integral T, T P, T g>
struct NTT {
  using V = conditional_t<sizeof(T)<=4,uint64_t,__uint128_t>;
  
  T mod_pow(T b, T e) {
    T ans = 1;
    while (e) {
      if (e&1) ans = V(ans)*b%P;
      b = V(b)*b%P; e >>= 1;
    }
    return ans;
  }

  vector<T> roots{0,1}, inv_roots{0,1};
  void precompute_roots(int n) {
    if ((int)roots.size() >= n) return;
    int k = __builtin_ctz(roots.size());
    roots.resize(n);
    inv_roots.resize(n);

    for (; (1 << k) < n; k++) {
      T w0 = mod_pow(g, (P-1) >> (k+1));
      T w0_inv = mod_pow(w0, P-2);
      for (int i = 1 << (k-1); i < (1 << k); i++) {
        roots[i<<1] = roots[i];
        roots[i<<1|1] = V(roots[i])*w0%P;

        inv_roots[i<<1] = inv_roots[i];
        inv_roots[i<<1|1] = V(inv_roots[i])*w0_inv%P;
      }
    }
  }

  void bit_reverse(vector<T> &a) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
      int bit = n >> 1;
      for (; j & bit; bit >>= 1) j ^= bit;
      j |= bit;
      if (i < j) swap(a[i], a[j]); 
    }
  }

  void ntt(vector<T> &A, bool invert) { 
    int n = A.size(); // must be power of 2 <= 2**k
    precompute_roots(n);
    bit_reverse(A);

    
    for (int cn = 1; cn < n; cn <<= 1) {
      for (int i = 0; i < n; i += (cn<<1)) {
        for (int j = 0; j < cn; j++) {
          T w = invert ? inv_roots[cn+j] : roots[cn+j];
          T u = A[i+j], v = V(A[i+j+cn])*w%P;
          A[i+j] = u+v < P ? u+v : u+v-P;
          A[i+j+cn] = u-v >= 0 ? u-v : u-v+P;
        }
      }
    }
    if (invert) {
      T n_inv = mod_pow(n, P-2);
      for (T &x : A) x = V(x)*n_inv%P;
    }
  }
  
  vector<T> ntt_convolve(const vector<T> &p1, const vector<T> &p2) {
    int deg = p1.size() + p2.size() - 1;
    int n = bit_ceil<size_t>(deg);
    vector<T> A(p1), B(p2);
    A.resize(n); B.resize(n);
    
    ntt(A, false); ntt(B, false);
    for (int k = 0; k < n; k++) A[k] = V(A[k])*B[k]%P;
    ntt(A, true);
    
    A.resize(deg);
    return A;
  }
};

const int P = 998244353, g = 3;
NTT<int, P, g> ntt;
vi convolve(const vi &p1, const vi &p2) {
  return ntt.ntt_convolve(p1, p2);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m; cin >> n >> m;

  vi A00(10, 1), A10(11), b0(n+1, 1), b1(n+1);
  A10[10] = 1;

  if (m == 1) {
    cout << 1 << "\n";
    return 0;
  }

  auto mult = [&](const vi &a, const vi &b) {
    auto ret = convolve(a, b);
    if (ret.size() > n) ret.resize(n+1);
    return ret;
  };

  auto add = [&](vi a, const vi &b) {
    if (a.size() < b.size()) a.resize(b.size());
    for (int i = 0; i < b.size(); i++) 
      a[i] = a[i]+b[i] < P ? a[i]+b[i] : a[i]+b[i]-P;
    return a;
  };

  m -= 2;
  while (m) {
    if (m&1) {
      b1 = add(mult(A10, b0), b1);
      b0 = mult(A00, b0);
    }
    A10 = add(mult(A10, A00), A10);
    A00 = mult(A00, A00);
    m >>= 1;
  }
  int ans = 0;
  for (int &x : b0) ans = ans+x<P ? ans+x : ans+x-P;
  for (int &x : b1) ans = ans+x<P ? ans+x : ans+x-P;
  cout << ans << "\n"; 
  
  return 0;
}