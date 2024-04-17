#!/usr/bin/env Rscript





############# CHANGE THIS #################


# Smallest S to use in linear extrapolation in 1/S
smallest_N <- 10

smallest_S <- 30

plot_labels <- c("energy", "Polyak", "s_trx2")

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
library(MASS) 



######### Start of program output

PrintScriptName("R observable simulatenous large N large S extrapolation")

printf("Arguments: U/G(B) file_pattern_starting_at_T \n\n")

printf("e.g.: ./r_data_analysis.Rd GB T0.885M0D9_F1_ \n\n")






file_pattern_read <- p("observables_", UGB, "*", file_pattern, "*.csv")


### Extract T
T <- as.numeric(gsub(".*T(.+)M.*", "\\1", file_pattern))
printf("T extracted as %f\n\n", T)



printf("\n\nLoading read libraries... \n ")
library(data.table)
printf("\n")






printf("Looking for files in current directory matching %s\n\n", file_pattern_read)

# Get list of files to include in data
#files_out <- list.files(pattern=file_pattern_read, full.names=TRUE, recursive=FALSE)



files_to_read <- Sys.glob(file_pattern_read)

printf("Found the following files:\n")
print(files_to_read)


printf("\n\nReading content into database:\n")

library(foreach)

all_data <- foreach(i=1:length(files_to_read), .combine=rbind) %do% {
        f <- as.character(files_to_read[i])
        printf("\tReading %s.\n", f)	
        tryCatch({
            try(fread(f, select = c(1,2,3,4,5))) # The sep="" command messes up fread, needs to be dropped. 
            ### Extract Nc and add to N_list
            #Nc <- as.numeric(gsub(".*N(.+)S.*", "\\1", f))
            #printf("Nc extracted as %d\n\n", Nc)
            #Nc_list <- c(Nc_list, Nc)
        }, warning = function(w) {
            warning(w)
        }, error = function(e) {
            stop(e)
        }, finally = {
        })
    }

print(all_data)

printf("\nDone reading.\n\n")



# Remove all observables not contained in plot_labels
all_data <- all_data[observable %in% plot_labels]

# divide s_trx2 by divide_sum_by to fit into plot
all_data[observable %in% "s_trx2"][, 2] <- all_data[observable %in% "s_trx2"][, 2] / divide_sum_by

n_obs <- length(plot_labels)


#N_list <- c()
#unique_S <- unique(all_data[ ,get("S")])
#print(unique_S)
#for(i in 1:length(unique_S)) {
#    print(N_list)
#    N_list = c(N_list, unique_S[i])
#}

#print(N_list)
#print(unique_S)

print(all_data)

N_list <- unique(all_data[ ,get("N")])
S_list <- unique(all_data[ ,get("S")])
printf("N list extracted as\n")
#N_list <- as.numeric(N_list)
print(N_list)


printf("S list extracted as\n")
print(S_list)



N_ticks <- as.character(N_list)

S_ticks <- as.character(S_list)



#N_ticks <- sapply(N_ticks, function(x) paste0("1/", x), USE.NAMES=FALSE)

N_ticks <- c(expression(infinity), N_ticks)
#printf("S list for fitting:\n")
#print(N_ticks)
N_list <- as.numeric(N_list)

# Make P into string so we have a discrete legend
#all_data$P <- as.character(all_data$P)

#print(all_data)





#Load plot libraries 
printf("\n\nLoading plot library...  ")
invisible(library("ggplot2"))
invisible(library("dplyr"))






# Add Ninv (inverse) column for linear fit

all_data <- as.data.frame(all_data)

all_data$Ninv <- 1 / all_data$N

all_data$Sinv <- 1 / all_data$S

all_data$var <- all_data$error^2






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


all_data$predicted <- NA




