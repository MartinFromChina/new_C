import matplotlib.pyplot as plt

print("draw.py start")

X,Y = [],[]
for num in range(0,10):#3
    X.append(1)#5
    Y.append(2)

print(X)  
print(Y)


plt.plot(X, Y)
plt.show()