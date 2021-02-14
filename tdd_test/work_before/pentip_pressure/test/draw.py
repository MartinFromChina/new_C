# encoding: utf-8

import matplotlib.pyplot as plt
import numpy as np

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

fig = plt.figure(figsize=(10, 5))
ax1 = fig.add_subplot(2,1,1)
ax1.scatter(X, Y,s = 1)
plt.grid()

x = np.arange(-3.0,3.0,0.1)

"""
sigema n = 0 ~ 无穷 0.1*f(-∞ + 0.1*n) = 1
sigema n = 0 ~ 无穷 k*f(-∞ + 0.1*n) = 500
=> k = 50
"""
y = normfun(x,0.0,1.0) * 50

ax2 = fig.add_subplot(2,1,2)
ax2.hist(Y,bins = 60,rwidth=0.8) # 60 = (3 - (-3 ))/0.1
ax2.plot(x,y)
plt.grid()




plt.savefig('normal_column.jpg')
plt.show()
