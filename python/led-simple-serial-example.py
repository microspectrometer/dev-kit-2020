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
import codes

# Turn commands into byte arrays ready to write over serial.
# led1_red   = commands.send_led1_red_key.to_bytes(1,byteorder='big')
# led1_green = commands.send_led1_green_key.to_bytes(1,byteorder='big')

def test_SetBridgeLED():
    _print_and_log("--- SetBridgeLED ---")
    _tx_and_log_cmd(commands.SetBridgeLED, "Command is SetBridgeLED")
    _tx_and_log_cmd(commands.led_0, "LED is LED0")
    # _tx_and_log_cmd(commands.led_green, "State is GREEN")
    _tx_and_log_cmd(commands.led_red, "State is RED")
    _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=codes.OK,
        optional_expectation="Expect OK"
        )

def test_DoesUsbBuffer():
    print("Send lots of bytes one after another without print to screen inbetween.")

def test_GetBridgeLED():
    _print_and_log("--- GetBridgeLED ---")
    _tx_and_log_cmd(commands.GetBridgeLED, "Command is GetBridgeLED")
    _tx_and_log_cmd(commands.led_0, "LED is LED0")
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=codes.OK,
        optional_expectation="Expect OK"
        )
    if reply == codes.OK:
        reply = _rx_and_log_reply(
            device_name="BRIDGE",
            reply_type="led_status",
            expected_reply_byte=commands.led_green,
            optional_expectation="Expect GREEN"
            )

def test_InvalidSensorCommand():
    _print_and_log("--- InvalidSensorCommand ---")
    _tx_and_log_cmd(
        commands.test_invalid_sensor_cmd,
        "Command valid on Bridge, invalid on Sensor"
        )
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    # read SENSOR status byte, stop if it is *not* an error
    expected = codes.ERROR
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect ERROR: invalid command"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    _print_and_log("(note Sensor LEDs turn red to indicate Sensor ERROR)")
    _print_and_log("TEST PASSED")

def test_GetSensorLED_Invalid_LED():
    _print_and_log("--- GetSensorLED for Invalid LED ---")
    _tx_and_log_cmd(commands.GetSensorLED, "Command is GetSensorLED")
    _tx_and_log_cmd(commands.led_2, "LED is LED2")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    # read two bytes from SENSOR
    while (kit.inWaiting() < 2): pass # (message size is two bytes)
    expected = codes.ERROR
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect ERROR"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    # Read the dummy byte.
    while (kit.inWaiting() < 1): pass
    rx_byte = int.from_bytes(
        kit.read(1),
        byteorder='big', signed=False
        )
    _print_and_log(f"Rx.. SENSOR: led_status 0x{rx_byte:02X}"
        )
    _print_and_log("(note Sensor LEDs turn red to indicate Sensor ERROR)")
    _print_and_log("TEST PASSED")

def test_GetSensorLED():
    _print_and_log("--- GetSensorLED ---")
    _tx_and_log_cmd(commands.GetSensorLED, "Command is GetSensorLED")
    _tx_and_log_cmd(commands.led_0, "LED is LED0")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    # read two bytes from SENSOR
    while (kit.inWaiting() < 2): pass # (message size is two bytes)
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    expected = commands.led_green
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="led_status",
        expected_reply_byte=expected,
        optional_expectation="Expect GREEN"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    _print_and_log("TEST PASSED")

def oldtest_InvalidSensorCommand():
    _print_and_log("--- InvalidSensorCommand ---")
    _tx_and_log_cmd(commands.test_invalid_sensor_cmd, "Command valid on Bridge, invalid on Sensor")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    # read SENSOR status byte, stop if it is *not* an error
    expected = codes.ERROR
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect ERROR: invalid command"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    _print_and_log("(note Sensor LEDs turn red to indicate Sensor ERROR)")
    _print_and_log("TEST PASSED")

def oldtest_GetSensorLED_Invalid_LED():
    _print_and_log("--- GetSensorLED for Invalid LED ---")
    _tx_and_log_cmd(commands.GetSensorLED, "Command is GetSensorLED")
    _tx_and_log_cmd(commands.led_2, "LED is LED2")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    # read SENSOR status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    # read SENSOR remaining message size
    while (kit.inWaiting() < 2): pass # (message size is always a 16-bit word)
    rx_size = int.from_bytes(
        kit.read(2),
        byteorder='big', signed=False
        )
    _print_and_log(f"Rx.. SENSOR: "
        f"reply size: 0x{rx_size:04X} (Expect 1 byte)"
        )
    # ------------------------------------------------------
    # read the rest of the message, expecting one byte
    # TODO: read all remaining bytes (regardless of size) and print them
    while (kit.inWaiting() < rx_size): pass
    # read SENSOR status byte, stop if it is *not* an error
    expected = codes.ERROR
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect ERROR: invalid LED number"
        )
    # Stop either way.
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    _print_and_log("(note Sensor LEDs turn red to indicate Sensor ERROR)")
    _print_and_log("TEST PASSED")

