from typing import Optional
import os
import re

import requests
from bs4 import BeautifulSoup
import pandas as pd
import tqdm

from utils import AtCoderTask, AtCoderContest

ATCODER_CONTESTS_CACHE = "contest_data/atcoder_contests.csv"
ATCODER_CONTEST_DETAILS_CACHE = "contest_data/atcoder_contest_details.csv"
ATCODER_PATH = "AtCoder/"

def get_atcoder_contest_details(task_url: str) -> AtCoderContest:
  response = requests.get(task_url)
  soup = BeautifulSoup(response.text, features="html.parser")

  contest_title_tag = soup.find(class_="contest-title")
  contest_name = contest_title_tag.text
  contest_code = contest_title_tag.get("href").rsplit("/", 1)[-1].upper()

  contest = AtCoderContest(
    contest_code=contest_code,
    contest_name=contest_name
  )
  table_rows = soup.find("div", class_="panel").find("tbody").find_all("tr")
  for row in table_rows:
    data = row.find_all("td")
    task_code, task_name, *_ = [d.text for d in data]
    task_link = data[0].contents[0].get("href")
    task = AtCoderTask(
      task_code=task_code,
      task_name=task_name,
      task_link=task_link
    )
    contest.add_task(task)

  return contest

def cache_atcoder_contests(contests: AtCoderContest, atcoder_contests_df: Optional[pd.DataFrame] = None, atcoder_contest_details_df: Optional[pd.DataFrame] = None):
  if atcoder_contests_df is None:
    atcoder_contests_df = pd.read_csv(ATCODER_CONTESTS_CACHE)
  if atcoder_contest_details_df is None:
    atcoder_contest_details_df = pd.read_csv(ATCODER_CONTEST_DETAILS_CACHE)
  
  cached_contests = set(atcoder_contests_df["contest_code"].values)

  contest_codes = []
  contest_names = []

  task_contest_codes = []
  task_codes = []
  task_names = []
  task_links = []
  for contest in contests:
    if contest.contest_code in cached_contests:
      print(f"Using '{contest.contest_code}' cache.")
      continue
    contest_codes.append(contest.contest_code)
    contest_names.append(contest.contest_name)

    # drop contest details with this contest code
    indices_to_drop = atcoder_contest_details_df[atcoder_contest_details_df["contest_code"]==contest.contest_code].index
    atcoder_contest_details_df.drop(indices_to_drop, inplace=True)

    for task in contest.tasks:
      task_contest_codes.append(contest.contest_code)
      task_codes.append(task.task_code)
      task_names.append(task.task_name)
      task_links.append(task.task_link)

  new_contest_df = pd.concat([
    atcoder_contests_df,
    pd.DataFrame({"contest_code": contest_codes, "contest_name": contest_names})
  ])
  new_details_df = pd.concat([
    atcoder_contest_details_df,
    pd.DataFrame({
      "contest_code": task_contest_codes,
      "task_code": task_codes,
      "task_name": task_names,
      "task_link": task_links
    })
  ])
  atcoder_contests_df = new_contest_df
  atcoder_contest_details_df = new_details_df

  atcoder_contests_df.to_csv(ATCODER_CONTESTS_CACHE, index=False)
  atcoder_contest_details_df.to_csv(ATCODER_CONTEST_DETAILS_CACHE, index=False)

def cache_abc(nums: list[int]):
  task_urls = [f"https://atcoder.jp/contests/abc{str(i).rjust(3, '0')}/tasks" for i in nums]
  atcoder_contest_df = pd.read_csv(ATCODER_CONTESTS_CACHE)
  cached_contests = set(atcoder_contest_df["contest_code"].values)

  is_cached_list = [f"ABC{str(i).rjust(3, '0')}" in cached_contests for i in nums]

  contests = []

  for url, is_cached in tqdm.tqdm(list(zip(task_urls, is_cached_list)), desc="Caching ABC"):
    if is_cached:
      print(f"Using cache for {url}.")
      continue
    
    try:
      contest = get_atcoder_contest_details(url)
      contests.append(contest)
    except:
      print(f"Failed to get {url}.")
    

  
  cache_atcoder_contests(contests, atcoder_contests_df=atcoder_contest_df)


def main():
  abc_nums = []
  for dirpath, dirnames, filenames in os.walk(ATCODER_PATH):
    if len(filenames) == 0:
      continue
    
    base_dirname = os.path.basename(dirpath)
    if re.match(r"^ABC[0-9]{3}$", base_dirname):
      val = int(base_dirname[-3:])
      abc_nums.append(val)
  cache_abc(abc_nums)


if __name__ == "__main__":
  main()