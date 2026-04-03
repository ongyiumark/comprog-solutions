"""https://atcoder.jp/contests/abc451/tasks/abc451_c"""
from heapq import heappush, heappop

q = int(input())
trees = []

for _ in range(q):
  qt, h = [int(x) for x in input().split()]
  if qt == 1:
    heappush(trees, h)
  else:
    while len(trees) > 0 and trees[0] <= h:
      heappop(trees)
  print(len(trees))