# DataLogger_ARDTimed
# Gray Theobald, Tecknosys Ltd.
# WORKING - as of 17.04.20.
# CSV datalogger for Arduino / IIS3DHHC_Time_Interval sketch: 3 Axis accelerometer with millis timestamp (for lack of RTC with required accuracy for data rate)
# 4 column (t,x,y,z) CSV data output file can then be analysed for acceleration (each axis), RMS and fast-fourier transform for vibration frequency using Matplotlb_CSVDataProcessing.py script.
# Intent will be to integrate this functionality to the data processing script.


import serial
import keyboard
import csv
import time

try:
    ser = serial.Serial('COM14',115200)								# open com port 4 at baudrate of 115200
    time.sleep(1)
    ser.flushInput()
    print('logging...... - press "esc" to quit')											

    while True:
        ser_bytes = (ser.readline().decode('utf-8'))				# read serial and decode to unicode (issue with '-' character otherwise)
        ser_bytes = ser_bytes.rstrip('\n')							# remove any line feeds at end of data
        ser_bytes = ser_bytes.rstrip('\r')							# remove carriage returns at end of data
        dataArray = ser_bytes.split(',')							# parse the data to a list

        #timestampedData = [time.time()]+dataArray					# concatenate timestamp to serial data **** Use millis / micros function from arduino until milli / micro second RTC available
        
        #print(dataArray)											# print data to serial monitor (testing purposes only)
 
        with open('kst2_Test.csv', 'a', newline='', encoding='utf-8') as file:		# create csv file, remove newlines, write CSV data  
        	writer = csv.writer(file, delimiter=',')								# ** 'a'= create new or append to existing, 'w'= create new or overwrite existing
        	writer.writerow(dataArray)
    
       	if keyboard.is_pressed('esc'):								# 'escape' button exits (breaks out of while loop) and closes serial port
           break;
    ser.close
except:
    print("Unexpected error:")
    print("Unexpected error:")
    







   
