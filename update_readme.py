import os
import re
import typing
import subprocess

from utils import Markdown

import pandas as pd

ATCODER_CONTESTS_CACHE = "contest_data/atcoder_contests.csv"
ATCODER_CONTEST_DETAILS_CACHE = "contest_data/atcoder_contest_details.csv"
ATCODER_PATH = "AtCoder/"
atcoder_contests = None
atcoder_contest_details = None


def is_comment(text: str, lang: str) -> bool:
  if lang == "cpp":
    pattern = r"(\".*?\"|\'.*?\')|(/\*.*?\*/|//.*?)"
    return re.match(pattern, text).group(2)

  if lang == "py":
    pattern = r"(\"\"\".*?\"\"\"|\'\'\'.*?\'\'\'|#.*?)"
    return re.match(pattern, text).group(0)

  return NotImplementedError(f"Language {lang} is not supported.")

def split_extension(text: str) -> tuple[str, str]:
  last_dot = text.rfind(".")
  if last_dot == -1:
    return (text, None)
  
  return (text[:last_dot], text[last_dot+1:])

def get_link(path: str) -> str:
  with open(path, "r") as f:
    # grab comment on first line
    first_line = f.readline()

  basename, ext = split_extension(os.path.basename(path))
  if not is_comment(first_line, ext):
    return None

  prefixes = ["\"\"\"", "\'\'\'", "/*", "//", "#"]
  suffixes = ["\"\"\"", "\'\'\'", "*/"]
  first_line = first_line.strip()
  for prefix in prefixes:
    first_line = first_line.removeprefix(prefix)
  for suffix in suffixes:
    first_line = first_line.removesuffix(suffix)
  href = first_line.strip()
  
  return href

def dir_to_collapsed_section(directory: str, start_level: int, file_parser: typing.Callable[[str], str]):
  def traverse(path: str, level: str) -> str:
    files_and_dir = sorted(os.listdir(path))
    
    lines = []
    for fd in files_and_dir:
      if os.path.isdir(os.path.join(path, fd)):
        result = traverse(os.path.join(path, fd)+"/", level+1)
        lines.extend(result.split("\n"))
      else:
        lines.append(f"- {file_parser(os.path.join(path, fd))}")
    
    if level < start_level:
      return "\n".join(lines)

    immediate_dir = path.rsplit("/", 2)[-2]

    if immediate_dir in atcoder_contests["contest_name"]:
      immediate_dir = immediate_dir + " - " + atcoder_contests["contest_name"][immediate_dir]
    section = Markdown.collapsed_section(
      summary=immediate_dir,
      lines=lines
    )
    return section
  
  return traverse(directory, 0)

def atcoder_parser(path: str) -> str:
  filename = os.path.basename(path)
  basename, ext = split_extension(filename)
  contest_code, task_code = basename.split("_")

  task_name = atcoder_contest_details["task_name"][(contest_code, task_code)]
  task_link = f"https://atcoder.jp{atcoder_contest_details["task_link"][(contest_code, task_code)]}"

  return filename + " - " + Markdown.link(text=task_name, href=task_link)

def main():
  subprocess.run(["python3", "update_links.py"])
  readme = Markdown("README.md")
  readme.add_lines(
    [
      Markdown.header("Competitive Programming Solutions", level=1),
      "This is an archive of the problems that I've solved."
    ]
  )

  global atcoder_contests
  global atcoder_contest_details
  atcoder_contests = pd.read_csv(ATCODER_CONTESTS_CACHE).set_index("contest_code").to_dict()
  atcoder_contest_details = pd.read_csv(ATCODER_CONTEST_DETAILS_CACHE).set_index(["contest_code", "task_code"]).to_dict()

  atcoder_section = dir_to_collapsed_section(
    ATCODER_PATH, 
    start_level=0,
    file_parser=atcoder_parser
  )
  readme.add_line(atcoder_section)
  readme.write()


if __name__ == "__main__":
  main()