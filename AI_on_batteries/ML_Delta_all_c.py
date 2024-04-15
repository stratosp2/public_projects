#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Feb 23 20:01:31 2023
make a script that predicts c0,c1,c2,c3 from ΔQ ΔIR from first few cycles. 
Then put these into the model and compare with data.
@author: stratos
"""

import numpy as np
import matplotlib.pyplot as plt
import pickle
import pandas.plotting as pp
import pandas as pd
from sklearn.metrics import r2_score 
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error
from sklearn.multioutput import MultiOutputRegressor
from sklearn import ensemble

#import data
delta_data = pd.read_csv("DeltaQ.csv")
#clean a bit 
c_data = pd.read_csv("fit_coefficients_QD_QC.csv")
c_data = c_data[['Battery', 'c0','c1','c2','c3']]
print(delta_data)

#merge them
all_data = pd.merge(delta_data, c_data)
#more cleaning. Predict all c's from ΔQ ΔIR
all_data = all_data[['c0','c1','c2','c3', 'ΔQ', 'δQD', 'ΔIR', 'δIR', 'Δt','δt', 'ΔTavg','δT']]
#remove outliers
all_data = all_data[all_data['c0']<4]
#print(all_data)

#pp.scatter_matrix(all_data, diagonal='kde')
#plt.show()

result = all_data
prediction = ['c0', 'c1', 'c2','c3']


"""best =1000 
for _ in range(5):
    X =np.array(result.drop(['c0', 'c1', 'c2','c3'], axis=1))
    y =np.array(result[prediction])
    
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.2
    )
    
    params = {
       "n_estimators": 1000,
       "max_depth": 5,
       "min_samples_split": 5,
       "learning_rate": 0.002,
       "loss": "absolute_error",
    }
    reg = MultiOutputRegressor(ensemble.GradientBoostingRegressor(**params))
    reg.fit(X_train, y_train)
    rmse = mean_squared_error(y_test, reg.predict(X_test))
    #print(mse)
    acc = round(reg.score(X_test,y_test),2)
    if rmse < best:
        best = rmse
        with open("_GBR_model_all_c.pickle", "wb") as f: 
            pickle.dump(reg,f)
            print('stored model with rmse:', np.sqrt(rmse))
            
    print(np.sqrt(rmse))"""

# for some reasone the drop commant messses up if prediction is used...
X =np.array(result.drop(['c0', 'c1', 'c2','c3'], axis=1))
y =np.array(result[prediction])

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.991
)

params = {
    "n_estimators": 1000,
    "max_depth": 5,
    "min_samples_split": 5,
    "learning_rate": 0.002,
    "loss": "absolute_error",
}
pickle_in = open("_GBR_model_all_c.pickle","rb")
print("model is _GBR_model_all_c.pickle")
reg = pickle.load(pickle_in)
best = round(reg.score(X_test,y_test),2)
rmse = round(np.sqrt(mean_squared_error(y_test, reg.predict(X_test))),5)
pred_dummy = reg.predict(X_test)
C = []
#len(reg.estimators_)

#feature importances. Read more about this. Probably each estimator, e.g 0,1,2,3 
#concerns c0,c1,c2,c3 and how fearures contribute on them.
features = ['ΔQ','δQD', 'ΔIR', 'δIR', 'Δt','δt', 'ΔTavg','δT']

for i in range(len(reg.estimators_)):
   # print(reg.estimators_[i].feature_importances_)

    fi = pd.DataFrame(data = reg.estimators_[i].feature_importances_,index=features,
                  columns=['importance'])
    fi.sort_values('importance').plot(kind='barh', title ='feature importance for c'+str(i) )
     #print(reg.feature_names_in_)


for y in range(len(pred_dummy)):
   # print(pred_dummy[y],x_test[y], y_test[y])
    C.append((pred_dummy[y], y_test[y]))


# create two dataframes one for prediction data and one for test data
df1 = pd.DataFrame(pred_dummy, columns=['pc0', 'pc1', 'pc2', 'pc3'])
#print(df1)
df2 = pd.DataFrame(y_test, columns=['c0', 'c1', 'c2', 'c3'])
#print(df2)
#merge the two dataframes 
df_compare = pd.merge(df1, df2, left_index=True, right_index=True)
#print(df_compare)

print('Accuracy:', best)
print('RMSE:', rmse)


final_data= pd.merge(c_data, df_compare)
#print(final_data)
final_data.to_csv('ML_first100_cycles_all_c.csv')


"""df_compare = df_compare.reset_index()
plt.scatter(df_compare['index']+1,df_compare[test_variable], label="ML prediction")
plt.scatter(df_compare['index']+1,df_compare['p'+test_variable], label="data prediction")
plt.ylabel(test_variable)
plt.xlabel("battery")
plt.legend()
plt.show()"""


"""final_data.set_index("Battery", inplace = True)
print(final_data.loc['b3c4'])
par = final_data.loc['b3c4']
print(par[0])
print(par[1])
#print(final_data['b1c4','Battery'])

t = np.linspace(1,1000,1000)
fn1 = par[0]*np.exp(par[1]*t)+par[2]*np.exp(par[3]*t)
fn2 = par[4]*np.exp(par[5]*t)+par[6]*np.exp(par[7]*t)

plt.plot(t, fn1, label = 'data fit',linewidth =2)
plt.plot(t, fn2, label = 'ML pred', ls='dotted', linewidth =3)
plt.ylim(0.8, 1.1)
#plt.xlim(0, 1060)
plt.title('b3c4')
plt.legend()
plt.show()"""
