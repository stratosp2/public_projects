library("fpp3")
library("patchwork")
library("quantmod")
library("PerformanceAnalytics")

y_data <- getSymbols("AUDEUR=X", auto.assign = F)

y_data <- Ad(y_data)%>%na.omit()
y_data$Return <- dailyReturn(y_data$`AUDEUR=X.Adjusted`)
colnames(y_data) <-c("Ad_Close", "Return")
tail(y_data)

xts_data <- y_data#%>%select(c("daily_return"))

xts_data <- na.omit(xts_data)


tail(xts_data)

charts.PerformanceSummary(xts_data$Return)


n_bins = 800
hist(xts_data$Return,breaks = n_bins)

mu <- mean(xts_data$Return)
sigma <-sd(xts_data$Return)
mu
sigma

target <- 0.01
z <- (target - mu)/sigma
z 

pnorm(q=z, lower.tail = F)

data %>% gg_tsdisplay(difference(log(EUR)), plot_type = 'partial')


last_day <- length(data$day)
last_day
train_data <- data %>% filter(day <= last_day)

fit_1 <- train_data %>%
  model(#ANN = ETS(EUR ~ trend('A') ),
        ar = ARIMA(EUR, stepwise = FALSE, approx = FALSE),
        #search=ARIMA(EUR, stepwise = FALSE, approx = FALSE), 
        )
report(fit_1)

fit_1 %>% select(ar)%>%gg_tsresiduals()
augment(fit_1) %>%
  filter(.model == "ar") %>%
  features(.innov, ljung_box, lag=60, dof = 0) 

horizon = 5

recent_data <- data %>% filter(day > 3400)
pred_1 <- fit_1 %>% forecast(h=horizon, times = 100, bootstrap = TRUE) 
pred_1 %>% autoplot(recent_data) + ggtitle(paste("Forecast for the next", horizon , "days"))
print(pred_1)

ggsave("predictions_ARIMA.pdf",
  plot = last_plot())

tail(recent_data)


sim <- fit_1%>% select(ar) %>% generate(h = 15, times = 50, bootstrap = TRUE)
recent_data %>%
  ggplot(aes(x = day)) +
  geom_line(aes(y = EUR)) +
  geom_line(aes(y = .sim, colour = as.factor(.rep)),
            data = sim) +
  labs(title="AUD Exchange rate simulations", y="Euro" ) +
  guides(colour = "none")


