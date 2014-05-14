__author__      = "Nhan Nguyen"
import sys
import linecache
from random import randint

datafile = open(sys.argv[1],'rb');
data = datafile.readlines()
noOfSamples = 0
noOfPosSamples = 0
noOfNegSamples = 0
for row in data:
	splittedata = row.split()
	noOfSamples += 1
	if "-1" in splittedata[0]:
		noOfNegSamples += 1
	else:
		noOfPosSamples += 1
noOfNegSamples = noOfPosSamples
percenttraining = int(sys.argv[2])
noOfPosTrainingSamples = int(percenttraining*noOfPosSamples/100)
noOfNegTrainingSamples = int(percenttraining*noOfNegSamples/100)
print "noOfSamples :" + str(noOfSamples)
#print "noOfPosTrainingSamples: " + str(noOfPosTrainingSamples)
#print "noOfNegTrainingSamples: " + str(noOfNegTrainingSamples)

countPosSamples = 0
countNegSamples = 0
markedlineIDs = {}

posTraining = open("temp/posTraining.dat","w")
negTraining = open("temp/negTraining.dat","w")
cTest = 0
while countPosSamples < noOfPosTrainingSamples or countNegSamples < noOfNegTrainingSamples:
	rlineID = randint(0, noOfSamples)
	if rlineID in markedlineIDs:
		while rlineID in markedlineIDs:
			rlineID = randint(0, noOfSamples)
	line = linecache.getline(sys.argv[1],rlineID)
	if line:	
#		print line
		splittedata = line.split()
		if "-1" in splittedata[0] and countNegSamples < noOfNegTrainingSamples:
			countNegSamples += 1
			negTraining.write(line)
			markedlineIDs[rlineID] = 1
	#		print countNegSamples
		elif not "-1" in splittedata[0] and countPosSamples < noOfPosTrainingSamples:
			countPosSamples += 1
			posTraining.write(line)
			markedlineIDs[rlineID] = 1
			cTest += 1
	#		print countPosSamples
		line = ''

posTraining.close()
negTraining.close()

#print cTest
posTest = open("temp/posTest.dat","w")
negTest = open("temp/negTest.dat","w")
i = 0
while i <= noOfSamples:
	if not i in markedlineIDs:
		line = linecache.getline(sys.argv[1],i)
        	if line:
#               print line
                	splittedata = line.split()
               	 	if "-1" in splittedata[0]:
                        	countNegSamples += 1
                      	  	negTest.write(line)
        #               print countNegSamples
                	else:
                        	countPosSamples += 1
                        	posTest.write(line)
        #               print countPosSamples
                	line = ''
	i += 1
posTest.close()
negTest.close()

Training = open("train" + sys.argv[2] + sys.argv[3]+".dat","w")
posTrainingF = open("temp/posTraining.dat","r")
posTraining = posTrainingF.readlines()
for line in posTraining:
	Training.write(line)
posTrainingF.close()

negTrainingF = open("temp/negTraining.dat","r")
negTraining = negTrainingF.readlines()
for line in negTraining:
        Training.write(line)
negTrainingF.close()
Training.close()

Label = open("label" + sys.argv[2] + sys.argv[3]+".dat","w")
Test = open("test" + sys.argv[2] + sys.argv[3]+".dat","w")
posTestF = open("temp/posTest.dat","r")
posTest = posTestF.readlines()
for line in posTest:
        Test.write(line)
	data = line.split()
	Label.write(data[0] + "\n")
posTestF.close()

negTestF = open("temp/negTest.dat","r")
negTest = negTestF.readlines()
for line in negTest:
        Test.write(line)
	data = line.split()
	Label.write(data[0] + "\n")
negTestF.close()
Label.close()
Test.close()



#print noOfSamples
print "noOfPosSamples: " + str(noOfPosSamples)
print "noOfNegSamples: " + str(noOfNegSamples)
print "noOfPosTrainingSamples: " + str(noOfPosTrainingSamples)
print "noOfNegTrainingSamples: " + str(noOfNegTrainingSamples)

		
	
