/* https://atcoder.jp/contests/abc451/tasks/abc451_e */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;


int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int n; cin >> n;
  vector<vi> dist_mat(n, vi(n, 0));
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      int d; cin >> d;
      dist_mat[i][j] = dist_mat[j][i] = d;
    }
  }

  bool valid = true;
  vector<int> par(n, -1);
  // assume rooted at 0
  for (int i = 1; i < n; i++) {
    int best_dist = numeric_limits<int>::max();
    int best_j = -1;
    for (int j = 0; j < n; j++) {
      if (i == j) continue;
      if (dist_mat[0][j] + dist_mat[j][i] == dist_mat[0][i]) {
        if (dist_mat[j][i] < best_dist) {
          best_dist = dist_mat[j][i];
          best_j = j;
        }
      }
    } 
    if (best_j == -1) valid = false;
    else par[i] = best_j;
  }

  vector<vi> adj_list(n, vi());
  vector<vi> actual_dist(n, vi(n));
  for (int i = 1; i < n; i++) {
    adj_list[par[i]].emplace_back(i);
    adj_list[i].emplace_back(par[i]);
    actual_dist[par[i]][i] = actual_dist[i][par[i]] = dist_mat[par[i]][i];
  }
  

  function<void(int, int, int)> dfs = [&](int u, int p, int s) {
    for (int &v : adj_list[u]) {
      if (v == p) continue;
      actual_dist[s][v] = actual_dist[s][u] + actual_dist[u][v]; 
      dfs(v, u, s);
    }
  };
  for (int i = 0; i < n; i++)
    dfs(i, -1, i);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      valid &= (actual_dist[i][j] == dist_mat[i][j]);
    }
  }
  cout << (valid ? "Yes\n" : "No\n" );
  
  
  return 0;
}