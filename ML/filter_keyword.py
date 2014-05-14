import sys
import json
import re

keywords_file = sys.argv[1]
reviews_file = sys.argv[2]
keywords = {}
count = 0
out = open("suspected_reviews.txt", "w")
with open(keywords_file) as lines:
	for line in lines:
		keywords[line.strip("\n")] = 0

with open(reviews_file) as lines:
	for line in lines:
		js = json.loads(line)
		review = js['text']
		isWrite = False
		matched_keywords = ""
		for keyword in keywords.keys():
			match = re.search(keyword, review)		
			if match:
				vote = int(js['stars'])
				if vote < 3:
					matched_keywords = matched_keywords + keyword + ","
					keywords[keyword] += 1
					count += 1
					if count%1000 == 0:
						print count
					isWrite = True
		if isWrite:
			review_id = js['review_id'].encode('utf-8')
			out.write(review_id + "\t" + review.encode('utf-8') .replace("\n", " ") + "\t" + matched_keywords.strip(",") + "\n\n")
print keywords
print count
out.close()
