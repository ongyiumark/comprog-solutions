/* https://atcoder.jp/contests/abc448/tasks/abc448_a */
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
  
  int n, x; cin >> n >> x;
  for (int i = 0; i < n; i++) {
    int a; cin >> a;
    if (a < x) x = a, cout << 1 << "\n";
    else cout << 0 << "\n";
  }

  return 0;
}