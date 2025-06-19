import requests
import re
from bs4 import BeautifulSoup
import time
import joblib
res_dict = {}


def extract_range(line):
    match = re.search(r"(-?\d+)\D+(-?\d+)", line)
    if match:
        return match.groups()
    return None

def take_temp(line):
    line_pre = line.split(' ')[2]
    if "…" in line_pre:
        len_pre_pre = line_pre.split("…")
        first_t = int(len_pre_pre[0])
        second_t = int(len_pre_pre[1].split("°C.")[0])
    else:
        first_t = second_t = int(line_pre.split("°C.")[0])
    return first_t, second_t

def take_vlaz(line):
    line_pre = line.split(' ')[2]
    if "—" in line_pre:
        len_pre_pre = line_pre.split("—")
        first_t = int(len_pre_pre[0])
        second_t = int(len_pre_pre[1].split("%.")[0])
    else:
        first_t = second_t = int(line_pre.split("%.")[0])
    return first_t, second_t


for i in ['2024']:
    for j in [v for v in range(1,13)]:
        res_dict[(i,j)] = []

for dates in res_dict.keys():
    if dates[1] < 10:
        url = f'https://www.meteoservice.ru/archive/moskva/{str(dates[0])}/0{str(dates[1])}'
    else:
        url = f'https://www.meteoservice.ru/archive/moskva/{str(dates[0])}/{str(dates[1])}'
    response = requests.get(url)
    # print(response)
    status_code = response.status_code
    html_snippet = response.text
    soup = BeautifulSoup(html_snippet, 'html.parser')


    pre_links = f'{soup.select("div.off-canvas-wrapper div.off-canvas-content div.wallpaper div.content div.row div.large-9.small-12.columns ul li a")}'
    # print(pre_links)
    soup1 = BeautifulSoup(pre_links, "html.parser")
    links = soup1.find_all("a")
    hrefs = [link["href"] for link in links]

    full_links = ["https://www.meteoservice.ru" + href for href in hrefs]
    for day in range(len(full_links)):
        print(day)
        time.sleep(5)
        html1 = requests.get(full_links[day]).text
        soup2 = BeautifulSoup(html1, "html.parser")
        pre_text = f'{soup2.select("div.off-canvas-wrapper div.off-canvas-content div.wallpaper div.content div.row div.large-9.small-12.columns div.row div.column.small-12 div.callout p")}'
        soup3 = BeautifulSoup(pre_text, "html.parser")
        try:
            whole_text = soup3.find_all("p")[1].get_text()
            # soup = BeautifulSoup(html, "html.parser")
            # text = soup.get_text()

            temp_line = re.search(r"(Температура воздуха[^\n]+)", whole_text)
            humid_line = re.search(r"(Влажность воздуха[^\n]+)", whole_text)


            # print(whole_text)
            first_t, second_t = take_temp(temp_line.group())


            print(humid_line,humid_line.group())
            h1, h2 = take_vlaz(humid_line.group())
            print((day, first_t, second_t, h1, h2))
            res_dict[(dates[0], dates[1])].append((day, first_t, second_t, int(h1), int(h2)))
        except:
            pass
    print(res_dict)

joblib.dump(res_dict, 'res_dict_p1.joblib')
