/* https://atcoder.jp/contests/abc452/tasks/abc452_c */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n; cin >> n;
  vi A(n), B(n);
  for (int i = 0; i < n; i++) cin >> A[i] >> B[i];
  int m; cin >> m;
  vector<string> S(m);
  for (string &s : S) cin >> s;

  vector<vector<string>> S_index(11, vector<string>());
  for (int j = 0; j < m; j++) {
    S_index[S[j].size()].emplace_back(S[j]);
  }

  vector<vector<string>> choices(n, vector<string>());
  for (int i = 0; i < n; i++) {
    choices[i] = S_index[A[i]];
    // sort by B[i]-1
    sort(choices[i].begin(), choices[i].end(), [&](const string &lhs, const string &rhs) {
      return lhs[B[i]-1] < rhs[B[i]-1];
    });
  }

  for (int j = 0; j < m; j++) {
    string spine = S[j];
    if ((int)spine.size() != n) {
      cout << "No\n";
      continue;
    }
    // choose rib[i] such that len(rib[i]) == A[i] and rib[i][B[i]-1] == spine[i]
    // binary search 
    bool valid = true;
    for (int i = 0; i < n ; i++) {
      int lo = 0, hi = (int)choices[i].size()-1;
      bool found = false;
      while (lo <= hi) {
        int mid = hi - (hi-lo)/2;
        if (choices[i][mid][B[i]-1] > spine[i]) hi = mid-1;
        else if (choices[i][mid][B[i]-1] < spine[i]) lo = mid+1;
        else {
          found = true;
          break;
        }
      }
      valid &= found;
    }
    cout << (valid ? "Yes\n" : "No\n");
  }
  
  return 0;
}