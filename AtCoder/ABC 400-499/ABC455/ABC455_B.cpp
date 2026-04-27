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

  int h, w;
  cin >> h >> w;
  vector<string> grid(h);
  for (string &s : grid)
    cin >> s;

  auto check = [&](int h1, int h2, int w1, int w2) {
    bool valid = true;
    for (int i = h1; i <= h2; i++) {
      for (int j = w1; j <= w2; j++) {
        valid &= (grid[i - 1][j - 1] == grid[h1 + h2 - i - 1][w1 + w2 - j - 1]);
      }
    }
    return valid;
  };

  int ans = 0;
  for (int h1 = 1; h1 <= h; h1++) {
    for (int h2 = h1; h2 <= h; h2++) {
      for (int w1 = 1; w1 <= w; w1++) {
        for (int w2 = w1; w2 <= w; w2++) {
          ans += check(h1, h2, w1, w2);
        }
      }
    }
  }
  cout << ans << "\n";

  return 0;
}
