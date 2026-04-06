/* https://codeforces.com/contest/2218/my */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

void testcase() {
  int x, y; cin >> x >> y;
  if (x > y) {
    cout << "NO\n";
    return;
  } 
  if (x == 0 && (x+y)%2 == 0) {
    cout << "NO\n";
    return;
  }

  cout << "YES\n";
  int num_even = ((x+y)%2 == 0);
  for (int i = 2, p = 1; i <= x+y; i++) {
    if (num_even++ < x) {
      cout << p << " " << i << "\n";
      cout << i << " " << i+1 << "\n";
      p = ++i;
    }
    else {
      cout << p << " " << i << "\n";
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int t; cin >> t;
  while(t--) {
    testcase();
  }
  
  return 0;
}