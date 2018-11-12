import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import spline
from scipy.interpolate import interp1d
from scipy import interpolate
#keeping the weight constant wt=10 - sparse + low weight
Alpha = []
BU = []
TD = []
RB = []
i = 0
f = open("table2.txt","r")

f1 = f.readlines()
for line in f1:
    temp = line.split()
    if i % 4 == 0:
        Alpha.append(float(temp[-1]))
    elif i % 4 == 1:
        BU.append(float(temp[-2]))
    elif i % 4 == 2:
        TD.append(float(temp[-2]))
    elif i % 4 == 3:
        RB.append(float(temp[-2]))
    i += 1

# plt.plot(Alpha, BU, 'r-', Alpha, TD, 'g-', Alpha, RB, 'b-')
# plt.plot(Alpha, BU, 'r-', Alpha, TD, 'g-', Alpha, RB, 'b-')
plt.plot(Alpha,BU,label="Bottom up Splay tree",color="red")
plt.plot(Alpha,TD,label="Top down Splay tree",color="green")
plt.plot(Alpha,RB,label="Red Black Tree",color="cyan")
plt.xlim(0,1)
plt.xlabel('Skew Fector')
plt.ylabel('Rotations')
plt.title('Mean Rotations(Rot) for an activity ratio of 20 per cent updates and 80 per cent search')
plt.legend()
plt.show()

