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
si = ChromaSpecSimpleInterface(
    serial_number='091103', # dev-kit I'm using for my system tests
    timeout=0.1 # timeout if there is no response in 100ms
    )
print(f"Using: {si}")

# ----------------
# | GetBridgeLED |
# ----------------

# Send the command and get the reply.
reply = si.getBridgeLED(0)
# Report the command and its reply.
report(reply)

