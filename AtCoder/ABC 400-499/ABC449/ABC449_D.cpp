/* https://atcoder.jp/contests/abc449/tasks/abc449_d */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int l, r, d, u; cin >> l >> r >> d >> u;

  function<ll(int, int, int, int)> count_overlap = [&](int l1, int r1, int l2, int r2) {
    // count number of cells in both [l1,r1] and [l2,r2]
    if (r1 < l2 || r2 < l1 || r1 < l1 || r2 < l2) return 0LL;
    return (ll)min(r1,r2)-max(l1,l2)+1;
  };

  function<ll(int, int, int, int)> count_even_overlap = [&](int l1, int r1, int l2, int r2) {
    // count number of cells in both [l1,r1] and [l2,r2]
    if (r1 < l2 || r2 < l1 || r1 < l1 || r2 < l2) return 0LL;
    ll length = min(r1,r2)-max(l1,l2)+1;
    ll result = length/2;
    if (length % 2 == 1) result += (min(r1,r2)%2 == 0);
    return result;
  };


  const int MAXN = 1e6;
  ll ans = 0;
  for (int x = l; x <= r; x++) {
    ll subtotal = 0;
    if (x % 2 == 0) subtotal += count_overlap(d,u,-abs(x),abs(x)); // count [-x,x] and [d,u]
    // count even in [x+1, inf) and [d, u]

    subtotal += count_even_overlap(d,u,abs(x)+1,MAXN);
    subtotal += count_even_overlap(d,u,-MAXN,-abs(x)-1);

    ans += subtotal;
  }
  cout << ans << "\n";
  
  return 0;
}