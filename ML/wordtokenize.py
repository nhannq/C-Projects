__author__      = "Nhan Nguyen"
import csv
import sys
import re
import string
import nltk
import json
from nltk.corpus import stopwords

def bigram_combine(text):
	try:
        	textarray = nltk.word_tokenize(text.strip().lower())
        	bigram = textarray[0].translate(None, string.punctuation)
        	endsentence = 0
        	result = ''
		duplicatedfstwordeliminate = 0
        	for word in textarray:
			if duplicatedfstwordeliminate > 0:
                		if word.translate(None, string.punctuation):
					if endsentence == 0:
                        			bigram = bigram + "-" + word.translate(None, string.punctuation)
                        			result = result + " " + bigram
                        			#print bigram
                        			bigram = word.translate(None, string.punctuation)
                			else:   
                        			bigram = word.translate(None, string.punctuation)
                        			endsentence = 0
                			if "." in word:
                        			endsentence = 1
				else:
					endsentence = 1
			duplicatedfstwordeliminate += 1
        	return result
	except Exception: 
  		pass

def trigram_combine(text):
	try:
        	textarray = nltk.word_tokenize(text.strip().lower())
        	stword = textarray[0].translate(None, string.punctuation)
		ndword = textarray[1].translate(None, string.punctuation)
		trigram = ''
        	endsentence = 0
        	result = ''
		duplicated2fstwordeliminate = 0
        	for word in textarray:
			if duplicated2fstwordeliminate > 1:
				if word.translate(None, string.punctuation):
                			if endsentence == 0:
                        			trigram = stword + "-" + ndword + "-" + word.translate(None, string.punctuation)
                        			result = result + " " + trigram
                        			#print trigram
						stword = ndword
						ndword = word.translate(None, string.punctuation)
                			elif endsentence == 1:
                        			stword = word.translate(None, string.punctuation)
                        			endsentence = 2
					else:	
						ndword = word.translate(None, string.punctuation)
						endsentence = 0
                			if "." in word:
                        			endsentence = 1
				else:
					endsentence = 1
			duplicated2fstwordeliminate += 1
        	return result
	except Exception: 
  		pass

count = 0;
csvfile = open(sys.argv[1],'rb');
pattern = re.compile(r'\b(' + r'|'.join(stopwords.words('english')) + r')\b\s*') #pattern to eliminate stopwords
outfile = open(sys.argv[2],'wb')
data = csv.reader(csvfile, delimiter=',')
output = csv.writer(outfile, delimiter=',', quotechar='"', quoting=csv.QUOTE_ALL)
for row in data:
	if row[0]:
		if not "-1" in row [3]:
#		print row[0];
		#	count += 1;
			stopwordselimiated = pattern.sub('', row[1])
			stopwordselimiated = stopwordselimiated.strip().lower()
			text = stopwordselimiated.translate(None, string.punctuation)
			count += len(text)
			bigrams = bigram_combine(stopwordselimiated)			
			if bigrams:
				text += bigrams
				count += len(bigrams)
			trigrams = trigram_combine(stopwordselimiated)
			if trigrams:
				text += trigrams
				count += len(trigrams)
			if row[3] == "0":
				output.writerow([row[0], text, row[2], "-1"]);
			else:
				output.writerow([row[0], text, row[2], row[3]]);
		
outfile.close()
		
	
print count
text = 'test whatever the n q f\' . d a fdsa. fd. d d e'
#print pattern.sub('', text)
#print text
bigram_combine(text)
