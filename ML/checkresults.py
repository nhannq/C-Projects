__author__      = "Nhan Nguyen"
import sys
import linecache
from random import randint
from decimal import *

datadict = {}
datafile = open(sys.argv[1],'rb');
data = datafile.readlines()
idx = 0
for row in data:
	if "-" in row:
		datadict[idx] = -1
	else:
		datadict[idx] = 1
	idx += 1

datafile = open(sys.argv[2],'rb')
data = datafile.readlines()
count = 0
idx = 0
fn = 0
fp = 0
for row in data:
	if datadict[idx] == int(row):
		count += 1
	else:
		if datadict[idx] == -1 and int(row) == 1:
			fn += 1;
		elif datadict[idx] == 1 and int(row) == -1:
			fp += 1;	
	idx += 1
print "false negative: " + str(Decimal(fn)/Decimal(count)*100)
print "false positive: " + str(Decimal(fp)/Decimal(count)*100)
#print count
#print idx
