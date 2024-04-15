library("fpp3")
library("reshape2")


oil_data = read.csv("Fluorescence_olive_oil_dataset.csv")

oil_data%>%select(!Data) -> oil_data 

tail(oil_data)

virgin_data = oil_data%>%group_by(Quality)%>%summarise(no_samples = sum(Repetition), mean_acidity = mean(Acidity, na.rm = TRUE), mean_Faees = mean(FAEES, na.rm = TRUE), mean_K232 = mean(K232, na.rm = TRUE), mean_K270 = mean(K270, na.rm = TRUE), mean_PI = mean(Peroxide.Index, na.rm = TRUE) )
tail(virgin_data)

data_melted <- melt(virgin_data[c('Quality', 'mean_acidity', 'mean_Faees','mean_K232', 'mean_K270', 'mean_PI')],id.vars = 1)
data_melted


ggplot(data_melted, aes(x = variable, y = value, fill = Quality)) +  theme(text=element_text(size=15))+
  geom_bar(stat = 'identity', position = 'dodge') # position = stack shows one on top of the othr and their sum. It s more like a percentage.

