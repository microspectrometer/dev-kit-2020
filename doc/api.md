API

# See Sean's UsbProtocolTable
- PDF pretty version
- [ ] get text version and generate table in Vim

The rest of this doc are notes I was taking while we
generated the table together. The table is the complete
protocol, these notes are just to jog my memory. Use
the table as the bible while writing the firmware.

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
    - commands for the `sensor` contain the word
      `Sensor`
    - commands for the `bridge` contain the word
      `Bridge`
    - rename occurrences of `Lis` to `Array`
    - rename lib `Usb` to `Serial`
        - it is not specific to Usb
        - it just wraps my `Ft1248` lib so I can forget
          about how FT1248 works

# protocol changes
- reply with status but *not* with command sent!
    - replying with the command sent is a redundant
      checksum
- examples:
    - SetBridgeBlah:
        - nbytes returned: 1
        - bytes returned:
            status
    - GetBridgeBlah:
        - nbytes returned: 2
        - bytes returned:
            status
            data
    - SetSensorBlah:
        - nbytes returned: 4
        - bytes returned:
            - bridge status
            - message size from sensor MSB
            - message size from sensor LSB
            - sensor status
        - example of SetSensor command:
            - message size: 1 byte
            - 1 byte because Set only returns a status

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
- GetBridgeLed
    - even though there is only one bridge led, there
      might be more in the future, so use same
      command/parameter pattern
- GetSensorLed
    - firmware checks:
        - is LED on or off
        - if off, return 0x00
        - if on, return:
            - 0x01 if green
            - 0x02 if red
- ResetBridge
    - same as pushing physical reset button
- VerifyConnection
    - after ResetBridge, this command asks the Bridge
      to response 0 (OK) or 1 (some hardware problem at
      startup -- some future thing we can implement),
      for now it's just returning 0
    - motivation:
        - hostPC issues a reset
        - hostPC verifies connection to wait until
          hardware is ready for communication again
        - the command sits in the FTDI buffer until the
          MCU is awake, then the MCU reads it out first
          thing (main loop)
