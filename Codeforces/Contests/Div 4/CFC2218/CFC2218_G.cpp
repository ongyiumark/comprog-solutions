/* https://codeforces.com/contest/2218/problem/G */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

const ll MOD = 676767677;

void testcase() {
  int n, m; cin >> n >> m;
  vi b(n);
  for (int &x : b) cin >> x;

  vi cnt(m);
  for (int i = 0; i < n; i++) cnt[b[i]]++;
  for (int i = 1; i < m; i++) cnt[i] += cnt[i-1];

  ll ans = 1;
  for (int i = 0; i < n; i++) {
    if (b[i] == 0) continue;
    int left = (i-1 >= 0 ? b[i-1] : m);
    int right = (i+1 < n ? b[i+1] : m);
    if (b[i] <= left && b[i] <= right) {
      cout << "0\n";
      return; 
    }
    if (min(left, right) < b[i]-1) ans = ans * (cnt[b[i]-1]-cnt[b[i]-2]) % MOD;
    else ans = ans * cnt[b[i]-1] % MOD;
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