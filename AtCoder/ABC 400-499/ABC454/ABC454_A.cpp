#include <cassert>
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

  int l, r;
  cin >> l >> r;
  cout << r - l + 1 << "\n";

  return 0;
}
