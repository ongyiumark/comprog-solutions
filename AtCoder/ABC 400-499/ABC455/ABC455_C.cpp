#include <algorithm>
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

  int n, k;
  cin >> n >> k;
  vi A(n);
  for (int &x : A)
    cin >> x;

  sort(A.begin(), A.end());
  vll pf(n);
  pf[0] = A[0];
  for (int i = 1; i < n; i++) {
    if (A[i] == A[i - 1])
      pf[i] = pf[i - 1] + A[i];
    else
      pf[i] = A[i];
  }

  vll totals;
  for (int i = 0; i < n - 1; i++) {
    if (A[i] != A[i + 1])
      totals.emplace_back(pf[i]);
  }
  totals.emplace_back(pf[n - 1]);

  sort(totals.begin(), totals.end());
  reverse(totals.begin(), totals.end());

  ll removed = 0;
  for (int i = 0; i < k && i < totals.size(); i++)
    removed += totals[i];

  ll ans = 0;
  for (int i = 0; i < n; i++)
    ans += A[i];

  ans -= removed;
  cout << ans << "\n";
  return 0;
}