# create large S extrapolation
for(i in 1:n_obs) {
	data_i <- subset(all_data, all_data[,"observable"] == plot_labels[i])
    data_i <- subset(data_i, data_i$N>=smallest_N)

    #print(data_i)
    print(plot_labels[i])



    ## Simple linear model
    model <- rlm(value ~ Ninv + Sinv, data = data_i, weights = var)
    #print(model)
    print(summary(model))
    
  


    res_by_error <- summary(model)$residuals / data_i$error
    printf("\nResiduals weighted by errors: %f \n", (sum(res_by_error * res_by_error))^0.5 )
    print(res_by_error)
    printf("\n\n")

    print(model$coefficients)


    model$coefficients[1] + model$coefficients[2]*all_data[all_data$observable == plot_labels[i], ]$Ninv + model$coefficients[3]*all_data[all_data$observable == plot_labels[i], ]$Sinv

    all_data[all_data$observable == plot_labels[i], ]$predicted <- model$coefficients[1] + model$coefficients[2]*all_data[all_data$observable == plot_labels[i], ]$Ninv + model$coefficients[3]*all_data[all_data$observable == plot_labels[i], ]$Sinv



    if(plot_labels[i] == "energy") {
        ## More complicated model:
      print("2D fit follows")
        model2Func <- function(Ninv,Sinv,a,b,c,d){a + b*Ninv + c*Sinv + d*Ninv*Sinv}

        model2 <- nls(value~model2Func(Ninv,Sinv,a,b,c,d), data=data_i, start=list(a=0.0,b=0.0,c=0.0, d=0.0))
        print(summary(model2))
        
       two_d_function <-function(x) 0.322 +11.192*(-4.438) +8.397*x - 509*(-4.438)*x 
       ggplot()+stat_function(fun=two_d_function)+ ylab("E")+xlab("L")
         #scale_x_continuous(expand = c(0, 0), limits = c(-2 * bar_width, max(1 / S_list)*1.1), breaks=c(0.0, S_list^(-1)), labels=S_ticks)
       ggsave("2D fit.pdf")
       # model3Func <- function(Ninv,Sinv,a,b,c,d,e,f){a + b * Ninv + c * Sinv + d * Ninv * Sinv + e * Ninv * Ninv + f * Sinv * Sinv}

        #model3 <- nls(value~model3Func(Ninv,Sinv,a,b,c,d,e,f), data=data_i, start=list(a=0.0,b=0.0,c=0.0, d=0.0, e=0.0, f=0.0))
       # print(summary(model3))
    }
    

}

#print(data_i)

all_data$predicted_normalized <- (all_data$predicted - all_data$value) / all_data$error



all_data <- subset(all_data, all_data$S<999)
all_data <- subset(all_data, all_data$N<999)

x_label <- "L"

bar_width = 1/max(S_list)/10
#print(Nc_list)
#print(Nc_list^N_fit_power)

#wo_d_model <- function(x)(0.248+1.917/16^2+11.398*x-84.578/(16^2)*x)





#print(all_data)

ggplot(aes(x=Sinv, y=Ninv, color=predicted_normalized), data=subset(all_data, all_data[,"observable"] == "energy")) +
    geom_point(aes(size = (predicted_normalized*predicted_normalized)^0.25)) +
    scale_colour_gradient2() + 
    scale_x_continuous(breaks=S_list^(-1), labels=S_list) + 
    scale_y_continuous(breaks=N_list^(-1), labels=N_list) + 
    theme_bw() + 
    theme(legend.position="bottom")




