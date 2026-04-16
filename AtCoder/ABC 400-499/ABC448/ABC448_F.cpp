/* https://atcoder.jp/contests/abc448/tasks/abc448_f */
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cmath>

using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

struct Point {
  int x, y, i;
  Point() : x(0), y(0), i(-1) {}
  Point(int x, int y, int i) : x(x), y(y), i(i) {}
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n; cin >> n;
  int max_xy = 0;
  
  vector<Point> locs(n);
  for (int i = 0; i < n; i++) {
    int x, y; cin >> x >> y;
    locs[i] = Point{x,y,i};

    max_xy = max(max_xy, max(x, y));
  }
  int sqrt_n = (int)sqrt(n);
  int width = max_xy/sqrt_n;
  int sz = max_xy/width + 1;

  vector<vector<Point>> mos(sz);
  for (int i = 0; i < n; i++) {
    auto &[x,y,ii] = locs[i];
    int k = x/width;
    mos[k].emplace_back(locs[i]);
  }
  for (int k = 0; k < sz; k++) {
    if (k&1) sort(mos[k].begin(), mos[k].end(), [](const Point &lhs, const Point &rhs) {
      return lhs.y < rhs.y;
    });
    else sort(mos[k].begin(), mos[k].end(), [](const Point &lhs, const Point &rhs) {
      return lhs.y > rhs.y;
    });
  }

  vector<int> ans;
  for (int k = 0; k < sz; k++) {
    for (Point &p : mos[k]) ans.emplace_back(p.i);
  }
  int st = 0;
  while (st < n && ans[st] != 0) st++;
  for (int i = 0; i < n; i++) {
    cout << ans[(st+i)%n]+1 << " \n"[i+1==n];
  }
  
  return 0;
}