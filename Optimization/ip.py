''' Nhan Nguyen
The IP approaches to cancer radiotherapy delivery optimization
Minimize:
 - the beam-on time: the length of time the radiation source is switched on during the treatment
 - the cardinality: the number of machine set-ups required to deliver the planned treatment

More details on the paper: http://link.springer.com/article/10.1007%2Fs10601-010-9104-1
'''
from gurobipy import *
from collections import namedtuple
import sys
import math
import time

def ip(filename):
	f = open(filename).readlines()
	m = int(f[0].strip().split()[0])  #number of rowss
	n = int(f[0].strip().split()[1])  #number of columns
	#print m
	#print n
	I = dict() #intensity matrix
	maxI = 0
	for i in range(m):
		row = []
		line = f[i+1].strip().split()
		for j in range(n):
			Iij = int(line[j])
			I[i+1,j+1] = Iij
			if Iij > maxI: #find the maximum value of I, that is the upper bound of b
				maxI = Iij
	#print maxI
	maxI += 1 #increase these values by 1 because the index starts from 1
	m += 1
	n += 1
	'''
	for i in range(1,m):
		s = ""
		for j in range(1,n):
			s+= str(I[i,j]) + " " 
		print s	
	'''
	model = Model("IP")

	N = dict() #vector N
	Q = dict() #matrix Q
	S = dict() #matrix S

	K = model.addVar(lb = 0, vtype=GRB.INTEGER, name="K") #the cardinality of the decomposition
	B = model.addVar(lb = 1, ub = maxI-1, vtype=GRB.INTEGER, name="B") #the total beam-on time
	for b in range(1,maxI):
	#	print "N[%d]" %(b)
		N[b] = model.addVar(lb=0, vtype=GRB.INTEGER, name="N[%d]" %(b))
		for i in range(1,m):
			Q[b,i,0] = model.addVar(lb=0,ub=0,vtype=GRB.INTEGER, name="Q[%d,%d,%d]" %(b,i,0))		
			for j in range(1,n):
			#	print "Q[%d,%d,%d]" %(b,i,j)
				Q[b,i,j] = model.addVar(lb=0, vtype=GRB.INTEGER, name="Q[%d,%d,%d]" %(b,i,j))
				S[b,i,j] = model.addVar(lb=0, vtype=GRB.INTEGER, name="S[%d,%d,%d]" %(b,i,j))

	model.update()

	model.addConstr(quicksum(N[b] for b in range(1,maxI)) - K == 0)
	for i in range(1,m):
		for j in range(1,n):
			model.addConstr(quicksum(b*Q[b,i,j] for b in range(1,maxI)) - I[i,j] == 0)
			for b in range(1,maxI):
				model.addConstr(N[b] - S[b,i,j] >= 0)
				model.addConstr(S[b,i,j] - Q[b,i,j] + Q[b,i,j-1] >= 0)
				model.addConstr(math.floor(I[i,j]/b) - Q[b,i,j] >= 0)
					
	model.addConstr(B - quicksum(b * N[b] for b in range(1,maxI)) == 0)


	model.setObjective(K,GRB.MINIMIZE) #objective: maximize the cardinality K
	model.optimize()
	model.write("hw3.lp")
	model.printAttr('X')

if __name__ == "__main__":
	if len(sys.argv) != 2:
        	print "Usage: ip.py filename"
	        print "  filename   Name of a file which stores the intensity matrix"
	        sys.exit(-1)
	ip(sys.argv[1])

