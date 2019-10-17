#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Dictionary of Status Codes

The USB-SPI bridge is the SPI master. This module, `codes.py`, calls
it the `bridge`.

The spectrometer digital interface is the SPI slave. This module,
`codes.py` calls it the `sensor`.

`bridge_status` is a dictionary of status codes sent by the bridge.
The bridge sends a status report in response to every command sent by
the USB Host. If there are no errors, the status report is the single
byte `status_ok`. TODO: make this a two-byte code, standardizing the
behavior of bridge and sensor. Number of bytes in a status report
should always be two, even for `status_ok`.

`sensor_status` is a dictionary of status codes sent by the sensor.
The sensor sends a status report in response to every command sent by
the USB Host. If there are no errors, the status report is two bytes:
`status_ok` and the command that it just executed.

If the command sent by the USB Host causes the bridge to send a
command to the sensor, the bridge first responds to the USB Host with
its status once it knows whether it has successfully pushed the
command to the sensor. Then, once it has pulled the respsone from the
sensor, whatever that response is, it pushes it to the USB Host.

Messages can only reach the USB Host via the bridge. This protocol
gives the USB Host a context for parsing all messages from the
bridge.
"""

# =====[ Dictionary of Communication Codes (bridge_status) ]=====
OK = 0x00
ERROR = 0x01
INVALID = 0xFF
status_ok               = 0x00
status_invalid_cmd      = 0xff
status_cmd_has_bad_args = 0xfe
status_cmd_missing_args = 0xfd
status_spi_bus_error    = 0xfc
bridge_status = {}
bridge_status[status_ok] = 'status_ok'
bridge_status[status_invalid_cmd] = 'status_invalid_cmd'
bridge_status[status_cmd_has_bad_args] = 'status_cmd_has_bad_args'
bridge_status[status_cmd_missing_args] = 'status_cmd_missing_args'
bridge_status[status_spi_bus_error] = 'status_spi_bus_error'

# =====[ Dictionary of Communication Codes (sensor_status) ]=====
sensor_status = {}
sensor_status[status_ok] = 'status_ok'
sensor_status[status_invalid_cmd] = 'status_invalid_cmd'

if __name__ == '__main__':
    print("\n# Communication Status Codes")
    print("\n## Status Codes for the USB-SPI Bridge")
    for k,v in reversed(sorted(bridge_status.items())):
        print("{name:{width}} = 0x{code:02X}".format(name=v,width=23,code=k))
    print("\n## Status Codes for the Spectrometer Digital Inteface")
    for k,v in reversed(sorted(sensor_status.items())):
        print("{name:{width}} = 0x{code:02X}".format(name=v,width=23,code=k))
