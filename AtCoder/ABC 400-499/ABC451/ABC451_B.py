"""https://atcoder.jp/contests/abc451/tasks/abc451_b"""
n, m = [int(x) for x in input().split()]
this_term = [0]*m
next_term = [0]*m

for i in range(n):
  a, b = [int(x)-1 for x in input().split()]
  this_term[a] += 1
  next_term[b] += 1

for i in range(m):
  print(next_term[i]-this_term[i])