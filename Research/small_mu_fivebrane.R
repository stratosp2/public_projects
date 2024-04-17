#!/usr/bin/env Rscript





############# CHANGE THIS #################


# Smallest mu to use in linear extrapolation in 1/S

smallest_mu <- 0.5

plot_labels <- c("S2", "S5")

divide_sum_by <- 10


#options(error=traceback)

############# STOP CHANGING ###############






######### Get command arguments
args = commandArgs(trailingOnly=TRUE)

UGB <- args[1]
file_pattern <- args[2]
N_fit_power <- as.numeric(args[3])




######### Settings

source("r_GeneralSettings.R")


######### Function defines

source("r_GeneralFunctions.R")
source("r_ReadPhaseFilesToDataTable.R")
source("r_ErrorBars.R")

library(geometry) 


######### Start of program output

PrintScriptName("R fivebrane small mu extrapolation")

printf("Arguments: U/G(B)Nxx file_pattern_starting_at_D \n\n")

printf("e.g.: ./r_data_analysis.Rd GBN64S48T0.885 D9_F1_ \n\n")






file_pattern_read <- p("matrix_sizes_", UGB, "M*", file_pattern, "*.csv")



### Extract T
T <- as.numeric(gsub(".*T(.+)", "\\1", UGB))
printf("T extracted as %f\n\n", T)


### Extract S
S <- as.numeric(gsub(".*S(.+)T.*", "\\1", UGB))
printf("S extracted as %f\n\n", S)

### Extract N
N <- as.numeric(gsub(".*N(.+)S.*", "\\1", UGB))
printf("N extracted as %f\n\n", N)


printf("\n\nLoading read libraries... \n ")
library(data.table)
printf("\n")



printf("Looking for files in current directory matching %s\n\n", file_pattern_read)




files_to_read <- Sys.glob(file_pattern_read)

rbind(files_to_read)


printf("Found the following files:\n")
print(files_to_read)


printf("\n\nReading content into database:\n")


library(foreach)

all_data <- foreach(i=1:length(files_to_read), .combine=rbind) %do% {
  f <- as.character(files_to_read[i])
  printf("\tReading %s.\n", f)	
  tryCatch({
    try(fread(f, select = c(1,2,3,4,5,6,7,8,9,10,11))) # The sep="" command messes up fread, needs to be dropped. 
 
  }, warning = function(w) {
    warning(w)
  }, error = function(e) {
    stop(e)
  }, finally = {
  })
}
printf("\nDone reading.\n\n")


all_data <- na.omit(unique(data.frame(all_data[,"M"], all_data[,"S2"],all_data[,"error_S2"], all_data[,"S5"], all_data[,"error_S5"], all_data[,"comm2"],all_data[,"comm2_error"])))

print(all_data)


n_obs <- length(plot_labels)



mu_list <- all_data[,"M"]
printf("mu list extracted as\n")
mu_list <- as.numeric(mu_list)
print(mu_list)

M <- mu_list







#Load plot libraries 
printf("\n\nLoading plot library...  ")
invisible(library("ggplot2"))
invisible(library("dplyr"))






# Add M (inverse) column for linear fit

all_data <- as.data.frame(all_data)

#all_data$M <- 1 / all_data$S



# vector of linear fits
pred <- c()

# Get vector of standard ggplot colors to give to the linear fits
gg_color_hue <- function(n) {
  hues = seq(15, 375, length = n + 1)
  hcl(h = hues, l = 65, c = 100)[1:n]
}
cols = gg_color_hue(n_obs)



# linear function whose a,b values are set via args in statfunction
linear <- function(x, u.values){
  X <- cbind(1, x)
  return(X %*% u.values)
}



# create small mu extrapolation
  data_i<- all_data
  data_i <- subset(data_i, data_i$M>=smallest_mu)
  num_mu_fit <- length(mu_list[mu_list >= smallest_mu])
  print(num_mu_fit)
  print(data_i)
  
error_min_S2<- (data_i$S2-data_i$error_S2)
error_max_S2<- (data_i$S2+data_i$error_S2)
error_min_S5<- data_i$S5-data_i$error_S5
error_max_S5<- data_i$S5+data_i$error_S5

error_scale <- 10

comm_error_min <-  (data_i$comm2-error_scale*data_i$comm2_error)
comm_error_max <-  (data_i$comm2+error_scale*data_i$comm2_error) 



bar_width = 0.05

eq1<- function(x) 15/x+15*1/x^2


myplot <- ggplot(aes(x=M, y=S2, colour="S2"), data=data_i) +  theme_bw()+geom_point(aes(x=M,y=S5, colour="S5"), size=3)+geom_errorbar(aes(x=M, ymin=error_min_S2, ymax=error_max_S2, colour="S2"),width=0.01)+
  geom_errorbar(aes(x=M, ymin=error_min_S5, ymax=error_max_S5, colour="S5"),width=0.01)+
  labs(colour='spheres') + geom_point(size=3, alpha=1.0)+
  theme(legend.position="bottom") + xlab(expression(mu))+ylab("radius")+theme(text = element_text(size=15))+ theme(axis.text=element_text(size=13))



plot_file <- p("observable_small_mu_sphere_radii_", UGB, file_pattern, ".pdf")

suppressMessages(ggsave(myplot, file=plot_file))

myplot <- ggplot(aes(x=M, y=comm2), data=data_i) +
  theme_bw()+
  geom_errorbar(aes(x=M, ymin=comm_error_min, ymax=comm_error_max),width=0.01, colour="blue")+
  theme(legend.position="bottom") + xlab(expression(mu))+ylab("F^2")+theme(text = element_text(size=15))+ theme(axis.text=element_text(size=13))

