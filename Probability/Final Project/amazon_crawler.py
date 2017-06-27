import pickle
import selenium
import re
import time
from selenium import webdriver
from bs4 import BeautifulSoup

stream = ["Java", "C", "C%2B%2B", "Python", "C%23", ".Net", "Javascript", "PHP", "Perl", "Assembly%20language"]
browser = webdriver.Firefox()
time.sleep(9)

result = []
for lang in stream :
    url = "https://www.amazon.com/s/ref=nb_sb_noss_2?url=node%3D3952&field-keywords=" + lang + "&rh=n%3A3952%2Ck%3" + lang
    browser.get(url)
    time.sleep(9)
    soup = BeautifulSoup(browser.page_source, 'lxml')
    end = int(soup.select('.pagnDisabled')[0].text)

    sum = float(0)
    count = int(0)
    for i in range(1, end+1) :
        url = "https://www.amazon.com/s/ref=nb_sb_noss_2?url=node%3D3952&field-keywords=" + lang + "&page=" + str(i) + "&rh=n%3A3952%2Ck%3" + lang
        browser.get(url)
        time.sleep(9)
        soup = BeautifulSoup(browser.page_source, 'lxml')
        stars = soup.select('#atfResults i.a-icon-star span.a-icon-alt')
        for star in stars:
            sum = sum + float(re.split(' ', star.text)[0])
            count = count + 1
    result.append(sum/count)

pickle.dump(result, open("statistics.pkl", "wb"))
