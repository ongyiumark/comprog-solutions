#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  int n, m; cin >> n >> m;
  vi F(n), cnt(m);
  for (int &f : F) {
    cin >> f;
    cnt[f-1]++;
  }

  bool all_diff = true;
  sort(F.begin(), F.end());
  for (int i = 1; i < n; i++) 
    if (F[i] == F[i-1])
      all_diff = false;
  
  bool at_least_one = true;
  for (int i = 0; i < m; i++) 
    if (cnt[i] == 0)
      at_least_one = false;
  
  cout << (all_diff ? "Yes\n" : "No\n");
  cout << (at_least_one ? "Yes\n" : "No\n");
  return 0;
}