# public_projects

A collection of personal, mostly data-science projects spanning quantitative
finance, physics research, machine learning and hobbyist embedded systems.

Organised by area; each project folder carries its own `Project description.txt`
with a fuller write-up.

## Contents

### AI_on_batteries
Characterisation of battery behaviour — correlations, machine learning and
neural-network models applied to both real-world data and synthetic data
generated with PyBaMM (Python Battery Mathematical Modelling). Large data files
are excluded; representative scripts, key figures and a final report are
included.

### General

- **AUDeuro** — automated R script that pulls the AUD→EUR exchange rate, runs
  elementary econometric analysis and produces ARIMA forecasts.
- **Bitcoin Sentiment analysis** — pedagogical study of whether daily news
  sentiment can predict Bitcoin's price/return. A scrapper pulls news via
  Google (since 2017), scripts build a sentiment database, and analysis looks
  for correlations. *Not* a trading indicator.
- **ETF_Portfolio** — constructs an ETF portfolio with statistically
  significant exposure to Fama–French factors (Mkt-RF, SMB, HML), benchmarked
  against the S&P 500 since February 2015.
- **Olive_oil** — Python scrapper pulls olive-oil prices to CSV; an R script
  runs data analysis and basic price forecasts.
- **Online_Retail** — data analysis of an online retail dataset: revenue by
  country/month, top products, returns likelihood and a prediction model.

### Microcontrollers

- **GPS_logger** — battery-powered ESP32 with a GPS module, temperature/
  pressure sensor, TFT screen and SD module; records GPS and barometric data
  for meta-analysis and map creation.
- **weather_data_gsheets_esp32** — solar-powered ESP32 with temperature,
  humidity, pressure, rain and wind sensors. Stays in deep sleep, wakes hourly,
  takes measurements and logs to a Google Sheet.

### Research
R/Python scripts used in the production of scientific research that led to
published papers.

## Languages & tools

Python, R, C++ (Arduino/PlatformIO), shell — depending on the project.

## Note

The **Bitcoin Sentiment analysis** project contains a large historical news
corpus (`hist_news/`), which is why this repository is large.
