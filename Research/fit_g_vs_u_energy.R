#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

# Reads with prod_broad_output_GNooNA

file_pattern_g <- args[1]
file_pattern_u <- args[2]

# Set output option to non-scientific four-digit
options("scipen"=100, "digits"=4)

# Define the column labels
column_labels <- c("temp", "flux", "energy", "s_trx2","comm2", "N")

plot_labels <- c("energy", "s_trx2", "comm2")


temperatures <- c("0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8","0.9","1.0", "1.1")

fluxes <- c( "2.0", "3.0", "4.0", "5.0")


# Import printf function 
printf <- function(...) cat(sprintf(...))

# Define paste function for multiple strings
p <- function(..., sep='') {
    paste(..., sep=sep, collapse=sep)
}


# Define joint database

database_labels <- c("temp","flux", "energy", "s_trx2","comm2","N")

# Create data frame to collect data from files and name the columns
database_mean_g <- data.frame(matrix(ncol = 7, nrow = 0))
database_sd_g <- data.frame(matrix(ncol = 7, nrow = 0))
colnames(database_mean_g) <- database_labels
colnames(database_sd_g) <- database_labels

database_mean_u <- data.frame(matrix(ncol = 7, nrow = 0))
database_sd_u <- data.frame(matrix(ncol = 7, nrow = 0))
colnames(database_mean_u) <- database_labels
colnames(database_sd_u) <- database_labels



# Start output
printf("\n\n\n|------------------------------------|\n| R gauged vs ungauged script: plots |\n|------------------------------------|\n\n")

printf("Usage: e.g. arg1 = therm_broad_output_GNooCont, arg2 = therm_broad_output_UNooCont\n\n\n")
            
# Read Data from  file    





outfile_mean_g <- p(file_pattern_g, "_mean.csv")
outfile_sd_g <- p(file_pattern_g, "_error.csv")
outfile_mean_u <- p(file_pattern_u, "_mean.csv")
outfile_sd_u <- p(file_pattern_u, "_error.csv")



printf("Reading data from files \n    %s\n    %s\n\n\n", outfile_mean_g, outfile_mean_u)
            
            
## and to read this file back into R one needs

database_mean_g <- read.table(outfile_mean_g, header = TRUE, sep = ",", row.names = 1)
database_mean_u <- read.table(outfile_mean_u, header = TRUE, sep = ",", row.names = 1)

database_sd_g <- read.table(outfile_sd_g, header = TRUE, sep = ",", row.names = 1)
database_sd_u <- read.table(outfile_sd_u, header = TRUE, sep = ",", row.names = 1)

# Create Plots:

library("ggplot2")

library("pracma")
library("dplyr")
library("splines")


printf("Creating plots\n")


colmax <- function(data) sapply(data, max, na.rm = TRUE)

colmin <- function(data) sapply(data, min, na.rm = TRUE)

# Autocorrelation correction for errors:
autoc_c=10.0



#take into account only the energies

results<- data.frame(matrix(nrow=6,ncol=11))
colnames(results)<-c("mu","Energy_Slope","Energy_Slope_er", "Energy_D", "Energy_D_er", "C1", "C1_error", "C2", "C2_error", "n", "n_error")

