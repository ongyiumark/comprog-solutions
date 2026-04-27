""" Caches contest details """

from typing import Optional
import os

import requests
import cloudscraper
from bs4 import BeautifulSoup
import pandas as pd
import tqdm

from utils import Task, Contest
from utils import CONTESTS_CACHE, CONTEST_DETAILS_CACHE, ATCODER_PATH, CODEFORCES_PATH


# pylint: disable=C0116
def get_contest_codes_from_directory(directory: str) -> list[str]:
  contest_codes = []
  for dirpath, _, filenames in os.walk(directory):
    if len(filenames) == 0:
      continue

    base_dirname = os.path.basename(dirpath)
    contest_codes.append(base_dirname)

  return contest_codes


def get_atcoder_contest_details(task_url: str) -> Contest:
  response = requests.get(task_url, timeout=5)
  if response.status_code != 200:
    raise RuntimeError(
      f"Failed to get {task_url}. Status code {response.status_code}.")
  soup = BeautifulSoup(response.text, features="html.parser")

  contest_title_tag = soup.find(class_="contest-title")
  contest_name = contest_title_tag.text
  contest_code = contest_title_tag.get("href").rsplit("/", 1)[-1].upper()

  contest = Contest(contest_code=contest_code, contest_name=contest_name)
  table_rows = soup.find("div", class_="panel").find("tbody").find_all("tr")
  for row in table_rows:
    data = row.find_all("td")
    task_code, task_name, *_ = [d.text for d in data]
    task_link = data[0].contents[0].get("href")
    task = Task(task_code=task_code, task_name=task_name, task_link=task_link)
    contest.add_task(task)

  return contest


def get_codeforces_contest_details(contest_url: str) -> Contest:
  scraper = cloudscraper.create_scraper()
  response = scraper.get(contest_url)
  if response.status_code != 200:
    raise RuntimeError(
      f"Failed to get {contest_url}. Status code {response.status_code}.")

  soup = BeautifulSoup(response.text, features="html.parser")
  contest_title_tag = soup.find(id="sidebar").find("tbody").find("a")
  contest_name = contest_title_tag.text
  contest_code = contest_title_tag.get("href").rsplit("/", 1)[-1].upper()

  contest = Contest(contest_code=f"CFC{contest_code}",
                    contest_name=contest_name)
  table_rows = soup.find("table", class_="problems").find_all("tr")[1:]
  for row in table_rows:
    data = row.find_all("td")
    task_code = data[0].text.strip()
    task_link = data[0].find("a").get("href")
    task_name = data[1].find("a").text.strip()
    task = Task(task_code=task_code, task_name=task_name, task_link=task_link)
    contest.add_task(task)

  return contest


def cache_contests(contests: Contest,
                   contests_df: Optional[pd.DataFrame] = None,
                   contest_details_df: Optional[pd.DataFrame] = None):
  if contests_df is None:
    contests_df = pd.read_csv(CONTESTS_CACHE)
  if contest_details_df is None:
    contest_details_df = pd.read_csv(CONTEST_DETAILS_CACHE)

  cached_contests = set(contests_df["contest_code"].values)

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
    indices_to_drop = contest_details_df[contest_details_df["contest_code"] ==
                                         contest.contest_code].index
    contest_details_df.drop(indices_to_drop, inplace=True)

    for task in contest.tasks:
      task_contest_codes.append(contest.contest_code)
      task_codes.append(task.task_code)
      task_names.append(task.task_name)
      task_links.append(task.task_link)

  new_contest_df = pd.concat([
    contests_df,
    pd.DataFrame({
      "contest_code": contest_codes,
      "contest_name": contest_names
    })
  ])
  new_details_df = pd.concat([
    contest_details_df,
    pd.DataFrame({
      "contest_code": task_contest_codes,
      "task_code": task_codes,
      "task_name": task_names,
      "task_link": task_links
    })
  ])
  contests_df = new_contest_df
  contest_details_df = new_details_df

  contests_df.to_csv(CONTESTS_CACHE, index=False)
  contest_details_df.to_csv(CONTEST_DETAILS_CACHE, index=False)


def cache_atcoder_contests(contest_codes: list[str]):
  task_urls = [
    f"https://atcoder.jp/contests/{contest_code}/tasks"
    for contest_code in contest_codes
  ]
  contest_df = pd.read_csv(CONTESTS_CACHE)
  cached_contests = set(contest_df["contest_code"].values)

  is_cached_list = [
    contest_code in cached_contests for contest_code in contest_codes
  ]

  contests = []
  for url, is_cached in tqdm.tqdm(list(zip(task_urls, is_cached_list)),
                                  desc="Caching AtCoder"):
    if is_cached:
      print(f"Using cache for {url}.")
      continue

    try:
      contest = get_atcoder_contest_details(url)
      contests.append(contest)
    except RuntimeError as e:
      print(f"Failed to get {url}. Error: {e}")

  cache_contests(contests, contests_df=contest_df)


def cache_codeforces_contests(contest_codes: list[str], prefix: str):
  task_urls = [
    f"https://codeforces.com/contest/{contest_code}"
    for contest_code in contest_codes
  ]
  contest_df = pd.read_csv(CONTESTS_CACHE)
  cached_contests = set(contest_df["contest_code"].values)

  is_cached_list = [
    f"{prefix}{contest_code}" in cached_contests
    for contest_code in contest_codes
  ]

  contests = []
  for url, is_cached in tqdm.tqdm(list(zip(task_urls, is_cached_list)),
                                  desc="Caching Codeforeces"):
    if is_cached:
      print(f"Using cache for {url}.")
      continue

    try:
      contest = get_codeforces_contest_details(url)
      contests.append(contest)
    except RuntimeError as e:
      print(f"Failed to get {url}. Error: {e}")

  cache_contests(contests, contests_df=contest_df)


def main():
  atcoder_codes = get_contest_codes_from_directory(ATCODER_PATH)
  cache_atcoder_contests(atcoder_codes)

  codeforces_codes = get_contest_codes_from_directory(CODEFORCES_PATH)
  cfc_codes = [code[3:] for code in codeforces_codes if code.startswith("CFC")]
  cache_codeforces_contests(cfc_codes, prefix="CFC")


if __name__ == "__main__":
  main()
