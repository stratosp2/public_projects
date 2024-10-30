library("fpp3")
library("zoo")
library("PerformanceAnalytics")
library("quantmod")
library("RCurl")
library("data.table")
library("patchwork")
library("palettes")  

#pull_data <- getURL("https://query1.finance.yahoo.com/v7/finance/download/BTC-USD?period1=1410912000&period2=1701388800&interval=1d&events=history&includeAdjustedClose=true")

#using quantmod package pulls data from yahoo
full_data <- getSymbols("BTC-EUR", scr = "yahoo" , from = '2014-09-17', to = Sys.Date(),auto.assign = F)
full_data$`BTC-EUR.ret` <- dailyReturn(full_data$`BTC-EUR.Adjusted`)
full_data

cond_risk <- CVaR(full_data$`BTC-EUR.ret`, p = 0.95, method='historical')
cond_risk



data <- data.frame(Date=index(full_data),coredata(full_data))


colnames(data) <- c("Date","Open", "High", "Low", "Close", "Volume", "Adjusted", "Ad_return")

tail(data)
sub_data <- data%>%filter(Date>"2023-10-01")
chartSeries(sub_data)
addMACD()
addBBands()

charts.PerformanceSummary(full_data$`BTC-EUR.ret`)


write.csv(data, file ="BTC-EUR.csv")