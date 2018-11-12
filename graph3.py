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
i = 1
# j = 1
f = open("table2.txt","r")

f1 = f.readlines()
for line in f1:
    if i > 28:
        temp = line.split()
        Alpha.append(float(temp[0]))
        RB.append(float(temp[1]))
        BU.append(float(temp[2]))
        TD.append(float(temp[3]))
    i += 1

print (Alpha)
print (RB)
print (TD)
print (BU)
# plt.plot(Alpha, BU, 'r-', Alpha, TD, 'g-', Alpha, RB, 'b-')
# plt.plot(Alpha, BU, 'r-', Alpha, TD, 'g-', Alpha, RB, 'b-')
plt.plot(Alpha,BU,label="Bottom up Splay tree",color="red")
plt.plot(Alpha,TD,label="Top down Splay tree",color="green")
plt.plot(Alpha,RB,label="Red Black Tree",color="cyan")
plt.xlim(0,1)
plt.xlabel('Skew Fector')
plt.ylabel('CPU time')
plt.title('CPU times versus skewness for 100,000 searches for an activity ratio of 80 per cent updates and 20 per cent search')
plt.legend()
plt.show()

