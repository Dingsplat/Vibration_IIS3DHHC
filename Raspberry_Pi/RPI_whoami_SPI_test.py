#!/usr/bin/python3
# -*- coding: utf-8 -*-
import time
import spidev

# Setup SPI
spi = spidev.SpiDev()
spi.open(0,0)
spi.max_speed_hz = 7800000
spi.mode = 3

# Read the Device ID (should be 0x11 or 17dec)
while True:
   resp = spi.xfer2([0x80 | 0x0F, 0x00])
   print ('Device ID:\n'+str(hex(resp[1])) + '\n')
   time.sleep(1)
