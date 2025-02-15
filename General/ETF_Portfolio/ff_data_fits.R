#function to get ff_data
get_us_ff_data <- function(){
  
  temp <- tempfile()
  name <- "F-F_Research_Data_5_Factors_2x3_CSV.zip"
  full_url <-  paste0("https://mba.tuck.dartmouth.edu/pages/faculty/ken.french/ftp/",name)
  download.file(full_url,temp,quiet = TRUE)
  US_five_Factors <- read_csv(unz(temp, "F-F_Research_Data_5_Factors_2x3.csv"), skip = 2)%>%rename(Date = 1) %>% 
    mutate_at(vars(-Date), as.numeric)%>%suppressWarnings()
  
  write.csv(US_five_Factors, file = "F-F_Research_Data_5_Factors_2x3.csv", row.names = F)
  file.remove(temp)
  return(US_five_Factors)
}

us_five_Factors <- get_us_ff_data()

get_eu_ff_data <- function(){
  
  temp <- tempfile()
  name <- "Europe_5_Factors_CSV.zip"
  full_url <-  paste0("https://mba.tuck.dartmouth.edu/pages/faculty/ken.french/ftp/",name)
  download.file(full_url,temp,quiet = TRUE)
  EU_five_Factors <- read_csv(unz(temp, "Europe_5_Factors.csv"), skip = 3)%>%rename(Date = 1) %>% 
    mutate_at(vars(-Date), as.numeric)%>%suppressWarnings()
  
  write.csv(EU_five_Factors, file = "Europe_5_Factors.csv", row.names = F)
  file.remove(temp)
  return(EU_five_Factors)
}

eu_five_Factors <- get_eu_ff_data()


get_dev_ff_data <- function(){
  
  temp <- tempfile()
  name <- "Developed_5_Factors_CSV.zip"
  full_url <-  paste0("https://mba.tuck.dartmouth.edu/pages/faculty/ken.french/ftp/",name)
  download.file(full_url,temp,quiet = TRUE)
  DEV_five_Factors <- read_csv(unz(temp, "Developed_5_Factors.csv"), skip = 3)%>%rename(Date = 1) %>% 
    mutate_at(vars(-Date), as.numeric)%>%suppressWarnings()
  
  write.csv(DEV_five_Factors, file = "Developed_5_Factors.csv", row.names = F)
  file.remove(temp)
  return(DEV_five_Factors)
}

dev_five_Factors <- get_dev_ff_data()


get_em_ff_data <- function(){
  
  temp <- tempfile()
  name <- "Emerging_5_Factors_CSV.zip"
  full_url <-  paste0("https://mba.tuck.dartmouth.edu/pages/faculty/ken.french/ftp/",name)
  download.file(full_url,temp,quiet = TRUE)
  EM_five_Factors <- read_csv(unz(temp, "Emerging_5_Factors.csv"), skip = 3)%>%rename(Date = 1) %>% 
    mutate_at(vars(-Date), as.numeric)%>%suppressWarnings()
  
  write.csv(EM_five_Factors, file = "Emerging_5_Factors.csv", row.names = F)
  file.remove(temp)
  return(EM_five_Factors)
}

em_five_Factors <- get_em_ff_data()

us_five_Factors%>% mutate(Date = as.yearmon(as.character(us_five_Factors$Date), "%Y%m"))%>%
  mutate(`us_Mkt-RF`=`Mkt-RF`/100,   us_SMB=SMB/100,  us_HML=HML/100, us_RMW=RMW/100,  us_CMA=CMA/100, us_RF=RF/100)%>%
  select(Date,`us_Mkt-RF`, us_SMB, us_HML, us_RMW, us_CMA, us_RF)-> us_fama_french_factors


eu_five_Factors%>% mutate(Date = as.yearmon(as.character(eu_five_Factors$Date), "%Y%m"))%>%
  mutate(`eu_Mkt-RF`=`Mkt-RF`/100,   eu_SMB=SMB/100,  eu_HML=HML/100, eu_RMW=RMW/100,  eu_CMA=CMA/100, eu_RF=RF/100)%>%
  select(Date,`eu_Mkt-RF`, eu_SMB, eu_HML, eu_RMW, eu_CMA, eu_RF)-> eu_fama_french_factors