for(fluxloop in fluxes){
for(plotloop in "energy"){
	plot_file <- p("largeN_g_vs_u_M", fluxloop, "_", plotloop, ".pdf")

	printf("Plotting %s\n", plot_file)
	
	# Create plot dataframe
	plotlabel=p("g_vs_u_Noo", "M = ", fluxloop)
	


	database_mean_flux_g <- distinct(database_mean_g[database_mean_g$flux==as.numeric(as.character(fluxloop)), ])
	database_sd_flux_g <- distinct(database_sd_g[database_sd_g$flux==as.numeric(as.character(fluxloop)), ])
  database_mean_flux_u <- distinct(database_mean_u[database_mean_u$flux==as.numeric(as.character(fluxloop)), ])
	database_sd_flux_u <- distinct(database_sd_u[database_sd_u$flux==as.numeric(as.character(fluxloop)), ])

    
	database_mean_flux_g <-database_mean_flux_g[database_mean_flux_g$temp %in% temperatures,]
	database_sd_flux_g <-database_sd_flux_g[database_sd_flux_g$temp %in% temperatures,]
	database_mean_flux_u <-database_mean_flux_u[database_mean_flux_u$temp %in% temperatures,]
	database_sd_flux_u <-database_sd_flux_u[database_sd_flux_u$temp %in% temperatures,]
	
	
	colnames(database_mean_flux_g) <- database_labels
	colnames(database_sd_flux_g) <- database_labels 
	colnames(database_mean_flux_u) <- database_labels
	colnames(database_sd_flux_u) <- database_labels 
	
	


    # Make temp and flux variables in dataframe from factor to numeric
    database_mean_flux_g <- mutate_all(database_mean_flux_g, function(x) as.numeric(as.character(x)))
    database_sd_flux_g <- mutate_all(database_sd_flux_g, function(x) as.numeric(as.character(x)))
    
    database_mean_flux_u <- mutate_all(database_mean_flux_u, function(x) as.numeric(as.character(x)))
    database_sd_flux_u <- mutate_all(database_sd_flux_u, function(x) as.numeric(as.character(x)))




# Default fit from r package

D_E <-abs(database_mean_flux_u[,plotloop]-database_mean_flux_g[,plotloop])
D_E_2 <- (database_mean_flux_g[,plotloop]-database_mean_flux_u[,plotloop])


#Error scaled by
error_scale <-1

sd <- sqrt((error_scale*database_sd_flux_g[,plotloop])^2+(error_scale*database_sd_flux_u[,plotloop])^2)

#assign weights to data

w <- 1/sd^2

y_min <- D_E - sd

y_max <- D_E + sd

T <- (database_mean_flux_g[,"temp"])^(-1)


data <- data.frame(T, D_E, sd,w)


if(fluxloop == "1.0"){
	data <- subset(data, T >= 0)

exp.mod <- nls(D_E ~ D*exp(-C * T), data, weights = w,  start = list(C =0.5 , D=5))

sum <- summary(exp.mod)

print(sum)

C <- signif(coef(exp.mod)[1], digits = 4)
D <- signif(coef(exp.mod)[2], digits = 4)
C_Er <- signif(coef(sum)[1, "Std. Error"] ,digits=4)
D_Er <- signif(coef(sum)[2,"Std. Error"], digits = 4)


eq <- paste("D_E=",D ,"(", D_Er,")","*exp(-1/T*",C ,"(", C_Er,"))",sep="")


ggplot(data, aes(x=T,y=D_E), add="reg.line") + theme_bw()+
  geom_errorbar(aes(x=T, ymin=y_min, ymax=y_max), width=0.01, color ="blue")+
  geom_point(data=subset(data,  T>=0))+
  labs(title="N=oo, S=Cont,"~mu~"=1", x="1/T")+ geom_smooth(data=data, method="nls", size = 0.5, mapping = aes(weight = w), formula = y~D*exp(-C*x),method.args = list(start = list(C=0.5, D=5)),se=F,color="red")+
  ylab(expression(Delta~E/N^2))+ annotate("label", x = min(data$x), y = min(data$y), vjust = 3.0, hjust = 1.2, label = eq, color="red", size = 4.0, parse=FALSE, fill = "#FFCC66")

fluxloop_energies <- p("E_T_mu1.csv")


write.csv(format(data, digits=4), file = fluxloop_energies)

energies_error <- p("E_T_mu1_errors.csv")

write.csv(format(sd, digits=4), file = energies_error)


} # end of if mu==1


else if(fluxloop == "2.0"){
	
#weighted data
 data <- subset(data, T>0)

exp.mod <- nls(D_E ~ D*exp(-C * T), data=subset(data, T>2 & T<10), weights = w,   start = list(C = 1.0, D=15))
exp.mod_1 <- nls(D_E ~ C*(2+9*2/4)*exp(-C * T), data=subset(data, T>2),weights = w, start = list(C =1.0))
exp.mod_2 <- nls(D_E ~  (C*n*exp(-C * T)), data=subset(data, T>0),weights = w, start = list(C=1, n=1))

print(data)
sum <- summary(exp.mod)
sum_1 <- summary(exp.mod_1)
sum_2 <- summary(exp.mod_2)

rse<-signif(mean(sum$sigma), digits = 4)

print(chisq.test(data))

print(sum)
print(sum_1)
print(sum_2)

C <- signif(coef(exp.mod)[1], digits = 4)
D <- signif(coef(exp.mod)[2], digits = 4)
C_Er <- signif(coef(sum)[1, "Std. Error"], digits=4)
D_Er <- signif(coef(sum)[2,"Std. Error"], digits = 4)
C_2 <-  signif(coef(exp.mod_2)[1], digits = 4)
n <- signif(coef(exp.mod_2)[2], digits = 4)
C_2_Er <- signif(coef(sum_2)[1, "Std. Error"], digits=4)
n_Er <- signif(coef(sum_2)[2,"Std. Error"], digits = 4)

C1 <-signif(coef(exp.mod_1)[1], digits = 4)
C1_Er <- signif(coef(sum_1)[1, "Std. Error"], digits = 4)
print(C1)
print(C1_Er)


eq <- paste("D_E=",D ,"(", D_Er,")","*exp(-1/T*",C ,"(", C_Er,"))",sep="")
eq_1<-function(x) ((6*2/2)*exp(-2/2*x)+8*3*2/4*exp(-(3*2/4)*x)+3*2*exp(-2*x))


ggplot(data, aes(x=T,y=D_E)) + theme_bw()+ #xlim(1.8,8)+
  geom_errorbar(data=subset(data, T>=2),aes(x=T, ymin=y_min, ymax=y_max), width=0.01, color ="blue")+
  geom_point(mapping = NULL, data=subset(data, T<=2), alpha=0.5)+ 
  geom_point(mapping = NULL, data=subset(data, T>2))+
  labs(title= "N ="~infinity~", L ="~infinity~","~mu~"= 2", x="1/T", size=2.0)+
  ylab(expression(Delta~E/N^2))+geom_smooth(data=subset(data, T>2 & T<10),weights = w, method="nls", size = 1.2, formula = y~D*exp(-C*x),method.args = list(start = list(C=1, D=16)),se=F,aes(colour="two-parameter"))+
 stat_function(fun=eq_1, aes(colour="perturbative U(1)"))+
   theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))+
  scale_color_manual(name="Fits", values=c("orange", "red"))+
  theme(legend.position = c(.95, .95),
    legend.justification = c("right", "top"),
    legend.box.just = "right",
    legend.margin = margin(6, 6, 6, 6)
  )+theme(
    legend.box.background = element_rect(color="black", size=0.5),
    legend.box.margin = margin(5,5, 6, 6)
  )

fluxloop_energies <- p("E_T_mu2.csv")

write.csv(format(data, digits=4), file = fluxloop_energies)



} # end of if mu==2

