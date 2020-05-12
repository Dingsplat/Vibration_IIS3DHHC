#!/usr/bin/python3
# -*- coding: utf-8 -*-
#IIS3DHHC Accelerometer SPI comms - adapted from "Example on how to read the ADXL345 accelerometer; Kim H. Rasmussen, 2014"

# Reads SPI data and outputs to console

import time
import spidev

# Setup SPI
spi = spidev.SpiDev()
spi.open(0,0)
spi.max_speed_hz = 7800000
spi.mode = 3

# Read the Device ID (should be 0x11): 0x80 = 0b10000000 (read command), 0x0F = device who_am_i register
id = spi.xfer2([0x80 | 0x0F, 0x00])
print 'Device ID:\n'+str(hex(id[1])) + '\n'

# Initialize the IIS3DHHC
def initiis3dhhc():
    # Enter normal mode (0xC0 = normal mode, incremented addressing // 0x80 = normal mode, do not increment)
    spi.xfer2([0x20, 0xC0])

    # Configure filter and bandwidth 0x00 = FIR Linear Phase, 440Hz
    spi.xfer2([0x23, 0x00])

   

# Read IIS3DHHC - start at xL register 0x28
def readiis3dhhc():
    rx = spi.xfer2([0x80 | 0x28,0,0,0,0,0,0])
    # cocatenate and format response (accleration value expressed as 16 bit word, 2's complement, left justified)
    out = [rx[1] | (rx[2] << 8),rx[3] | (rx[4] << 8),rx[5] | (rx[6] << 8)]
    # SCALE = 0.076294: +/- 2.5G full range = 5000mG total range / 65536 counts (16 bit)
    # Format x-axis and scale to mG
    if (out[0] & (1<<16 - 1 )):
        out[0] = out[0] - (1<<16)
    out[0] = out[0] * 0.076294
    # Format y-axis and scale to mG
    if (out[1] & (1<<16 - 1 )):
        out[1] = out[1] - (1<<16)
    out[1] = out[1] * 0.076294
    # Format z-axis and scale to mG
    if (out[2] & (1<<16 - 1 )):
        out[2] = out[2] - (1<<16)
    out[2] = out[2] * 0.076294

    return out

# Initialize the IIS3DHHC accelerometer
initiis3dhhc()

# Read the IIS3DHHC every half second
timeout = 0.5
while(1):
    axia = readiis3dhhc()
    # Print the reading
    print axia[0]
    print axia[1]
    print str(axia[2]) + '\n'

    elapsed = time.clock()
    current = 0
    while(current < timeout):
        current = time.clock() - elapsed