library(quantmod)
library(PerformanceAnalytics)
library(PortfolioAnalytics)
library(DEoptim)
library(reshape)
library(ggplot2)
library(tidyverse)
library(ROI)
library(psych)
library(rootSolve)
library(data.table)


Ticks <- c("IUSQ.DE", "ZPRV.DE", "ZPRX.DE",  "DGSE.L")

since = "2015-02-01"

SPY <- ROC(Ad(to.monthly(getSymbols("^GSPC", auto.assign = F, from = since))), type = "discrete" )
SPY[is.na(SPY)] <- 0
colnames(SPY) <- "SPY"

#create a new environement
e <- new.env()
getSymbols(Ticks, env = e, from = since)


#create  monthly series
monthly <- eapply(e, function(x)to.monthly(x, name = names(x)))
port <- do.call(merge, lapply(monthly, Ad))
colnames(port) <- gsub(".Adjusted", "", names(port))
port <- ROC(port, type = "discrete")
port[is.na(port)] <- 0


etfs <- port



#cheeck correlations of etfs portfolio
pairs.panels(etfs%>%data.frame())
charts.PerformanceSummary(etfs, main = "ETF cumulative performance")


source("ff_data_fits.R")

ggplot(merged_data, aes(x = Coefficients , y = Values, color = Ticker)) + geom_point(size=5) +
  ggtitle("Fama French 3 factor coefficients regional adjusted.") + ylab("Values")+ 
  geom_errorbar(aes(x = Coefficients ,ymin=Values-Std, ymax=Values+Std, color = Ticker), width=0.2)




#transpose the dataframe except for the first row (intercept alpha) and last column (n+1)
coef_df_t <- coef_df[-1,-(n+1)]%>%t()
colnames(coef_df_t) <- coef_df[-1,]$Coefficients
coef_df_t
coef_matrix <- coef_df_t[,-c(4,5)]%>%as.matrix()#%>%unname()

#the betas we are after to fit
betas = coef_matrix
betas
lower <- c(0.5, 0.2, 0.1)
upper <- c(0.7, 0.4, 0.3)

#' Create portfolio object
pspec <- portfolio.spec(assets=Ticks)

#' Leverage constraint. Sum of weights is one
lev_constr <- weight_sum_constraint(min_sum=0.99, max_sum=1.01)

#' Box constraint, min and max weights
lo_constr <- box_constraint(assets=pspec$assets, min=c(0.05, 0.05, 0.05,0.05), max=0.65)

# Fama-French factor exposure constraint
exp_constr <- factor_exposure_constraint(assets=Ticks, B=betas, lower=lower, upper=upper)

#' Objective to minimize variance.
var_obj <- portfolio_risk_objective(name="var")

# Number of assets
pl_constr <- position_limit_constraint(assets=pspec$assets, min_pos=4)

#' Objective to maximize return.
ret_obj <- return_objective(name="mean")

#' Objective to minimize ETL.
etl_obj <- portfolio_risk_objective(name="ETL")

optb <- optimize.portfolio(R=etfs, portfolio=pspec, 
                           constraints=list(lev_constr,lo_constr, exp_constr,pl_constr), 
                           objectives=list(ret_obj, var_obj),  maxSR= T, maxSTARR=TRUE,#this we can decide to include or not.
                           optimize_method="DEoptim", search_size = 30000, maxiter=10000)

#optimal weights from optb
opt_weights <- optb$weights
print(opt_weights)
sum(opt_weights)

#manually add weights
etf_weights <- c(0.15, 0.2, 0.2, 0.45)
sum(etf_weights)

#create the portfolio returns based on optimal weights  
etf_port <- reclass(coredata(etfs)%*%opt_weights, match.to = etfs)
colnames(etf_port) <- "opt_returns"

df_etf <- etf_port%>%fortify.zoo %>%data.frame()
colnames(df_etf) <- c("Date","opt_returns")

#create the portfolio returns based on manual weights
etf_port_pre <- reclass(coredata(etfs)%*%etf_weights, match.to = etfs)
colnames(etf_port_pre) <- "man_returns"

#manually weighted df returns
df_etf_stand <- etf_port_pre%>%fortify.zoo %>%data.frame()
colnames(df_etf_stand) <- c("Date","man_returns")

# merging all returns (optimized, manual) and etfs with fama frenc factors for portfolio regression
port_ret_factors <- merge(x=all_etfs_joined, y=c(df_etf, df_etf_stand), on = 'Date', all.x= F)

