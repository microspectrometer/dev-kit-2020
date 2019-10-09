#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Dictionary of Commands

Usage:
    # Run this file to print the command dictionaries:
    $ ./commands.py

    # Import this file to use the dictionaries:
    >>> import commands as cmd

    # Print the value of key 0 in dict `bridge`:
    >>> print(f'{cmd.bridge[0]}')
    led_red

    Do not use keys by number.
    Use the constant *name* instead.

    # Here is the previous example, but using the *name*:
    >>> print(f'{cmd.bridge[cmd.led_red_key]}')
    led_red

    Be careful which constant *names* you use.
    The USB Host only *transmits* commands for the `bridge`.
    So a `usb.write` must use `cmd.bridge`
    (and only uses keys defined in the `bridge` dict)!

Example `bridge` command:
    >>> key=cmd.led_red
    >>> usb.write(key.to_bytes(1,byteorder='big'))

    The `bridge` led turns red.
    The `bridge` sends a two-byte response.
    Again, use the `bridge` dict to look up what the response is.

    # Read the response:
    >>> bridge_status = int.from_bytes(usb.read(1), byteorder='big', signed=False)
    >>> bridge_cmd = int.from_bytes(usb.read(1), byteorder='big', signed=False)

    `bridge_cmd` is a key in the `cmd.bridge` dictionary.

    # Use `bridge_cmd` to look up the command the bridge executed.
    >>> print(f"{cmd.bridge.get(bridge_cmd)}")

    # Fancier: default to the byte value if not recognized:
    >>> print(f"{cmd.bridge.get(
            bridge_cmd,
            '0x{:02X}'.format(bridge_cmd)
            )}")

    `bridge_status` is an error code:
    0x00 is OK, any other value is ERROR
    Use `codes.py` to look up the error code.
    See `codes.py` for details.

Example `sensor` command:
    A command for the `sensor` has to pass *through* the `bridge`.
    (It's a command to the `bridge` to send a command to the `sensor`.)
    So really it's a `bridge` command *and* a `sensor` command.
    The USB Host receives responses from both.

    # Turn `sensor` led1 red:
    >>> key=cmd.send_led1_red_key
    >>> usb.write(key.to_bytes(1,byteorder='big'))

    This time the response is more than two bytes.
    The first two bytes back are from the `bridge`.
    These work like the `led_red` example, except here
    the bridge is reporting success/fail that it sent the
    command to the sensor.

    The remaining bytes are from the sensor.
    The bridge is just forwarding whatever the sensor sent.

    The first two bytes from the sensor is the message length.
    # Find out how many more bytes are coming:
    >>> nbytes_of_data = int.from_bytes(usb.read(2), byteorder='big', signed=False)

    `nbytes_of_data` is always >= 2
    because the `sensor`, just like the `bridge`,
    sends a byte of ERROR code and a byte stating
    which command it just executed.

    This is where the USB Host makes uses of the `sensor` dict.
    This command was executed by the sensor.
    So look up the command in the `sensor` dict,
    not the `bridge` dict.

    >>> sensor_status = int.from_bytes(usb.read(1), byteorder='big', signed=False)
    >>> sensor_cmd = int.from_bytes(usb.read(1), byteorder='big', signed=False)
    >>> print(f"{cmd.sensor.get(
            sensor_cmd,
            '0x{:02X}'.format(sensor_cmd)
            )}")

Sensor response length:
    Turning an LED red does not return data,
    so the entire sensor message is only 4 bytes:
        - 2 bytes stating number of bytes remaining
        - 1 bytes error code
        - 1 byte stating which cmd it did

    Additional bytes would follow these four if the command
    results in the sensor returning data.

How this script is organized:
    From the USB Host perspective, there is just *one* set of commands.
    But the eval kit is really *two* independent embedded systems:

1. `bridge`
    a USB-SPI bridge to provide the eval kit with a USB interface

2. `sensor`
   a Spectrometer Digital Interface to provide the spectrometer as a
   SPI Slave.

The intent is flexibility to use the eval hardware as is with the
USB-SPI bridge as the SPI master, or disconnect the eval kit at the
ribbon cable to interface the SPI slave with your own SPI bus.

Of course you can discard the eval kit with its commands and status
codes altogether by just connecting the flat-flexible cable to your
own digital interface, or better, eliminate the cabling altogether by
purchasing the surface-mount spectrometer chip on its own for PCB
assembly.

If you are using the eval kit hardware and writing your own Python
interface to communicate over USB, read on.

*Two Dictionaries Of Commands And Two Dictionaries Of Status Codes*

The two embedded systems in the eval kit maintain independent jump
tables. The jump table is an array of function pointers. The jump
tables on the microcontrollers use simple array indexing as the key
for each command in the jump table. Therefore both tables start with
key 0x00, then 0x01, etc. Since the bridge and sensor use the same
keys to refer to different commands, the USB Host maintains the sensor
commands and bridge commands in separate dictionaries.

This is exactly how this Python module, `commands.py`, is set up. It
maintains *two* separate dictionaries of commands for the USB Host to
reference when interpreting command response bytes.

