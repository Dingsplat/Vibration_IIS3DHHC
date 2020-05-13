# Tecknosys Ltd: Gray Theobald
# Realtime plot for 3 Axis accelerometer on Arduino UNO (IIS3DHH)
# For use with DUE or UNO sketches with serial.print to CSV
# 4 column data output - column 1 = millis

import serial
import numpy as np
import matplotlib.pyplot as plt
from drawnow import *
import keyboard

xAcc = []
yAcc = []
zAcc = []

arduinoData = serial.Serial('COM4', 115200)
plt.ion()
cnt=0

def makefig():
	plt.ylim(-2500,2500)
	plt.grid(True)
	plt.plot(xAcc, c='red')
	plt.plot(yAcc, c='green')
	plt.plot(zAcc, c='blue')
	
while True:														
	while (arduinoData.inWaiting()==0):
		pass													
	arduinoString = arduinoData.readline()	

	arduinoString = arduinoString.decode('utf-8')				
	dataArray = arduinoString.split(',')

	x = int (dataArray[0])
	y = int (dataArray[1])
	z = int (dataArray[2])

	# x = float (dataArray[0].replace('\U00002013', '-'))
	# y = float (dataArray[1].replace('\U00002013', '-'))
	# z = float (dataArray[2].replace('\U00002013', '-'))
	
	xAcc.append(x)
	yAcc.append(y)
	zAcc.append(z)

	drawnow(makefig)
	plt.pause(.000001)
	cnt=cnt+1
	if(cnt>250):
		xAcc.pop(0)
		yAcc.pop(0)
		zAcc.pop(0)

	if keyboard.is_pressed('esc'):
           break;
           ser.close






	






