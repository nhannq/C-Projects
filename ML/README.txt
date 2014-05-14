The system was built by using Python. 
Requirement: python-nltk library and stopword. If you use Unix OS, you can use the command: 
sudo apt-get install python-nltk to install the nltk library
In python: type nltk.download() then select the stopword to download
The main modules:
filter_keyword.py: retrieve the reviews that related to food poisoning 
arguments: argv[1] argv[2]
	argv[1]: the name of keyword file (keyword.txt)
	argv[2]: the name of the output file (Dataset.csv)
Then we label the data from output file (Dataset.csv)
wordtokenize.py: tokenize the labeled data
	arguments: argv[1] argv[2] argv[3]
	argv[1]: the name of data labeled file (Dataset.csv)
	argv[2]: the output file which inclused the tokens (unigram, bigram, trigram) (data.csv)
processdata.py: calculate tf-idf and create the data for svm-perf format 
	arguments: argv[1] argv[2] 
	argv[1]: the name of tokenized file (data.csv)
	argv[2]: the data for svm-perf (data.txt)
runsvm.py: create the unbalanced dataset including training, test and label to check the result later
	argv[1]: name of data file (data.txt)
	argv[2]: percentage of the training dataset (we used 70% for the training dataset)
	argv[3]: the id of dataset (if we create many dataset)
runsvm2.py: create the alanced dataset including training, test and label to check the result later
	argv[1]: name of data file (data.txt)
	argv[2]: percentage of the training dataset (we used 70% for the training dataset)
	argv[3]: the id of dataset (if we create many dataset)
Then we run svm-perf as the instructions on the website (http://www.cs.cornell.edu/people/tj/svm_light/svm_perf.html). 
checkresults.py: to calculate the false negative, false positive of the classified result
	argv[1]: the prediction result (predictions)
	argv[2]: the true label file (which is extracted from the test set by the runsvm.py program)

Remember to create the folder named “temp” before run the program. If you have any question, feel free to send an email to: nhan.q.nguyen@uconn.edu.
