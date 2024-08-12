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


Ticks <- c("VWCE.DE", "VUAA.DE",  "XMME.DE",  "ZPRX.DE")

since = "2020-02-01"

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

#function to get ff_data
get_ff_data <- function(){
  
  temp <- tempfile()
  name <- "F-F_Research_Data_5_Factors_2x3_CSV.zip"
  full_url <-  paste0("https://mba.tuck.dartmouth.edu/pages/faculty/ken.french/ftp/",name)
  download.file(full_url,temp,quiet = TRUE)
  five_Factors <- read_csv(unz(temp, "F-F_Research_Data_5_Factors_2x3.csv"), skip = 2)%>%rename(Date = `...1`) %>% 
    mutate_at(vars(-Date), as.numeric)%>%suppressWarnings()
  
  write.csv(five_Factors, file = "F-F_Research_Data_5_Factors_2x3.csv", row.names = F)
  file.remove(temp)
  return(five_Factors)
}

five_Factors <- get_ff_data()

five_Factors%>% mutate(Date = as.yearmon(as.character(five_Factors$Date), "%Y%m"))%>%
  mutate(`Mkt-RF`=`Mkt-RF`/100,   SMB=SMB/100,  HML=HML/100, RMW=RMW/100, CMA=CMA/100, RF=RF/100)-> fama_french_factors

fama_french_factors%>%drop_na()%>%tail(5)


#create dataframe of etfs
df_full_etfs <- etfs%>%fortify.zoo %>%data.frame()
colnames(df_full_etfs) <- c("Date",colnames(etfs))

#merge etfs with fama french factors to run regressions
all_etfs_joined <- merge(x=df_full_etfs, y=fama_french_factors, on = 'Date', all.x= F)#%>%merge(joined_data, on = "Date")
all_etfs_joined


n=ncol(port)

#make a loop that fits each Etf with ff5factor and store the factors and std in lists  
coeff <- c()
std <- c()
for (i in 1:n){
  sum <- summary(lm(all_etfs_joined[,colnames(port)[i]]-RF ~ `Mkt-RF` + SMB + HML+ RMW + CMA  , data = all_etfs_joined))
  coeff[[i]] <- c(sum$coefficients[,1])
  std[[i]] <- c(sum$coefficients[,2])
}

#extract coefficients and stds and create df to be plotted later.
coeff_list <- lapply(1:length(coeff), function(x) as.numeric(coeff[[x]]))
std_list <-  lapply(1:length(std), function(x) as.numeric(std[[x]]))

coef_df <- sapply(coeff, as.numeric)%>%data.frame()
colnames(coef_df) <- colnames(port)
coef_df$Coefficients <-c("Intercept","Mkt-RF", "SMB","HML","RMW","CMA")

std_df <- sapply(std, as.numeric)%>%data.frame()
colnames(std_df) <- colnames(port)
std_df$Coefficients <-c("Intercept","Mkt-RF", "SMB","HML","RMW","CMA")



#transpose the dataframe except for the first row (intercept alpha) and last column (n+1)
coef_df_t <- coef_df[-1,-(n+1)]%>%t()
colnames(coef_df_t) <- coef_df[-1,]$Coefficients
coef_df_t
coef_matrix <- coef_df_t[,-c(4,5)]%>%as.matrix()#%>%unname()

#the betas we are after to fit
betas = coef_matrix
betas
lower <- c(0.55, 0.2, 0.1)
upper <- c(0.7, 0.4, 0.3)

#' Create portfolio object
pspec <- portfolio.spec(assets=Ticks)

#' Leverage constraint. Sum of weights is one
lev_constr <- weight_sum_constraint(min_sum=0.99, max_sum=1.01)

#' Box constraint, min and max weights
lo_constr <- box_constraint(assets=pspec$assets, min=c(0.05, 0.05, 0.05, 0.05), max=0.65)

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
                           objectives=list(ret_obj),  maxSTARR=TRUE,#this we can decide to include or not.
                           optimize_method="DEoptim", search_size = 30000, maxiter=10000)
#manually add weights
etf_weights <- c(0.4, 0.3, 0.15, 0.15)
#optimal weights from optb
opt_weights <- optb$weights
print(opt_weights)

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
charts.PerformanceSummary(merge(etf_port_pre, SPY))
charts.RollingPerformance(merge(etf_port_pre, SPY),  legend.loc = "topleft")
charts.RollingPerformance(etf_port_pre-SPY,  legend.loc = "topleft")

#Check contitional value at risk
CVaR(etfs)
CVaR(etf_port)
CVaR(etf_port_pre)
CVaR(SPY)