else if(fluxloop == "3.0"){
	
	#weighted data 	
	data <- subset(data, T >0)

	print(data)
	
	modelfit <- mexpfit(data$T, data$D_E, p0=c(0.1, 0.1), const=FALSE)
	print(modelfit)
	
	
exp.mod <- nls(D_E ~ D*exp(-C * T), data=subset(data, T>=0),weights = w,  start = list(C =1, D=15))
exp.mod_1 <- nls(D_E ~  C*(2+9*3/4)*exp(-C * T), data=subset(data, T>=0),weights = w, start = list(C =1.0))
exp.mod_2 <- nls(D_E ~  (C*n*exp(-C * T)), data=subset(data, T>=0),weights = w, start = list(C=0.2, n=1))

sum <- summary(exp.mod)
sum_1<- summary(exp.mod_1)
sum_2<- summary(exp.mod_2)
print(chisq.test(data))

rse<-signif(mean(sum$sigma), digits = 4)

print(sum)
print(sum_1)
print(sum_2)



C <- signif(coef(exp.mod)[1], digits = 4)
D <- signif(coef(exp.mod)[2], digits = 4)
C_Er <- signif(coef(sum)[1, "Std. Error"] ,digits=4)
D_Er <- signif(coef(sum)[2,"Std. Error"], digits = 4)
C_2 <-  signif(coef(exp.mod_2)[1], digits = 4)
n <- signif(coef(exp.mod_2)[2], digits = 4)
C_2_Er <- signif(coef(sum_2)[1, "Std. Error"], digits=4)
n_Er <- signif(coef(sum_2)[2,"Std. Error"], digits = 4)

C1 <- signif(coef(exp.mod_1)[1], digits = 4)
C1_Er <- signif(coef(sum_1)[1, "Std. Error"] ,digits=4)



eq <- paste("D_E=",D ,"(", D_Er,")","*exp(-1/T*",C ,"(", C_Er,"))",sep="")
eq_1<-function(x)(6*(3/2)*exp(-3/2*x)+8*3*3/4*exp(-(3*3/4)*x)+3*3*exp(-3*x))


ggplot(data, aes(x=T,y=D_E)) + theme_bw()+
  geom_errorbar(aes(x=T, ymin=y_min, ymax=y_max), width=0.01, color ="blue")+
  geom_point(mapping = NULL, data=subset(data, T<=0), alpha=0.5)+ 
  geom_point(mapping = NULL, data=subset(data, T>0))+
  labs(title="N ="~infinity~", L ="~infinity~","~mu~"= 3", x="1/T")+
  ylab(expression(Delta~E/N^2))+
  geom_smooth(data=subset(data, T>=0),weights = w, method="nls", size = 1.2, formula = y~D*exp(-C*x),method.args = list(start = list(C=1, D=16)),se=F,aes(colour="two-parameter"))+
   stat_function(fun=eq_1, aes(colour="perturbative U(1)"))+
    theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))+
  scale_color_manual(name="fits", values=c("orange", "red"))+
  theme(legend.position = c(.95, .95),
        legend.justification = c("right", "top"),
        legend.box.just = "right",
        legend.margin = margin(6, 6, 6, 6)
  )+theme(
    legend.box.background = element_rect(color="black", size=0.5),
    legend.box.margin = margin(5,5, 6, 6)
  )


