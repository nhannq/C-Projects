__author__      = "Nhan Nguyen"
import csv
import sys
import re
import string
import nltk
import math
import json
import collections
from nltk.corpus import stopwords

num_docs = 0
term_num_docs = {}     # term : num_docs_containing_term
term_ids = {}
num_terms = 1
idf_default = 1.5

def add_input_review(input):
   # """Add terms in the specified document to the idf dictionary."""
	global num_docs
	global term_num_docs
	global term_ids
	global num_terms
	num_docs += 1
	words = set(nltk.word_tokenize(input))
	#print words
    	for word in words:
      		if word in term_num_docs:
        		term_num_docs[word] += 1
      		else:
        		term_num_docs[word] = 1
			term_ids[word] = num_terms
			#print word + " : " + str(num_terms)
			num_terms += 1

def get_idf(term):
    #"""Retrieve the IDF for the specified term. 
    
     #  This is computed by taking the logarithm of ( 
     #  (number of documents in corpus) divided by (number of documents
     #   containing this term) ).
    # """
	global term_num_docs
    	global num_docs
	if not term in term_num_docs:
      		return idf_default
	#print str(float(1 + num_docs)/(1 + term_num_docs[term]))	
    	result =  math.log(float(1 + num_docs)/(1 + term_num_docs[term]))
	return result

count = 0;
csvfile = open(sys.argv[1],'rb');
pattern = re.compile(r'\b(' + r'|'.join(stopwords.words('english')) + r')\b\s*') #pattern to eliminate stopwords
outfile = open(sys.argv[2],'wb')
data = csv.reader(csvfile, delimiter=',')
bkdata= data
for row in data:
        if row[0]:
                #print row[0];
                if row[1]:
               		add_input_review(row[1])
		 #output.writerow([row[0], text, row[2], row[3]]);
csvfile.close()


csvfile = open(sys.argv[1],'rb');
data = csv.reader(csvfile, delimiter=',')
count = 0
for row in data:
	if row[0] and row[3]:
#		print row[0];
		text = row[1]		
		if text:
			d = {}
#			print text
			outfile.write(row[3] + " ")
			tokens = nltk.word_tokenize(text)
			tokens_set = set(tokens)
			#calculate the maximum raw frequence of any term in the document
			maxvalue = 0
			for word in tokens_set:
				if tokens.count(word) > maxvalue:
					maxvalue = tokens.count(word)	

			#calculate the number of different tokens
			noOfUnigram = 0;
			noOfBigram = 0
			noOfTrigram = 0
			for word in tokens_set:
				if not "_" in word:
					noOfUnigram += 1
				else:
					tword = work.split("_")
					if len(tword) == 2:
						noOfBigram += 1
					else:
						noOfTrigram += 1
		        count += 1  
			for word in tokens_set:
				mytf = float(tokens.count(word)) / maxvalue
      				#mytf = float(tokens.count(word)) / len(tokens) #another tf formula
      				myidf = get_idf(word)
				# if count % 100 == 0:
				#	print word + " : " + str(tokens.count(word)) + " : " + str(len(tokens)) + " : " + str(mytf) + " : " + str(myidf) + "\n"
				tfidf = mytf * myidf
				d[term_ids[word]] = tfidf
				#outfile.write(str(term_ids[word]) + ":" + str(tfidf) + " ")
			sortedfeatures = collections.OrderedDict(sorted(d.items()))
			for feature in sortedfeatures.keys():
				outfile.write(str(feature) + ":" + str(sortedfeatures[feature]) + " ")
			outfile.write("#" + row[0])
			outfile.write("\n")
		#output.writerow([row[0], text, row[2], row[3]]);
csvfile.close()
outfile.close()
		
	
