/* https://atcoder.jp/contests/abc448/tasks/abc448_b */
#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m; cin >> n >> m;
  vi C(m);
  for (int &c : C) cin >> c;
  vi A(n), B(n);
  for (int i = 0; i < n; i++) {
    cin >> A[i] >> B[i];
    A[i]--;
  }


  int ans = 0;
  for (int i = 0; i < n; i++) {
    ans += min(B[i], C[A[i]]);
    C[A[i]] -= min(B[i], C[A[i]]);
  }
  cout << ans << "\n";
  
  return 0;
}