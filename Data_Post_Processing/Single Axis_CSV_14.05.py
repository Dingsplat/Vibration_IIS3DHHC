import matplotlib.pyplot as plt
from matplotlib import style
import numpy as np
from scipy.fftpack import fft
import tkinter as tk
from tkinter import filedialog
import time

#style.use('fivethirtyeight')
style.use('dark_background')
#style.use('seaborn-dark-palette')

#Prompt user for file
root = tk.Tk()
root.withdraw()
file_path = filedialog.askopenfilename(filetypes=[("Two Column CSV","*.csv")])  # Expects CSV data format of 't, X, Y, Z'
print(file_path)

#Load Data (assumes four column array (t,x,y,z))
tic = time.process_time()
t, x = np.genfromtxt(file_path,delimiter=',', unpack=True)		
t = t/1000																			# 16.04.20: convert millis to seconds for data plotting
toc = time.process_time()
print("Load Time:",toc-tic)

#Determine variables
N = np.int(np.prod(t.shape))														#length of the array
# Fs = 1/(t[1]-t[0])																#sample rate (Hz) - original; works for SerialPlot Test.csv, doesn't work for Datalogger_ARDTimed.py data
Fs = 1/(t[2]-t[1])																	#sample rate (Hz) - altered for Datalogger_ARDTimed.py data
T = 1/Fs;
print("# Samples:",N)
print("Sample Rate:",Fs,"(Hz)")

#Plot X Data
tic = time.process_time()
plt.figure(1) #plots X data on separate figure
#plt.subplot(311) #subplot method 1 -  plots X data in top field of a 3 graph display (X,Y,Z on 1 figure)
plt.subplot2grid((12,1), (0,0), rowspan=3, colspan=1) #try different subplot method
plt.plot(t, x, c='r', linewidth=0.7)
plt.xlabel('Time (seconds)')
plt.ylabel('Accel (mg)')
plt.ylim(-2500, 2500)
plt.title('X Axis')
plt.grid()
toc = time.process_time()
print("Plot Time:",toc-tic)


#Compute X axis RMS and Plot
tic = time.process_time()
w = np.int(np.floor(Fs)); #width of the window for computing RMS
steps = np.int_(np.floor(N/w)); #Number of steps for RMS
t_RMS = np.zeros((steps,1)); #Create array for RMS time values
x_RMS = np.zeros((steps,1)); #Create array for RMS values
for i in range (0, steps):
	t_RMS[i] = np.mean(t[(i*w):((i+1)*w)]);
	x_RMS[i] = np.sqrt(np.mean(x[(i*w):((i+1)*w)]**2));  
plt.figure(2) 
plt.subplot2grid((12,1), (0,0), rowspan=3, colspan=1) 
plt.plot(t_RMS, x_RMS, c='r', linewidth=0.7)
plt.xlabel('Time (seconds)')
plt.ylabel('RMS Accel (mg)')
#plt.ylim(-2500,2500)
plt.title('RMS - ' + 'X Axis')
plt.grid()
toc = time.process_time()
print("X-RMS Time:",toc-tic)

#Compute and Plot X axis FFT
tic = time.process_time()
plt.figure(3) 
plt.subplot2grid((12,1), (0,0), rowspan=3, colspan=1) 
xf = np.linspace(0.0,1.0//(2.0*T), N//2)    
yf = fft(x)
plt.plot(xf, 2.0/N * np.abs(yf[0:np.int(N/2)]), c='r', linewidth=0.7)
plt.grid()
plt.xlabel('Frequency (Hz)')
plt.ylabel('Accel (mg)')
plt.title('FFT - ' + 'X Axis')
toc = time.process_time()
print("X-FFT Time:",toc-tic)


plt.show()
  