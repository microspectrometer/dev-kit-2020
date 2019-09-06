#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Examples of low-level serial communication calls.

USAGE
    Run this as a system-level test suite.

    Windows
    > cd C:\chromation-dropbox\Dropbox\c\dev-kit\python
    > python .\main.py

    Cygwin
    $ cd /cygdrive/c/chromation-dropbox/Dropbox/c/dev-kit/python
    $ ./main.py

    Then open `main.log` to see the test results.

MATPLOTLIB BACKEND

    Move this part of the documentation to whatever application example I write
    for using `matplotlib` as the Python GUI.

    On Cygwin, start an XWindow Server before attempting to run `main.py`.
    $ startx /usr/bin/openbox &

    A windowing system is necessary because the script imports `matplotlib` and
    my custom Cygwin matplotlibrc file `~/.matplotlib/matplotlibrc` sets
    `Gtk3Agg` as the backend. This is a GUI backend, so matplotlib needs to
    connect to a window to display the GUI. Most operating systems provide a
    windowing system by default. Cygwin does not.

    The default matplotlibrc file is in
    `site-packages/matplotlib/mpl-data/matplotlibrc`.

    My Windows matplotlibrc file is in
    `C:\Python37\Lib\site-packages\matplotlib\mpl-data`
    and it sets `TkAgg` as the backend.

DOC
    Windows:
    > python -m pydoc .\main.py

    Cygwin:
    $ pydoc3 ./main.py