fluxloop_energies <- p("E_T_mu3.csv")

write.csv(format(data, digits=4), file = fluxloop_energies)



} #end of mu==3

else if (fluxloop =="4.0"){
	
	data <- subset(data, T > 0)

	print(data)
	
	modelfit <- mexpfit(data$T, data$D_E, p0=c(0.1, 0.1), const=FALSE)
	print(modelfit)
	
exp.mod <- nls(D_E ~ D*exp(-C * T) , data=subset(data, T>1.25), weights = w, start = list(C = 1, D=15))
exp.mod_1 <- nls(D_E ~  C*(2+9*4/4)*exp(-C * T), data=subset(data, T>1.25), weights = w, start = list(C =1.0))
exp.mod_2 <- nls(D_E ~  (C*n*exp(-C * T)), data=subset(data, T>0),weights = w, start = list(C=0.5, n=1))
exp.mod_3 <- nls(D_E ~ C*2*exp(-C * T)+D*6*exp(-D * T), data=subset(data, T>1.5),weights = w,   start = list(C = 0.7, D = 1.2))


sum <- summary(exp.mod)
sum_1 <- summary(exp.mod_1)
sum_2 <- summary(exp.mod_2)
sum_3 <- summary(exp.mod_3)
rse<-signif(mean(sum$sigma), digits = 4)

print(chisq.test(data))

print(sum)
print(sum_1)
print(sum_2)
print(sum_3)




C <- signif(coef(exp.mod)[1], digits = 4)
D <- signif(coef(exp.mod)[2], digits = 4)
C_Er <- signif(coef(sum)[1, "Std. Error"] ,digits=4)
D_Er <- signif(coef(sum)[2,"Std. Error"], digits = 4)
C_2 <-  signif(coef(exp.mod_2)[1], digits = 4)
n <- signif(coef(exp.mod_2)[2], digits = 4)
C_2_Er <- signif(coef(sum_2)[1, "Std. Error"], digits=4)
n_Er <- signif(coef(sum_2)[2,"Std. Error"], digits = 4)

C1 <- signif(coef(exp.mod_1)[1], digits = 4)
C1_Er <- signif(coef(sum_1)[1, "Std. Error"] ,digits=4)


eq <- paste("D_E=",D ,"(", D_Er,")","*exp(-1/T*",C ,"(", C_Er,"))",sep="")

eq_1<-function(x)(6*(4/2)*exp(-(4/2*x))+8*3*4/4*exp(-(3*4/4)*x)+3*4*exp(-4*x))


ggplot(data, aes(x=T,y=D_E)) + theme_bw()+#xlim(1.5,3)+
  geom_errorbar(aes(x=T, ymin=y_min, ymax=y_max), width=0.01, color ="blue")+
  geom_point(mapping = NULL, data=subset(data, T<=1.5), alpha=0.5)+ 
  geom_point(mapping = NULL, data=subset(data, T>1.5))+
  labs(title="N ="~infinity~", L ="~infinity~","~mu~"= 4", x="1/T")+
  ylab(expression(Delta~E/N^2))+geom_smooth(data=subset(data, T > 1.25),weights = w, method="nls", size = 1.2,  formula = y~D*exp(-C*x),method.args = list(start = list(C=1, D=16)),se=F,aes(colour="two-parameter"))+
   stat_function(fun=eq_1, aes(colour="perturbative U(1)"))+
 theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))+
  scale_color_manual(name="Fits", values=c("orange", "red", "blue"))+
  theme(legend.position = c(.95, .95),
        legend.justification = c("right", "top"),
        legend.box.just = "right",
        legend.margin = margin(6, 6, 6, 6)
  )+theme(
    legend.box.background = element_rect(color="black", size=0.5),
    legend.box.margin = margin(5,5, 6, 6)
  )


