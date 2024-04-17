library("ggplot2")

setwd("/Users/stratos/Documents/projects/programm/mod")
# Define paste function for multiple strings
p <- function(..., sep='') {
  paste(..., sep=sep, collapse=sep)
}


energies <- read.csv("/Users/stratos/Documents/projects/programm/mod/C_E.csv", header = TRUE, sep = ",")
strx2 <- read.csv("/Users/stratos/Documents/projects/programm/mod/C_strx2.csv",header = TRUE, sep = ",")
comm2 <- read.csv("/Users/stratos/Documents/projects/programm/mod/C_comm2.csv",header = TRUE, sep = ",")

energies[5,] <- NA
energies <- na.omit(energies)

mu <- c(energies[,"mu"])

slope_E <- c(energies[,"Energy_Slope"])
slope_S <- c(strx2[,"strx2_Slope"])
slope_C <- c(comm2[,"comm2_Slope"])

slope_E_error <- c(energies[,"Energy_Slope_er"])
slope_S_error <- c(strx2[,"strx2_Slope_er"])
slope_C_error <- c(comm2[,"comm2_Slope_er"])

D_E <- c(energies[,"Energy_D"])
D_S <- c(strx2[,"strx2_D"])
D_C <- c(comm2[,"comm2_D"])

D_E_error <- c(energies[,"Energy_D_er"])
D_S_error <- c(strx2[,"strx2_D_er"])
D_C_error <- c(comm2[,"comm2_D_er"])

y_min_E <- slope_E-slope_E_error
y_max_E <- slope_E+slope_E_error
y_min_S <- slope_S-slope_S_error
y_max_S <- slope_S+slope_S_error
y_min_C <- slope_C-slope_C_error
y_max_C <- slope_C+slope_C_error

y_min_DE <- D_E-D_E_error
y_max_DE <- D_E+D_E_error
y_min_DS <- D_S-D_S_error
y_max_DS <- D_S+D_S_error
y_min_DC <- D_C-D_C_error
y_max_DC <- D_C+D_C_error

slope_file <- p("slope_observables.pdf")

data1 <- data.frame(observables=c(rep("E",4),rep("R",4), rep("F",4)),x=mu, y=c(slope_E,slope_S,slope_C))


ggplot(data1, aes(x=x, y=y, colour=observables))+geom_point(aes(shape=observables,colour=observables))+theme_bw()+scale_shape_manual(values = c(15, 17, 16) )+
  geom_errorbar(aes(x=x,width= 0.1,  ymin=c(y_min_E, y_min_S,y_min_C), ymax=c(y_max_E, y_max_S,y_max_C)))+
  xlab(expression(mu))+
  ylab(expression("C"))+ 
  theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))

ggsave(width = 20, height = 15, unit="cm",slope_file)

data2 <- data.frame(observables=c(rep("E",4),rep("R",4), rep("F",4)),x=mu, y=c(D_E,D_S,D_C))


D_file <- p("D_observables.pdf")

ggplot(data2, aes(x=x, y=y, colour=observables))+geom_point(aes(shape=observables, colour=observables))+theme_bw()+scale_shape_manual(values = c(15,17,16))+
  geom_errorbar(aes(x=x, width= 0.1,  ymin=c(y_min_DE, y_min_DS,y_min_DC), ymax=c(y_max_DE, y_max_DS,y_max_DC)))+
  xlab(expression(mu))+
  ylab("D")+ 
  theme(text = element_text(size=16))+ theme(axis.text=element_text(size=16))

ggsave(width = 20, height = 15, unit="cm", D_file)

print("done")


