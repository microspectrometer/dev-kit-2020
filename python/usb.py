#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Chromation USB API wrapping pyserial.
"""

import serial
import serial.tools.list_ports as usb_ports
import contextlib
import sys # for sys.stderr

def dev_name(serial_number):
    """Return the device name of the USB device matching grep(serial_number).

        The device name is the id string for passing to Serial.open(), e.g.,
        `COM7` on Windows, or `/dev/ttyS6` on Cygwin.
        If there are multiple matches, only return the first match.

        Arg `serial_number` is a string.
        The intent is that `serial_number` is the *serial number* or a
        sub-string in the serial number.

        Examples on Windows:
        >>> dev_name('CHROMATION091234')
        'COM7'
        >>> dev_name('CHROMATION')
        'COM7'
        >>> dev_name('091234')
        'COM7'

        The serial number string shows up in
        `serial.tools.list_ports_common.ListPortInfo.hwid`

        The serial number is visible to `pyserial` on Windows, but not on
        Cygwin. On Cygwin, use the port name instead.

        Examples on Cygwin:
        >>> dev_name('/dev/ttyS6')
        '/dev/ttyS6'
        >>> dev_name('ttyS6')
        '/dev/ttyS6'

        This is equivalent to calling `dev_name('COM7')` on Windows.

        Calling dev_name(device_name) is silly since the point of the function
        is to find the port name but you have to pass in the port name (or part
        of it).

        In the data structures of `pyserial`, the device name is stored in
        `ListPortInfo.device`. See:
        /usr/lib/python3.6/site-packages/serial/tools/list_ports_common.py

    """
    with contextlib.suppress(StopIteration):
        return next(usb_ports.grep(serial_number)).device
def open_spectrometer(serial_number='CHROMATION012301'):
    """Open USB communication. Search by serial number of device name."""
    usb=serial.Serial()
    usb.baudrate = 115200 # max baudrate in serialwin32.py
    usb.port = dev_name(serial_number)
    if usb.port is None:
        print(
            f"Cannot `open_spectrometer('{serial_number}')`. "
            "Make sure it is connected.",
            file=sys.stderr
            )
        return
    usb.open()
    while not usb.is_open: pass
    return usb
