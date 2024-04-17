#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

# test if there is at least one argument: if not, return an error
if (length(args)==0) {
  stop("At least one argument must be supplied (input file).n", call.=FALSE)
} 
#else if (length(args)==1) {
# default output file
#  args[2] = "out.txt"
#}

# Additions needed: 
# - dtau files shoudl be displayed and plotted 







file_pattern_g <- args[1]
file_pattern_u <- args[2]

# Set output option to non-scientific four-digit
options("scipen"=100, "digits"=6)

# Define the column labels
column_labels <- c("temp", "flux", "energy", "s_trx2","comm2", "N")

plot_labels <- c("comm2")

#plot_labels <- c("CG_it", "energy", "Polyak", "s_trx2", "trx2(1)", "trx2(2)", "trx2(3)", "trx2(4)", "trx2(5)", "trx2(6)", "trx2(7)", "trx2(8)", "trx2(9)", "comm2","Myers","accept", "dt_x", "dt_a", "m5")



plot_labels_dtau <- c("dt_x", "dt_a")

column_labels_dtau <- c("itraj", "dt_x", "dt_a")


#temperatures <- c("0.1","0.12","0.14","0.16","0.18","0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1.0", "1.1", "1.2", "1.3", "1.4", "1.5")

temperatures <- c("0.1","0.12","0.16","0,18", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1.0", "1.1")




#fluxes <- c("2.0", "3.0", "4.0", "5.0")
fluxes <- c("2.0", "3.0", "4.0", "5.0")


# Import printf function 
printf <- function(...) cat(sprintf(...))

# Define paste function for multiple strings
p <- function(..., sep='') {
  paste(..., sep=sep, collapse=sep)
}


# Define joint database

database_labels <- c("temp","flux", "energy", "s_trx2","comm2","N")


# Create data frame to collect data from files and name the columns
database_mean_g <- data.frame(matrix(ncol = 8, nrow = 0))
database_sd_g <- data.frame(matrix(ncol = 8, nrow = 0))
colnames(database_mean_g) <- database_labels
colnames(database_sd_g) <- database_labels

database_mean_u <- data.frame(matrix(ncol = 8, nrow = 0))
database_sd_u <- data.frame(matrix(ncol = 8, nrow = 0))
colnames(database_mean_u) <- database_labels
colnames(database_sd_u) <- database_labels



# Start output
printf("\n\n\n|------------------------------------|\n| R gauged vs ungauged script: log plots |\n|------------------------------------|\n\n")

printf("Usage: e.g. arg1 = prod_broad_output_GNooCont, arg2 = prod_broad_output_UUooCont\n\n\n")

# Read Data from  file    





outfile_mean_g <- p(file_pattern_g, "_mean.csv")
outfile_sd_g <- p(file_pattern_g, "_error.csv")

outfile_mean_u <- p(file_pattern_u, "_mean.csv")
outfile_sd_u <- p(file_pattern_u, "_error.csv")



printf("Reading data from files \n    %s\n    %s\n\n\n", outfile_mean_g, outfile_sd_g)


## and to read this file back into R one needs
database_mean_g <- read.table(outfile_mean_g, header = TRUE, sep = ",", row.names = 1)
database_mean_u <- read.table(outfile_mean_u, header = TRUE, sep = ",", row.names = 1)


database_sd_g <- read.table(outfile_sd_g, header = TRUE, sep = ",", row.names = 1)
database_sd_u <- read.table(outfile_sd_u, header = TRUE, sep = ",", row.names = 1)






# Create Plots:

library("ggplot2")

library("dplyr")

#for weighted fit




printf("Creating plots\n")


colmax <- function(data) sapply(data, max, na.rm = TRUE)

colmin <- function(data) sapply(data, min, na.rm = TRUE)

# Autocorrelation correction for errors:
autoc_c=10.0



results<- data.frame(matrix(nrow=5,ncol=2))
colnames(results)<-c("mu","Slope")



for(fluxloop in fluxes) {
  for(plot_loop in plot_labels){
    plot_file <- p("log_largeN_plot_g_vs_u_", regmatches(file_pattern_g,gregexpr("(?<=G).*",file_pattern_g,perl=TRUE)), "_M", fluxloop, "_", plot_loop, ".pdf")
    printf("Plotting %s\n", plot_file)
    
    # Create plot dataframe
    
    plotlabel=p("large_cont_log_g_vs_u_", plot_loop, "    M = ", fluxloop)
    
    #distinct() to remove dublicate rows from readed data
    
    database_mean_flux_g <- distinct(database_mean_g[database_mean_g$flux==as.numeric(as.character(fluxloop)), ])
    database_sd_flux_g <- distinct(database_sd_g[database_sd_g$flux==as.numeric(as.character(fluxloop)), ])
    
    database_mean_flux_u <- distinct(database_mean_u[database_mean_u$flux==as.numeric(as.character(fluxloop)), ])
    database_sd_flux_u <- distinct(database_sd_u[database_sd_u$flux==as.numeric(as.character(fluxloop)), ])
    
    
    
    colnames(database_mean_flux_g) <- database_labels
    colnames(database_sd_flux_g) <- database_labels 
    
    colnames(database_mean_flux_u) <- database_labels
    colnames(database_sd_flux_u) <- database_labels 
    
    database_mean_flux_g <-distinct(database_mean_flux_g[database_mean_flux_g$temp %in% temperatures,])
    database_mean_flux_u <-distinct(database_mean_flux_u[(database_mean_flux_u$temp %in% temperatures & database_mean_flux_u$temp %in% database_mean_flux_g$temp),])
    
    database_sd_flux_g <-distinct(database_sd_flux_g[database_sd_flux_g$temp %in% temperatures,])
    database_sd_flux_u <-distinct(database_sd_flux_u[(database_sd_flux_u$temp %in% temperatures & database_sd_flux_u$temp %in% database_sd_flux_g$temp),])
    
    
    
    
    
    # Make temp and flux variables in dataframe from factor to numeric
    database_mean_flux_g <- mutate_all(database_mean_flux_g, function(x) as.numeric(as.character(x)))
    database_sd_flux_g <- mutate_all(database_sd_flux_g, function(x) as.numeric(as.character(x)))
    
    database_mean_flux_u <- mutate_all(database_mean_flux_u, function(x) as.numeric(as.character(x)))
    database_sd_flux_u <- mutate_all(database_sd_flux_u, function(x) as.numeric(as.character(x)))
    
    print(database_mean_flux_g)
    
    Log_E_1 <- log(abs(-database_mean_flux_g[,plot_loop]+database_mean_flux_u[,plot_loop]))
    Log_E <- abs(-database_mean_flux_g[,plot_loop]+database_mean_flux_u[,plot_loop])
    de <- database_mean_flux_g[,plot_loop]-database_mean_flux_u[,plot_loop]
    print(Log_E)
    
    # included the correlated term, i.e the covariance. But is it correct?
    
    sd <- sqrt((database_sd_flux_g[,plot_loop])^2+(database_sd_flux_u[,plot_loop])^2)
    
    
    log_sd <- 0.434*sd/(-database_mean_flux_g[,plot_loop]+database_mean_flux_u[,plot_loop])
    
    #y_min <- log(abs(-database_mean_flux_g[,plot_loop] + database_mean_flux_u[,plot_loop])-sd)
   
    y_min <- Log_E-sd
    y_max <- Log_E+sd
    #y_min <- Log_E-log_sd
    #y_max <- Log_E+log_sd
    
    #y_max <- log(abs(-database_mean_flux_g[,plot_loop]+database_mean_flux_u[,plot_loop])+ sd)
    
    T <- (database_mean_flux_g[,"temp"])^(-1)
    
    w <- abs(Log_E/log_sd^2)
    
    data <- data.frame(T, Log_E)
    
    if (plot_loop == "energy"){
      label <- p("E")
    }
    else if (plot_loop == "strx_2"){
      label <- p("R^2")
    }
    else {
      label <- p("F^2")
    }
    
    
    
    if (fluxloop == "1.0" ) {
      
      
      ggplot(data, aes(x=T,y=Log_E), add="reg.line") + theme_bw()
        geom_errorbar(aes(x=T, ymin=y_min, ymax=y_max), width=0.01, color ="blue")+
        geom_point(data=subset(data, T >= 0))+labs(title="N=oo, S=Cont,"~mu~"=1", x="1/T", y=Delta~"LogF^2/N^2")+ 
        geom_smooth(data=subset(data, T >= 0), method='lm', mapping = aes(weight = w),  color  ='red',se=T)+
        annotate("label", x = min(data$x)-0.5, y = min(data$y)-0.5, vjust = 5.0, hjust = 4, label =p("RSE=",rse), color="red", size = 4.0, parse=F, fill = "#FFCC66")
      
      #regression <- lm(Log_E ~ T, data <- subset(data, T >=5  & T<= 10), w=Log_E/log_sd^2)
      
      #sum <- summary(regression)
      
      print(sum)
      
      
      
    }
    
    
    else if (fluxloop == "2.0" ) {
      
      data <- subset(data, T>=2)
      
      regression <- lm(Log_E ~ T, data=subset(data,T>2 & T<10))
      
      sum <- summary(regression)
      
      rse<-signif(mean(sum$sigma), digits = 4)
      
      
      print(sum)
      print(w)
      
      LogD <- signif(coef(regression)[1], digits = 4)
      C <- signif(coef(regression)[2], digits = 4)
      LogD_err <- signif(coef(sum)[1, "Std. Error"] ,digits=3)
      C_Er <- signif(coef(sum)[2,"Std. Error"], digits = 3)
      eq <- function(x) LogD+C*x
      
      
      
      ggplot(data, aes(x=T,y=Log_E), add="reg.line") + theme_bw()+ xlim(1.8,5)+scale_y_log10()+
        geom_errorbar(aes(x=T, ymin=y_min, ymax=y_max), width=0.01, color ="blue")+
        geom_point(data=subset(data, T >2 &T<10 ))+geom_point(data=subset(data, T <=2 ), alpha=.5)+
        labs(title="N ="~infinity~", L ="~infinity~","~mu~"= 2", x="1/T", y=expression(Log~Delta~F^2/N^2))+ #stat_function(fun=eq)+
        geom_smooth(data=subset(data, T > 2 & T<10), method='lm', mapping = NULL,size=0.7, color  ='red',se=T)+
        theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))#+
      # annotate("label", x = min(data$x)-0.5, y = min(data$y)-0.5, vjust = 5.0, hjust = 4, label =p("RSE=",rse), color="red", size = 4.0, parse=F, fill = "#FFCC66")
      
      #+annotate("text", x = 3, y = 2, label = eq, color="red", size = 5, parse=FALSE)
      
      
      
    }
    
    else if (fluxloop == "3.0" ) {
      
      regression <- lm(Log_E ~ T, data <- subset(data, T >=0))
      
      sum <- summary(regression)
      rse<-signif(mean(sum$sigma), digits = 4)
      print(sum)
      
      LogD <- signif(coef(regression)[1], digits = 4)
      C <- signif(coef(regression)[2], digits = 4)
      LogD_err <- signif(coef(sum)[1, "Std. Error"] ,digits=3)
      C_Er <- signif(coef(sum)[2,"Std. Error"], digits = 3)
      
      eq <- function(x) LogD +C*x
      
      ggplot(data, aes(x=T,y=Log_E), add="reg.line") + theme_bw()+scale_y_log10()+
        geom_errorbar(aes(x=T, ymin=y_min, ymax=y_max), width=0.01, color ="blue")+
        geom_point(data=subset(data, T >= 0))+labs(title="N ="~infinity~", L ="~infinity~","~mu~"= 3", x="1/T", y=expression(Log~Delta~F^2/N^2))+ #stat_function(fun=eq)+
        geom_smooth(data=subset(data, T >= 0), method='lm', mapping = aes(weight = w),size=0.7,  color  ='red',se=T)+
        theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))#+
      # annotate("label", x = min(data$x)-0.5, y = min(data$y)-0.5, vjust = 5.0, hjust = 4, label =p("RSE=",rse), color="red", size = 4.0, parse=F, fill = "#FFCC66")
      
      
      
      
      
    }
    
    else if (fluxloop == "4.0" ) {
      
      print(database_mean_flux_g[,"flux"]) 
      
      regression <- lm(Log_E ~ T, data <- subset(data, T >=0), w=abs(Log_E/log_sd^2))
      
      sum <- summary(regression)
      rse<-signif(mean(sum$sigma), digits = 4)
      print(sum)
      
      LogD <- signif(coef(regression)[1], digits = 4)
      C <- signif(coef(regression)[2], digits = 4)
      LogD_err <- signif(coef(sum)[1, "Std. Error"] ,digits=3)
      C_Er <- signif(coef(sum)[2,"Std. Error"], digits = 3)
      
      eq <- function(x) LogD+C*x
      
      ggplot(data, aes(x=T,y=Log_E), add="reg.line") + theme_bw()+scale_y_log10()+
        geom_errorbar(aes(x=T, ymin=y_min, ymax=y_max), width=0.01, color ="blue")+
        geom_point(data=subset(data, T >= 0))+labs(title="N ="~infinity~", L ="~infinity~","~mu~"= 4", x="1/T", y=expression(Log~Delta~F^2/N^2))+ #stat_function(fun=eq)+
        geom_smooth(data=subset(data, T >= 0), method='lm', mapping = aes(weight = w),size=0.7,  color  ='red',se=T)+
        theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16)) #+
      #annotate("label", x = min(data$x)-0.5, y = min(data$y)-0.5, vjust = 5.0, hjust = 4, label =p("RSE=",rse), color="red", size = 4.0, parse=F, fill = "#FFCC66")
      
      
      
      
      
      
      #regression <- lm(Log_E ~ T, data <- subset(data, T >= 1), weights <- w)
      
      #print(T_limit)
      
      #coefficients <- coef(regression)
      
      #print (coefficients)
      
      
    }
    
    
    else { 
      
      regression <- lm(Log_E ~ T, data <- subset(data, T >= 0 ), w=abs(Log_E/log_sd^2))
      
      sum <- summary(regression)
      rse<-signif(mean(sum$sigma), digits = 4)
      print(sum)
      
      eq <- function(x) LogD+C*x
      
      LogD <- signif(coef(regression)[1], digits = 4)
      C <- signif(coef(regression)[2], digits = 4)
      LogD_err <- signif(coef(sum)[1, "Std. Error"] ,digits=3)
      C_Er <- signif(coef(sum)[2,"Std. Error"], digits = 3)
      
      
      ggplot(data, aes(x=T,y=Log_E), add="reg.line") + theme_bw()+scale_y_log10()+
        geom_errorbar(aes(x=T, ymin=y_min, ymax=y_max), width=0.01, color ="blue")+
        geom_point(data=subset(data, T >= 0))+labs(title="N ="~infinity~", L ="~infinity~","~mu~"= 5", x="1/T", y=expression(Log~Delta~F^2/N^2))+ #stat_function(fun=eq)+
        geom_smooth(data=subset(data, T >= 0), method='lm', mapping = aes(weight = w),size=0.7,  color  ='red',se=T)+
        theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16)) #+
      # annotate("label", x = min(data$x)-0.5, y = min(data$y)-0.5, vjust = 5.0, hjust = 4, label =p("RSE=",rse), color="red", size = 4.0, parse=F, fill = "#FFCC66")
      
      
      
      
      
      
      
      print ("finished")
      
      
    }
    
    
    
    ggsave(plot_file)
    
    
  }
  
  C_mu_file <- p("log_slope-mu.pdf")
  
  results[fluxloop,1] <- fluxloop
  results[fluxloop,2] <- C
  results[fluxloop,3] <- C_Er
  results <-na.omit(results)
}

print(results)

muvalue<- as.numeric(results$mu)
print(muvalue)
slope<- results$Slope
print(slope)
logD<- results$V3 


C_mu <- data.frame(muvalue,slope)

logfit <- lm(slope~muvalue, C_mu)

sum <- summary(logfit)
print(sum)
a <- signif(coef(logfit)[1], digits = 4)
b <- signif(coef(logfit)[2], digits = 4)

print(a)
print(b)
eq1 <- function(x) b-a*x


ggplot(C_mu, aes(x=results[,1],y=results[,2]),add="reg.line") + 
  geom_errorbar(aes(x=results[,1], width= 0.1,  ymin=results[,2]-results[,3], ymax=results[,2]+results[,3]), color ="blue") +
  geom_point(data=C_mu)+labs(title= "Large N, Continuous", x=expression(mu), y="C")+
  theme(text = element_text(size=15))+ theme(axis.text=element_text(size=13))#+stat_function(fun=eq1)



ggsave(C_mu_file)

exit









printf("done.\n\n\n")


