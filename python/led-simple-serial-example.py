#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Turn red the first LED on the dev-kit `sensor` board.

USAGE

    Windows
    > cd $dev-kit\python
    > python .\led-red.py
"""

import usb
import commands

# Turn commands into byte arrays ready to write over serial.
led1_red   = commands.send_led1_red_key.to_bytes(1,byteorder='big')
led1_green = commands.send_led1_green_key.to_bytes(1,byteorder='big')

if __name__ == '__main__':

    # Find the spectrometer by its serial number.
    sernum='091113'
    # usb.open_spectrometer() returns an instance of `pyserial` serial.Serial.
    kit = usb.open_spectrometer(sernum)
    if kit.is_open:
        print(f"Opened CHROMATION{sernum} on {usb.dev_name(sernum)}.")
        kit.write(led1_red)
    kit.close()
    print(f"Closed CHROMATION{sernum}.")
