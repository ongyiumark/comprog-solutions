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
  
class AtCoderTask:
  def __init__(self, task_code, task_name, task_link):
    self.task_code = task_code
    self.task_name = task_name
    self.task_link = task_link
  
  def __repr__(self):
    return f"<AtCoderTask {self.task_code} - {self.task_name}>"

class AtCoderContest:
  def __init__(self, contest_code, contest_name):
    self.contest_code = contest_code
    self.contest_name = contest_name
    self.tasks = []
  
  def __repr__(self):
    return f"<AtCoderContest {self.contest_code} - {self.contest_name}>"

  def __len__(self):
    return len(self.tasks)

  def add_task(self, task: AtCoderTask):
    self.tasks.append(task)
  
  def add_tasks(self, tasks: list[AtCoderTask]):
    self.tasks.extend(tasks)
