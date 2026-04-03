import os


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


def parse_file(path: str) -> str:
  filename = os.path.basename(path)
  with open(path, "r") as f:
    # grab comment on first line
    first_line = f.readline()
  
  prefixes = ["\"\"\"", "\'\'\'", "/*", "//", "#"]
  suffixes = ["\"\"\"", "\'\'\'", "*/"]
  first_line = first_line.strip()
  for prefix in prefixes:
    first_line = first_line.removeprefix(prefix)
  for suffix in suffixes:
    first_line = first_line.removesuffix(suffix)
  href = first_line.strip()
  
  return Markdown.link(text=filename, href=href)

def dir_to_collapsed_section(directory: str, start_level: int):
  def traverse(path: str, level: str) -> str:
    files_and_dir = sorted(os.listdir(path))
    
    lines = []
    for fd in files_and_dir:
      if os.path.isdir(os.path.join(path, fd)):
        result = traverse(os.path.join(path, fd)+"/", level+1)
        lines.extend(result.split("\n"))
      else:
        lines.append(f"- {parse_file(os.path.join(path, fd))}")
    
    if level < start_level:
      return "\n".join(lines)

    immediate_dir = path.rsplit("/", 2)[-2]
    section = Markdown.collapsed_section(
      summary=immediate_dir,
      lines=lines
    )
    return section
  
  return traverse(directory, 0)


def main():
  readme = Markdown("README.md")
  readme.add_lines(
    [
      Markdown.header("Competitive Programming Solutions", level=1),
      "This is an archive of the problems that I've solved."
    ]
  )
  
  atcoder_section = dir_to_collapsed_section("AtCoder/", start_level=0)
  readme.add_line(atcoder_section)
  readme.write()


if __name__ == "__main__":
  main()