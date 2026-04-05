""" https://atcoder.jp/contests/abc452/tasks/abc452_a """
festivals = [(1,7), (3,3), (5,5), (7,7), (9, 9)]
m, d = [int(x) for x in input().split()]
if (m, d) in festivals:
  print("Yes")
else:
  print("No")