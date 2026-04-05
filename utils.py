from typing import Literal

class Markdown:
  def __init__(self, output_path: str):
    self.output_path = output_path
    self.text = ""
  
  def write(self):
    with open(self.output_path, "w") as f:
      f.write(self.text)

  def add(self, other_text: str):
    self.text += other_text

  def add_line(self, line: str):
    self.add(line)
    self.add("\n")
  
  def add_lines(self, lines: list[str]):
    for line in lines:
      self.add_line(line)
  
  def __repr__(self):
    return self.text

  @staticmethod
  def dedent(text: str) -> str:
    lines = text.split("\n")
    lines = [line.lstrip() for line in lines]
    return "\n".join(lines)

  @staticmethod
  def collapsed_section(summary: str, lines: list[str]) -> str:
    """Returns a collapsed section in markdown."""
    text = f"""<details>
    <summary>{summary}</summary>

    {"\n".join(lines)}
    </details>"""
    return Markdown.dedent(text)

  @staticmethod
  def link(text: str, href: str) -> str:
    """Returns a link in markdown."""
    return f"[{text}]({href})"

  @staticmethod
  def header(text: str, level: int = 1):
    return f"{'#'*level} {text}"
  
  @staticmethod
  def table(header: list[str], alignment: list[Literal["l","r","c"]], data: list[list[str]]):
    if len(header) != len(alignment):
      raise RuntimeError(f"Header has {len(header)} columns, but data has {len(alignment)}.")
    
    if len(header) != len(data):
      raise RuntimeError(f"Header has {len(header)} columns, but data has {len(data)}.")
    
    if len(header) == 0:
      raise RuntimeError("Number of columns must be 1 or greater.")
    
    h = len(data[0])
    for i in range(1, len(data)):
      if h != len(data[i]):
        raise RuntimeError("Number of rows must be the same for all columns.")
    
    allowed_alignment = set(["l", "r", "c"])
    for a in alignment:
      if a not in allowed_alignment:
        raise RuntimeError(f"Unknown alignement '{a}'.")

    def map_alignment(ch: Literal["l", "r", "c"]):
      if ch == "l": return ":-"
      elif ch == "r": return "-:"
      else: return ":-:"

    lines = [
      f"|{'|'.join(header)}|",
      f"|{'|'.join([map_alignment(a) for a in alignment])}|",
      *[f"|{'|'.join(row)}|" for row in zip(*data)]
    ]
    return '\n'.join(lines)
  
  @staticmethod
  def hrule():
    return "---"
  
class Task:
  def __init__(self, task_code, task_name, task_link):
    self.task_code = task_code
    self.task_name = task_name
    self.task_link = task_link
  
  def __repr__(self):
    return f"<Task {self.task_code} - {self.task_name}>"

class Contest:
  def __init__(self, contest_code, contest_name):
    self.contest_code = contest_code
    self.contest_name = contest_name
    self.tasks = []
  
  def __repr__(self):
    return f"<Contest {self.contest_code} - {self.contest_name}>"

  def __len__(self):
    return len(self.tasks)

  def add_task(self, task: Task):
    self.tasks.append(task)
  
  def add_tasks(self, tasks: list[Task]):
    self.tasks.extend(tasks)

CONTESTS_CACHE = "contest_data/contests.csv"
CONTEST_DETAILS_CACHE = "contest_data/contest_details.csv"
ATCODER_PATH = "AtCoder"
CODEFORCES_PATH = "Codeforces"