# check some performance metrics
charts.PerformanceSummary(merge(etf_port_pre, etf_port, SPY))
charts.RollingPerformance(merge(etf_port_pre, SPY),  legend.loc = "topleft")
charts.RollingPerformance(etf_port_pre-SPY,  legend.loc = "topleft")


#since = "Jul 1963"

#Fama French 3 factors
#fama_french_factors%>%filter(Date >= since) -> subset_fama_french
#fff_zoo <- as.xts(subset_fama_french[, c("Mkt-RF", "SMB", "HML")], order.by = as.Date(subset_fama_french$Date))
#pairs.panels(fff_zoo%>%data.frame())

#charts.PerformanceSummary(fff_zoo)
#charts.RollingPerformance(fff_zoo, legend.loc = "topleft")

#Check contitional value at risk
CVaR(etfs)
CVaR(etf_port)
CVaR(etf_port_pre)
CVaR(SPY)


# run a regression for the 3 factors on either the optimized weights returns or manual weights returns 
us_factor_model <- lm(man_returns-us_RF ~ `us_Mkt-RF` + us_SMB + us_HML, data = port_ret_factors)
summary(us_factor_model)%>%print()
eu_factor_model <- lm(man_returns-eu_RF ~ `eu_Mkt-RF` + eu_SMB + eu_HML+  eu_RMW + eu_CMA, data = port_ret_factors)
summary(eu_factor_model)%>%print()
dev_factor_model <- lm(man_returns-dev_RF ~ `dev_Mkt-RF` + dev_SMB + dev_HML , data = port_ret_factors)
summary(dev_factor_model)%>%print()
em_factor_model <- lm(man_returns-em_RF ~ `em_Mkt-RF` + em_SMB + em_HML+em_RMW + em_CMA, data = port_ret_factors)
summary(em_factor_model)%>%print()


###################### Monte Carlo for portfolio #################################

#mean and stantard deviation of returns
which_port = etf_port_pre
colnames(which_port) <- "returns"
mn <- mean(etf_port)
std <- sd(etf_port) 


#create a normal distribution from these 
nd <- rnorm(10000, mn, std)
hist(nd, breaks = 300, 
     main = paste0("Histogram of portfolio returns, mean: ", round(mn, 4)*100,"%"),
     xlab = "Monthly Returns",
     col = "magenta"
)

#generate future expectations
set.seed(0)
nmonths <- 36
returns <- as.numeric(which_port$returns)
#sample to be Montme carlo. mean(paths) -> mean(returns) as n>>1. See documentation ?sample()
paths <- replicate(n=10000, 
                   expr = sample(returns, nmonths, replace = T)) # (.,x,.) x is the size of the sample and future

# pass the cumprod function for all n paths
paths <- apply(paths, 2, function(column) cumprod(1 + column)-1)

#prepare
paths <- data.table(paths)

paths$months <- 1:nrow(paths)
paths <- melt(paths, id.vars = "months")

MC_mean <- round(mean(paths$value),3)
MC_sd <- round(sd(paths$value),3)

mean_end <- round(mean(paths$value[paths$months== nmonths]),4)*100
sd_end <- round(sd(paths$value[paths$months== nmonths]),4)*100

# Probability to  have negative retults
mn_nq <- round(mean(paths$value[paths$months == nmonths]<0), 4)
mn_nq

#subset for negative values to calculate the mean
negative_values <- paths$value[paths$months == nmonths & paths$value < 0]

#mean of negative cummkulative returns
avg_neg <- negative_values%>%mean()%>%round(4)*100

#get the last nmonths returns of portfolio to calculate where we are. 
last_returns <- tail(which_port, nmonths)
months <- 1:nmonths

last_36_months <- data.frame(
  months = months,
  value = cumprod(1+last_returns)-1,
  variable = "last_36_months"
)

#current cummulative return of portfolio after nmonths 
current_cumulative = round(last_36_months$returns%>%tail(1),4)*100


ggplot(paths, aes(x = months, y = value, col = variable)) + geom_line() +theme_bw() + theme(legend.position = "none") +
  labs(x = "Months", y = "Cummulative returns", 
       title = paste("MC simulation for portfolio, end mean:",
                     mean_end, "%. P for negative cummulative returns:",
                     mn_nq*100,"% with avg loss:", abs(avg_neg),"%.","Bold line port returns:",current_cumulative,"%"))+
  geom_line(data = last_36_months, aes(x = months, y = returns), color = "black", size = 1.2)