Similarly, module `codes.py` maintains *two* separate dictionaries of
status codes for the USB Host to interpret the status bytes sent from
the two embedded systems. The status codes do not have the same
restriction as the command codes because the microcontrollers never
need to look up the codes, and Python's dictionary data structure has
no such limitations on key values as my simple jump table. But I still
split these into two dictionaries since the plumbing is already in
place for the USB Host to know which system originated the status
report.

*Communication Flow, Error Handling, and Intent for the USB Host*

The hardware is in a physical chain: USB cable to bridge, header or jumper
to sensor. The bridge receives commands from the USB Host and always
responds with a status report.

Most commands also require the bridge to send a command to the sensor.
The sensor receives commands from the bridge and reports its status
after every command executed. The bridge just passes these responses
back up to the USB Host. It does not attempt to parse the sensor's
status, except to validate that the sensor responded with the expected
number of bytes.

The command code provides context for the USB Host as to the state
each system was in when it reported its status. Python module
`logging` makes it easy to collect these reports in a file for
troubleshooting. The command-and-response nature of the protocol makes
it easy to use the log file as output for system tests (i.e., unit
tests that make actual calls to the eval kit hardware).

In the usual case there is no error on the SPI bus and the USB Host
parses the status codes in the responses from the two systems.
Both systems report their status after *every* command executed. The
status report is a one-byte status code, followed by the one-byte
command code that was just executed. If the command requires sending
data, and the status is `OK`, then that data follows the status
report.

If the status is an error code, it is up to the USB Host
how to proceed. For most errors, I expect resending the command fixes
the problem. One exception is auto exposure.

The sensor handles its own auto exposure algorithm, adjusting
integration time until the peak reading is within a target range.
Occassionally this algorithm fails to settle on a final choice of
integration time. In this instance, the bridge times out because it
thinks the sensor is taking too long to respond (TODO: make the time
out for the bridge configurable from the USB Host). The bridge sends a
command to abandon the current task, gets a response from the sensor,
then sends a status report to the USB Host with a timed out error and,
from the sensor's status report, the USB Host knows which command the
sensor was executing when it timed out.

In this case, regardless of how the USB Host will remedy the
situation, it should first send a new command setting the integration
time. This is to get the sensor's integration time back in sync with
the value in memory at the USB Host.

There are non-recoverable errors. For example, the bridge reports a
SPI Bus Error if the sensor is not responding as the bridge expects.
This scenario is more dire than the usual error because it means the
bridge and sensor have somehow lost synchronization in their
communication protocol. In this case, the USB Host should do a system
reset, then resynchronize values in memory by resending the
spectrometer configuration and integration time. This whole process is
very fast.
"""

led_0 = 0
# =====[ Dictionary of Bridge Command Keys ]=====
# For the USB Host both to *send* to the bridge and to *parse* its reports.
GetBridgeLED = 0
SetBridgeLED = 1
# led_red_key = 0
# led_green_key = 1
# cfg_spectrometer_key = 2
# send_led1_red_key = 3
# send_led1_green_key = 4
# send_led2_red_key = 5
# send_led2_green_key = 6
# send_led3_red_key = 7
# send_led3_green_key = 8
# send_led4_red_key = 9
# send_led4_green_key = 10
bridge = {}
bridge[GetBridgeLED] = 'GetBridgeLed'
bridge[SetBridgeLED] = 'SetBridgeLed'
# bridge[led_green_key] = 'led_green'
# bridge[led_red_key] = 'led_red'
# bridge[cfg_spectrometer_key] = 'cfg_spectrometer'
# bridge[send_led1_green_key] = 'send_led1_green'
# bridge[send_led1_red_key] = 'send_led1_red'
# bridge[send_led2_green_key] = 'send_led2_green'
# bridge[send_led2_red_key] = 'send_led2_red'
# bridge[send_led3_green_key] = 'send_led3_green'
# bridge[send_led3_red_key] = 'send_led3_red'
# bridge[send_led4_green_key] = 'send_led4_green'
# bridge[send_led4_red_key] = 'send_led4_red'

# =====[ Dictionary of Sensor Command Keys ]=====
# Only for USB Host to parse the sensor status report.
# The commands are sent to the sensor by the bridge.
led1_green_key = 0
led1_red_key = 1
led2_green_key = 2
led2_red_key = 3
led3_green_key = 4
led3_red_key = 5
led4_green_key = 6
led4_red_key = 7
sensor = {}
sensor[led1_green_key] = 'led1_green'
sensor[led1_red_key] = 'led1_red'
sensor[led2_green_key] = 'led2_green'
sensor[led2_red_key] = 'led2_red'
sensor[led3_green_key] = 'led3_green'
sensor[led3_red_key] = 'led3_red'
sensor[led4_green_key] = 'led4_green'
sensor[led4_red_key] = 'led4_red'

if __name__ == '__main__':
    print("\n# Command Keys")
    print("\n## Commands Handled by the USB-SPI Bridge")
    for k,v in sorted(bridge.items()):
        print("{name:{width}} = 0x{key:02X}".format(name=v,width=23,key=k))
    print("\n## Commands Handled by the Spectrometer Digital Inteface")
    for k,v in sorted(sensor.items()):
        print("{name:{width}} = 0x{key:02X}".format(name=v,width=23,key=k))

