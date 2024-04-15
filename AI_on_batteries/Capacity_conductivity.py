# -*- coding: utf-8 -*-
"""
Created on Mon Oct 24 19:19:03 2022

@author: stratos

Script reads data from batch1, fits double exponentials to QD and QC, storing the coefficients fits in .csv file
for correlogram

"""

# -*- coding: utf-10 -*-
import numpy as np
import matplotlib.pyplot as plt
import pickle
import pandas as pd
import numpy as np

from scipy.optimize import curve_fit

batch = pickle.load(open('/Users/stratos/Documents/Python_projects/AVL_project/batch_full.pkl', 'rb'))

bat_dict = batch

D = []
batteries = []
bat_list = bat_dict.keys()
for i, bat_num in enumerate(bat_list): 
    #print(i+1, bat_num)
    
    data = bat_dict[bat_num]['summary']
    
    #df_900 = df_900[df_900['cycle']>500]
    
    df = pd.DataFrame(data)
    df = df[df['cycle']>10]

    
    # Predict for full data QC
    def bd(t,c0,c1,c2,c3):
        return c0*np.exp(+c1*t)+c2*np.exp(+c3*t)
    
    #def bl(t,c4,c5):
    #   return c4+c5*t 
    
    #h = [1.45758216e+00,-8.12381798e-04]
    
    g = [1.07924382e+00,1.39974442e-04,-1.18746823e-02,4.67990934e-03]
    
    t = bat_dict[bat_num]['summary']['cycle'][()]
    con = bat_dict[bat_num]['summary']['QD'][()]
    c,cov = curve_fit(bd,t,con,g,maxfev=50000)
   # print(c[0],c[1],c[2],c[3]) 
    
    t = bat_dict[bat_num]['summary']['cycle'][()]
    con = bat_dict[bat_num]['summary']['QC'][()]
    d,cov = curve_fit(bd,t,con,g,maxfev=50000)
    
    tmax_QD = np.log(abs(c[0]/c[2]))/(c[3]-c[1]) 
    tmax_QC = np.log(abs(d[0]/d[2]))/(d[3]-d[1]) 
    
    D.append((i+1, bat_num,c[0],c[1],c[2],c[3], d[0], d[1], d[2], d[3], tmax_QD, tmax_QC))#,d))
    #print("fitting list is", D)  
    
    funcdata_QD = bd(df['cycle'],c[0],c[1],c[2],c[3])
    funcdata_QC = bd(df['cycle'],d[0],d[1],d[2],d[3])
   
cols = ['number','Battery', 'c0', 'c1', 'c2', 'c3', 'd0', 'd1', 'd2', 'd3','tmax_QD','tmax_QC' ]
result = pd.DataFrame(D, columns=cols) 
print(result)

result.to_csv('fit_coefficients_QD_QC.csv')



    



