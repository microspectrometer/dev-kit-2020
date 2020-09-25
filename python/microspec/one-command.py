#!python.exe
# -*- coding: utf-8 -*-
"""Send one command. Print the command and its reply.

Open communication. Send one command. Print reply. Close communication.

USAGE
-----
1. Edit line 88 with command to use.
2. Run this script:
    $ python.exe ./one-command.py

View help:
    $ python.exe -m pydoc one-command

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

def report(reply):
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

if __name__ == '__main__':
    import microspec as usp
    kit = usp.Devkit()
    print(f"\nOpened kit: {kit.serial.serial_number}...")

    # Send the command and get the reply.
    reply = kit.captureFrame() # <----- TRY COMMAND HERE
    report(reply)

    print(f"\n...Closed kit: {kit.serial.serial_number}")
