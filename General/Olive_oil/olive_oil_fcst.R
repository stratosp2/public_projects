library("fpp3")
library("zoo")
library("scales")
library("patchwork")
library("quantmod")
library("PerformanceAnalytics")

data <-read.csv('Olive_oil_data.csv')
data <- subset(data, select = c(Date, Price))
head(data)


data$Date <- as.Date(data$Date)

data$year_week <- yearweek(as.Date(data$Date))

data <- data%>%distinct(year_week, .keep_all=TRUE)

#sum(duplicates(data, index=Date))\
sub_data <- data%>%select(c("Price"))
dates <- as.Date(data$Date)

length(dates)
xts_data <- xts(x = sub_data, order.by = dates)

xts_data$Returns <- dailyReturn(xts_data$Price)

charts.PerformanceSummary(xts_data$Returns)


cond_risk <- CVaR(xts_data$Returns, p = 0.99, method='historical')
cond_risk 


ts_data <- data %>% as_tsibble(index = year_week, key = NULL, regular = TRUE)
ts_data <- fill_gaps(ts_data) 
na.locf(ts_data, fromLast = TRUE)-> ts_data

p1 <- ts_data%>%autoplot(Price)


return <- diff(log(ts_data$Price))
ts_data<- ts_data[-1,]
ts_data$Return <- return
tail(ts_data)

#seasonal plot of prices. It is done before transformed to weeks later on. Works when fill_gaps in tsibble. 
ts_data %>% gg_season(Price, max_col_discrete = 10, period = "year") +ggtitle("Seasonal price of olive oil. Checks patterns")+
  ylab("Price/kg")+xlab("Date") +theme_bw()

ts_data %>% gg_season(Return, max_col_discrete = 10, period = "year") +ggtitle("Seasonal returns of olive oil. Checks patterns")+
  ylab("Price/kg")+xlab("Date") +theme_bw()

tail(ts_data)

ts_data %>% filter(Date >= "2023-01-25") %>% autoplot(Price)



ts_data %>% gg_tsdisplay(difference(difference(Price)), plot_type = 'partial')

last_datetime <- tail(ts_data$Date, n=1)
report_datetime <- tail(ts_data$Date, n=5)
#last_datetime
#last_datetime <- "2023-10-09 08:00:00"
train_data_past <- ts_data %>% filter(Date <=report_datetime[1])
train_data <- ts_data %>% filter(Date <= last_datetime)
tail(train_data)

horizon = "10 weeks"

fit_past <- train_data_past %>%
  model(#ANN = SNAIVE(Temp ~ trend('A') ),
    ar = ARIMA(Price, stepwise = TRUE, approx = TRUE)#,
    #manual_1= ARIMA(Price~fourier(K=1)+PDQ(1,0,0)),
    #manual_2= ARIMA(Price~fourier(K=2)+PDQ(1,0,0))
    #manual_3= ARIMA(Price~fourier(K=3)+PDQ(1,0,0)),
    #manual_4= ARIMA(Price~fourier(K=4)+PDQ(1,0,0)),
    #manual_5= ARIMA(Price~fourier(K=5)+PDQ(1,0,0)),
    #anual_6= ARIMA(Price~fourier(K=6)+PDQ(1,0,0))
    #search=ARIMA(Temp, stepwise = TRUE, approx = TRUE)
  )

recent_data <- ts_data %>% filter(Date > "2023-01-01")
pred_past <- fit_past%>%select(ar) %>% forecast(h=horizon, times = 50, bootstrap = TRUE) 
pred_past %>% autoplot(recent_data)



fit <- train_data %>%
  model(#ANN = SNAIVE(Temp ~ trend('A') ),
    ar = ARIMA(Price, stepwise = TRUE, approx = TRUE)#,
    #manual_1= ARIMA(Price~fourier(K=1)+PDQ(1,0,0)),
   # manual_2= ARIMA(Price~fourier(K=2)+PDQ(1,0,0))
    #manual_3= ARIMA(Price~fourier(K=3)+PDQ(1,0,0)),
    #manual_4= ARIMA(Price~fourier(K=4)+PDQ(1,0,0)),
    #manual_5= ARIMA(Price~fourier(K=5)+PDQ(1,0,0)),
    #anual_6= ARIMA(Price~fourier(K=6)+PDQ(1,0,0))
    #search=ARIMA(Temp, stepwise = TRUE, approx = TRUE)
  )
fit%>%select(ar)%>%report(fit)

glance(fit)%>% arrange(AICc) %>% select(.model:BIC)

#fit %>% select(manual_2)%>%gg_tsresiduals()
fit %>% select(ar)%>%gg_tsresiduals()

augment(fit) %>%
  filter(.model == "ar") %>%
  features(.innov, ljung_box, lag=104, dof=2) 

#recent_data <- data %>% filter(Date > "2023-01-01")
pred <- fit%>%select(ar) %>% forecast(h=horizon, times = 50, bootstrap = TRUE) 
pred %>% autoplot(recent_data)+ggtitle(paste("Olive oil prediction for the next",horizon)) + xlab("Week of the year") + ylab("Price/Kg (Euros)")
pred
