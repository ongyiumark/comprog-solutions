/* https://atcoder.jp/contests/ABC449/tasks/abc449_f */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int H, W, h, w, N;
  cin >> H >> W >> h >> w >> N;
  vi R(N), C(N);
  for (int i = 0; i < N; i++) cin >> R[i] >> C[i];
  
  vector<tuple<int,int,int>> pR; // row, enter/exit, index
  for (int i = 0; i < N; i++) {
    // R[i]-h+1 to R[i], C[i]-w+1 to C[i]
    pR.push_back({max(R[i]-h+1, 1), 0, i});
    pR.push_back({min(R[i]+1, H-h+2), 1, i});
  }
  sort(pR.begin(), pR.end());

  multiset<pair<int,int>> segments;
  ll total = 0, black_count = 0;
  int idx = 0, curr_row = 0;
  while (idx < (int)pR.size()) {
    // update black_count here
    black_count = 0;
    int last_r = -1;
    for (auto it = segments.begin(); it != segments.end(); it++) {
      if (it->first >= it->second) continue;
      if (last_r > it->second) continue; // fully contained
      if (last_r > it->first) black_count += (it->second-last_r); // overlap
      else black_count += (it->second - it->first); 
      last_r = it->second;
    }
 
    total += black_count*(get<0>(pR[idx])-curr_row);
    curr_row = get<0>(pR[idx]);
    while (idx < 2*N && get<0>(pR[idx]) == curr_row) {
      int j = get<2>(pR[idx]);
      pair<int,int> segment = {max(C[j]-w+1, 1), min(C[j]+1,W-w+2)}; // [a,b)
      if (get<1>(pR[idx]) == 0) segments.insert(segment);
      else segments.erase(segments.find(segment));
      idx++;
    }
  }
  cout << (ll)(H-h+1)*(W-w+1)-total << "\n";

  
  return 0;
}