#exit




    num_N_fit <- length(N_list[N_list >= smallest_N])
   # print(num_N_fit)
   # print(data_i)


	# Calculate regression coefficients
    if(num_N_fit == 2) {
        b <- cov(data_i$value, data_i$Ninv)/var(data_i$Ninv)
        a <- mean(data_i$value)-b*mean(data_i$Ninv)
        #print(a)
        #print(b)
        xb <- mean(data_i$Ninv)
        dady <- 1/num_N_fit-xb*(data_i$Ninv-xb)/dot(data_i$Ninv-xb, data_i$Ninv-xb)
        da <- (dot(dady^2, (data_i$error)^2))^0.5
    } else if(num_N_fit > 2) {
        # We do weighted linear regression for 3 or more data points
        sum_inv_error2 <- sum(data_i$error^-2)
        gen_cov <- dot(data_i$Ninv/data_i$error, data_i$value/data_i$error)-sum(data_i$Ninv/data_i$error^2)*sum(data_i$value/data_i$error^2)/sum_inv_error2
        gen_var <- dot(data_i$Ninv/data_i$error, data_i$Ninv/data_i$error)-sum(data_i$Ninv/data_i$error^2)^2/sum_inv_error2
        b <- gen_cov / gen_var
        a <- sum(data_i$value/data_i$error^2-b*data_i$Ninv/data_i$error^2)/sum_inv_error2
        #print(a)
        #print(b)
        dbdy <- (data_i$Ninv-sum(data_i$Ninv/data_i$error^2)/sum_inv_error2) / gen_var / data_i$error^2
        dady <- 1/sum(data_i$Ninv/data_i$error^2)*(data_i$Ninv/data_i$error^2-sum(data_i$Ninv^2/data_i$error^2)*dbdy)
        da <- (dot(dady^2, data_i$error^2))^0.5
    } else {
        stop("Less than two data poitns for linear fit.")
    }


exit



    extrapolated_data <- data.frame(observable=plot_labels[i], value=a, error=da, N=999, Ninv=0)
    
    all_data <- rbind(all_data, extrapolated_data)
    
    print(extrapolated_data)

	pred <- c(pred, stat_function(value =1, fun = linear, color=cols[i], args=list(u.values =c(a, b))))
	#print(pred)
	





all_data$c=as.character(all_data$observable)
#print(all_data)

#for(i in 1:(nrow(all_data)/10)) {
#    all_data[i*10, "c"] <- p("sum/", divide_sum_by)
#}

all_data[all_data[, "observable"] == "s_trx2","c"] <- p("s_trx2/", divide_sum_by)



#all_data$c="blank"
# Add color column to dataframe
#for(i in 1:nrow(all_data)) {
#    if(i %% 10 == 0) {
#
#    } else {
#        all_data[i, "c"] <-
#    }
#}
#all_data$c=p("trx2(", as.character(all_data$matrix), ")")

#print(all_data)


x_label <- "N"

bar_width = 1/max(N_list)/10
#print(Nc_list)
#print(Nc_list^N_fit_power)

myplot <- ggplot(aes(x=Ninv, y=value, color=c), data=all_data) +
            scale_x_continuous(expand = c(0, 0), limits = c(-2 * bar_width, max(1 / N_list)*1.1), breaks=c(0.0, N_list^(-1)), labels=N_ticks) + 
            theme_bw() + 
            geom_point(size=2, alpha=1.0) + 
            xlab(x_label) + 
            ylab(p("observable")) +
            theme(text = element_text(size=20)) +
            theme(axis.text=element_text(size=15)) + 
            pred +
            #geom_errorbar(aes(x=jitter(all_data[,"Ninv"]), ymin=(all_data[,"value"]-all_data[,"error"]), ymax=(all_data[,"value"]+all_data[,"error"])), width=bar_width) + 
            geom_errorbar(aes(x=all_data[,"Ninv"], ymin=(all_data[,"value"]-all_data[,"error"]), ymax=(all_data[,"value"]+all_data[,"error"])), width=bar_width) + 
            geom_vline(xintercept=smallest_N^(-1)*1.05, linetype="dashed", color = "black", size=0.5) +
            theme(aspect.ratio=1) + 
            theme(legend.position="bottom") +
            labs(color=expression(paste(""))) 

### Additional options
#+geom_smooth(method=lm, fullrange=TRUE) # adds linear regression with error ranges from multiple data points

plot_file <- p("observable_large_N_", UGB, file_pattern, ".pdf")

suppressMessages(ggsave(myplot, file=plot_file))



