/* https://codeforces.com/contest/2218/problem/E */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;


void testcase() {
  int n; cin >> n;
  vll a(n);
  for (ll &x : a) cin >> x;

  // max a_i^a_j
  ll ans = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      ans = max(ans, a[i]^a[j]);
    }
  }
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