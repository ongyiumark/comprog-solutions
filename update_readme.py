import os
import re
import typing
import subprocess

from utils import Markdown
from utils import CONTESTS_CACHE, CONTEST_DETAILS_CACHE, ATCODER_PATH, CODEFORCES_PATH

import pandas as pd

contests = None
contest_details = None
contest_tasks = None


def is_comment(text: str, lang: str) -> bool:
  """Checks if the text is a comment in the specified language."""
  if lang == "cpp":
    pattern = r"(\".*?\"|\'.*?\')|(/\*.*?\*/|//.*?)"
    return re.match(pattern, text).group(2)

  if lang == "py":
    pattern = r"(\"\"\".*?\"\"\"|\'\'\'.*?\'\'\'|#.*?)"
    return re.match(pattern, text).group(0)

  return NotImplementedError(f"Language {lang} is not supported.")

def split_extension(text: str) -> tuple[str, str]:
  """Returns the base name and the extension of a file name."""
  last_dot = text.rfind(".")
  if last_dot == -1:
    return (text, None)
  
  return (text[:last_dot], text[last_dot+1:])

def get_link_from_task_file(path: str) -> str:
  """Retrieves the text on the first line of the file if its a comment."""
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

def get_task_code_from_filename(filename: str) -> str:
  basename, _ = split_extension(filename)
  _, task_code = basename.split("_", 1)
  task_code = task_code.split("_", 1)[0]
  return task_code

def count_tasks(directory: str) -> int:
  "Returns the number of unique tasks in the directory."
  total = 0
  for _, _, filenames in os.walk(directory):
    if len(filenames) == 0:
      continue
    
    tasks = set()
    for name in filenames:
      basename, _ = split_extension(name)
      _, task_code = basename.split("_", 1)
      task_code = task_code.split("_", 1)[0]
      
      tasks.add(task_code)

    total += len(tasks)
  return total

def get_missing_tasks(path: str) -> list[str]:
  files = os.listdir(path)

  all_tasks = contest_details["task_name"].keys()

  tasks_solved = set()
  contest_code = os.path.basename(path)
  for file in files:
    task_code = get_task_code_from_filename(file)
    tasks_solved.add(task_code)

  all_tasks = set(contest_tasks[contest_code])
  return list(all_tasks - tasks_solved)

def get_missing_tasks_fom_dir(directory: str) -> list[tuple[str, str]]:
  all_missing_tasks = []
  for dirpath, _, filenames in os.walk(directory):
    if len(filenames) == 0:
      continue

    contest_code = os.path.basename(dirpath)
    missing_tasks = get_missing_tasks(dirpath)
    all_missing_tasks.extend([(contest_code, task) for task in missing_tasks])

  return all_missing_tasks

def dir_to_collapsed_section(directory: str, start_level: int, file_parser: typing.Callable[[str], str]):
  """Generates a collapsed section based on the directory."""
  def traverse(path: str, level: str) -> str:
    files_and_dir = sorted(os.listdir(path))
    
    lines = []
    for fd in files_and_dir:
      if os.path.isdir(os.path.join(path, fd)):
        result = traverse(os.path.join(path, fd), level+1)
        lines.extend(result.split("\n"))
      else:
        lines.append(f"- {file_parser(os.path.join(path, fd))}")
    
    if level < start_level:
      return "\n".join(lines)

    section = Markdown.collapsed_section(
      summary=file_parser(path),
      lines=lines
    )
    return section
  
  return traverse(directory, 0)

def load_cache():
  """Loads cached names and links from contest data."""
  global contests
  global contest_details
  global contest_tasks
  contests = pd.read_csv(CONTESTS_CACHE).set_index("contest_code").to_dict()
  contest_details = pd.read_csv(CONTEST_DETAILS_CACHE).set_index(["contest_code", "task_code"]).to_dict()
  contest_tasks = pd.read_csv(CONTEST_DETAILS_CACHE).groupby("contest_code")["task_code"].apply(list).to_dict()

