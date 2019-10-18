% USB Protocol Design for Developing the Firmware and Host-PC Python API

# 2019-10-17 update
- [ ] reply size is always standard
    - e.g, GetBridgeLED: if status is error, still send one byte placeholder for
      led status
- [ ] Sensor reply to CaptureFrame starts with reply size, but no other command
  starts with reply size
- [ ] implement SPI without needing a handshake after the command byte (use
  interrupts)
- [ ] check that USB buffer does not overflow
    - do a system test confirming that FTDI chip is buffering
    - concern is that if SPI tucks away incoming bytes in the ISR, it will
      overflow whatever SPI buffer I designate or it will interrupt other
      activity

## Sean tasks
- [ ] figure out how to build with avr-gcc and flash with avrdude
- [ ] create script I can invoke from Makefile to generate C header file from
  JSON
- [ ] finish API to the point I can use it instead of my own Python code for
  system tests

## Mike tasks
- [ ] make programmer name (atmelice or avrisp) a variable we in the Makefile for
  setting programmer name when invoking make

# See Sean's UsbProtocolTable
- created as an OpenOffice spreadsheet
- PDF is the pretty-print version
- get csv from Sean and convert to table for viewing in a Vim window with ;xcsv

## JSON library for C
I have two Cygwin package installed for JSON:
- `libjson-c`
    - <https://openwrt.org/packages/pkgdata_lede17_1/libjson-c>
    - in version 0.12-1 as of this writing
- `libjson-c2`
    - seems to be part of `libjson-c`, version number is the same

- `libjson-glib 1.0_0`
    - This is *JSON-GLib*
    - <https://developer.gnome.org/json-glib/stable/json-base.html>

> JSON-GLib is a library aimed at providing an API for efficient parsing and
> writing of JSON (JavaScript Object Notation) streams, using GLib's data
> types and API.

# These notes
The rest of this doc are notes I was taking while we generated the table
together. The table is the complete protocol, these notes are just to jog my
memory. Use the table as the bible while writing the firmware.

# dev tasks
- [ ] add arguments to jump table
    - [ ] ask Sean for an example
    - jump table is overkill if just returning function
      pointers
    - *all* commands take parameters
    - example:
        - command `SendSensorLed1Red` becomes:
        - *command*: `SendSensorLed`
        - *parameters*: `1`, `red`
- [ ] rename commands:
    - drop the `Send` prefix in `SendSensor` prefix
    - commands for the `sensor` contain the word `Sensor`
    - commands for the `bridge` contain the word `Bridge`
    - rename occurrences of `Lis` to `Array`
    - rename lib `Usb` to `Serial`
        - it is not specific to Usb
        - it just wraps my `Ft1248` lib so I can forget about how FT1248 works

# protocol changes
- reply with status but *not* with command sent!
    - replying with the command sent is a redundant checksum

Here are example API calls using the new protocol. See *UsbProtocolTable* for
all API calls.

## SetBridgeLed(1,red)
- nbytes returned: 1
- bytes returned:
    - bridge status [`0x00` *ok* | `0xff` *invalid command*]

## GetBridgeLed(1)
- nbytes returned: 2
- bytes returned:
    1. bridge status [`0x00` *ok* | `0xff` *invalid command*]
    2. bridge LED1 state: [`0x00` *off* | `0x01` *green* | `0x02` *red*]

## SetSensorLed(1,red)
- nbytes returned: 4
- bytes returned:
    1. bridge status [`0x00` *ok* | `0xff` *invalid command*]
    2. `0x00` *MSB sensor message size*
    3. `0x01` *LSB sensor message size*
    4. sensor status [`0x00` *ok* | `0xff` *invalid command*]

## GetSensorLed()
- nbytes returned: 5
- bytes returned:
    1. bridge status [`0x00` *ok* | `0xff` *invalid command*]
    2. `0x00` *MSB sensor message size*
    3. `0x02` *LSB sensor message size*
    4. sensor status [`0x00` *ok* | `0xff` *invalid command*]
    5. sensor LED1 state: [`0x00` *off* | `0x01` *green* | `0x02` *red*]

# commands
- jump table is in `usb-bridge.c`: `LookupBridgeCmd()`
    - `LookupBridgeCmd()` is defined in `lib/Usb.c` --
      I am decoupling this in another branch
    - `LookupBridgeCmd()` defines a dictionary of
      commands
    - BridgeLedRed
    - BridgeLedGreen
    - ~BridgeCfgLis~ BridgeCfgArray
    - SendSensorLed1Red
    - SendSensorLed1Green
    - SendSensorLed2Red
    - SendSensorLed2Green

# new commands!
- GetBridgeLed(1)
    - even though there is only one bridge led, there might be more in the
      future, so use same command/parameter pattern
- GetSensorLed(1|2)
    - firmware checks:
        - is LED[1|2] on or off
        - return `0x00` if LED is off
        - otherwise, check the LED color
        - return:
            - `0x01` if *green*
            - `0x02` if *red*
- ResetBridge()
    - same as pushing physical reset button
- VerifyConnection()
    - call after ResetBridge()
    - this command asks the Bridge to respond:
        - `0x00` *ok*
        - `0x01` *error*
            - *error* indicates some hardware problem at startup
            - this is some future thing we can implement, for now it will always
              return `0x00`
    - motivation for calling `VerifyConnection()`
        - hostPC issues a reset because hardware is hanging longer than hostPC
          wants to tolerate
        - hostPC needs to know when the MCUs come out of reset and are ready to
          receive new commands
        - hostPC sends command `VerifyConnection()` to verify the connection
        - this command sits in the FTDI chip's serial buffer until the MCU is
          awake, then the `usb-bridge` MCU reads it out first thing in it's
          `main` loop, and it sends back `0x00`
        - when the hostPC receives `0x00`, it knows it is OK to proceed with
          sending new commands
