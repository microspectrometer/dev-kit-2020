#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Turn red/green the first LED on the dev-kit `sensor` board.

    The sequence of serial writes/reads prints to stdout and
    is logged to file with timestamps.

USAGE

    ---Windows---
    > cd $dev_kit\python
    > python .\led-simple-serial-example.py
    Output is printed to screen.

    ---bash---
    View the timestamped log of the output.
    $ cd $dev-kit/python/
    $ vim serial-example.log
"""

import usb
import commands
import logging

# Turn commands into byte arrays ready to write over serial.
led1_red   = commands.send_led1_red_key.to_bytes(1,byteorder='big')
led1_green = commands.send_led1_green_key.to_bytes(1,byteorder='big')

def _print_and_log(msg):
    """Print the message to stdout and log the message to file."""
    print(msg)
    logging.info(msg) # Log using severity level INFO.

def _print_and_log_serial(cmd):
    """Print and log the `cmd` Tx by host PC and response Rx from the mcus."""
    _print_and_log(f"Tx.. send 0x{cmd.hex()}")
    # There is always a response from the BRIDGE mcu.
    _rx_bridge_response()
    # If command is for the SENSOR, there is a SENSOR response too.
    if kit.inWaiting() == 0: return
    _rx_sensor_response()

def _ser_read_status_and_cmd_bytes(mcu='BRIDGE'):
    """Print and log the status and command bytes.

    `mcu`:
        - string describing which mcu originated the status and cmd bytes
        - mcu='BRIDGE' or mcu='SENSOR'

    This call destructively reads bytes in the serial buffer.
    It must be called in the correct sequence with other serial reads.
    """
    while (kit.inWaiting() < 2): pass
    rx_status = int.from_bytes(
            kit.read(1),
            byteorder='big', signed=False
            )
    rx_cmd = int.from_bytes(
            kit.read(1),
            byteorder='big', signed=False
            )
    _print_and_log(f"Rx.. {mcu}: "
        f"status 0x{rx_status:02X}, "
        f"cmd 0x{rx_cmd:02X}"
        )

def _rx_bridge_response():
    """Print and log the response from the BRIDGE mcu."""
    # First two bytes are the BRIDGE mcu:
    _ser_read_status_and_cmd_bytes(mcu='BRIDGE')

def _ser_read_sizeof_sensor_msg():
    """Get the number of bytes of data the sensor is sending.

    This call destructively reads bytes in the serial buffer.
    It must be called in the correct sequence with other serial reads.
    """
    while (kit.inWaiting() < 2 ): pass
    nbytes_of_data = int.from_bytes(
            kit.read(2),
            byteorder='big', signed=False
            )
    return nbytes_of_data

def _rx_sensor_response():
    """Print and log the response from the SENSOR mcu.

    First two bytes are a header that says how many more bytes to expect.
    Next two bytes are the status report and command received/executed.
    The rest of the response is the requested sensor data, if any.
    """
    nbytes_of_data = _ser_read_sizeof_sensor_msg()
    _ser_read_status_and_cmd_bytes(mcu='SENSOR')
    # status and cmd are two bytes, so num bytes left is nbytes_of_data - 2
    nbytes_left_in_buffer = nbytes_of_data - 2
    # Any bytes left? Read them. (for now just ignore them)
    if nbytes_left_in_buffer > 0:
        print("Host PC ignores additional sensor data.")

if __name__ == '__main__':

    # Set up the log file.
    logging.basicConfig(
        filename='serial-example.log',
        filemode='w', # overwrite old logs (do not append)
        format='%(asctime)s -- %(message)s',
        datefmt='%H:%M:%S', # only show HH:MM:SS, do not show date
        level=logging.DEBUG
        )

    # Find the spectrometer by its serial number.
    sernum='091113'

    # USB open/close is handled by `pyserial` context manager in serialutil.py
    with usb.open_spectrometer(sernum) as kit:
        # `kit` is an instance of `pyserial` class serial.Serial()
        _print_and_log(f"Opened CHROMATION{sernum} on {usb.dev_name(sernum)}")
        cmd = led1_red
        kit.write(cmd)
        _print_and_log_serial(cmd)
    _print_and_log(f"Closed CHROMATION{sernum}")
