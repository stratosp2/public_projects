#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Feb 25 09:50:12 2023

@author: stratos
"""


import numpy as np
import matplotlib.pyplot as plt
import pickle
import pandas as pd


data_c = pd.read_csv('ML_first100_cycles_all_c.csv')
#print(data_c)

#data_c['c0']

t_real = abs(np.log(abs(data_c['c2'])/data_c['c0'])/(data_c['c3']-data_c['c1']))
t_pred = abs(np.log(abs(data_c['pc2'])/data_c['pc0'])/(data_c['pc3']-data_c['pc1']))

data_c = data_c.reset_index()
print(data_c)
#print(data_c)

plt.scatter(data_c['index'], t_real, label = 'real cycle', s = 65)
plt.scatter(data_c['index'], t_pred, label = 'predicted cycle')
plt.legend()
plt.xlabel('Batteries')
plt.ylabel('fail cycle')
plt.title('Prediction from first 100 cycles')
plt.savefig( r"EOL_cycles_comparison.pdf")
plt.show()



t_EOL80_real = round(t_real*0.78+18.8)
t_EOL80_pred = round(t_pred*0.78+18.8)

plt.scatter(data_c['index'], t_EOL80_real, label = 'real EOL80', s = 65)
plt.scatter(data_c['index'], t_EOL80_pred, label = 'predicted EOL80')
plt.legend()
plt.xlabel('Batteries')
plt.ylabel('EOL80 cycle')
plt.title('EOL80 prediction from first 100 cycles')
plt.savefig( r"EOL80_cycles_comparison.pdf")
plt.show()