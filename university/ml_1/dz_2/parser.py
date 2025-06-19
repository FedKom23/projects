
"""
импортируем необходимые библиотеки
nltk.corpus-стопслова(междоиетия, местоимения, союзы, предлоги ...)
spacy-предобученная nlp модель для эмбеддинга слов из словаря (попсовая)
bs4-нахождение тегов в html коде
requests-интернет запросы
nltk- для токенезирования и определения частоты слов в тексте
"""
import nltk
from nltk.corpus import stopwords
import spacy
from bs4 import BeautifulSoup
import requests
import string
import re
import nltk
from nltk import word_tokenize
from nltk.probability import FreqDist
import pandas as pd

#переводим все символы в нижний регистр
lowercase_letters = string.ascii_lowercase
nltk.download('word_tokenize')
nltk.download('stopwords')
nlp_en = spacy.load("en_core_web_sm")
nlp_ru = spacy.load("ru_core_news_sm")
#массив где (исполинтель-кластер, кол-во песен)
artists = [("feduk", 31), ("og+buda", 33), ("lil+krystalll", 17),("morgenshtern", 28), ("big+baby+tape", 27), ("lil+morty", 15)]
columns = []
for i in range(480):
    columns.append(str(i)+"dim")
columns.append("artist")
#создаем пустой датафрейм для 480 пространств(т к одно слово в spacy = 96 размерный эмбеддинг) и столбец артист
df = pd.DataFrame(columns=columns)
for j in artists:
    #берем сайт с удобной(простой) html структурой
    url = "https://alloflyrics.cc/search/?s="+j[0]
    response = requests.get(url)
    soup = BeautifulSoup(response.text, "html.parser")
    #все ссылки идут с тегом a
    links = soup.find_all('a')
    mass = []
    for link in links:
        #делаем простую "регулярку" чтобы получать ссылки только песен и ставим исходный лимит из массива кортежей
        if link["href"][:5]!="/song":
            continue
        mass.append("https://alloflyrics.cc"+link["href"])
    for i in range(j[1]):
        response1 = requests.get(mass[i])
        soup1 = BeautifulSoup(response1.text, "html.parser")

        #находим текст по двум вложенным div классам с конечным тегом p
        paragraph = soup1.find('div', class_="jumbotron").find('div', class_='container').find('p')

        #получаем готовый текст
        text = paragraph.get_text()

        #убираем пунктуацию и заменяем перехолд новой строки на пробелы
        spec_chars = string.punctuation + '«»\t—…’'
        text = text[1:].lower()
        text = "".join([ch for ch in text if ch not in spec_chars])
        text = re.sub('\n', ' ', text)
        text = "".join([ch for ch in text if ch not in string.digits])

        #токенизируем текст
        text_tokens = word_tokenize(text)
        #убираем стопслова
        russian_stopwords = stopwords.words("russian")
        russian_stopwords += stopwords.words("french")
        text_tokens = [token.strip() for token in text_tokens if token not in russian_stopwords]
        text = nltk.Text(text_tokens)
        # считаем  частоту слов
        fdist_sw = FreqDist(text)
        #делаем запись превращаем слова в эмбеддинги, лобавляем в конец название артиста и добавляем запись в датафрейм
        res = []
        for n in fdist_sw.most_common(5):
            if n[0][0] in lowercase_letters:
                embedding = nlp_en(n[0]).vector
            else:
                embedding = nlp_ru(n[0]).vector
            res.extend(embedding)
        res.append(j[0])
        df = df._append(pd.Series(res, index=df.columns), ignore_index=True)
print(df)
df.to_csv('zad2.csv', index=False)