fluxloop_energies <- p("E_T_mu4.csv")

write.csv(format(data, digits=4), file = fluxloop_energies)


}# end of mu==4

else
{
#weighted data	
  data <- subset(data, T > 0)
  #data <- subset(data, T > 0)
  
  print(data)
  modelfit <- mexpfit(data$T, data$D_E, p0=c(0.1, 0.1), const=FALSE)
  print(modelfit)
  
  exp.mod <- nls(D_E ~ D*exp(-C * T), data=subset(data, T>0),weights = w,   start = list(C = 1, D=15))
  exp.mod_1 <- nls(D_E ~  C*(2+9*5/4)*exp(-C * T), data=subset(data, T>0), weights = w,  start = list(C =1.0))
  exp.mod_2 <- nls(D_E ~ C*n*exp(-C * T), data=subset(data, T>0),weights = w,   start = list(C = 1, n=1))
  exp.mod_3 <- nls(D_E ~ C*2*exp(-C * T)+D*6*exp(-D * T), data=subset(data, T>0),weights = w,   start = list(C = 1, D = 1.5))
  
  sum <- summary(exp.mod)
  sum_1 <- summary(exp.mod_1)
  sum <- summary(exp.mod)
  sum_2 <- summary(exp.mod_2)
  sum_3 <- summary(exp.mod_3)
  
  print(sum)
  print(sum_1)
  print(sum_2)
  print(sum_3)
  print(chisq.test(data))
  
  rse<-signif(mean(sum$sigma), digits = 4)
  
  C <- signif(coef(exp.mod)[1], digits = 4)
  D <- signif(coef(exp.mod)[2], digits = 4)
  C_Er <- signif(coef(sum)[1, "Std. Error"] ,digits=4)
  D_Er <- signif(coef(sum)[2,"Std. Error"], digits = 4)
  C_2 <-  signif(coef(exp.mod_2)[1], digits = 4)
  n <- signif(coef(exp.mod_2)[2], digits = 4)
  C_2_Er <- signif(coef(sum_2)[1, "Std. Error"], digits=4)
  n_Er <- signif(coef(sum_2)[2,"Std. Error"], digits = 4)
  
  C1 <- signif(coef(exp.mod_1)[1], digits = 4)
 C1_Er <- signif(coef(sum_1)[1, "Std. Error"] ,digits=4)
  
  
  eq <- paste("D_E=",D ,"(", D_Er,")","*exp(-1/T*",C ,"(", C_Er,"))",sep="")
  
  eq_1 <- function(x)((6*(5/2))*exp(-(5/2)*x)+8*3*5/4*exp(-(3*5/4)*x)+3*5*exp(-5*x))
  
  ggplot(data, aes(x=T,y=D_E)) + theme_bw()+ #xlim(1.5,2.5)+
    geom_errorbar(aes(x=T, ymin=y_min, ymax=y_max), width=0.01, color ="blue")+
    geom_point(mapping = NULL, data=subset(data, T<=0), alpha=0.5)+ 
    geom_point(mapping = NULL, data=subset(data, T>0))+
    labs(title="N ="~infinity~", L ="~infinity~","~mu~"= 5", x="1/T")+
    ylab(expression(Delta~E/N^2))+geom_smooth(data=subset(data, T>0), weights = w, method="nls", size = 1.2, formula = y~D*exp(-C*x),method.args = list(start = list(C=1.5, D=1.5)),se=F,aes(colour="two-parameter"))+
    stat_function(fun=eq_1, aes(colour="perturbative U(1)"))+
     theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))+
  scale_color_manual(name="Fits", values=c("blue","red", "blue"))+
    theme(legend.position = c(.95, .95),
          legend.justification = c("right", "top"),
          legend.box.just = "right",
          legend.margin = margin(6, 6, 6, 6)
    )+theme(
      legend.box.background = element_rect(color="black", size=0.5),
      legend.box.margin = margin(5,5, 6, 6)
    )
  
  fluxloop_energies <- p("E_T_mu5.csv")
  
  write.csv(format(data, digits=4), file = fluxloop_energies)
  
 
} # end ifs
ggsave(plot_file)
	



} # end of plotloop
  results[fluxloop,1] <- fluxloop
  results[fluxloop,2] <- C
  results[fluxloop,3] <- C_Er
  results[fluxloop,4]<- D
  results[fluxloop,5]<- D_Er
  results[fluxloop,6]<- C1
  results[fluxloop,7]<- C1_Er
  results[fluxloop,8]<- C_2
  results[fluxloop,9]<- C_2_Er
  results[fluxloop,10]<- n
  results[fluxloop,11]<- n_Er
  
  
 # print(results)  
} # end of fluxloop

