# -*- coding: utf-8 -*-
"""
Created on Mon Oct 31 16:52:05 2022

@author: stratos
"""
import pandas as pd
import matplotlib.pyplot as plt
import pandas.plotting as pp
import numpy as np
import statistics
import seaborn as sns

coeff_data = pd.read_csv("/Users/stratos/Documents/Python_projects/AVL_project/fit_QD_data_kneepoints.csv")
coeff_data = coeff_data[['number','c0','c1','c2','c3','tmax_QD', 't_knee']]

df = pd.DataFrame(coeff_data)

print(df)

function_1 = df['c0']*np.exp(df['c1']/df['c3'])+df['c2']*np.exp(df['c3']/df['c3'])
function_2 = np.log(abs(1/(df['c0']*df['c2'])))/(df['c1']+df['c3'])

print(function_2)

df = df.drop([115,2], axis=0)

plt.figure(1,dpi=220)
plt.plot(df['number'],df['tmax_QD'])

print(df.corr(method='spearman'))

matrix = df.corr().round(2)
sns.heatmap(matrix, annot=True, vmax=1, vmin=-1, center=0, cmap='vlag')
plt.savefig(r"only_correlation.pdf")

pp.scatter_matrix(df, diagonal='kde')
#plt.show()
plt.savefig(r"coefficients_kneepoint_correlation.pdf")
