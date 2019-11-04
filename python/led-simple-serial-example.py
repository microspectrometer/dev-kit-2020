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

def test_DoesUsbBuffer():
    """What happens when the USB bridge IC receives a lot of bytes.

    Send lots of bytes one after another *without* print to screen
    inbetween. Print to screen takes a long time, providing a delay that hides
    the need for buffering.
    """
    _print_and_log("--- DoesUsbBuffer ---")
    print("test_DoesUsbBuffer: FAIL: Implement test.")

def test_NullCommand():
    """NullCommand results in no response from Bridge or from Sensor.

    NullCommand does not cause an error.
    NullCommand causes Sensor LEDs to turn green since it executes successfully.
    """
    _print_and_log("--- NullCommand ---")
    # Send the NullCommand.
    _tx_and_log_cmd(
        commands.NullCommand,
        "Command is NullCommand."
        )
    if (kit.inWaiting() > 0):
        _print_and_log("test_NullCommand: FAIL")
        return
    # Send the NullCommand again.
    _tx_and_log_cmd(
        commands.NullCommand,
        "Command is NullCommand."
        )
    if (kit.inWaiting() > 0):
        _print_and_log("test_NullCommand: FAIL")
        return
    _print_and_log("test_NullCommand: PASS")

def test_InvalidBridgeCommand():
    """What happens when the USB Host sends a command the Bridge does not
    recognize.
    """
    _print_and_log("--- InvalidBridgeCommand ---")
    _tx_and_log_cmd(
        0xFF,
        "Command invalid on Bridge"
        )
    # read BRIDGE status byte, expect invalid command error
    expected = codes.INVALID_CMD
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect 0x02: INVALID COMMAND ERROR"
        )
    if reply != expected:
        _print_and_log(f"test_InvalidBridgeCommand: FAIL: Expected {expected}, received {reply}.")
        return
    # read SENSOR status byte, expect invalid command error
    expected = codes.INVALID_CMD
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect 0x02: INVALID COMMAND ERROR"
        )
    if reply != expected:
        _print_and_log(f"test_InvalidBridgeCommand: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_InvalidBridgeCommand: PASS")

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
        _print_and_log(f"test_InvalidSensorCommand: FAIL: Expected {expected}, received {reply}.")
        return
    # read SENSOR status byte, expect invalid command error
    expected = codes.INVALID_CMD
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect 0x02: INVALID COMMAND ERROR"
        )
    if reply != expected:
        _print_and_log(f"test_InvalidSensorCommand: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_InvalidSensorCommand: PASS")

def test_InvalidSensorCommandPlusPayload():
    """Send an invalid command and follow it with a one byte Payload that does
    not alias to a valid command. This test consistenly passes because the
    Bridge does *not* send the payload if the Sensor responds with an
    invalid-command-error.
    """
    _print_and_log("--- InvalidSensorCommand PlusPayload ---")
    _tx_and_log_cmd(
        commands.test_invalid_sensor_cmd_plus_payload,
        "Command with 1 byte of payload, valid on Bridge, invalid on Sensor"
        )
    payload = 0x11
    _tx_and_log_cmd(payload, f"Payload is {payload}")
    # read BRIDGE status byte, stop if it is *not* invalid-command-error
    expected = codes.INVALID_CMD
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect INVALID COMMAND ERROR from Sensor"
        )
    if reply != expected:
        _print_and_log(f"test_InvalidSensorCommandPlusPayload: FAIL: Expected {expected}, received {reply}.")
        return
    # There should be no more bytes waiting from the Bridge
    expected = 0
    actual = kit.inWaiting()
    if expected != actual:
        _print_and_log(f"test_InvalidSensorCommandPlusPayload: FAIL: Expected {expected} additional bytes, Bridge still has {actual} bytes.")
        return
    _print_and_log("test_InvalidSensorCommandPlusPayload: PASS")