w1<-results$C_Er
w2<-results$D_Er
w3<-results$C1_Er
w4<-results$C_2_Er
w5<-results$n_Er

poly <- y ~ poly(x,2)
poly1 <- y~ (1 +I(x^2)) 
linear <- y~x
nonlinear <- y~(1+I(x)+I(x^2))
C_mu_file <- p("slope_",plotloop,"-mu.pdf")
C_mu_1_file <- p("new_slope_",plotloop,"-mu.pdf")
C_mu_2_file <- p("new_slope_2",plotloop,"-mu.pdf")
D_mu_file <-p("D_",plotloop,"-mu.pdf")
n_mu_file <-p("degeneracies",plotloop,"-mu.pdf")
#remove NA
results<-na.omit(results)
results[,C]<-na.omit(results[,C])

flux<- as.numeric(fluxes)

dataC_mu <- data.frame(flux, results[,2])
dataC_mu_1 <- data.frame(flux, results[,6])
dataC_mu_2 <- data.frame(flux, results[,8])
data_n_mu <- data.frame(flux, results[,10])
model <- lm(results[,2] ~I(flux^2), dataC_mu, weights = w1)

model_1 <-lm(results[,6] ~ I(flux^2), dataC_mu_1, weights = w2)
model_3 <- lm(results[,8] ~ I(flux^2), dataC_mu_2,weights = w4)
model_4 <- lm(results[,10] ~ I(flux)+I(flux^2), data_n_mu,weights = w5)
sum <- summary(model)

sum_1 <- summary(model_1) 
sum_3 <- summary(model_3) 
sum_4 <- summary(model_4)