"""

import serial
import serial.tools.list_ports as usb_ports
import contextlib
import sys
import numpy as np
# import matplotlib.pyplot as plt
# import matplotlib.animation as animation
import logging
import codes    # eval hardware dictionary of communication status codes
import commands as cmd # eval hardware dictionary of commands

# =====[ Open communication with the spectrometer ]=====

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

def log_tx(fname, args_sent=None, expect='PASS'):
    logging.info(
        'Tx.. TEST{expect}: send cmd "{name}" with args {args}'
        .format(
            name=fname,
            args=args_sent,
            expect=''.join(['_ASSERT_', expect])
            )
        )
# deprecated
def log_rx_sensor(nbytes_expected=2):
    """Log the response received from the spectrometer digital interface."""
    if nbytes_expected == 2:
        status = int.from_bytes( usb.read(1), byteorder='big')
        cmd_byte = int.from_bytes( usb.read(1), byteorder='big')
        # Print human-readable message if the status code is recognized.
        if status in codes.sensor_status:
            logging.info(
                '..Rx sensor sent 0x{:02X} "{}" for cmd 0x{:02X}'
                .format(status, codes.sensor_status[status], cmd_byte)
                )
        else:
            logging.info(
                '..Rx sensor sent 0x{:02X} 0x{:02X}'
                .format(status, cmd_byte)
                )
    else:
        logging.info(
            '..Rx sensor sent {num} bytes: 0x{msg:X}'
            .format(
                num=usb.inWaiting(),
                msg=int.from_bytes(
                    usb.read_all(),
                    byteorder='big',
                    signed=False)
                )
            )

def log_rx(fname, nbytes_expected):
    while (usb.inWaiting() < nbytes_expected): pass
    bridge_status = int.from_bytes(usb.read(1), byteorder='big', signed=False)
    bridge_cmd = int.from_bytes(usb.read(1), byteorder='big', signed=False)
    # nbytes_remaining = nbytes_expected - 2
    logging.info(''.join([
        '..Rx BRIDGE_{}: '.format('FAIL' if bridge_status else 'PASS'),
        'to "{}" '.format(fname),
        'bridge says "{status}" '
        .format(status=codes.bridge_status.get(
            bridge_status, # show status name
            '0x{:02X}'.format(bridge_status) # show hex if status is unknown
            )),
        'for "{cmd_name}"'
        .format(cmd_name=cmd.bridge.get(
            bridge_cmd, # show cmd name
            '0x{:02X}'.format(bridge_cmd) # show hex if cmd is not recognized
            ))
        ]))
    # The response includes a sensor response if there are more bytes.
    # if nbytes_remaining == 0: return
    if usb.inWaiting() == 0: return
    # Get the sensor response.
    # First get the two-byte header that says how many more bytes to expect.
    while (usb.inWaiting() < 2 ): pass
    nbytes_of_data = int.from_bytes(usb.read(2), byteorder='big', signed=False)
    # Read the 2-byte status report.
    while (usb.inWaiting() < 2): pass
    sensor_status = int.from_bytes(usb.read(1), byteorder='big', signed=False)
    sensor_cmd = int.from_bytes(usb.read(1), byteorder='big', signed=False)
    nbytes_remaining = nbytes_of_data - 2
    logging.info(''.join([
        '..Rx SENSOR_{}: '.format('FAIL' if sensor_status else 'PASS'),
        'to "{}" '.format(fname),
        'sensor says "{status}" '
        .format(status=codes.sensor_status.get(
            sensor_status, # show status name
            '0x{:02X}'.format(sensor_status) # show hex if status is unknown
            )),
        'for "{cmd_name}"'
        .format(cmd_name=cmd.sensor.get(
            sensor_cmd, # show command name
            '0x{:02X}'.format(sensor_cmd) # show hex if cmd is not recognized
            ))
        ]))
    # The sensor response includes sensor data if there are more bytes.
    if nbytes_remaining == 0: return
    # Read the sensor data.
    while (usb.inWaiting() < nbytes_remaining): pass
    # TODO: implement something here to read into a list of length
    # nbytes_remaining. Report how many bytes were read.

    # if status == codes.status_ok:
    #     # if usb.inWaiting()==nbytes_expected:
    #     if nbytes_expected == 0:
    #         logging.info(
    #             '..Rx PASS: bridge replies to "{}" with 0x{:02X} "{}"'
    #             .format(fname, status, codes.bridge_status[status])
    #             )
    #     if nbytes_expected > 0:
    #         logging.info(
    #             '..Rx bridge received cmd "{}" with args 0x{:X}'
    #             .format(
    #                 fname,
    #                 int.from_bytes(
    #                     usb.read(nbytes_expected),
    #                     byteorder='big',
    #                     signed=False
    #                     )
    #                 )
    #             )
        # else:
        #     logging.warning(
        #         "command: {}, status: {}: {}"
        #         .format(
        #             fname,
        #             "success, but unexpected bytes at port",
        #             int.from_bytes(usb.read_all(), byteorder='big', signed=False)
        #             )
        #         )
    # elif status == codes.status_invalid_cmd:
    #     logging.warning(
    #         '..Rx FAIL: bridge replies to "{}" with 0x{:02X} "{}" received 0x{:X}'
    #         .format(
    #             fname, status, codes.bridge_status[status],
    #             int.from_bytes(usb.read_all(), byteorder='big', signed=False)
    #             )
    #         )
    # elif status == codes.status_cmd_has_bad_args:
    #     logging.warning("".join([
    #         '..Rx FAIL: bridge received cmd "{}" as valid cmd 0x{:02X}, '
    #         .format(
    #             fname,
    #             int.from_bytes(usb.read_all(), byteorder='big', signed=False)
    #             ),
    #         'but replied "{}" 0x{:02X}'
    #         .format(codes.bridge_status[status], status)
    #         ]))
    # else:
    #     logging.error(
    #             'command "{}" {}: 0x{:X}'
    #         .format(fname, "received unexpected status code from device", status)
    #         )

def black_hat(usb):
    usb.write(b'\xAB') # send an invalid command
    log_tx(fname="black_hat", expect="FAIL")
    log_rx(fname="black_hat", nbytes_expected=2)
    # while (usb.inWaiting()<1): pass
    # log_rx(
    #     fname="black_hat",
    #     status=int.from_bytes(usb.read(1), byteorder='big', signed=False)
    #     )
# TODO: turn all of these calls into one call passing the dict command as arg
def led_red(usb):
    usb.write(cmd.led_red_key.to_bytes(1,byteorder='big'))
    log_tx(fname="led_red")
    log_rx(fname="led_red", nbytes_expected=2)
def led_green(usb):
    usb.write(cmd.led_green_key.to_bytes(1,byteorder='big'))
    log_tx(fname="led_green")
    log_rx(fname="led_green", nbytes_expected=2)
def parse_response_to_cfg_spectrometer(usb):
    while (usb.inWaiting()<1): pass
    status=int.from_bytes(usb.read(1), byteorder='big', signed=False)
    # Log status reported from device.
    log_rx(fname="cfg_spectrometer", status=status)
    # Only read more bytes if device status is OK.
    if status == codes.status_ok:
        nbytes_expected=4
        while (usb.inWaiting()<nbytes_expected): pass
        log_rx(
            fname="cfg_spectrometer",
            status=status,
            nbytes_expected=nbytes_expected
            )

# =====[ Dictionary of Spectrometer Configurations (cfgs) ]=====
# Make a dictionary with the default cfg and a few additional valid cfgs.
# There are 255 total valid cfgs allowed for by the eval kit hardware.
all_rows = b'\x0F\xFF\xFF\xF9'
rows_1_2_3 = b'\x03\x9C\xE7\x39'
row_1 = b'\x00\x84\x21\x09'
row_2 = b'\x01\x08\x42\x11'
row_3 = b'\x02\x10\x84\x21'
row_4 = b'\x04\x21\x08\x41'
row_5 = b'\x08\x42\x10\x81'
cfgs = {}
cfgs['all-rows'] = all_rows
cfgs['rows-1-2-3'] = rows_1_2_3
cfgs['row-1'] = row_1
cfgs['row-2'] = row_2
cfgs['row-3'] = row_3
cfgs['row-4'] = row_4
cfgs['row-5'] = row_5

no_rows     = b'\x00\x00\x00\x01'
#                                    xxxx5432154321543215432154321ggb
bad_cfg_1   = b'\x0F\x83\xE0\xF9' #0b00001111100000111110000011111001
#                                    xxxx5432154321543215432154321ggb
bad_cfg_2   = b'\x0F\xFF\xE0\x01' #0b00001111111111111110000000000001
#                                    xxxx5432154321543215432154321ggb
bad_cfg_3   = b'\x00\x03\xFF\xF9' #0b00000000000000111111111111111001
bad_cfgs={}
bad_cfgs['no-rows']   = no_rows
bad_cfgs['bad-cfg-1'] = bad_cfg_1
bad_cfgs['bad-cfg-2'] = bad_cfg_2
bad_cfgs['bad-cfg-3'] = bad_cfg_3

def cfg_spectrometer_bad_args(usb, bad_cfg=bad_cfgs['no-rows']):
    # TODO: when microcontroller adds timer, try sending less than 4 bytes
    #
    # Send good command with right number of args but invalid values.
    # bad_cfg =  b'\x00\x00\x00\x01' # all row select bits changed to 0 meaning *no rows active*
    # usb.write(b''.join([b'\x02',bad_cfg]))
    usb.write( b''.join([
        cmd.cfg_spectrometer_key.to_bytes(1,byteorder='big'),
        bad_cfg
        ]))
    log_tx(
        fname="cfg_spectrometer_bad_args",
        args_sent=bad_cfg,
        expect="FAIL")
    parse_response_to_cfg_spectrometer(usb)

def cfg_spectrometer(usb, cfg=cfgs['all-rows']):
    """Default `cfg` is binning on, 1x gain, and all rows active."""
    # send command followed by four bytes of data
    # 0bxxxx1111111111111111111111111001
    # bits 0-27 configure the spectrometer
    usb.write(b''.join([b'\x02',cfg]))
    log_tx(fname="cfg_spectrometer", args_sent=cfg)
    parse_response_to_cfg_spectrometer(usb)

def sensor_led(usb, key=cmd.send_led1_red_key):
    # TODO: refactor this mess

    usb.write(key.to_bytes(1,byteorder='big'))
    log_tx(fname="sensor_led({})".format(cmd.bridge[key]))
    log_rx(fname="sensor_led({})".format(cmd.bridge[key]), nbytes_expected=2)

# =====[ OLD ]=====
# =====[ Configure the spectrometer's linear array ]=====

def old_cfg_spectrometer(usb):
    """0x07: cfg, 0x06: bin, 0x0B: 1x gain, 0x0C: all rows"""
    usb.write(b'\x07\x06\x0B\x0C')

# =====[ Read a frame of data ]=====

def request_frame(usb, npixels):
    usb.write(b'\x01')
    while (usb.inWaiting()<npixels*2): pass

def pixel_reading(usb):
    return int.from_bytes(usb.read(2), byteorder='big', signed=False)

def read_frame(usb, npixels=392):
    request_frame(usb, npixels)
    return np.array([pixel_reading(usb) for pixel in range(npixels)])

# =====[ Auto-expose and manually set integration time ]=====

def to_ms(tics):
    return tics/50

def auto_expose(usb):
    usb.write(b'\x15')
    while(usb.inWaiting()<2): pass
    return to_ms(int.from_bytes(usb.read(2), byteorder='big', signed=False))

def to_tics(ms):
    return ms*50

def exposure(ms):
    return b''.join((b'\x02',to_tics(ms).to_bytes(2,byteorder='big', signed=False)))

def set_integration_time(usb,ms=1):
    usb.write(exposure(ms))
    while(usb.inWaiting()<2): pass
    return to_ms(int.from_bytes(usb.read(2), byteorder='big', signed=False))

# =====[ Process data ]=====
def is_useful(pixel):
    # TODO: get start,stop from map
    return 300<=pixel<=392

# =====[ Plot data ]=====
def spectra(usb, npixels):
    """FuncAnimation sends this generator's yield as input to `update_plot`."""
    pixels = np.array([p+1 for p in range(npixels)])
    values = read_frame(usb, npixels)
    x = np.array([pixel for pixel in pixels if is_useful(pixel)])
    y = np.array([value for pixel,value in zip(pixels,values) if is_useful(pixel)])
    yield x,y