def oldtest_GetSensorLED():
    _print_and_log("--- GetSensorLED ---")
    _tx_and_log_cmd(commands.GetSensorLED, "Command is GetSensorLED")
    _tx_and_log_cmd(commands.led_0, "LED is LED0")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    # read SENSOR status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    # read SENSOR remaining message size
    while (kit.inWaiting() < 2): pass # (message size is always a 16-bit word)
    rx_size = int.from_bytes(
        kit.read(2),
        byteorder='big', signed=False
        )
    _print_and_log(f"Rx.. SENSOR: "
        f"reply size: 0x{rx_size:04X} (Expect 2 bytes)"
        )
    # ------------------------------------------------------
    # read the rest of the message, expecting two bytes
    # TODO: read all remaining bytes (regardless of size) and print them
    while (kit.inWaiting() < rx_size): pass
    # read SENSOR status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    # Stop if there is an error.
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    # Read the rest of the message.
    expected = commands.led_green
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="led_status",
        expected_reply_byte=expected,
        optional_expectation="Expect GREEN"
        )
    if reply != expected:
        _print_and_log("TEST FAILED")
        return
    _print_and_log("TEST PASSED")

def _tx_and_log_cmd(cmd, optional_description):
    """Send one-byte command (or command parameter) `cmd` and log the send to
    file.

    If string optional_description is given, it as appended in the log as a
    human-readable description of the command or command parameter.

    `cmd` is a numeric value defined in `commands.py`.

    TODO: make optional_description optional.
    """

    msg = cmd.to_bytes(1,byteorder='big')
    _print_and_log(f"Tx.. send 0x{msg.hex()} ({optional_description})")
    kit.write(msg)

def _rx_and_log_reply(device_name, reply_type, expected_reply_byte, optional_expectation):
    """Receive one-byte reply and log to file.

    Argument `expected_reply_byte` is numeric. All other input arguments are strings.

    device_name:
        - device sending the reply
        - either BRIDGE or SENSOR

    reply_type is one of the following:
        - msg_status
        - led_status

    expected_reply_byte:
        - numeric value of expected reply
        - use `codes.py` instead of passing a literal number
        - example: for OK (0x00), pass `codes.OK` instead of passing `0x00`

    If string optional_expectation is given, it as appended in the log as a
    human-readable description of the expected reply, for example, the string
    `Expect OK` in the log entry: `msg_status 0x00 (Expect OK)`.

    TODO: make optional_expectation optional.
    """

    while (kit.inWaiting() < 1): pass
    rx_byte = int.from_bytes(
        kit.read(1),
        byteorder='big', signed=False
        )
    _print_and_log(f"Rx.. {device_name}: {reply_type} 0x{rx_byte:02X}"
        )
    if rx_byte == expected_reply_byte: test_result="PASS"
    else: test_result="FAIL"
    _print_and_log(
        f"{test_result}: Expected 0x{expected_reply_byte:02X}, received 0x{rx_byte:02X}. "
        f"({optional_expectation})."
        )

    return rx_byte

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
    # sernum='093101' # Sean's Bridge
    sernum='091113' # Mike's Bridge

    # USB open/close is handled by `pyserial` context manager in serialutil.py
    with usb.open_spectrometer(sernum) as kit:
        # `kit` is an instance of `pyserial` class serial.Serial()
        # `kit.write` is defined in site-packages/serial/serialwin32.py:301
        _print_and_log(f"Opened CHROMATION{sernum} on {usb.dev_name(sernum)}")
        # TODO: setup kit.write to take GetBridgeLED with its argument
        # TODO: add cmd pre-formatted as bytes to package `commands`
        # test_GetBridgeLED()
        # test_SetBridgeLED()
        test_GetSensorLED()
        test_InvalidSensorCommand()
        test_GetSensorLED_Invalid_LED()
        # test_DoesUsbBuffer()
        # test_SetSensorLED()
    _print_and_log(f"Closed CHROMATION{sernum}")
    # _print_and_log(f"Closed CHROMATION{sernum}")
