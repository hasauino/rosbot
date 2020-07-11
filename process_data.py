#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
from utils import Signal

data = np.loadtxt("sinosoidal.csv", delimiter=',')
output_all = data[:, 0]  # w: rad/sec
input_all = data[:, 1]*5.86*0.01  # v_in: volts
freq_all = data[:, 2]
t_all = np.array([i*0.002 for i, _ in enumerate(output_all)])

output = []
input = []
t = []
signal = []

for f in np.unique(freq_all):
    idxs = np.argwhere(freq_all == f)
    output.append(output_all[idxs].flatten())
    input.append(input_all[idxs].flatten())
    t.append(np.array([i*0.01 for i, _ in enumerate(output[-1])]))
    try:
        signal.append(Signal(t[-1], input[-1], output[-1]))
    except:
        print(f)
        break

fig = plt.figure(figsize=(9,5))
ax1 = fig.add_subplot(111)
ax1.plot(t_all, output_all, 'b')
ax1.plot(t_all, input_all, 'r')
ax1.grid()

fig2 = plt.figure(figsize=(9,5))
ax2 = fig2.add_subplot(211)
ax3 = fig2.add_subplot(212)

w = []
mag = []
phase = []
for s in signal:
    w.append(s.frequency)
    mag.append(s.magnitude_change)
    phase.append(s.phase_shift)

ax2.plot(w, mag, '.r')
ax3.plot(w, np.rad2deg(phase), '.b')
ax2.set_xscale('log')
ax3.set_xscale('log')
ax2.set_xlabel("Frequency (rad/sec)")
ax3.set_xlabel("Frequency (rad/sec)")
ax2.set_ylabel("mag")
ax3.set_ylabel("phase")
ax2.grid()
ax3.grid()
plt.show()
