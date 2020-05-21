#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''Test hardware is visible.'''

# -----------
# | Helpers |
# -----------
def _print_the_command_sent(reply):
    print(f"Sent:\n\tCommand {reply.command_id}: {reply.name}")
def _print_the_reply_received(reply):
    received = reply.variables
    print(f"Received:")
    for response in received:
        print(f"\t{response}: {getattr(reply, response)}")
def report(reply):
    _print_the_command_sent(reply)
    _print_the_reply_received(reply)

from chromaspeclib.simple import ChromaSpecSimpleInterface
kit = ChromaSpecSimpleInterface(
    serial_number='091103', # dev-kit I'm using for my system tests
    timeout=2.0 # timeout if there is no response in 100ms
    )
print(f"Using: {kit}")

# ---------------------------------------
# | Replace line 32 with command to try |
# ---------------------------------------

# Send the command and get the reply.
# max_tries = 20; start_pixel = 100; stop_pixel = 200; target = 46000; target_tolerance = 3000
# kit.setAutoExposeConfig(max_tries, start_pixel, stop_pixel, target, target_tolerance)
# reply = kit.setAutoExposeConfig(10, 7, 392, 46420, 3277)
# reply = kit.getBridgeLED(0)
# Report the command and its reply.
# report(reply)

# kit.null()

kit.setAutoExposeConfig(1, 8, 200, 46420, 3277)
reply = kit.getAutoExposeConfig()
report(reply)
reply = kit.autoExposure()
report(reply)
# reply = kit.getExposure()
# report(reply)
