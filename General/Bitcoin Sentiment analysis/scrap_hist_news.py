from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from datetime import date,timedelta,time
import  pandas as pd

#prepare the server and website (in english)
service = Service(executable_path="chromedriver.exe")
options = webdriver.ChromeOptions()
options.add_experimental_option("detach", True)
driver = webdriver.Chrome(options=options, service=service)
driver.get("https://www.google.com/?hl=en")

ablehnen = driver.find_element(By.CLASS_NAME, "QS5gu.sy4vM")
ablehnen.click()

#prepare the serach and filter for last 24 hours
search_item = "bitcoin news today"
input = driver.find_element(By.CLASS_NAME, "gLFyf")
input.send_keys(search_item + Keys.ENTER)
news = driver.find_element(By.CLASS_NAME, "LatpMc.nPDzT.T3FoJb")
news.click()
filter = driver.find_element(By.CLASS_NAME, "nfSF8e")
filter.click()

from datetime import date
sdate = date(2017,2,11)   # start date
sdate
edate = date(2017,2,11)   # end date
date_list = pd.date_range(sdate,edate,freq='d')

print(date_list)

for Date in date_list:
    target_date = Date.strftime('%m/%d/%Y')

    print(target_date)
    recent = driver.find_element(By.CLASS_NAME, "KTBKoe")
    recent.click()
    custom = driver.find_element(By.XPATH, "//*[contains(text(), 'Custom range...')]")
    custom.click()
    input_init_1 = driver.find_element(By.CLASS_NAME, "OouJcb")
    input_init_1.clear()
    input_init_1.send_keys(target_date)
    input_init_2 = driver.find_element(By.CLASS_NAME, "rzG2be")
    input_init_2.clear()
    input_init_2.send_keys(target_date)
    go = driver.find_element(By.CLASS_NAME, "Ru1Ao.BwGU8e.fE5Rge")
    go.click()

    import time

    time.sleep(0.1)

    no_of_pages = driver.find_elements(By.CLASS_NAME, "fl")
    len(no_of_pages)

    no_of_links = driver.find_elements(By.CLASS_NAME, "n0jPhd.ynAwRc.MBeuO.nDgy9d")
    len(no_of_links)
    

    text_list = []

    for item in no_of_links: 
        full = item.text
        text_list.append(full+".\n ")

    text_list

    date = Date.strftime("%Y-%m-%d")
    file_name = "news_"+date+"_.txt"
    print(file_name)
    with open("hist_news/"+file_name, "a") as file:
        file.writelines(text_list)
        
    file.close()
    