#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
NN on OCV by reading terminal over time.
"""

import pandas as pd 
import numpy as np
import tensorflow as tf 
import matplotlib.pyplot as plt
from tensorflow.keras import layers
from sklearn.metrics import r2_score 
import glob
import random

#model = 'Ai'
global_file_list= glob.glob("./OCV_data/*.csv") 
len(global_file_list)
test_data = random.choice(global_file_list)
data_1 = pd.read_csv(test_data)
print("Data test is from database:", test_data)

model = data_1['Model'][0] 
model = model.replace('2020', '')
print("Model is:", model)

#create a list of available csv files
folder_path = './OCV_data/'+model
file_list = glob.glob(folder_path + "*.csv") 
files = random.sample(file_list, 10)
len(file_list) 



#This is needed for initial training of the model
df_list = []
for i in range(0,len(files)):
    full_data = pd.read_csv(files[i])
    print(files[i])
    df_list.append(full_data)

main_dataframe = pd.concat(df_list,axis=0)    

data = main_dataframe

data = data[['Terminal voltage [V]', 'Current [A]', 'Time [h]',   'Measured open circuit voltage [V]']]
data_1 = data_1[['Terminal voltage [V]', 'Current [A]', 'Time [h]', 'Measured open circuit voltage [V]']]


train_dataset = data.sample(frac=0.8, random_state= 0)
test_dataset = data.sample(frac=0.2, random_state= 0)
test_dataset.tail()
test_2_dataset = data_1
test_2_labels = test_2_dataset.pop('Measured open circuit voltage [V]')

train_features = train_dataset.copy()
test_features = test_dataset.copy()
train_labels = train_features.pop('Measured open circuit voltage [V]')
test_labels = test_features.pop('Measured open circuit voltage [V]')

train_features
train_labels

normalizer = tf.keras.layers.Normalization(axis=-1)
normalizer.adapt(np.array(train_features))

OCV = np.array(train_features)

"""
def build_and_compile_model(norm):
  model = tf.keras.Sequential([
      norm,
      layers.Dense(64, activation='relu'),
      layers.Dense(64, activation='relu'),
      layers.Dense(1)
  ])

  model.compile(loss='mean_absolute_error',
                optimizer='adam')
  return model


#define the model using normalized features as input
OCV_model = build_and_compile_model(normalizer)
OCV_model.summary()



trained_model = OCV_model.fit(
    train_features,
    train_labels,
    validation_split=0.2,
    verbose=1, epochs=100)

OCV_model.save('./NN_'+model+' _OCV_model')


def plot_loss(trained_model):
  plt.plot(trained_model.history['loss'], label='loss')
  plt.plot(trained_model.history['val_loss'], label='val_loss')
  #plt.plot(history.history['accuracy'], label='accuracy')
  #plt.ylim([0, 10])
  plt.xlabel('Epoch')
  plt.ylabel('Error [OCV]')
  plt.legend()
  plt.grid(True)
  
plot_loss(trained_model)
plt.show()
"""


print('Calling NN model for :', model)
new_model = tf.keras.models.load_model('./NN_'+model+' _OCV_model')


test_2_predictions = new_model.predict(test_2_dataset).flatten()

score = round(r2_score(test_2_labels, test_2_predictions),4)*100 
score = f'{score:0.2f}%'
print(score)
a = plt.axes(aspect='equal')
plt.scatter(test_2_labels, test_2_predictions)
plt.xlabel('True Values [OCV]')
plt.ylabel('Predictions [OCV]')
lims = [3.4, 3.9]
plt.xlim(lims)
plt.ylim(lims)
plt.title('$Accuracy:$' +str(score))
_ = plt.plot(lims, lims)

plt.show()

plt.plot(data_1['Time [h]'], test_2_labels, label= 'True data')
plt.plot(data_1['Time [h]'], test_2_predictions, label= 'Predicted data')
plt.legend()
plt.xlabel('Time [h]')
plt.ylabel('OCV [V]')
plt.title('Accuracy:'+str(score))
plt.show()