def update_plot(data, plot_line):
    """Gets `data` from `spectra` via FuncAnimation magic."""
    plot_line.set_data(data) # global spectrum_plot stores `plot`

if __name__ == '__main__':

    logging.basicConfig(
        filename='main.log',
        format='%(asctime)s %(levelname)s: %(message)s',
        level=logging.DEBUG
        )
    # =====[ Windows ]=====
    usb = open_spectrometer('CHROMATION09310')
    # =====[ Cygwin ]=====
    # usb = open_spectrometer('ttyS7')
    led_green(usb)
    led_red(usb)
    # cfg_spectrometer(usb) # default to all-rows
    # cfg_spectrometer(usb, cfg=cfgs['rows-1-2-3'])
    # cfg_spectrometer_bad_args(usb, bad_cfg=bad_cfgs['bad-cfg-1'])
    # led1_green(usb)
    sensor_led(usb, cmd.send_led1_green_key)
    sensor_led(usb, cmd.send_led1_red_key)
    sensor_led(usb, cmd.send_led2_green_key)
    sensor_led(usb, cmd.send_led2_red_key)
    sensor_led(usb, cmd.send_led3_green_key)
    sensor_led(usb, cmd.send_led3_red_key)
    sensor_led(usb, cmd.send_led4_green_key)
    sensor_led(usb, cmd.send_led4_red_key)
    black_hat(usb)
    usb.close()
    # cfg_spectrometer(usb, cfg=cfgs['row-1'])
    # cfg_spectrometer(usb, cfg=cfgs['row-2'])
    # cfg_spectrometer(usb, cfg=cfgs['row-3'])
    # cfg_spectrometer(usb, cfg=cfgs['row-4'])
    # cfg_spectrometer(usb, cfg=cfgs['row-5'])
    # cfg_spectrometer_bad_args(usb) # default to invalid cfg: no rows
    # cfg_spectrometer_bad_args(usb, bad_cfg=bad_cfgs['bad-cfg-2'])
    # cfg_spectrometer_bad_args(usb, bad_cfg=bad_cfgs['bad-cfg-3'])


