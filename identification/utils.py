#!/usr/bin/python3

import numpy as np
from scipy.signal import find_peaks


class Signal:
    def __init__(self, time, input, output):
        self.output = output
        self.input = input
        self.time = time
        distance = round(len(output)/4.0)
        width = int(len(output)/30)
        self.peaks_out = find_peaks(self.output, distance=distance, width=width)[0]
        self.peaks_in = find_peaks(self.input, distance=distance, width=width)[0]
        if len(self.peaks_in) != len(self.peaks_out) != 3:
            print(len(self.peaks_out), len(self.peaks_in))
            assert False
        self._frequency = None
        self._phase_shift = None
        self._magnitude_change = None

    @property
    def frequency(self):
        if not self._frequency:
            self._frequency = 2*np.pi/(self.time[self.peaks_out][-1] -
                                       self.time[self.peaks_out][-2])
        return self._frequency

    def plot(self, ax):
        ax.plot(self.time, self.output, 'b')
        ax.plot(self.time, self.input, 'r')
        ax.grid()

    @property
    def phase_shift(self):
        if not self._phase_shift:
            phase_shift_1 = self.time[self.peaks_out][-2] - \
                            self.time[self.peaks_in][-2]

            phase_shift_2 = self.time[self.peaks_out][-1] - \
                            self.time[self.peaks_in][-1]
            self._phase_shift = -((phase_shift_1 + phase_shift_2)/2.0) * \
                                self.frequency
        return self._phase_shift

    @property
    def magnitude_change(self):
        if not self._magnitude_change:
            mag1 = self.output[self.peaks_out[-1]] / self.input[self.peaks_in[-1]]
            mag2 = self.output[self.peaks_out[-2]] / self.input[self.peaks_in[-2]]
            self._magnitude_change = (mag1 + mag2)/2.0
        return self._magnitude_change

    def __str__(self):
        return "out_peaks:  " + str(self.time[self.peaks_out]) + '\n' + \
               "in_peaks:  " + str(self.time[self.peaks_in]) + '\n' + \
               "phase_shift:  " + str(np.rad2deg(self.phase_shift)) + \
                              "  deg\n" + \
               "frequency:  " + str(self.frequency) + '  rad/sec\n' + \
               "mag:  " + str(self.magnitude_change) + '\n'
