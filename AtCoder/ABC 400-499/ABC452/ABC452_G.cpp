/* https://atcoder.jp/contests/abc452/tasks/abc452_g */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<ll> vll;

struct SuffixArray {
  vi p, c, lcp;
  string t;
  SuffixArray(string s) : t(s) {
    s = s+"$";
    sort_cyclic(s);
    p.erase(p.begin()); // remove $
    build_lcp();
  };

  void sort_cyclic(const string &s, const int alphabet = 256) {
    int n = s.size();
    p.assign(n, 0); c.assign(n, 0); 
    vi cnt(max(alphabet, n));
    for (int i = 0; i < n; i++) cnt[s[i]]++; // sort size 1
    for (int i = 1; i < alphabet; i++) cnt[i] += cnt[i-1];
    for (int i = 0; i < n; i++) p[--cnt[s[i]]] = i;
    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; i++) {
      if (s[p[i]] != s[p[i-1]]) classes++;
      c[p[i]] = classes-1;
    }

    vi pn(n), cn(n);
    for (int h = 0; (1 << h) < n; h++) { // sort size 2**h
      for (int i = 0; i < n; i++) {
        pn[i] = p[i] - (1 << h); // sort by 2nd half
        if (pn[i] < 0) pn[i] += n;
      }
      fill(cnt.begin(), cnt.begin() + classes, 0);
      for (int i = 0; i < n; i++) cnt[c[pn[i]]]++;
      for (int i = 1; i < classes; i++) cnt[i] += cnt[i-1];
      for (int i = n-1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
      cn[p[0]] = 0; classes = 1;
      for (int i = 1; i < n; i++) {
        pair<int,int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
        pair<int,int> prev = {c[p[i-1]], c[(p[i-1] + (1 << h)) % n]};
        if (cur != prev) classes++;
        cn[p[i]] = classes-1;
      }
      swap(c, cn);
    }
  }

  void build_lcp() {
    int n = t.size();
    vi rank(n, 0);
    for (int i = 0; i < n; i++) rank[p[i]] = i;

    lcp.assign(n, 0);
    int k = 0;
    for (int i = 0; i < n; i++) {
      if (rank[i] == 0) { k = 0; continue; }
      int j = p[rank[i]-1];
      while (i+k < n && j+k < n && t[i+k] == t[j+k]) k++;
      lcp[rank[i]] = k;
      if (k) k--;
    }
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n; cin >> n;
  vi A(n);
  for (int &x : A) cin >> x;
  string s = "";
  for (int i = 0, cnt = 1; i < n; i++) {
    if (i == n-1 || A[i] != A[i+1]) {
      if (cnt < A[i]) s.push_back('0');
      else if (cnt > A[i]) {
        s.push_back('0'+A[i]);
        s.push_back('0');
        s.push_back('0'+A[i]);
      }
      else s.push_back('0'+A[i]);
      cnt=1;
    }
    else cnt++;
  }

  int m = s.size();
  vi d_to_zero(m);
  d_to_zero[m-1] = (s[m-1] == '0') ? 0 : 1;
  for (int i = m-2; i >= 0; i--) {
    if (s[i] == '0') d_to_zero[i] = 0;
    else d_to_zero[i] = d_to_zero[i+1]+1;
  }

  ll ans = 0;
  SuffixArray sa(s);
  for (int i = 0; i < m; i++) {
    ans += max(d_to_zero[sa.p[i]]-sa.lcp[i], 0);
  }
  cout << ans << "\n";

  
  return 0;
}