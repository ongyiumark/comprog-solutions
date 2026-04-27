#include <iostream>
#include <vector>
#include <cassert>

using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

string process(string &s) {
  int n = s.size();
  vector<int> grp(n);
  int cnt = 1;
  
  int i = 1;
  while (i < n) {
    if (s[i] == s[i-1] && s[i] == 'x') {
      int l = i-1, r = i;
      grp[l] = grp[r] = cnt;
      while (l-1 >= 0 && r+1 < n && s[l-1] == '(' && s[r+1] == ')') {
        l--;
        r++;
        grp[l] = grp[r] = cnt;
      } 
      cnt++;
      i++;
    }
    i++;
  }

  string res = "";
  i = 0;
  while (i < n) {
    if (grp[i] == 0) res += s[i++];
    else {
      int cur_grp = grp[i];
      while (i < n && grp[i] == cur_grp) i++;
      res += "xx";
    }
  }
  return res;
}


void testcase() {
  string a, b; cin >> a >> b;
  a = process(a);
  b = process(b);

  cout << (a==b ? "Yes\n" : "No\n");
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int t; cin >> t;
  while (t--) testcase();
  
  return 0;
}