#geom_line(last_returns$returns)


normal <- paths$value[paths$months == nmonths]

hist(normal, prob = T, breaks = 200, 
     main = paste("Histogram of cummulative returns after", nmonths, "months of portfolio, mean:", mean_end, ", sd:", sd_end),
     xlab = "Cummulative return", 
     col = "magenta")

x <- seq(min(normal), max(normal), length =100)
l <- dnorm(x, mean = mean(normal), sd = sd(normal))
sigma_m <- abline(v = mean(normal)-sd(normal), lwd =2)
sigma_p <- abline(v = mean(normal)+sd(normal), lwd =2)
lines(x,l, col = "black", lwd=2)

#mprobability to end more than 30%
mn_03 <- round(mean(paths$value[paths$months == nmonths]>=0.3),4)*100
mn_03
# Probability to have more than q cumulative returns at the end 
mn_q <- round(mean(paths$value[paths$months == nmonths]>0.5), 4)*100
mn_q

############################# End of Monte Carlo ####################################






##
## Calculate investments
##
df_inv <- which_port%>%fortify.zoo %>%data.frame()
colnames(df_inv) <- c("Date","returns")
total_invest = 10000

#add every mobth on portfolio
add = round(total_invest/length(df_inv$Date),0)

start = add

df_inv$cum_returns <- (cumprod(1+df_inv$returns)-1)%>%round(4)
df_inv$investments <- start
df_inv$exp_returns<- start

for (i in 1:length(df_inv$Date-1)) {
  df_inv[i+1, "investments"] <-  df_inv[i, "investments"]+add
  df_inv[i+1, "exp_returns"] <-  (df_inv[i, "exp_returns"]+add)*(1+df_inv[i+1,"returns"])
}

df_inv <- df_inv%>%drop_na()

df_inv$lump_sum <- total_invest*(1+df_inv$cum_returns)

avg_gain <- round(tail(df_inv$exp_returns)[6]-total_invest, 3)
ls_gain <- round(tail(df_inv$lump_sum)[6]-total_invest,3)
mkt_ls_gain <- round(tail(df_inv$market_lump_sum)[6]-total_invest,3)

investment_years <- time_length(difftime(df_inv$Date[1],df_inv$Date[length(df_inv$Date)]), "years")%>%abs()

months <- round((investment_years-round(investment_years,0))*12,2)

time_length(difftime(df_inv$Date[1],df_inv$Date[length(df_inv$Date)]), "months")%>%abs()

########
########
# Calculate returns by year and find geometric average for investement years
########
########
annual_etf_port <- data.frame(dates = df_inv$Date, monthly_returns = df_inv$returns)%>%group_by(year(dates)) %>%
  summarize(annual_return = prod(1 + monthly_returns) - 1)

colnames(annual_etf_port) <- c("Year", "Returns")

ggplot(annual_etf_port, aes(Year, Returns, fill = Returns))+geom_col()+
  scale_fill_gradient(low="red", high="green") +ggtitle("Yearly returns of our portfolio")+
  geom_text(aes(label = round(Returns*100,2)), vjust = -0.5)

end_value = 100*prod(1+annual_etf_port$Returns)


annualised_return = round((tail(df_inv$lump_sum)[6]/total_invest)^abs(1/investment_years)-1,4)*100


ggplot()+ geom_line(data = df_inv, aes(x=Date, y = exp_returns, color =paste0("Average dollar: ", avg_gain)))+
  geom_line(data = df_inv, aes(x=Date, y = lump_sum, color = paste0("Lump sum: ", ls_gain)))+theme_bw()+
  # geom_line(data = df_etf, aes(x=Date, y = market_lump_sum, color = paste0("SPY Lump sum: ", mkt_ls_gain)))+
  # geom_line(data = df_etf, aes(x=Date, y = investments, color = paste0("investment AVD")), linetype="dashed")+
  labs(x= "Date", y = "Money return", color = "Strategies and gains:") + 
  ggtitle(paste0("Lump sum vs average dollar strategies: ", round(investment_years,0), " years, and ", months, " months. Total invest ", total_invest, ", anualised return for lump sum: ", annualised_return,"%"))+
  theme(legend.position = "top")

