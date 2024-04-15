# A script that creates an interactive map and plots the route from .csv data.
import pandas as pd
import matplotlib.pyplot as plt
import folium

author      = "stratos"
date        = "2024-4-6"
input_file  = "gps_data.csv"
output_file = date + "_" + author + "_map.html"

#combine columns date and time to create a datetime column named as date_time.
data        = pd.read_csv("data/" + input_file, parse_dates=[['date', 'time']], keep_date_col=True)

data        = data[data['date'] == date]
data
data["Points"] = list(zip(data.latitude, data.longitude)) 
data.describe()
points         = list(zip(data.latitude, data.longitude)) 
beg_point      = points[0]
end_point      = points[len(points)-1] 

max_spd_cods   = data.loc[data['speed'].idxmax()].Points
max_speed      = data.loc[data['speed'].idxmax()].speed
max_alt_cods   = data.loc[data['sat_altitude'].idxmax()].Points
max_altitude   = data.loc[data['sat_altitude'].idxmax()].sat_altitude
total_distance = round(data.iloc[-1].distance/1000,2)

first_observ = data['date_time'].iloc[0] 
last_observ = data['date_time'].iloc[-1]
total_seconds = (last_observ - first_observ).total_seconds()
total_hours = int(total_seconds/3600)
total_minutes = total_seconds/60
minutes = int(total_minutes%60)
seconds = int(total_seconds%60)

beg_tooltip       = "Beginning of the route"
end_tooltip       = "End of the route. Total distance: " + str(total_distance) + " km"
max_speed_tooltip = "Max speed, click to see"
max_alt_tooltip   = "Max altitude, click to see"

mymap = folium.Map(location=[ data.latitude.mean(), data.longitude.mean() ], zoom_start=12, tiles=None)

folium.TileLayer('openstreetmap', name='OpenStreet Map').add_to(mymap)

folium.PolyLine(points, color='red', weight=4.5, opacity=.5).add_to(mymap)

beg_icon = folium.Icon(color="green", icon="glyphicon-map-marker")
end_icon = folium.Icon(color="red",   icon="glyphicon-map-marker")

max_speed_icon = folium.Icon(color="blue", icon="glyphicon-plane")
max_alt_icon   = folium.Icon(color="blue", icon="glyphicon-star")


beginning = folium.Marker(location = beg_point, tooltip = beg_tooltip, icon = beg_icon).add_to(mymap)
max_spd   = folium.Marker(location = max_spd_cods, popup = max_speed, tooltip = max_speed_tooltip, icon = max_speed_icon).add_to(mymap)
max_alt   = folium.Marker(location = max_alt_cods, popup = max_altitude, tooltip = max_alt_tooltip, icon = max_alt_icon).add_to(mymap)
end       = folium.Marker(location = end_point, tooltip = end_tooltip, icon = end_icon).add_to(mymap)

mymap.save("data/" + output_file)
print("Map " + output_file + " created and saved!")

data['av_altitude'] = (data.sat_altitude + data.bar_altitude)/2


 
fig, ax = plt.subplots()
data.plot(x = 'date_time', y = 'speed',label = 'Speed (km/h)', ax = ax) 
data.plot(x = 'date_time', y = 'temperature',label = 'Temperature ($^\circ$C)', ax = ax)
#data.plot(x = 'Date', y = 'pos',label = 'Positive', ax = ax)  
data.plot(x = 'date_time', y = 'av_altitude', label = "Altitude (m)", ax = ax, secondary_y=True)  
plt.xticks(rotation=45)
plt.title("Total distance: " + str(total_distance) + "km" + ". Total time: " + str(total_hours) + ":" + str(minutes) + ":" + str(seconds) + " (H:M:S)")
plt.grid(True)
plt.xlabel("Datetime")
plt.show()

fig, ax = plt.subplots()
data.plot(x = 'date_time', y = 'voltage', ax = ax)  
plt.xticks(rotation=45)
plt.title("Total time: " + str(total_hours) + ":" + str(minutes) + ":" + str(seconds) + " (H:M:S)")
plt.xlabel("Datetime")
plt.ylabel("Voltage (V)")
plt.grid(True)
plt.show()

