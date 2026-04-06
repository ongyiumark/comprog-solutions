"""https://codeforces.com/contest/2218/problem/B"""
t = int(input())
for _ in range(t):
  a = [int(x) for x in input().split()]
  max_a = max(a)
  print(2*max_a-sum(a))