Write-Host '|||||-----------Scrapping last news-----------|||||'

python.exe webscrap_bit_news.py
Write-Host "News scrapped wrote in txt."

sleep 0.5

Write-Host "Extracting sentiment..."

python.exe sia_model.py 

Write-Host "Sentiment extracted, added to database."

sleep 0.5 

Write-Host "Entering data analysis"

Write-Host "Pulling bitcoin close price and writing to csv"

Rscript.exe pull_bitcoin_values_to_csv.R

Write-Host "Price to csv written."


python.exe data_analysis.py