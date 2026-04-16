/* https://atcoder.jp/contests/abc448/tasks/abc448_e */
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

int moddot(const vi &a, const vi &b, int mod) {
  assert(a.size() == b.size());
  int n = a.size();
  int ans = 0;
  for (int i = 0; i < n; i++) {
    ans += (ll)a[i]*b[i]%mod;
    if (ans >= mod) ans -= mod;
  }
  return ans;
}

void matpow(vi &a0, vi& a1, vi& b, int e, int mod) {
  while (e) {
    if (e&1) {
      int b0 = moddot(a0, b, mod);
      int b1 = moddot(a1, b, mod);
      b[0] = b0;
      b[1] = b1;
    }
    e >>= 1;
    int a00 = moddot(a0, vi{a0[0], a1[0]}, mod);
    int a01 = moddot(a0, vi{a0[1], a1[1]}, mod);
    a0[0] = a00;
    a0[1] = a01;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int k, m; cin >> k >> m;
  const int p = 10007;
  int mod = m*p;

  vi b{0, 1};
  for (int i = 0; i < k; i++) {
    int c, l; cin >> c >> l;
    vi a0{10, c}, a1{0, 1};
    matpow(a0, a1, b, l, mod);
  }
  cout << b[0]/m << "\n";
  
  
  return 0;
}