def test_InvalidSensorCommandPlusPayloadThatAliases():
    """This test passes because the Bridge does *not* send the payload if the
    Sensor says the command is invalid.
    """
    _print_and_log("--- InvalidSensorCommand Plus payload aliased to valid command---")
    _tx_and_log_cmd(
        commands.test_invalid_sensor_cmd_plus_payload,
        "Command with 1 byte of payload, valid on Bridge, invalid on Sensor"
        )
    payload = 0x03
    _tx_and_log_cmd(payload, f"Payload is {payload}")
    _print_and_log(f"Payload {payload} aliases to command GetSensorLED")
    # read BRIDGE status byte, stop if it is *not* invalid-command-error
    expected = codes.INVALID_CMD
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect INVALID COMMAND ERROR from Sensor"
        )
    if reply != expected:
        _print_and_log(f"test_InvalidSensorCommandPlusPayload: FAIL: Expected {expected}, received {reply}.")
        return
    # There should be no more bytes waiting from the Bridge
    expected = 0
    actual = kit.inWaiting()
    if expected != actual:
        _print_and_log(f"test_InvalidSensorCommandPlusPayload: FAIL: Expected {expected} additional bytes, Bridge still has {actual} bytes.")
        return
    _print_and_log("test_InvalidSensorCommandPlusPayload: PASS")

def test_GetBridgeLED_ExpectGreen(led_number):
    _print_and_log("--- GetBridgeLED: Expect GREEN ---")
    if commands.led_0 != led_number:
        _print_and_log(f"test_GetBridgeLED: FAIL: LED {led_number} does not exist")
        return
    _tx_and_log_cmd(commands.GetBridgeLED, "Command is GetBridgeLED")
    _tx_and_log_cmd(led_number, f"LED is LED{led_number}")
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetBridgeLED: FAIL: Expected {expected}, received {reply}.")
        return
    expected = commands.led_green
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="led_status",
        expected_reply_byte=expected,
        optional_expectation="Expect GREEN"
        )
    if reply != expected:
        _print_and_log(f"test_GetBridgeLED: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_GetBridgeLED: PASS")
def test_GetBridgeLED_ExpectRed(led_number):
    _print_and_log("--- GetBridgeLED: Expect RED ---")
    if commands.led_0 != led_number:
        _print_and_log(f"test_GetBridgeLED: FAIL: LED {led_number} does not exist")
        return
    _tx_and_log_cmd(commands.GetBridgeLED, "Command is GetBridgeLED")
    _tx_and_log_cmd(led_number, f"LED is LED{led_number}")
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetBridgeLED: FAIL: Expected {expected}, received {reply}.")
        return
    expected = commands.led_red
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="led_status",
        expected_reply_byte=expected,
        optional_expectation="Expect RED"
        )
    if reply != expected:
        _print_and_log(f"test_GetBridgeLED: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_GetBridgeLED: PASS")
def test_GetBridgeLED_ExpectOff(led_number):
    _print_and_log("--- GetBridgeLED: Expect OFF ---")
    if commands.led_0 != led_number:
        _print_and_log(f"test_GetBridgeLED: FAIL: LED {led_number} does not exist")
        return
    _tx_and_log_cmd(commands.GetBridgeLED, "Command is GetBridgeLED")
    _tx_and_log_cmd(led_number, f"LED is LED{led_number}")
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetBridgeLED: FAIL: Expected {expected}, received {reply}.")
        return
    expected = commands.led_off
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="led_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OFF"
        )
    if reply != expected:
        _print_and_log(f"test_GetBridgeLED: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_GetBridgeLED: PASS")

def test_SetBridgeLED(led_number, led_state):
    _print_and_log("--- SetBridgeLED ---")
    _tx_and_log_cmd(commands.SetBridgeLED, "Command is SetBridgeLED")
    _tx_and_log_cmd(led_number, f"LED is LED{led_number}")
    # _tx_and_log_cmd(commands.led_green, "State is GREEN")
    if led_state == commands.led_red:
        _tx_and_log_cmd(led_state, "State is RED")
    elif led_state == commands.led_green:
        _tx_and_log_cmd(led_state, "State is GREEN")
    elif led_state == commands.led_off:
        _tx_and_log_cmd(led_state, "State is OFF")
    else:
        _print_and_log(f"test_SetBridgeLED: FAIL: {led_state} is an invalid led_state")
        return
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_SetBridgeLED: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_SetBridgeLED: PASS")

