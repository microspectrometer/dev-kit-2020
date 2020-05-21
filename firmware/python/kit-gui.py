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
    timeout=5.0 # seconds until timeout if there is no response
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
def to_ms(cycles):
    '''Convert exposure time to milliseconds from the 20µs clock
    cycles used by the dev-kit firmware.'''
    return cycles*20e-3

# LIS-770i config constants
BINNING_ON = 1
GAIN_1X = 0x01
ALL_ROWS_ACTIVE = 0x1F

# start with default config
kit.setSensorConfig(BINNING_ON, GAIN_1X, ALL_ROWS_ACTIVE)
# dead pixel at 236
max_tries=12; start_pixel=8; stop_pixel=235; target=46420; tol=3277
kit.setAutoExposeConfig(max_tries, start_pixel, stop_pixel, target, tol)

# start with 1ms exposure time
milliseconds = 1
kit.setExposure( to_cycles(milliseconds) )
# print(f"50? {kit.getExposure().cycles}")

# -------------
# | GUI Setup |
# -------------
rgb = pygs.RGB()
win = pygs.Window(
    caption='Chromation Kit',
    icon='icon.png' # TODO: find this even if pwd does not match
    )

# -------------------
# | Initialize Text |
# -------------------

# ---------------------------------
# | Fonts for labels              |
# ---------------------------------
h1_font = pygame.font.Font('consola.ttf', 40)
h2_font = pygame.font.Font('consola.ttf', 20)
h3_font = pygame.font.Font('consola.ttf', 16)

# initialize AutoExpose result display
success = True # kit.autoExposure().success
iterations = 1 # kit.autoExposure().iterations
ae_label = h3_font.render(
    'Auto-expose:', # text
    1, # antialias: 0: off, 1: on
    rgb.gravel, # text fg color
    None # text bg color, use None for transparent
    )
success_label = h3_font.render(
    f'{"HIT TARGET" if success else "GAVE UP"}', # text
    1, # antialias: 0: off, 1: on
    rgb.darkgravel, # text fg color
    None # text bg color, use None for transparent
    )
iterations_label = h3_font.render(
    f'iterations: {iterations}', # text
    1, # antialias: 0: off, 1: on
    rgb.darkgravel, # text fg color
    None # text bg color, use None for transparent
    )

exposure = kit.getExposure().cycles

exposure_tag_label = h3_font.render(
    f'exposure:', # text
    1, # antialias: 0: off, 1: on
    rgb.gravel, # text fg color
    None # text bg color, use None for transparent
    )
exposure_ms_label = h3_font.render(
    f'{to_ms(exposure)}ms', # text
    1, # antialias: 0: off, 1: on
    rgb.darkgravel, # text fg color
    None # text bg color, use None for transparent
    )
exposure_cycles_label = h3_font.render(
    f'{exposure} cycles', # text
    1, # antialias: 0: off, 1: on
    rgb.darkgravel, # text fg color
    None # text bg color, use None for transparent
    )

# Want to use counts values as pixel y-coordinate,
# but pixel 0,0 is the top-left, not the bottom-left.
# Flip the count values about the x-axis by making all values negative,
# and constrain the plot peak to the top of the screen by adding max_yval to all values.
# Add an extra offset to control where the top of the plot is.
max_data_length = 392 # pixels in LIS-770i with BINNING_ON
plot_height = 300 # later call scale_data_to_fit(counts, plot_height)
margin = 20 # space in screen pixels between plot top and window top
win.open_window( max_data_length, plot_height + margin )
print(f"Display window size: {win.width}x{win.height}")
clock = pygs.Clock(framerate=50)

class Cursor(object):
    '''Vertical line to inspect pixel number.'''
    def __init__(self, position=round(max_data_length/2), color=rgb.tardis):
        '''
        Parameters
        ----------
        position:
            - initial x-location of vertical line
        color:
            - color of the line
        '''
        self.position = position
        self.color = color
        self.ybot = plot_height+margin
        self.ytop = margin
        self.pixel_number = max_data_length-position
    def get_motions_pressed(self, event, key_pressed):
        self.motions = []
        if pygs.user.pressed_right(event, key_pressed): self.motions.append('right')
        if pygs.user.pressed_up   (event, key_pressed): self.motions.append('up')
        if pygs.user.pressed_left (event, key_pressed): self.motions.append('left')
        if pygs.user.pressed_down (event, key_pressed): self.motions.append('down')
    def move(self):
        big = 20
        for motion in self.motions:
            if motion == 'right':
                cursor.pixel_number -= 1
                cursor.position += 1
            if motion == 'left':
                cursor.pixel_number += 1
                cursor.position -= 1
            if motion == 'up':
                cursor.pixel_number -= big
                cursor.position += big
            if motion == 'down':
                cursor.pixel_number += big
                cursor.position -= big

