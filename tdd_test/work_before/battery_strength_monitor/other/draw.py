import matplotlib.pyplot as plt
plt.figure(figsize=(10, 5))
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


plt.scatter(X, Y,s = 1)
plt.savefig('battery.jpg')
plt.show()