plot_file_1 <- p("observable_small_mu_comm2_", UGB, file_pattern, ".pdf")
suppressMessages(ggsave(myplot, file=plot_file_1))

# for Large N F^2 
file_pattern_U_comm2 <- p("prod_broad_output_UNooCont_mean.csv")
file_pattern_G_comm2 <- p("prod_broad_output_GNooCont_mean.csv")
file_pattern_U_comm2_er <- p("prod_broad_output_UNooCont_error.csv")
file_pattern_G_comm2_er <- p("prod_broad_output_GNooCont_error.csv")

G_data <- read.csv(file_pattern_G_comm2)
U_data <- read.csv(file_pattern_U_comm2)


G_data_er <- read.csv(file_pattern_G_comm2_er)
U_data_er <- read.csv(file_pattern_U_comm2_er)

G_data_er<- subset(G_data_er, G_data_er[,"temp"]==T)
G_data_er <- unique(G_data_er)
U_data_er<- subset(U_data_er, U_data_er[,"temp"]==T)
U_data_er <- unique(U_data_er)


G_data<- subset(G_data, G_data[,"temp"]==T)
G_data <- unique(G_data)

U_data<- subset(U_data, U_data[,"temp"]==T)
U_data <- unique(U_data)

mu_list_1 <- unique(G_data[,"flux"])

  
M_1 <- mu_list_1


comm_2_largeGN <- unique(G_data[,"comm2"])
comm_2_largeUN <- unique(U_data[,"comm2"])

large_GN_data <- data.frame(M_1, comm_2_largeGN)
large_GN_data <- unique(subset(large_GN_data, M_1>=2))


large_UN_data <- data.frame(M_1, comm_2_largeUN)
large_UN_data <- unique(subset(large_UN_data, M_1>=2))

print(large_UN_data)

comm_2_largeGN_er <- unique(G_data_er[,"comm2"])
comm_2_largeUN_er <- unique(U_data_er[,"comm2"])


large_GN_data_er <- data.frame(M_1, comm_2_largeGN_er)
large_GN_data_er <- unique(subset(large_GN_data_er, M_1>=2))

w_G <- 1/large_GN_data_er 

large_UN_data_er <- data.frame(M_1, comm_2_largeUN_er)
large_UN_data_er <- unique(subset(large_UN_data_er, M_1>=2))

together_large_N_data <- data.frame(large_GN_data,comm_2_largeGN_er, comm_2_largeUN, comm_2_largeUN_er)

print(together_large_N_data)

y_min_G <- together_large_N_data$comm_2_largeGN-together_large_N_data$comm_2_largeGN_er
y_max_G <- together_large_N_data$comm_2_largeGN+together_large_N_data$comm_2_largeGN_er
y_min_U <- together_large_N_data$comm_2_largeUN-together_large_N_data$comm_2_largeUN_er
y_max_U <- together_large_N_data$comm_2_largeUN+together_large_N_data$comm_2_largeUN_er

print(y_min_G)

myplot_2 <-ggplot(together_large_N_data, aes(x=M_1, y=comm_2_largeGN))+
theme_bw()+geom_point(aes(x=M_1, y=comm_2_largeGN,shape="Gauged"), size=3)+
geom_point(aes(x=M_1, y=comm_2_largeUN,shape="Ungauged"), size=3)+
  geom_errorbar(aes(x=M_1, width=0.01,  ymin=y_min_G, ymax=y_max_G, shape="Gauged"), color="blue")+
  geom_errorbar(aes(x=M_1, width=0.01,  ymin=y_min_U, ymax=y_max_U, shape ="Ungauged"),color="blue")+
  ylab(expression(F^2))+xlab(expression(mu))+scale_shape_discrete(name ="")+
 theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))

plot_file_2 <- p("largeNS_comm2_for_T",T,".pdf")

suppressMessages(ggsave(width = 25, height = 15, unit="cm", myplot_2, file=plot_file_2))

log_reg <-lm(comm_2_largeGN ~ 1+M_1, data=large_GN_data)

summary(log_reg)
sum <-summary(log_reg)
LogF <- signif(coef(log_reg)[1], digits = 4)
LogM <- signif(coef(log_reg)[2], digits = 4)
LogD_err <- signif(coef(sum)[1, "Std. Error"] ,digits=3)
LogM_Er <- signif(coef(sum)[2,"Std. Error"], digits = 3)

log_log_eq <- function(x)LogF+LogM*x^(1)

myplot_3 <-ggplot(together_large_N_data, aes(x=M_1, y=comm_2_largeGN), add="reg.line")+
  theme_bw()+geom_point(aes(x=M_1, y=comm_2_largeGN,shape="Gauged"), size=3)+
  geom_point(aes(x=M_1, y=comm_2_largeUN,shape="Ungauged"), size=3)+
  stat_function(fun=log_log_eq, color="red")+
  geom_errorbar(aes(x=M_1, width=0.01,  ymin=y_min_G, ymax=y_max_G, shape="Gauged"), color="blue")+
  geom_errorbar(aes(x=M_1, width=0.01,  ymin=y_min_U, ymax=y_max_U, shape ="Ungauged"),color="blue")+
  ylab(expression(F^2))+xlab(expression(mu))+scale_shape_discrete(name ="")+
  theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))+scale_x_log10()+ 
  scale_y_log10()
  

plot_file_3 <- p("Log_Log_largeNS_comm2_for_T",T,".pdf")

suppressMessages(ggsave(width = 25, height = 15, unit="cm", myplot_3, file=plot_file_3))


