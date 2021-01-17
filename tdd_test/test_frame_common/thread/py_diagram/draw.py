import matplotlib.pyplot as plt

print("draw.py start")
filename = 'data.txt'
X,Y = [],[]
with open(filename, 'r') as f:#1
    lines = f.readlines()#2
    index = 0
    for line in lines:#3
        value = [float(s) for s in line.split()]#4
        X.append(value[0])#5
        Y.append(value[1])

#print(X)  
#print(Y)

plt.figure(figsize=(30, 2))
plt.scatter(X, Y,s = 1)
#plt.plot(X, Y)

plt.savefig('thread.jpg')

plt.show()

