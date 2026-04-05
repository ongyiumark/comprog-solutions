/* https://atcoder.jp/contests/abc452/tasks/abc452_d */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  string s, t; cin >> s >> t;
  int n = s.size(), m = t.size();

  // dp[i][k] = max L such that first k chars of T is found in [L...i] 
  vector<vi> dp(n, vi(m+1, -1));
  dp[0][1] = (t[0]==s[0]) ? 0 : -1;
  for (int i = 1; i < n; i++) {
    dp[i][1] = (t[0]==s[i]) ? i : dp[i-1][1]; 
  }
  for (int i = 1; i < n; i++) {
    for (int k = 2; k <= m; k++) {
      dp[i][k] = (t[k-1]==s[i]) ? dp[i-1][k-1] : dp[i-1][k];
    }
  }
 
  ll ans = 0;
  for (int i = 0; i < n; i++) {
    ans += i-dp[i][m];
  }
  cout << ans << "\n";
  
  return 0;
}