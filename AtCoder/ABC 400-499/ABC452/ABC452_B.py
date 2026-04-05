""" https://atcoder.jp/contests/abc452/tasks/abc452_b """
h, w = [int(x) for x in input().split()]
grid = [
  "#"*w,
  *[f"#{'.'*(w-2)}#" for _ in range(h-2)],
  "#"*w
]
print("\n".join(grid))