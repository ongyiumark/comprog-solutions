/* https://atcoder.jp/contests/abc451/tasks/abc451_d */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;

const int MAX_EXP = 30;
vi pow2(MAX_EXP);
vector<string> pow2str(MAX_EXP);
vi concats;

void rec_backtrack(string &s, int idx) {
  if (s.size() > 9) return;
  if (s.size() > 0) concats.emplace_back(stoi(s));

  for (int i = 0; i < MAX_EXP; i++) {
    s = s + pow2str[i];
    if (s.size() <= 9) rec_backtrack(s, i);
    s = s.substr(0, s.size()-pow2str[i].size());
  }
}

void initialize() {
  pow2[0] = 1;
  for (int i = 1; i < MAX_EXP; i++) {
    pow2[i] = pow2[i-1]*2;
  }
  for (int i = 0; i < MAX_EXP; i++) {
    pow2str[i] = to_string(pow2[i]);
  }

  string s = "";
  rec_backtrack(s, 0);
  sort(concats.begin(), concats.end());
  concats.erase(unique(concats.begin(), concats.end()), concats.end());
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  initialize();
  int n; cin >> n;
  cout << concats[n-1] << "\n";

  return 0;
}
