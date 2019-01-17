# plot the energies
# Created by Martin Gren 2014-10-25.

# imports
import matplotlib.pylab as plt
import numpy as np
import math
import matplotlib.mlab as mlab
from scipy.stats import norm
# input file
filename = 'walkers.dat'
# import data
data = np.loadtxt(filename)

# initial size of plot window
plt.figure(figsize=(8,6))

# plot
plt.hist(data, bins=50,alpha=0.7, density=True,  rwidth=0.85)

x = np.linspace(-4, 4, 1000)
y = []
for i in range(0, 1000):
    y = 1.0 / np.sqrt(2*np.pi)*np.exp(- x**2 /2)
plt.plot(x, y, '-r', label='Density with analytical solution')

# labels
plt.xlabel('Position / [a.u.]', fontsize=20)
plt.ylabel('Probability density', fontsize=20)
#plt.text(-100, 600, r'$Going towards Gaussian dist.$')

# legend
plt.legend(loc='upper left')
leg = plt.gca().get_legend()
ltext  = leg.get_texts()
plt.setp(ltext, fontsize=12) 

# axis limits
# tick fontsize
plt.xticks(fontsize=12)
plt.yticks(fontsize=12)
plt.title('Histogram of position of the walkers in the oscillator')


plt.show()
