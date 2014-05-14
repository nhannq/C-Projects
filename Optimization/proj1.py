''' Nhan Nguyen 
 A cargo plane has three holds for storing cargo: front, centre and rear. These
holds have the following limits on both weight and space:

 Hold  |     Weight Capacity (Tonnes)    |      Space Capacity (Cubic Meters)   |
Front  |                10               |                  6800                | 
Center |                16               |                  8700                |
Rear   |                8                |                  5300                |

Furthermore, the weight of the cargo in the respective holds must be the same
proportion of that hold s weight capacity to maintain the balance of the plane. 
The following four cargoes are available for shipment on the next flight

Cargo  |           Weight (Tonnes)       |          Volumes (Cubic Meters)      |          Profit / Tonnes ($)       |
  C1   |                18               |                  480                 |                  310               |
  C2   |                15               |                  650                 |                  380               |
  C3   |                23               |                  580                 |                  350               |
  C4   |                12               |                  390                 |                  285               |

Any proportion of these cargoes can be accepted. The objective is to determine
how much (if any) of each cargo C1, C2, C3 and C4 should be accepted and
how to distribute each among the compartments so that the total profit for the
flight is maximized.

Formulate the above problem as a linear program
'''

import sys
from gurobipy import *

model = Model("project1")
#the number of tonnes of cargo i (i = 1,2,3,4 for C1,C2,C3,C4 respectively) that was put into hold j (j = 1,2,3 for Front, Center, Rear respectively)  
x = dict()
M = 4
N = 3
for i in range(1,M+1):
	for j in range(1,N+1):
		x[i,j] = model.addVar(vtype=GRB.CONTINUOUS, name="x[%d,%d]" %(i,j))

model.update()
model.setObjective(310 * x[1,1] + 310 * x[1,2] + 310 * x[1,3] + 380 * x[2,1] + 380 * x[2,2] + 380 * x[2,3] + 350 * x[3,1] + 350 * x[3,2] + 350 * x[3,3] + 285 * x[4,1] + 285 * x[4,2] + 285 * x[4,3], GRB.MAXIMIZE)
#add constraints
model.addConstr(x[1,1] + x[1,2] + x[1,3] <= 18, "c1")
model.addConstr(x[2,1] + x[2,2] + x[2,3] <= 15, "c2")
model.addConstr(x[3,1] + x[3,2] + x[3,3] <= 23, "c3")
model.addConstr(x[4,1] + x[4,2] + x[4,3] <= 12, "c4")
model.addConstr(x[1,1] + x[2,1] + x[3,1] + x[4,1] <= 10, "c5")
model.addConstr(x[1,2] + x[2,2] + x[3,2] + x[4,2] <= 16, "c6")
model.addConstr(x[1,3] + x[2,3] + x[3,3] + x[4,3] <= 8, "c7")
model.addConstr(480 * x[1,1] + 650 * x[2,1] + 580 * x[3,1] + 390 * x[4,1] <= 6800, "c8")
model.addConstr(480 * x[1,2] + 650 * x[2,2] + 580 * x[3,2] + 390 * x[4,2] <= 8700, "c9")
model.addConstr(480 * x[1,3] + 650 * x[2,3] + 580 * x[3,3] + 390 * x[4,3] <= 5300, "c10")
model.addConstr(1/10 * x[1,1] + 1/10 * x[2,1] + 1/10 * x[3,1] + 1/10 * x[4,1] - 1/16 * x[1,2] - 1/16 * x[2,2] - 1/16 * x[3,2] - 1/16 * x[4,2] >= 0, "c11")
model.addConstr(1/10 * x[1,1] + 1/10 * x[2,1] + 1/10 * x[3,1] + 1/10 * x[4,1] - 1/8* x[1,3] - 1/8* x[2,3] - 1/8* x[3,3] - 1/8* x[4,3]  >= 0, "c12")
model.addConstr(1/10 * x[1,1] + 1/10 * x[2,1] + 1/10 * x[3,1] + 1/10 * x[4,1] - 1/16 * x[1,2] - 1/16 * x[2,2] - 1/16 * x[3,2] - 1/16 * x[4,2] <= 0, "c13")
model.addConstr(1/10 * x[1,1] + 1/10 * x[2,1] + 1/10 * x[3,1] + 1/10 * x[4,1] - 1/8* x[1,3] - 1/8* x[2,3] - 1/8* x[3,3] - 1/8* x[4,3]  <= 0, "c14")

model.optimize() #find the optimal solution

if model.status == GRB.status.INF_OR_UNBD:
    model.setParam(GRB.param.presolve, 0)
    model.optimize()

if model.status == GRB.status.OPTIMAL:
    print('Optimal objective: %g' % model.objVal)
    for i in range(len(model.getVars())):
        print('{} - {}'.format(model.getVars()[i].varName, model.getVars()[i].X))
    model.write('model.sol')
    exit(0)
elif model.status != GRB.status.INFEASIBLE:
    print('Optimization was stopped with status %d' % model.status)
    exit(0)