resc<-signif(mean(sum$sigma), digits = 4)

a <- signif(coef(model)[1], digits = 4)
b <- signif(coef(model)[2], digits = 4)

a_er <- signif(coef(sum)[1, "Std. Error"] ,digits=4)
b_er <- signif(coef(sum)[2, "Std. Error"] ,digits=4)
eq <-paste("C_0=",a,"(",a_er,")+",b,"(",b_er,")", expression(mu^2))

print(sum)

ggplot(dataC_mu, aes(x=flux,y=results[,2]),add="reg.line")+ theme_bw()+ xlim(0,5.5)+ ylim(0,3.5)+
  geom_errorbar(aes(x=flux, width= 0.1,  ymin=results[,2]-results[,3], ymax=results[,2]+results[,3]), color ="blue")+
  geom_point(data=dataC_mu) +  geom_point(aes(0, 0.83), color="red") +
  geom_errorbar(aes(x=0, ymin=0.83-0.21, ymax=0.83+0.21), width=0.01, color ="blue", hjust=1)+
  labs(title= "", x=expression(mu),y=expression(C[E]))+
  geom_smooth(method = "lm", size = 0.5, weights=w1, se = T, formula = poly1, colour = "red",fullrange = TRUE)+
  theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))


ggsave(C_mu_file)

print(sum_1)

a_1 <- signif(coef(model_1)[1], digits = 4)
b_1 <- signif(coef(model_1)[2], digits = 4)
a_3 <- signif(coef(model_3)[1], digits = 4)
b_3 <- signif(coef(model_3)[2], digits = 4)
a_er_3 <- signif(coef(sum_3)[1, "Std. Error"] ,digits=4)
b_er_3 <- signif(coef(sum_3)[2, "Std. Error"] ,digits=4)
a_er_1 <- signif(coef(sum_1)[1, "Std. Error"] ,digits=4)
b_er_1 <- signif(coef(sum_1)[2, "Std. Error"] ,digits=4)
eq_1 <-paste("C_0=",a_1,"(",a_er_1,")+",b_1,"(",b_er_1,")", expression(mu^2))
eq_C_2 <-paste("C_0=",a_3,"(",a_er_3,")+",b_3,"(",b_er_3,")", expression(mu^2))

ggplot(dataC_mu_1, aes(x=flux,y=results[,6]),add="reg.line")+ theme_bw()+ xlim(0,5.5)+ ylim(0,3.5)+
  geom_errorbar(aes(x=flux, width= 0.1,  ymin=results[,6]-results[,7], ymax=results[,6]+results[,7]), color ="blue")+
  geom_point(data=dataC_mu) + geom_point(aes(0, 0.83), color="red") +
  geom_errorbar(aes(x=0, ymin=0.83-0.21, ymax=0.83+0.21), width=0.01, color ="blue", hjust=1)+
  labs(title= "Large N, Continuous", x=expression(mu),y=expression(C[E]))+
  geom_smooth(method = "lm", size = 0.5,weights=w3, se = T, formula = poly1, colour = "red",fullrange = TRUE)+
  annotate("label", x = min(data$x), y = min(data$y), vjust = 3.0, hjust = 1.5, label = eq_1, color="red", size = 4.0, parse=F, fill = "#FFCC66")+
  theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))
ggsave(C_mu_1_file)


ggplot(dataC_mu_2, aes(x=flux,y=results[,8]),add="reg.line")+ theme_bw()+ xlim(0,5.5)+ ylim(0,3.5)+
  geom_errorbar(aes(x=flux, width= 0.1,  ymin=results[,8]-results[,9], ymax=results[,8]+results[,9]), color ="blue")+
  geom_point(data=dataC_mu_2) + geom_point(aes(0, 0.83), color="red") +
  geom_errorbar(aes(x=0, ymin=0.83-0.21, ymax=0.83+0.21), width=0.01, color ="blue", hjust=1)+
  labs(title= "Large N, Continuous", x=expression(mu),y=expression(C[E]))+
  geom_smooth(method = "lm", size = 0.5,weights=w4, se = T, formula = poly1, colour = "red",fullrange = TRUE)+
  annotate("label", x = min(data$x), y = min(data$y), vjust = 3.0, hjust = 1.5, label = eq_C_2, color="red", size = 4.0, parse=F, fill = "#FFCC66")+
  # annotate("label", x = min(data$x)-0.5, y = min(data$y)-0.5, vjust = 5.0, hjust = 4, label =p("RSE=",resc), color="red", size = 4.0, parse=F, fill = "#FFCC66")+
  theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))
