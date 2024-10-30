# -*- coding: utf-8 -*-
"""
Created on Thu Jan 25 10:38:58 2024

@author: stratos
"""

import pandas as pd 
import matplotlib.pyplot as plt
import seaborn as sns
import pandas.plotting as pp
from datetime import date

feel_data = pd.read_csv("btc_hist_feeling.csv")
bitcoin_data = pd.read_csv("BTC-EUR.csv",index_col = [0])
bitcoin_data.Date = pd.to_datetime(bitcoin_data.Date, format = 'mixed')
feel_data.Date = pd.to_datetime(feel_data.Date, format = 'mixed')

bitcoin_data = bitcoin_data[bitcoin_data["Date"]>="2017-01-01"]
merged_data = pd.DataFrame(bitcoin_data.merge(feel_data))

merged_data.Date = pd.to_datetime(merged_data['Date'], format= 'mixed')

up_date = date.today().strftime("%Y-%m-%d")
explored_data = merged_data[(merged_data["Date"] >= "2024-01-01") & (merged_data["Date"] <= up_date)]

explored_data['compound'] = explored_data['compound'].shift(1) # The compound feeling of the previous day aligned with today. 

merged_data = merged_data.drop_duplicates(subset=["Date","Open", "High", "Close"],keep = 'first')

explored_data = explored_data.drop_duplicates(subset=["Date","Open", "High", "Close"],keep = 'first') 

explored_data.head()

target = "compound"

init_date = pd.to_datetime(explored_data["Date"].iloc[0])
str_date = str(init_date.date())

fig, ax = plt.subplots()
explored_data.plot(x = 'Date', y = target, label = target, ax = ax)  
explored_data.plot(x = 'Date', y = "Ad_return", label = "Adjusted Return", ax = ax) 
plt.xticks(rotation=45)
plt.grid(True)
plt.title("Comparison since "+str_date)
explored_data.plot(x = 'Date', y = 'Close', ax = ax, secondary_y = True, lw=3) 
plt.show()


pp.scatter_matrix(explored_data[["compound", "Close", "Ad_return", "Volume"]], diagonal='kde')
plt.show()


matrix = explored_data[["compound", "Close", "Ad_return", "Volume"]].corr()
print(matrix)

sns.heatmap(matrix, annot=True)
plt.show()

#plt.hist(feel_data["compound"])
