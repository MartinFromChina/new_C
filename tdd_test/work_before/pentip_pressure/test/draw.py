# encoding: utf-8

import matplotlib.pyplot as plt
import numpy as np
from sympy import *
import sympy


print("draw.py start")
filename = './text/gauss.txt'
X,Y = [],[]
with open(filename, 'r') as f:#1
    lines = f.readlines()#2
    index = 0
    for line in lines:#3
        value = [float(s) for s in line.split()]#4
        X.append(index)#5
        index = index + 1
        Y.append(value[0])

#print(X)  
#print(Y) 

def normfun(x, mu, sigma):
  pdf = np.exp(-((x - mu)**2)/(2*sigma**2)) / (sigma * np.sqrt(2*np.pi))
  return pdf

def Symnormfun(x, mu, sigma):
  pdf = sympy.exp(-((x - mu)**2)/(2*sigma**2)) / (sigma * sympy.sqrt(2*sympy.pi))
  return pdf


fig = plt.figure(figsize=(20, 20))
ax1 = fig.add_subplot(3,1,1)
ax1.scatter(X, Y,s = 3)
plt.grid()

x1 = np.arange(1650,1750,1)

"""
sigema n = 0 ~ 无穷 1*f(-∞ + 1*n) = 1
sigema n = 0 ~ 无穷 k*f(-∞ + 1*n) = 500
=> k = 500
"""
y1 = normfun(x1,1700.0,15.0) * 500

ax2 = fig.add_subplot(3,1,2)
ax2.hist(Y,bins = 100,rwidth=0.8) # 100 = (100)/1
ax2.plot(x1,y1)
plt.grid()

ax3 = fig.add_subplot(3,1,3)
x2 = symbols('x')
gauss = integrate(Symnormfun(x2,1700.0,14.66) ,x2) #root 215
print gauss
x_value = []
y_value = []
get_small = 0
get_big = 0
for  i in np.arange(1650,1750,1):
    x_value.append(i)
    y_value.append(gauss.subs('x',i) + 0.5)# the 0.5 shift is wired , i can not see why
    if get_small == 0 and y_value[i - 1650]>=0.01: 
        get_small = i  
        print i
    if get_big == 0 and y_value[i - 1650]>=0.99: 
        get_big = i  
        print i
ax3.plot(x_value,y_value )
plt.grid()

ax1.axhline(y=get_small,xmin=0,xmax=500,c="red",linewidth=1.5,zorder=0)
ax1.axhline(y=get_big,xmin=0,xmax=500,c="red",linewidth=1.5,zorder=0)

ax2.axvline(x=get_small,ymin=0,ymax=15,c="red",linewidth=1.5,zorder=0)
ax2.axvline(x=get_big,ymin=0,ymax=15,c="red",linewidth=1.5,zorder=0)




plt.savefig('normal_column.jpg')
plt.draw()
plt.pause(3)  # 该句显示图片15秒
plt.close() 





