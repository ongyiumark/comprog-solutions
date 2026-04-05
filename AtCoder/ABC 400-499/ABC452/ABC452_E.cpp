/* https://atcoder.jp/contests/abc452/tasks/abc452_e */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

template <typename T, auto M>
struct ModInt {
  using V = conditional_t<sizeof(T) <= 4, uint64_t, __uint128_t>;
  static V inv(V x, V m) { 
    for (V a = exchange(x, 1), b = exchange(m, 0); b; a = exchange(b, a % b)) {
      x = exchange(m, x - (a / b) * m);
    }
    return x >= m ? x + m : x;
  }
  make_unsigned_t<T> x;
  ModInt() : x(0) {}
  ModInt(auto y) : x(y % M) { x >= M ? x += M : x; }
  operator T() const { return x; }
  ModInt operator-() const { return ModInt() -= *this; }
  ModInt operator+(auto rhs) const { return ModInt(*this) += rhs; }
  ModInt operator-(auto rhs) const { return ModInt(*this) -= rhs; }
  ModInt operator*(auto rhs) const { return ModInt(*this) *= rhs; }
  ModInt operator/(auto rhs) const { return ModInt(*this) /= rhs; }
  ModInt operator+=(ModInt rhs) { return (x += rhs.x) >= M ? x -= M : x, *this; }
  ModInt operator-=(ModInt rhs) { return (x -= rhs.x) >= M ? x += M : x, *this; }
  ModInt operator*=(ModInt rhs) { return x = x * V(rhs.x) % M, *this; }
  ModInt operator/=(ModInt rhs) { return x = x * inv(rhs.x, M) % M, *this; }
};

typedef ModInt<ll, 998244353> mint;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  int n, m; cin >> n >> m;
  vll A(n), B(m);
  for (ll &x : A) cin >> x;
  for (ll &x : B) cin >> x;

  mint total_Ai_times_i = 0;
  for (int i = 1; i <= n; i++) {
    total_Ai_times_i += mint(A[i-1])*i;
  }

  vector<mint> pA(n+1);
  for (int i = 1; i <= n; i++) {
    pA[i] = pA[i-1] + A[i-1];
  }

  /*  
  sum of B_j * A_i * (i % j) = sum_j B_j * (sum_i A_i * (i%j)) 
  -- sum_i A_i*(i%j) = sum_i A_i*(i-j*(i//j)) = sum_i A_i*i + j*(sum_i A_i*(i//j))
  ---- sum_i A_i*(i//j) = sum_{k=1}^{n//j} sum_{i=jk}^{min(jk+j-1,n)} A_i
  */
  mint total = 0;
  for (int j = 1; j <= m; j++) {
    mint subtotal = total_Ai_times_i;

    mint subsubtotal = 0;
    for (int k = 1; k <= n/j; k++) {
      subsubtotal += mint(k)*(pA[min(j*(k+1)-1,n)] - pA[j*k-1]);
    }

    subsubtotal *= j;
    subtotal -= subsubtotal;
    
    subtotal *= B[j-1];
    total += subtotal;
  }
  cout << total << "\n";

  return 0;
}