def atcoder_cf_parser(path: str, base_link: str) -> str:
  if os.path.isfile(path):
    filename = os.path.basename(path)
    basename, _ = split_extension(filename)
    contest_code, task_code = basename.split("_", 1)
    task_code = task_code.split("_", 1)[0]

    task_name = contest_details["task_name"][(contest_code, task_code)]
    task_link = f"{base_link}{contest_details["task_link"][(contest_code, task_code)]}"

    return f"{filename} - {Markdown.link(text=task_name, href=task_link)}"

  dirname = os.path.basename(path)
  if dirname in contests["contest_name"]:
    dirname = f"{dirname} - {contests["contest_name"][dirname]}"
  return dirname


def all_problems_section() -> str:
  atcoder_section = dir_to_collapsed_section(
    ATCODER_PATH, 
    start_level=0,
    file_parser=lambda x: atcoder_cf_parser(x, base_link="https://atcoder.jp")
  )

  codeforces_section = dir_to_collapsed_section(
    CODEFORCES_PATH,
    start_level=0,
    file_parser=lambda x: atcoder_cf_parser(x, base_link="https://codeforces.com")
  )

  return "\n".join([
    "All Problems",
    Markdown.hrule(),
    atcoder_section,
    codeforces_section
  ])

def missing_problems_section(target_folder: list[str]) -> str:
  missing_problems = []
  for folder in target_folder:
    missing_problems.extend(get_missing_tasks_fom_dir(folder))
  
  if len(missing_problems) == 0:
    return ""
  
  missing_problems.sort()
  missing_problems_by_contest = dict()
  for key, value in missing_problems:
    missing_problems_by_contest.setdefault(key, []).append(value)
  
  contests_lines = []
  for contest_code, tasks in missing_problems_by_contest.items():
    task_lines = []
    for task_code in tasks:
      task_name = contest_details["task_name"][(contest_code, task_code)]
      task_link = contest_details["task_link"][(contest_code, task_code)]
      task_lines.append(f"- {task_code} - {Markdown.link(text=task_name, href=task_link)}")

    collapsed_contest = Markdown.collapsed_section(
      summary=f"{contest_code} - {contests["contest_name"][contest_code]}",
      lines=task_lines
    )
    contests_lines.append(collapsed_contest)

  
  return "\n".join([
    "To Upsolve",
    Markdown.hrule(),
    *contests_lines
  ])

def usage_instructions() -> str:
  lines = [
    "Usage Instructions",
    Markdown.hrule(),
    "This section is mainly for my future self, but this also serves as instructions \
      for anyone who wants to clone this repository to track their own solutions.",
    "",
    "To update the README, simply run `update_readme.py`. This will crawl the target folders and update the the README accordingly. \
      The target directories can have any number of subdirectories for organization, but the final directory (i.e., the directory that contains files) \
      must follow a specific format, depending on the website.",
    "- For AtCoder, this is the name of the contest as it appears in the URL.",
    "- For Codeforces, a prefix is added to differentiate it from other contests. For Codeforces regular contests, a 'CFC' prefix is used.",
    "",
    "To add sections to the README, you must add lines to the `update_readme.py` file directly. There is a `Markdown` class in `utils.py` with helper functions for \
      different markdown syntax."
  ]

  return "\n".join(lines);


def main():
  subprocess.run(["python3", "update_links.py"])
  load_cache()
  
  target_folders = [ATCODER_PATH, CODEFORCES_PATH]

  stat_table = Markdown.table(
    header=["Category", "Number Solved"],
    alignment=["c", "c"],
    data=[target_folders,[str(count_tasks(path)) for path in target_folders]]
  )

  readme = Markdown("README.md")
  readme.add_lines(
    [
      Markdown.header("Competitive Programming Solutions", level=1),
      "This is an archive of the problems that I've solved starting Apr 2026.",
      "",
      stat_table,
      "",
      all_problems_section(),
      "",
      missing_problems_section(target_folders),
      "",
      usage_instructions()
    ]
  )
  readme.write()


if __name__ == "__main__":
  main()