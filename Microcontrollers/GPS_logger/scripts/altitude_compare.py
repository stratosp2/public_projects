import pandas as pd
import matplotlib.pyplot as plt

input_file  = "gps_data_2.csv"
title       = "Fonzaso-Regensburg"
date        = "2024-1-3"

df = pd.read_csv("data/" + input_file)

df = df[df.date == date]

fig, ax = plt.subplots(figsize = (25,5))

ax.plot(df["bar_altitude"], label="bar altitude")
ax.plot(df["sat_altitude"], label="sat altitude")

ax.set_title(title + " " + date)
ax.set_ylabel("MAMSL")

ax.legend()

plt.show()