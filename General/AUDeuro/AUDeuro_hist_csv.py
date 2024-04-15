#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Jul 29 17:44:48 2023

@author: stratos
"""

import pandas as pd
import numpy as np
from datetime import date
import matplotlib.dates as dates
import glob
import matplotlib.pyplot as plt


folder_path =  'E:/Mac-back-up/Rstudio_projects/AUDeuro/'
print(folder_path)
file_list = glob.glob(folder_path + "*.xls")
print(file_list)

df_list = []
for i in range(0,len(file_list)):
    rskip = [0,1,2,3,4,5,6,7,8,9]
    usec = [0,1,5]
    data = pd.read_excel(file_list[i], skiprows=rskip, usecols= usec)
    data['Date'] = data.iloc[:, 0]
    data['USD'] = data.iloc[:,1]
    data['EUR'] = data.iloc[:, 2]
    data = data[['Date', 'USD' , 'EUR']]
    data = pd.DataFrame(data)
    #print(data)
    print(file_list[i])
    df_list.append(data)



main_dataframe = pd.concat(df_list, ignore_index=True)        
df =  main_dataframe
df.Date = pd.to_datetime(df.Date)
df = df.sort_values(by="Date", ascending= True)
df

today = df.iloc[-1].EUR

recent_data = df[df["Date"] >= "2023-01-01"]


plt.plot(recent_data.Date, recent_data.EUR, label = 'AUD to EUR')
#plt.plot(df.Date, df.USD, label = 'AUD to USD')
plt.legend()
plt.xlabel('Date')
plt.ylabel('Exchange rate')
plt.title('Data since 01.01.2023. Today: '+ str(today))
plt.xticks(rotation=45)
plt.savefig(r"latest_values.pdf")
plt.show()

"""plt.plot(df.Date, df.EUR, label = 'AUD to EUR')
#plt.plot(df.Date, df.USD, label = 'AUD to USD')
plt.legend()
plt.xlabel('Year')
plt.ylabel('Exchange rate')
plt.title('Data from Australian bank since 2010.')
plt.show()"""

pd.DataFrame.to_csv(df, "AUD_historical.csv")

