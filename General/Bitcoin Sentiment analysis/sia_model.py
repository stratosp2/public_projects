from nltk.sentiment import SentimentIntensityAnalyzer
from tqdm.notebook import tqdm
import pandas as pd 
from datetime import date,datetime

sia = SentimentIntensityAnalyzer()


def fin_dictionary():
    dict = {}
    dict['pump'] = 2.5
    dict['rally'] = 2.5
    dict['rises'] = 2.5
    dict['rise'] = 2.5
    dict['climbs'] = 2.5
    dict['peaks'] = 3
    dict['peak'] = 3
    dict['dropped'] = -2
    dict['drops'] = -2
    dict['falls'] = -2
    dict['fall'] = -2
    dict['falled'] = -2
    dict['regains'] = 2
    dict['momentum'] = -2
    dict['breaches'] = 1
    dict['explodes'] = 3.5
    dict['soared'] = 1.5
    dict['soares'] = 1.5
    dict['hits'] = 1
    dict['dips'] = -1
    dict['dipped'] = -1
    dict['edges'] = 0.2
    dict['above'] = 0.5
    dict['is up'] = 0.5
    dict['up'] = 0.5
    
    return dict


sia.lexicon.update(fin_dictionary())

labels = list(sia.polarity_scores("I feel ok that bitcoin went down a bit").keys()) #take the keys from dict and make another list



previous_data = pd.read_csv("btc_hist_feeling.csv")
sdate = previous_data.loc[len(previous_data["Date"])-1, "Date"]
sdate = pd.to_datetime(sdate).date() + pd.Timedelta(days=1) # take the last date from dataframe and shift one, to start from the next day
edate = datetime.now().date()
date_list = pd.date_range(sdate-pd.Timedelta(days=1),edate,freq='d')

mean_scores_list = []
for Date in date_list:
    temp_date = Date.strftime("%Y-%m-%d")
    temp_date
    Date.date()
    file_name = "hist_news/news_"+temp_date+"_.txt"
    temp_file = open(file_name, "r")
    data = temp_file.read()
    data = data.replace('...', '').replace(':', '').replace('\n','').split(". ")
    len(data)
    temp_file.close()
    data
    len(data)

    score_list = []
    for i in range(0,len(data)):
        text = data[i]
        scores = list(sia.polarity_scores(text).values()) #take the values as returned, and make a list of them
        score_list.append(scores)
    
    df = pd.DataFrame(score_list, columns = labels)
    mean_neg = round(df['neg'].mean(),5)
    mean_neu = round(df['neu'].mean(),5)
    mean_pos = round(df['pos'].mean(),5)
    mean_comp = round(df['compound'].mean(),5)

    mean_scores_list.append([Date.date(),mean_neg, mean_neu, mean_pos, mean_comp])
    mean_scores_list
    labels 
    data = pd.DataFrame(mean_scores_list, columns = ['Date', 'neg', 'neu', 'pos', 'compound'])
    type(data)
    #data['Date']=pd.to_datetime(data['Date']).date()

print("Concating dataframes")
conc_data = pd.concat((previous_data,data), ignore_index=True)
conc_data.to_csv("btc_hist_feeling.csv", index = False)
print("Done")