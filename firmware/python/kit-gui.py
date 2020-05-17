#!python.exe
# -*- coding: utf-8 -*-
'''GUI for the Chromation dev-kit.'''

import pygame # from PyPi
import pygs # I wrote this to simplify using pygame
from chromaspeclib.simple import ChromaSpecSimpleInterface

# ----------------------
# | Spectrometer Setup |
# ----------------------

# Open communication. Communication closes when this app quits.
kit = ChromaSpecSimpleInterface(
    serial_number='091103', # dev-kit I'm using for my system tests
    timeout=0.1 # timeout if there is no response in 100ms
    )

# led_setting
OFF = 0
GREEN = 1
RED = 2

# start with indicator LEDs green
kit.setBridgeLED(0,GREEN)
kit.setSensorLED(0,GREEN)
kit.setSensorLED(1,GREEN)

def to_cycles(ms):
    '''Convert exposure time in milliseconds to units of 20µs
    clock cycles used by the dev-kit firmware.'''
    # limit cycles: <= 65500 because max exposure is 2^16-1 cycles
    if ms > 1310: ms = 1310
    # limit cycles: >= 1 because min exposure is 1 cycle
    if ms < 0.02: ms = 0.02
    return round(ms*1e-3/20e-6)

# start with 1ms exposure time
milliseconds = 1
kit.setExposure( to_cycles(milliseconds) )
# print(f"50? {kit.getExposure().cycles}")

# LIS-770i config
BINNING_ON = 1
GAIN_1X = 0x01
ALL_ROWS_ACTIVE = 0x1F

# start with default config
kit.setSensorConfig(BINNING_ON, GAIN_1X, ALL_ROWS_ACTIVE)

# -------------
# | GUI Setup |
# -------------
rgb = pygs.RGB()
win = pygs.Window()

# Want to use counts values as pixel y-coordinate,
# but pixel 0,0 is the top-left, not the bottom-left.
# Flip the count values about the x-axis by making all values negative,
# and constrain the plot peak to the top of the screen by adding max_yval to all values.
# Add an extra offset to control where the top of the plot is.
max_data_length = 392 # pixels in LIS-770i with BINNING_ON
plot_height = 300 # later call scale_data_to_fit(counts, plot_height)
margin = 50 # space in screen pixels between plot top and window top
win.open_window( max_data_length, plot_height + margin )
print(f"Display window size: {win.width}x{win.height}")
clock = pygs.Clock(framerate=50)

# ------------
# | GUI Loop |
# ------------
quit = False
while not quit:
    clock.tick()

    '''---EVENTS---'''
    for event in pygame.event.get():
        kp = pygame.key.get_pressed()
        km = pygame.key.get_mods()
        quit = pygs.user.quit(event, kp, km)

    '''--- ACQUIRE SPECTRUM ---'''
    # capture one frame
    counts = kit.captureFrame().pixels

    '''--- CREATE PLOT DATA ---'''
    # create x-axis: 1,2,...,391,392
    pixnum = list(range(len(counts)))

    # scale counts to plot height
    counts = pygs.plot.scale_data_to_fit(counts, plot_height)

    # flip to plot upright
    max_val = max(counts)
    counts = [ max_val + margin - val for val in counts ]

    # turn x and y data arrays into x,y coordinate pairs
    zipped = list(zip(pixnum, counts))

    '''--- UPDATE SCREEN ---'''
    # Blank screen
    win.surface.fill(rgb.blackestgravel)
    # Draw plot
    pygame.draw.aalines(
        win.surface,
        rgb.tardis,
        False, # if True, connect first and last points
        zipped # XY plot data [(x0,y0), ... (xn,yn)]
        )

    # Flip to new surface drawing
    pygame.display.flip()

