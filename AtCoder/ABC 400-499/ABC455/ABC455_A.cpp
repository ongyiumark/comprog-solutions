#include <cassert>
#include <iostream>
#include <vector>

using namespace std;
typedef int64_t ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int a, b, c;
  cin >> a >> b >> c;
  cout << (((a != b) && (b == c)) ? "Yes\n" : "No\n");
  return 0;
}