ggsave(C_mu_2_file)

dataD <- data.frame(flux, results[,4])
modelD <- lm(results[,4] ~ I(flux^2), dataD, weights=w2)
sum <- summary(modelD)

resd<-signif(mean(sum$sigma), digits = 4)

a_d <- signif(coef(modelD)[1], digits = 4)
b_d <- signif(coef(modelD)[2], digits = 4)

a_d_er <- signif(coef(sum)[1, "Std. Error"] ,digits=4)
b_d_er <- signif(coef(sum)[2, "Std. Error"] ,digits=4)
eqd <-paste("D_0=",a_d,"(",a_d_er,")+",b_d,"(",b_d_er,")", expression(mu^2))



ggplot(dataD, aes(x=flux,y=results[,4]),add="reg.line")+ theme_bw()+
  geom_errorbar(aes(x=flux, width= 0.1,  ymin=results[,4]-results[,5], ymax=results[,4]+results[,5]), color ="blue")+
  geom_point(data=dataD)+ geom_point(aes(0, 1.59), color="red")+
  geom_errorbar(aes(x=0, ymin=1.59-0.51, ymax=1.59+0.51), width=0.01, color ="blue", hjust=1)+
  labs(title="", x=expression(mu),y=expression(D[E]))+
  geom_smooth(method = "lm", size =0.5, weights=w2, se = T, formula = poly1, colour = "red",fullrange = TRUE)+
   theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))

ggsave(D_mu_file)


a_n <- signif(coef(model_4)[1], digits = 4)
b_n <- signif(coef(model_4)[2], digits = 4)
a_n_er <- signif(coef(sum_4)[1, "Std. Error"] ,digits=4)
b_n_er <- signif(coef(sum_4)[2, "Std. Error"] ,digits=4)
print(sum_4)

eq_n <-paste("n_0=",a_n,"(",a_n_er,")+",b_n,"(",b_n_er,")", expression(mu))




ggplot(data_n_mu, aes(x=flux,y=results[,10]),add="reg.line")+ theme_bw()+
  geom_errorbar(aes(x=flux, width= 0.1,  ymin=results[,10]-results[,11], ymax=results[,10]+results[,11]), color ="blue")+
  geom_point(data=data_n_mu)+ geom_point(aes(0, 1.91), color="red")+
  geom_errorbar(aes(x=0, ymin=1.91-0.78, ymax=1.91+0.78), width=0.01, color ="blue", hjust=1)+
  labs(x=expression(mu),y="n")+
  geom_smooth(method = "lm", size =0.5, weights=w5, se = T, formula = nonlinear, colour = "red",fullrange = TRUE)+
  annotate("label", x = min(data$x), y = min(data$y), vjust = 3.0, hjust = 1.5, label =eq_n, color="red", size = 4.0, parse=F, fill = "#FFCC66")+
  annotate("label", x = min(data$x)-0.5, y = min(data$y)-0.5, vjust = 5.0, hjust = 4, label =p("RSE=",resd), color="red", size = 4.0, parse=F, fill = "#FFCC66")+
  theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))

ggsave(n_mu_file)



output_table_energies <- p("C_E.csv")

results["1",1]<- 0
results["1",2]<- 0.83
results["1",3]<- 0.21
results["1",4]<- 1.59
results["1",5]<- 0.51
results["1",6]<- 0
results["1",7]<- 0
results["1",10]<- results["1",4]/results["1",2]
results["1",11]<- 0.78
print(results)

write.csv(format(results, digits=4), file = output_table_energies)

exit





1



printf("done.\n\n\n")


