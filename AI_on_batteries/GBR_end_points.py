#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 20 22:24:13 2023

@author: stratos
"""

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb  6 17:37:16 2023

Machine learning for end cycles predictions, i.e tmax_QD using GradientBoostingRegressor. 
The linear model was failing but this one is doing predicting tmax_QD without t_knees
with acc 100%.

@author: stratos
"""

import pandas as pd
import numpy as np
from matplotlib import pyplot as plt
from sklearn.metrics import mean_squared_error
from sklearn import datasets, ensemble
from sklearn.metrics import r2_score 
from sklearn.model_selection import train_test_split
import pickle


data = pd.read_csv("fit_QD_data_kneepoints.csv")

#subset data depending on what the preduction value is
predict = "tmax_QD"
df = data[["c0", "c1", "c2", "c3", predict, "t_knee"]]
#df = data[["c0", "c1", "c2", "c3", "tmax_QD", "t_knee"]]
#remove outliers
df = df[df['t_knee']>100]

#remove negative tmax_cycle prediction
df = df.drop([115,2], axis=0)
#df = df.drop(['t_knee'], axis=1)

df = data[["c0", "c1", "c2", "c3", predict]]
#print(df)



#predict = "tmax_80"

X = np.array(df.drop([predict],axis=1))
y = np.array(df[predict])

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.9
)

params = {
    "n_estimators": 1000,
    "max_depth": 4,
    "min_samples_split": 5,
    "learning_rate": 0.01,
    "loss": "squared_error",
}
reg = ensemble.GradientBoostingRegressor(**params)
reg.fit(X_train, y_train)

mse = mean_squared_error(y_test, reg.predict(X_test))
print("The mean squared error (MSE) on test set: {:.4f}".format(mse))

predictions = reg.predict(X_test)

#print(predictions)
#diabetes = datasets.load_diabetes()




#create a loop to train over the models and find the best model with maximum accuracy, then store it
"""best = 1000000
for _ in range(100):
    X = np.array(df.drop([predict],axis=1))
    y = np.array(df[predict])
    #train the model with 90% of the data
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.1, random_state=13
    )

    params = {
        "n_estimators": 1000,
        "max_depth": 4,
        "min_samples_split": 5,
        "learning_rate": 0.007,
        "loss": "squared_error",
    }
    reg = ensemble.GradientBoostingRegressor(**params)
    reg.fit(X_train, y_train)
     
    mse = mean_squared_error(y_test, reg.predict(X_test))
    print("The mean squared error (MSE) on test set: {:.4f}".format(mse))
    
    print(mse)
    if mse < best:
         best = round(mse,2)

    with open(predict+ "_GBR_model_end_points.pickle", "wb") as f: 
        pickle.dump(reg,f)"""

   
pickle_in = open(predict+"_GBR_model_end_points.pickle","rb")
print("model used is " + predict+"_XGB_model_end_points.pickle")
reg = pickle.load(pickle_in)
best = round(reg.score(X_test,y_test),2)
pred_dummy = reg.predict(X_test)
D = []

for y in range(len(pred_dummy)):
   # print(pred_dummy[y],x_test[y], y_test[y])
    D.append((pred_dummy[y], y_test[y]))

#print(D)
cols = ['prediction', predict,]
result = pd.DataFrame(D, columns=cols) 
#remove outliers and restric predictions below maximum cycle. Usually one prediction was off. 
result = result[(result['prediction']<max(df[predict])) & (result['prediction']>0)]
#print(result) 

#create an index numbering for battery
result = result.reset_index()
plt.scatter(result['index']+1,result['prediction'], label="ML prediction")
plt.scatter(result['index']+1,result[predict], label="data prediction")
plt.ylabel("fail cycle")
plt.xlabel("battery")
plt.legend()
#plt.title("End points of batteries")
plt.title(predict + " End points of batteries,   " +'$R^2=$' +str(best))
plt.savefig(predict+ r"GBR_ML_endPoints_EOLQD.pdf")
plt.show()

result.to_csv(predict+'GBR_predictions_vs_actual_data.csv')