def test_GetSensorLED_ExpectOff(led_number):
    # led_0 is labeled TxRx
    # led_1 is labeled Done
    _print_and_log("--- GetSensorLED: Expect OFF ---")
    _tx_and_log_cmd(commands.GetSensorLED, "Command is GetSensorLED")
    _tx_and_log_cmd(led_number, f"LED is LED{led_number}")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    # read status byte from SENSOR
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    expected = commands.led_off
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="led_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OFF"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_GetSensorLED: PASS")
def test_GetSensorLED_ExpectGreen(led_number):
    # led_0 is labeled TxRx
    # led_1 is labeled Done
    _print_and_log("--- GetSensorLED: Expect GREEN ---")
    _tx_and_log_cmd(commands.GetSensorLED, "Command is GetSensorLED")
    _tx_and_log_cmd(led_number, f"LED is LED{led_number}")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    # read status byte from SENSOR
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    expected = commands.led_green
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="led_status",
        expected_reply_byte=expected,
        optional_expectation="Expect GREEN" if expected==commands.led_green else "Expect RED"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_GetSensorLED: PASS")
def test_GetSensorLED_ExpectRed(led_number):
    # led_0 is labeled TxRx
    # led_1 is labeled Done
    _print_and_log("--- GetSensorLED: Expect RED ---")
    _tx_and_log_cmd(commands.GetSensorLED, "Command is GetSensorLED")
    _tx_and_log_cmd(led_number, f"LED is LED{led_number}")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    # read status byte from SENSOR
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    expected = commands.led_red
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="led_status",
        expected_reply_byte=expected,
        optional_expectation="Expect GREEN" if expected==commands.led_green else "Expect RED"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_GetSensorLED: PASS")
def test_SetSensorLED(led_number, led_state):
    _print_and_log("--- SetSensorLED ---")
    _tx_and_log_cmd(commands.SetSensorLED, "Command is SetSensorLED")
    _tx_and_log_cmd(led_number, f"LED is LED{led_number}")
    _tx_and_log_cmd(led_state, f"LED state is {led_state}")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_SetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    # read status byte from SENSOR
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_SetSensorLED: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_SetSensorLED: PASS")
def test_SetSensorConfig(binning, gain, active_rows):
    """Test sending the photodiode array configuration."""
    _print_and_log("--- SetSensorConfig ---")
    _tx_and_log_cmd(commands.SetSensorConfig, "Command is SetSensorConfig")
    _tx_and_log_cmd(binning, f"binning is {binning}")
    _tx_and_log_cmd(gain, f"gain is {gain}")
    _tx_and_log_cmd(active_rows, f"active_rows is {active_rows}")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_SetSensorConfig: FAIL: Expected {expected}, received {reply}.")
        return
    # read status byte from SENSOR
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_SetSensorConfig: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_SetSensorConfig: PASS")
def test_GetSensorConfig(expected_binning, expected_gain, expected_active_rows):
    """Test reading the photodiode array configuration."""
    _print_and_log("--- GetSensorConfig ---")
    _tx_and_log_cmd(commands.GetSensorConfig, "Command is GetSensorConfig")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorConfig: FAIL: Expected {expected}, received {reply}.")
        return
    # read status byte from SENSOR
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorConfig: FAIL: Expected {expected}, received {reply}.")
        return
    expected = expected_binning
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="pixel binning config",
        expected_reply_byte=expected,
        optional_expectation=f"Expect binning is {expected}"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorConfig: FAIL: Expected {expected}, received {reply}.")
        return
    expected = expected_gain
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="gain config",
        expected_reply_byte=expected,
        optional_expectation=f"Expect gain is {expected}"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorConfig: FAIL: Expected {expected}, received {reply}.")
        return
    expected = expected_active_rows
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="active pixel row config",
        expected_reply_byte=expected,
        optional_expectation=f"Expect active_rows is {expected}"
        )
    if reply != expected:
        _print_and_log(f"test_GetSensorConfig: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_GetSensorLED: PASS")
