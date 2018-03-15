from numpy import genfromtxt
import matplotlib.pyplot as plt

data = list(zip(*genfromtxt('test.csv', delimiter=',')))

for i in data[1:]:
    plt.plot(data[0], i)
plt.show();

