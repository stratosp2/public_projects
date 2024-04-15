from bs4 import BeautifulSoup
import requests
import pandas as pd
import re
from datetime import datetime, date
import matplotlib.pyplot as plt


url= "https://www.agro24.gr/%CE%B5%CE%BB%CE%B1%CE%B9%CF%8C%CE%BB%CE%B1%CE%B4%CE%BF"

headers = {'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) '\
                'AppleWebKit/537.36 (KHTML, like Gecko) '\
                'Chrome/75.0.3770.80 Safari/537.36'}
                
                
result = requests.get(url,headers=headers)
doc = BeautifulSoup(result.content, "html.parser")

#charts-google chart
tag=doc.find("div", attrs = {'class': 'charts-google chart'})

text = tag.get_text
text = str(text)
 
re.findall("\d+\.\d+",text)

text = text.replace('\\', '').replace('.','')
type(text)


list_price =re.findall(r"\,\d+",text)
list_date = re.findall("\d+/\d+/\d+",text)
df= pd.DataFrame( {"Date": list_date, "Price": list_price})

df.Price = df['Price'].str.rsplit(',').str[-1]
type(df.Price)

f_list = []
loop_list = list(df.Price)

#Normalizing the price to three digits.
f_list = []
i=0
for i in loop_list:
  #print(i)
  if len(i)==2:
    k = int(i)*10
  elif len(i)==1:
    k=int(i)*100
  else:
    k=int(i)
  f_list.append(k)
  #print(len(i), k)
#Passing the f_list to df.pirce
df.Price = f_list
df.Price = df.Price/100

df['Date']=pd.to_datetime(df.Date, format= "%d/%m/%Y")

df.plot(x = 'Date', y = 'Price', label = 'Price')
plt.title("Historical price data for the olive oil.")
plt.ylabel("Euros/kgs")
plt.grid(True)
plt.show()
df.to_csv("Olive_oil_data.csv")
print(df.tail())

plt.savefig(r"Olive_oil_historical_data.pdf")
plt.savefig(r"Olive_oil_historical_data.png")

print("Data written to csv and plotted. Done!")
