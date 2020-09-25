#!python.exe
# -*- coding: utf-8 -*-
"""Send one command. Print the command and its reply.

Open communication. Send one command. Print reply. Close communication.

USAGE
-----
1. Edit line 135 with command to use.
2. Run this script:
    $ python.exe ./one-command.py

View help:
    $ python.exe -m pydoc one-command
    $ python.exe -m pydoc one-command.more_help

COMMANDS TO TRY (go to line 145)
--------------------------------
kit.null()
kit.getBridgeLED()
kit.setBridgeLED(usp.GREEN) # or usp.OFF or usp.RED
kit.getSensorConfig()
kit.setSensorConfig(binning=usp.BINNING_OFF) # use 7.8um-wide pixels
kit.setSensorConfig(binning=usp.BINNING_ON)  # (default) use 15.6um-wide pixels
kit.setSensorConfig(gain=usp.GAIN1X) # (default) 1x gain
kit.setSensorConfig(gain=usp.GAIN2_5X) # 2.5x gain
kit.setSensorConfig(gain=usp.GAIN5X) # 4x gain
kit.getAutoExposeConfig()
kit.setAutoExposeConfig(start_pixel=228, stop_pixel=363)
kit.getExposure()
kit.setExposure(ms=1)
kit.captureFrame()
kit.autoExposure()
"""

def more_help():
    '''More help on this script:

The command's reply prints to stdout, neatly formatted in a
table.

Below are commands to try. Commands typically used in an
application are flagged as IMPORTANT.

COMMANDS TO TRY (go to line 145 and paste the command there)
---------------
kit.null()
kit.getSensorConfig()
kit.setSensorConfig(        # <----- IMPORTANT: SETUP
    binning=1,      # 0x01 is BINNING_ON
    gain=1,         # 0x01 is GAIN_1X
    row_bitmap=0x1F # 0001 1111 is ALL_ROWS_ACTIVE
    )
kit.getAutoExposeConfig()
kit.setAutoExposeConfig(    # <----- IMPORTANT: SETUP
    max_tries=12,           # give up after 12 tries
    start_pixel=8,          # ignore pixels below pixel 8
    stop_pixel=392,         # ignore pixels above pixel 392
    target=46420,           # target peak is 46420 counts
    target_tolerance=3277,  # success if peak is 46420 +/- 3277
    max_exposure=10000      # give up if exposure hits 200ms
    )
kit.getExposure()           # <----- IMPORTANT
kit.setExposure(cycles=50)  # <----- IMPORTANT
kit.captureFrame()          # <----- IMPORTANT
kit.autoExposure()          # <----- IMPORTANT

The Python API also gives control over the indicator LEDs.

MORE COMMANDS TO TRY -- Indicator LEDs
--------------------------------------
kit.getBridgeLED(led_num=0) # 0=LED0 (Bridge has LED0 only)
kit.setBridgeLED(led_num=0, led_setting=0) # 0=off, 1=green, 2=red
kit.getSensorLED(led_num=0) # 0=LED0, 1=LED1
kit.setSensorLED(led_num=0, led_setting=0) # 0=off, 1=green, 2=red

The primary purpose of the kit's bi-color green/red LEDs are is
for the kit firmware to indicate state:
- led_num: 0 (both Bridge and Sensor)
    - red or flashing red/green to indicate busy while executing
      commands
    - return to green when commands are done
- led_num: 1 (Sensor only)
    - red to indicate autoExposure() failed to hit its target
'''

# -----------
# | Helpers |
# -----------
def _print_the_reply_received(reply):
    '''Format the reply in a nice table.

    Variable | Value | Number of bytes
    
    Use getattr(reply, variable) to look up the value of each
    variable.

    Usage
    -----
    reply = kit.getAutoExposeConfig() # <----- TRY COMMAND HERE
    _print_the_reply_received(reply)

    '''

    # display what was received
    print(f"Received:")
    print(f"{'Field':>20} | {'Value'}")
    print(f"{'-----':>20} | {'-----'}")
    for field in reply._fields:
        value = getattr(reply,field)
        if type(value) is list:
            print(f"{field:>20} | list: "
                  f"[{value[0]}, {value[1]}, ..., "
                  f"{value[-2]}, {value[-1]}]"
                  )
        elif type(value) is dict:
            first_key = list(value.keys())[0]
            second_key = list(value.keys())[1]
            penult_key = list(value.keys())[-2]
            last_key = list(value.keys())[-1]
            print(f"{field:>20} | dict: "
                f"{{{first_key}:{value[first_key]}, "
                f"{second_key}:{value[second_key]}, ..., "
                f"{penult_key}:{value[penult_key]}, "
                f"{last_key}:{value[last_key]}}}"
                )
        else:
            print(f"{field:>20} | {value}"
        )

def report(reply):
    _print_the_reply_received(reply)

if __name__ == '__main__':
    import microspec as usp
    kit = usp.Devkit()
    print(f"\nOpened kit: {kit.serial.serial_number}...")

    # Send the command and get the reply.
    reply = kit.captureFrame() # <----- TRY COMMAND HERE
    report(reply)

    print(f"\n...Closed kit: {kit.serial.serial_number}")
