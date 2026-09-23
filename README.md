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

- **Bitcoin Sentiment analysis** — pedagogical study of whether daily news
  sentiment can predict Bitcoin's price/return. Scripts pull news via Google
  (since 2017), build a sentiment database, and analysis looks for
  correlations. *Not* a trading indicator. (Scripts only — the scraped news
  corpus and raw data are not committed.)
- **ETF_Portfolio** — constructs an ETF portfolio with statistically
  significant exposure to Fama–French factors (Mkt-RF, SMB, HML), benchmarked
  against the S&P 500 since February 2015.
- **Olives** — olive-yield forecasting for Nea Zichni, Greece: web-scraped
  cultivation data, a trained ML model and a FastAPI backend with a web
  frontend. Also lives as its own repo ([olives](https://github.com/stratosp2/olives),
  with a GitHub Pages frontend).

### Microcontrollers

- **weather_data_gsheets_esp32** — solar-powered ESP32 with temperature,
  humidity, pressure, rain and wind sensors. Stays in deep sleep, wakes hourly,
  takes measurements and logs to a Google Sheet.

### Research
R/Python scripts used in the production of scientific research that led to
published papers.

## Languages & tools

Python, R, C++ (Arduino/PlatformIO), shell — depending on the project.

## Note

The **Bitcoin Sentiment analysis** project ships scripts only; the scraped
news corpus and raw data are deliberately not committed to keep the repo
light.
