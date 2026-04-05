"""https://codeforces.com/contest/2218/problem/C"""
t = int(input())
for _ in range(t):
  n = int(input())
  ans = [0]*(3*n)
  for i in range(n):
    ans[3*i] = 3*n-2*i
    ans[3*i+1] = 3*n-2*i-1
    ans[3*i+2] = i+1
  print(" ".join([str(x) for x in ans]))