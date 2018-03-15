from numpy import genfromtxt
import matplotlib.pyplot as plt
import sys

data = list(zip(*genfromtxt(sys.argv[1]+'.csv', delimiter=',')))

for i in data[1:]:
    plt.plot(data[0], i)
plt.show();