cursor = Cursor(position=max_data_length-stop_pixel)

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
        cursor.get_motions_pressed(event, kp)
        quit = pygs.user.quit(event, kp, km)
        if pygs.user.pressed_spacebar(event, kp):
            # auto-expose
            reply = kit.autoExposure()
            success = True if reply.success else False
            iterations = reply.iterations
            # update labels "success" and "iterations"
            success_label = h3_font.render(
                f'{"HIT TARGET" if success else "GAVE UP"}', # text
                1, # antialias: 0: off, 1: on
                rgb.dirtyblonde, # text fg color
                None # text bg color, use None for transparent
                )
            iterations_label = h3_font.render(
                f'iterations: {iterations}', # text
                1, # antialias: 0: off, 1: on
                rgb.dirtyblonde, # text fg color
                None # text bg color, use None for transparent
                )
            # get new exposure
            exposure = kit.getExposure().cycles
            # update label "exposure"
            exposure_ms_label = h3_font.render(
                f'{to_ms(exposure)}ms', # text
                1, # antialias: 0: off, 1: on
                rgb.saltwatertaffy, # text fg color
                None # text bg color, use None for transparent
                )
            exposure_cycles_label = h3_font.render(
                f'{exposure} cycles', # text
                1, # antialias: 0: off, 1: on
                rgb.dirtyblonde, # text fg color
                None # text bg color, use None for transparent
                )


    '''---UPDATE PIXEL NUMBER LABEL---'''
    cursor.move()
    pixel_label = h1_font.render(
        f'pixel: {cursor.pixel_number}', # text
        1, # antialias: 0: off, 1: on
        rgb.tardis, # text fg color
        rgb.blackestgravel # text bg color, use None for transparent
        # None # text bg color, use None for transparent
        )
    '''--- ACQUIRE SPECTRUM ---'''
    # capture one frame
    counts = kit.captureFrame().pixels
    # display peak
    peak = max(counts[start_pixel-1:stop_pixel])
    peak_label = h2_font.render(
        f'peak: {peak}', # text
        1, # antialias: 0: off, 1: on
        rgb.taffy, # text fg color
        None # text bg color, use None for transparent
        )


    '''--- CREATE PLOT DATA ---'''
    # put short wavelengths on left side of plot
    # counts.reverse()
    # create x-axis: 1,2,...,391,392
    pixnum = list(range(1,len(counts)+1))
    # convert to screen pixels 0:391
    screen_pixels = [p-1 for p in pixnum]
    # put short wavelengths on left side of plot
    screen_pixels.reverse()

    # scale counts to plot height
    yrange = 65535
    counts = pygs.plot.scale_data_to_fixed_yrange(counts, plot_height, yrange)

    # flip to plot upright
    counts = [ plot_height + margin - val for val in counts ]

    # turn x and y data arrays into x,y coordinate pairs
    zipped = list(zip(screen_pixels, counts))

    '''--- UPDATE SCREEN ---'''
    # Blank screen
    win.surface.fill(rgb.blackestgravel)

    # Full scale level
    # top
    pygame.draw.aaline(
        win.surface,
        rgb.darkgravel,
        (0,margin), (len(counts),margin) # start, end
        )
    # bottom
    pygame.draw.aaline(
        win.surface,
        rgb.darkgravel,
        (0,0), (len(counts),0) # start, end
        )

    # AutoExpose target level
    target = 46420
    ae_y = round(plot_height + margin - plot_height/yrange * target)
    pygame.draw.aaline(
        win.surface,
        rgb.gravel,
        (0,ae_y), (len(counts),ae_y) # start, end
        )
    target = 46420 + 3277
    ae_y = round(plot_height + margin - plot_height/yrange * target)
    pygame.draw.aaline(
        win.surface,
        rgb.gravel,
        (0,ae_y), (len(counts),ae_y) # start, end
        )
    target = 46420 - 3277
    ae_y = round(plot_height + margin - plot_height/yrange * target)
    pygame.draw.aaline(
        win.surface,
        rgb.gravel,
        (0,ae_y), (len(counts),ae_y) # start, end
        )
    max_dark = 4500
    ae_y = round(plot_height + margin - plot_height/yrange * max_dark)
    pygame.draw.aaline(
        win.surface,
        rgb.gravel,
        (0,ae_y), (len(counts),ae_y) # start, end
        )

    # Draw plot: meaningful data
    meaningful_data = zipped[start_pixel-1:stop_pixel]
    ignored_lower_data = zipped[0:start_pixel-1]
    ignored_upper_data = zipped[stop_pixel:]
    pygame.draw.aalines(
        win.surface,
        rgb.gravel,
        False, # if True, connect first and last points
        ignored_lower_data # XY plot data [(x0,y0), ... (xn,yn)]
        )
    pygame.draw.aalines(
        win.surface,
        rgb.gravel,
        False, # if True, connect first and last points
        ignored_upper_data # XY plot data [(x0,y0), ... (xn,yn)]
        )
    pygame.draw.aalines(
        win.surface,
        rgb.taffy,
        False, # if True, connect first and last points
        meaningful_data # XY plot data [(x0,y0), ... (xn,yn)]
        )

    # Draw pixel label
    win.surface.blit(pixel_label,           (10, margin+20))
    win.surface.blit(exposure_tag_label,    (max_data_length-140, margin+110))
    win.surface.blit(exposure_ms_label,     (max_data_length-55, margin+110))
    win.surface.blit(exposure_cycles_label, (max_data_length-140, margin+130))
    win.surface.blit(ae_label,              (10, margin+110))
    win.surface.blit(success_label,         (30, margin+130))
    win.surface.blit(iterations_label,      (30, margin+150))
    win.surface.blit(peak_label,            (10, margin+190))
    # Draw pixel label line
    pygame.draw.aaline(
        win.surface,
        cursor.color,
        (cursor.position, cursor.ybot), # start
        (cursor.position,cursor.ytop) # end
        )

    # Flip to new surface drawing
    pygame.display.flip()

