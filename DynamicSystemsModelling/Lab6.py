import numpy as np
import matplotlib.pyplot as plt

    
def my_func(r, x):
    return r*np.tan(x)

n = 1000
r = np.linspace(0, 5, n)
iterations = 1000
last = 100

x = 1e-5 * np.ones(n)
fig, ax1 = plt.subplots(1, 1, figsize=(8, 9), sharex=True)

for i in range(iterations):
    x = my_func(r, x)
    if i >= (iterations - last):
        ax1.plot(r, x, ',k', alpha=.25)

plt.show()
