/* https://codeforces.com/contest/2218/problem/D */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

vi linear_sieve(int n) {
  vi lp(n+1), pr;
  for (int i = 2; i <= n; i++) {
    if (lp[i] == 0) { lp[i] = i; pr.emplace_back(i); }
    for (int j = 0; j < pr.size() && i*pr[j] <= n; j++) {
      lp[i*pr[j]] = pr[j];
      if (i%pr[j] == 0) break;
    }
  }
  return pr;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  const int max_n = 2e5;
  vi primes = linear_sieve(max_n);
  
  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    for (int i = 0; i < n; i++) {
      cout << (ll)primes[i]*primes[i+1] << " \n"[i+1==n];
    }
  }
  
  return 0;
}