# run a regression for the 3 factors on either the optimized weights returns or manual weights returns 
five_factor_model <- lm(man_returns-RF ~ `Mkt-RF` + SMB + HML, data = port_ret_factors)
summary(five_factor_model)


###################### Monte Carlo for portfolio #################################

#mean and stantard deviation of returns
mn <- mean(etf_port_pre)
std <- sd(etf_port_pre) 


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
returns <- as.numeric(etf_port_pre$man_returns)
#sample to be Montme carlo. mean(paths) -> mean(returns) as n>>1. See documentation ?sample()
paths <- replicate(n=5000, 
                   expr = sample(returns, nmonths, replace = T)) # (.,x,.) x is the size of the sample and future
# apply commulative sum in columns -> (.,2,.)
paths <- apply(paths, 2, cumsum)
#prepare
paths <- data.table(paths)
paths$months <- 1:nrow(paths)
paths <- melt(paths, id.vars = "months")

MC_mean <- round(mean(paths$value),3)
MC_sd <- round(sd(paths$value),3)

p_nq <- round(pnorm(q = 0, mean = MC_mean, sd = MC_sd),4)

# Mean of negative cumulative prod's?
mn_nq <- mean(paths$value[paths$months == nmonths]<0)
mn_nq

mean_end <- round(mean(paths$value[paths$months== nmonths]),4)

ggplot(paths, aes(x = months, y = value, col = variable)) + geom_line() + 
  theme_bw() + theme(legend.position = "none") +
  labs(x = "Months", y = "Cummulative returns", 
       title = paste("Monte Carlo for future returns, mean end:", mean_end*100,"%",
                     ". P for negative cummulative returns:",p_nq*100,
                     "%, average negative returns:-", mn_nq*100, "%"))

#mean of cum prods more than 30%
mn_03 <- mean(paths$value[paths$months == nmonths]>=0.3)
mn_03

hist(paths$value[paths$months== nmonths], breaks = 200, 
    main = paste("Histogram of cummulative returns after", nmonths, "months of portfolio. Mean:", mean_end*100,"%"),
     xlab = "Cummulative return", 
     col = "magenta")

#probability to be more than q. Note the lower.tail=F argument.
p_ps <- round(pnorm(q = 0.3, mean = MC_mean, sd = MC_sd, lower.tail = FALSE),4)
p_ps

############################# End of Monte Carlo ####################################






##
## Calculate investments
##
df_inv <- etf_port_pre%>%fortify.zoo %>%data.frame()
colnames(df_inv) <- c("Date","returns")
total_invest = 10000

#add every mobth on portfolio
add = round(total_invest/length(df_inv$Date),0)

start = add

df_inv$cum_returns <- cumsum(df_inv$returns)
df_inv$investments <- start
df_inv$exp_returns<- start

for (i in 1:length(df_inv$Date-1)) {
  df_inv[i+1, "investments"] <-  df_inv[i, "investments"]+add
  df_inv[i+1, "exp_returns"] <-  (df_inv[i, "exp_returns"]+add)*(1+df_inv[i+1,"returns"])
}

df_inv <- df_inv%>%drop_na()

df_inv$lump_sum <- total_invest*(1+df_inv$cum_returns)

length(df_inv$Date)
df_inv$market_lump_sum <- total_invest*(1+cumsum(SPY$SPY))

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
  scale_fill_gradient(low="red", high="green") +ggtitle("Yearly returns of our portfolio")

prod(1+annual_etf_port$Returns)

end_value = 100*prod(1+annual_etf_port$Returns)

# writing the function
f1 = function(x) {
  100*(x^abs(investment_years)) - end_value
}
roots = uniroot.all(f1, c(0, 3))

annualised_return = round(100*(roots-1),2)

ggplot()+ geom_line(data = df_inv, aes(x=Date, y = exp_returns, color =paste0("Average dollar: ", avg_gain)))+
  geom_line(data = df_inv, aes(x=Date, y = lump_sum, color = paste0("Lump sum: ", ls_gain)))+theme_bw()+
  # geom_line(data = df_etf, aes(x=Date, y = market_lump_sum, color = paste0("SPY Lump sum: ", mkt_ls_gain)))+
  # geom_line(data = df_etf, aes(x=Date, y = investments, color = paste0("investment AVD")), linetype="dashed")+
  labs(x= "Date", y = "Money return", color = "Strategies and gains:") + 
  ggtitle(paste0("Lump sum vs average dollar strategies: ", round(investment_years,0), " years, and ", months, " months. Total invest ", total_invest, ", anualised return ", annualised_return,"%"))+
  theme(legend.position = "top")

