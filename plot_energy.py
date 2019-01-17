# plot the energies
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pylab as plt
import numpy as np

# input file
filename = 'energy.dat'

# import data
data = np.loadtxt(filename)

# initial size of plot window
plt.figure(figsize=(8,6))

# plot
plt.plot(data,'r-')#, label='$\mu + \sigma$')
plt.plot([0, len(data)], [0.5, 0.5], 'b--', linewidth=1.6, label='E_0 = 1/2 a.u.')
a = np.std(data)
print(a)
plt.plot([0, len(data)], [0.5-a, 0.5-a], 'b--', linewidth=1, label='E_0$\pm$ one standard deviation')

plt.plot([0, len(data)], [0.5+a, 0.5+a], 'b--', linewidth=1)

# labels
plt.xlabel('Iterations', fontsize=20)
plt.ylabel('Energy/ [a.u.]', fontsize=20)
# legend
plt.legend(loc='upper right')
leg = plt.gca().get_legend()
ltext  = leg.get_texts()
plt.setp(ltext, fontsize=12) 

# axis limits
# tick fontsize
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.title('The energy in each iteration step with E(0) = 0.7 a.u.')

# display the plot

plt.savefig('energy.png')
plt.show()
