import matplotlib.pyplot as plt

print("draw.py start")
filename = 'basic_data.txt'
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
filename = 'display_data.txt'
X1,Y1 = [],[]
with open(filename, 'r') as f1:#1
    lines = f1.readlines()#2
    for line in lines:#3
        value = [float(s) for s in line.split()]#4
        X1.append(value[0])#5
        Y1.append(value[1])

plt.figure(figsize=(10, 5))
plt.scatter(X, Y,s = 1)
plt.savefig('battery.jpg')
plt.figure(figsize=(10, 5))
plt.scatter(X1, Y1,s = 1)
plt.savefig('display.jpg')
plt.show()