dev_five_Factors%>% mutate(Date = as.yearmon(as.character(dev_five_Factors$Date), "%Y%m"))%>%
  mutate(`dev_Mkt-RF`=`Mkt-RF`/100,   dev_SMB=SMB/100,  dev_HML=HML/100, dev_RMW=RMW/100,  dev_CMA=CMA/100, dev_RF=RF/100)%>%
  select(Date,`dev_Mkt-RF`, dev_SMB, dev_HML, dev_RMW, dev_CMA, dev_RF)-> dev_fama_french_factors

em_five_Factors%>% mutate(Date = as.yearmon(as.character(em_five_Factors$Date), "%Y%m"))%>%
  mutate(`em_Mkt-RF`=`Mkt-RF`/100,   em_SMB=SMB/100,  em_HML=HML/100, em_RMW=RMW/100,  em_CMA=CMA/100, em_RF=RF/100)%>%
  select(Date,`em_Mkt-RF`, em_SMB, em_HML, em_RMW, em_CMA, em_RF)-> em_fama_french_factors


us_fama_french_factors%>%drop_na()%>%tail(5)
eu_fama_french_factors%>%drop_na()%>%tail(5)
dev_fama_french_factors%>%drop_na()%>%tail(5)
em_fama_french_factors%>%drop_na()%>%tail(5)


#create dataframe of etfs
df_full_etfs <- etfs%>%fortify.zoo %>%data.frame()
colnames(df_full_etfs) <- c("Date",colnames(etfs))

#merge etfs with fama french factors to run regressions
all_etfs_joined <- merge(x=df_full_etfs, y=us_fama_french_factors,  on = 'Date', all.x= F)#%>%merge(joined_data, on = "Date")
all_etfs_joined <- merge(x=all_etfs_joined, y=eu_fama_french_factors,  on = 'Date', all.x= F)
all_etfs_joined <- merge(x=all_etfs_joined, y=dev_fama_french_factors,  on = 'Date', all.x= F)
all_etfs_joined <- merge(x=all_etfs_joined, y=em_fama_french_factors,  on = 'Date', all.x= F)


factors <- merge(x = us_fama_french_factors, y = eu_fama_french_factors,  on = 'Date', all.x= F)%>%drop_na()
factors <- merge(x = factors, y = dev_fama_french_factors,  on = 'Date', all.x= F)
factors <- merge(x = factors, y = em_fama_french_factors,  on = 'Date', all.x= F)
#class(factors_xts)

#charts.PerformanceSummary(factors)

#pairs.panels(factors%>%as.data.frame())

n=ncol(port)

#make a loop that fits each Etf with ff5factor and store the factors and std in lists  
coeff <- c()
std <- c()
for (i in 1:n){
  sum <- summary(lm(all_etfs_joined[,colnames(port)[i]]-us_RF ~ `us_Mkt-RF` + us_SMB + us_HML+ us_RMW + us_CMA  , data = all_etfs_joined))
  if (colnames(port)[i] == "ZPRX.DE")
  {
    sum <- summary(lm(all_etfs_joined[,colnames(port)[i]]-eu_RF ~ `eu_Mkt-RF` + eu_SMB + eu_HML+ eu_RMW + eu_CMA  , data = all_etfs_joined))
  }
  if (colnames(port)[i] == "X5MVL.DE" | colnames(port)[i] == "IS3N.DE" | colnames(port)[i] == "EMSD.L")
  {
    sum <- summary(lm(all_etfs_joined[,colnames(port)[i]]-em_RF ~ `em_Mkt-RF` + em_SMB + em_HML+ em_RMW + em_CMA  , data = all_etfs_joined))
  }
  if (colnames(port)[i] == "IS3S.DE" | colnames(port)[i] == "EUNL.DE" | colnames(port)[i] == "AVDV")
  {
    sum <- summary(lm(all_etfs_joined[,colnames(port)[i]]-dev_RF ~ `dev_Mkt-RF` + dev_SMB + dev_HML+ dev_RMW + dev_CMA  , data = all_etfs_joined))
  }
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

coef_melted_data <- coef_df%>%pivot_longer(!Coefficients,names_to = "Ticker",values_to = "Values")

std_melted_data <- std_df%>%pivot_longer(!Coefficients,names_to = "Ticker",values_to = "Std")

merged_data <- merge(coef_melted_data, std_melted_data, on = c("Ticker", "Coefficients"))