def test_GetExposure():
    """Test reading the exposure time."""
    _print_and_log("--- GetExposure ---")
    _tx_and_log_cmd(commands.GetExposure, "Command is GetExposure")
    # read BRIDGE status byte, stop if it is an error
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="BRIDGE",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetExposure: FAIL: Expected {expected}, received {reply}.")
        return
    # read status byte from SENSOR
    expected = codes.OK
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect OK"
        )
    if reply != expected:
        _print_and_log(f"test_GetExposure: FAIL: Expected {expected}, received {reply}.")
        return
    expected = expected_exposure
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="exposure time",
        expected_reply_byte=expected,
        optional_expectation=f"Expect exposure time is {expected}"
        )
    if reply != expected:
        _print_and_log(f"test_GetExposure: FAIL: Expected {expected}, received {reply}.")
        return
    _print_and_log("test_GetExposure: PASS")

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
        _print_and_log("test_GetSensorLED_Invalid_LED: FAIL")
        return
    # read status byte from SENSOR
    expected = codes.ERROR
    reply = _rx_and_log_reply(
        device_name="SENSOR",
        reply_type="msg_status",
        expected_reply_byte=expected,
        optional_expectation="Expect ERROR"
        )
    if reply != expected:
        _print_and_log("test_GetSensorLED_Invalid_LED: FAIL")
        return
    _print_and_log("(note Sensor LED TxRx turns red to indicate Command Parameter ERROR)")
    _print_and_log("test_GetSensorLED_Invalid_LED: PASS")

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
    sernum='091103' # Mike's Bridge

    # USB open/close is handled by `pyserial` context manager in serialutil.py
    with usb.open_spectrometer(sernum) as kit:
        # `kit` is an instance of `pyserial` class serial.Serial()
        # `kit.write` is defined in site-packages/serial/serialwin32.py:301
        _print_and_log(f"Opened CHROMATION{sernum} on {usb.dev_name(sernum)}")
        # TODO: setup kit.write to take GetBridgeLED with its argument
        # TODO: add cmd pre-formatted as bytes to package `commands`
        # test_NullCommand()
        # test_SetBridgeLED(commands.led_0, commands.led_off)
        # test_GetBridgeLED_ExpectOff(commands.led_0)
        # test_SetBridgeLED(commands.led_0, commands.led_green)
        # test_GetBridgeLED_ExpectGreen(commands.led_0)
        # test_SetBridgeLED(commands.led_0, commands.led_red)
        # test_GetBridgeLED_ExpectRed(commands.led_0)

        # test_SetSensorLED(commands.led_0, commands.led_off)
        # test_GetSensorLED_ExpectOff(commands.led_0)
        # test_SetSensorLED(commands.led_0, commands.led_green)
        # test_GetSensorLED_ExpectGreen(commands.led_0)
        # test_SetSensorLED(commands.led_0, commands.led_red)
        # test_GetSensorLED_ExpectRed(commands.led_0)

        # test_SetSensorLED(commands.led_1, commands.led_off)
        # test_GetSensorLED_ExpectOff(commands.led_1)
        # test_SetSensorLED(commands.led_1, commands.led_green)
        # test_GetSensorLED_ExpectGreen(commands.led_1)
        # test_SetSensorLED(commands.led_1, commands.led_red)
        # test_GetSensorLED_ExpectRed(commands.led_1)

        # test_GetSensorLED_Invalid_LED()
        # test_InvalidBridgeCommand()
        # test_NullCommand()
        # test_InvalidSensorCommand()
        # test_InvalidSensorCommandPlusPayload()
        # test_InvalidSensorCommandPlusPayloadThatAliases()
        # test_SetBridgeLED(commands.led_0, commands.led_green)
        # test_SetSensorLED(commands.led_0, commands.led_green)
        # test_SetSensorLED(commands.led_1, commands.led_green)

        # TODO: use these to bit set a byte for active_rows
        active_rows = 0x00
        row1 = 0
        row2 = 1
        row3 = 2
        row4 = 3
        row5 = 4
        # test_SetSensorConfig(
        #     binning=commands.binning_on,
        #     gain=commands.gain1x,
        #     active_rows=commands.all_rows_active
        #     )
        # test_GetSensorConfig(
        #     expected_binning=commands.binning_on,
        #     expected_gain=commands.gain1x,
        #     expected_active_rows=commands.all_rows_active
        #     )
        test_GetExposure()
        # test_SetExposure()

        # test_DoesUsbBuffer()
    _print_and_log(f"Closed CHROMATION{sernum}")
    # _print_and_log(f"Closed CHROMATION{sernum}")
