# plot the energies
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pylab as plt
import numpy as np

# input file
filename = 'number_walkers.dat'

# import data
data = np.loadtxt(filename)

# initial size of plot window
plt.figure(figsize=(8,6))

# plot
plt.plot(data,'r-')#, label='$\mu + \sigma$')

plt.plot([0, len(data)], [800, 800], 'b--', linewidth=1.6, label='N_0 = 800')
a = np.std(data)
print(a)
plt.plot([0, len(data)], [800-a, 800-a], 'b--', linewidth=1, label='E_0$\pm$ one standard deviation')

plt.plot([0, len(data)], [800+a, 800+a], 'b--', linewidth=1)
# labels
plt.xlabel('Iterations', fontsize=20)
plt.ylabel('Number of walkers', fontsize=20)

# legend
plt.legend(loc='upper right')
leg = plt.gca().get_legend()
ltext  = leg.get_texts()
plt.setp(ltext, fontsize=12)

# axis limits
# tick fontsize
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.title('Number of walkers in each iteration')

# display the plot
plt.savefig('nbr_walkers.png')

plt.show()
