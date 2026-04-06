"""https://atcoder.jp/contests/abc449/tasks/abc449_b"""
h, w, q = [int(x) for x in input().split()]
for _ in range(q):
  t, v = [int(x) for x in input().split()]
  if t == 1:
    print(v*w)
    h -= v
  else:
    print(v*h)
    w -= v
