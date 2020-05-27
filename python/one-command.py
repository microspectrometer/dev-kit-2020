#!python.exe
# -*- coding: utf-8 -*-
'''Send one command. Print the command and its reply.

Open communication. Send one command. Print reply. Close communication.

USAGE
-----
1. Edit line 145 with command to use.
2. Run this script:
    $ python.exe ./one-command.py

View help:
    $ python.exe -m pydoc one-command
    $ python.exe -m pydoc one-command.more_help

COMMANDS TO TRY (go to line 145)
--------------------------------
kit.null()
kit.getBridgeLED(led_num=0)
kit.setBridgeLED(led_num=0, led_setting=0)
kit.getSensorLED(led_num=0)
kit.setSensorLED(led_num=0, led_setting=0)
kit.getSensorConfig()
kit.setSensorConfig(binning=1, gain=1, row_bitmap=0x1F)
kit.getAutoExposeConfig()
kit.setAutoExposeConfig(max_tries=12, start_pixel=8, stop_pixel=392, target=46420, target_tolerance=3277, max_exposure=10000)
kit.getExposure()
kit.setExposure(cycles=50)
kit.captureFrame()
kit.autoExposure()
'''

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
def _print_the_command_sent(reply):
    print(f"Sent:\n\tCommand {reply.command_id}: {reply.name}")
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

    # do not show "received" if there is no reply
    if reply.variables == []:
        print("\t(There is no reply to this command.)")
        return

    # display what was received
    print(f"Received:")

    # do not display as a table if the command is captureFrame
    # (frame of pixel values do not fit in this table)
    # just print each variable and its value
    if reply.name == 'SensorCaptureFrame':
        for variable in reply.variables:
            print(f"{variable}: {getattr(reply, variable)}")

    # for all other commands, show the reply in a table
    else:
        print(f"{'Variable':>19} |{'Value':>8} | {'Number of bytes'}")
        print(f"{'--------':>19} |{'-----':>8} | {'---------------'}")
        #                    ----------response[0] is variable name
        #                    |       --response[1] is variable size in bytes
        #                    |       |
        #                    v       v
        # example response: (status, 1)
        for response in zip(reply.variables, reply.sizes):
            print(f"{response[0]:>19} |" # Variable
            f"{getattr(reply, response[0]):>8} | " # Value
            f"{response[1]}"
            ) # Number of bytes

def report(reply):
    _print_the_command_sent(reply)
    _print_the_reply_received(reply)

if __name__ == '__main__':
    from chromaspeclib.simple import ChromaSpecSimpleInterface
    kit = ChromaSpecSimpleInterface(timeout=2.0)
    print(f"\nOpened kit: {kit.serial.serial_number}...")

    # Send the command and get the reply.
    reply = kit.getSensorConfig() # <----- TRY COMMAND HERE
    report(reply)

    print(f"\n...Closed kit: {kit.serial.serial_number}")
