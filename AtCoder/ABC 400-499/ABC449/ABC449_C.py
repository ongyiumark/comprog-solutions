"""https://atcoder.jp/contests/abc449/tasks/abc449_c"""
n, l, r = [int(x) for x in input().split()]
s = input()

pS = [[0]*(len(s)+1) for _ in range(26)]
for i in range(26):
  for j, ch in enumerate(s):
    pS[i][j+1] = pS[i][j] + int(ch == chr(i + ord("a")))

total = 0
for i, ch in enumerate(s):
  # i+L to i+R
  total += pS[ord(ch)-ord("a")][min(i+r+1, n)]-pS[ord(ch)-ord("a")][min(i+l, n)]

print(total)
