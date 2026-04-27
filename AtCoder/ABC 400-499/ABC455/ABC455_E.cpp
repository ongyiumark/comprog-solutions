#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace std;
typedef int64_t ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

template <typename T> ll count_dups(vector<T> ar) {
  int n = ar.size();
  map<T, int> mp;
  ll ans = 0;
  for (int i = 0; i < n; i++) {
    ans += mp[ar[i]];
    mp[ar[i]]++;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n;
  cin >> n;
  string s;
  cin >> s;

  vi a(n + 1), b(n + 1), c(n + 1), ab(n + 1), bc(n + 1), ac(n + 1);
  vector<pair<int, int>> abc(n + 1);
  for (int i = 1; i <= n; i++) {
    a[i] = a[i - 1] + (s[i - 1] == 'A');
    b[i] = b[i - 1] + (s[i - 1] == 'B');
    c[i] = c[i - 1] + (s[i - 1] == 'C');

    ab[i] = a[i] - b[i];
    bc[i] = b[i] - c[i];
    ac[i] = a[i] - c[i];
    abc[i] = {ab[i], ac[i]};
  }

  ll ans = count_dups<int>(ab) + count_dups<int>(bc) + count_dups<int>(ac) -
           2 * count_dups<pair<int, int>>(abc);
  cout << (ll)n * (n + 1) / 2 - ans << "\n";

  return 0;
}
