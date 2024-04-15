library(fpp3)

## DAta from : https://www.rba.gov.au/statistics/historical-data.html

data <- read.csv('/Users/stratos/Documents/Rstudio_projects/AUDeuro/AUD_historical.csv')
data$Date <- as.Date(data$Date, format = "%Y-%m-%d")
data <- data %>% as_tsibble(index = Date, key = NULL)
#update_tsibble(index = day, regular=TRUE )

loss =28000*(data$EUR) - (28000*0.714)

data %>% autoplot(loss)
 data %>% autoplot(EUR)
data <- data %>% mutate(day=row_number())%>%
  update_tsibble(index = day, regular=TRUE )
tail(data)


data %>% gg_tsdisplay(difference(log(EUR)), plot_type = 'partial')

ar_fit <- data%>%
  model(ar = ARIMA(EUR),
        search=ARIMA(EUR, stepwise = FALSE, approx = FALSE), 
        
)
ar_fit

#ar_fit %>% pivot_longer(EUR, names_to="models", 
                        #values_to = 'orders')
        
length(data$day)

train_data <- data %>% filter(day <= 3436)
fit_1 <- train_data %>%
  model(#ANN = ETS(EUR ~ trend('A') ),
        ar = ARIMA(EUR, stepwise = FALSE, approx = FALSE),
        search=ARIMA(EUR, stepwise = FALSE, approx = FALSE), 
        )
report(fit_1)

#components(fit_1)%>%autoplot()

#ar_fit %>% select(ar) %>% gg_tsresiduals()

#fit_1 %>% select(ran) %>%gg_tsresiduals()

#fit_1%>% forecast(h=50)%>%autoplot()
#ar_fit%>% forecast(h=50)%>%autoplot()


recent_data <- data %>% filter(day > 3400)
pred_1 <- fit_1 %>% forecast(h=2, times = 50, bootstrap = TRUE) 
pred_1 %>% autoplot(recent_data, level = NULL)
print(pred_1)

tail(recent_data)

#report(ar_fit)

sim <- fit_1%>% select(ar) %>% generate(h = 3, times = 5, bootstrap = TRUE)
recent_data %>%
  ggplot(aes(x = day)) +
  geom_line(aes(y = EUR)) +
  geom_line(aes(y = .sim, colour = as.factor(.rep)),
            data = sim) +
  labs(title="AUD Exchange rate", y="Euro" ) +
  guides(colour = "none")


