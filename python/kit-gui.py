#!python.exe
# -*- coding: utf-8 -*-
'''Chromation dev-kit interactive application.

Requirements
------------
pip install pyserial
pip install microspec
pip install pygame
pip install pygstuff

keyboard
--------
q   - quit
a   - auto-expose (spacebar works also)
x   - decrease exposure time
X   - increase exposure time
h/l - navigate wavelength slow
j/k - navigate wavelength fast
0   - go to shortest wavelength
$   - go to longest wavelength

xbox controller
---------------
BACK - quit
A    - auto-expose
X    - decrease exposure
Y    - increase exposure
right-stick   - navigate wavelength slow
left-stick    - navigate wavelength fast
left-trigger  - go to shortest wavelength
right-trigger - go to longest wavelength
'''
'''TODO
[x] vertical line automatically drawn to show where the peak is
[x] add space BELOW plot to print peak pixel number
[ ] get start_pixel and stop_pixel from map file
'''

import pygame # from PyPi
import pygstuff as pygs # I wrote this to simplify using pygame
from microspeclib.simple import MicroSpecSimpleInterface
from pathlib import Path

# ----------------------
# | Spectrometer Setup |
# ----------------------

# Open communication. Communication closes when this app quits.
kit = MicroSpecSimpleInterface(
    timeout=2.0 # seconds until timeout if there is no response
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
BINNING_OFF = 0
GAIN_1X = 0x01
ALL_ROWS_ACTIVE = 0x1F

binning = BINNING_ON
# start with default config
kit.setSensorConfig(binning, GAIN_1X, ALL_ROWS_ACTIVE)
max_tries=12;
# TODO: get start_pixel and stop_pixel from map file
# dead pixel at 236 (BINNING_ON)
start_pixel=8 if binning else 16
stop_pixel=235 if binning else 470
target=46420; tol=3277; max_exposure=10000
# kludge (doesn't work) to workaround 82ms bug: call set twice, followed by get
kit.setAutoExposeConfig(max_tries, start_pixel, stop_pixel, target, tol, max_exposure)
kit.getAutoExposeConfig()
kit.setAutoExposeConfig(max_tries, start_pixel, stop_pixel, target, tol, max_exposure)
kit.setAutoExposeConfig(max_tries, start_pixel, stop_pixel, target, tol, max_exposure)
kit.getAutoExposeConfig()

# initial value for data to plot
counts = [0 for pixels in range(start_pixel,stop_pixel+1)]

# start with 1ms exposure time
milliseconds = 1
kit.setExposure( to_cycles(milliseconds) )
# print(f"50? {kit.getExposure().cycles}")

# -------------
# | GUI Setup |
# -------------

# -----------------------------------
# | Get the path to the window icon |
# -----------------------------------

# make resource path agnostic to path kit-gui is launched from
path = Path(__file__)
here = path.parent
chromation_logo = str(Path(here).joinpath('icon.png'))

win = pygs.Window(
    caption=f'Chromation Kit: {kit.serial.serial_number.strip("CHROMATION")}',
    icon=chromation_logo
    )

# import Steve Losh's Badwolf color names
# example: rgb.saltwatertaffy
rgb = pygs.RGB()

# -------------------
# | Initialize Text |
# -------------------

# ---------------------------------
# | Fonts for labels              |
# ---------------------------------
consola = str(Path(here).joinpath('consola.ttf'))

class Text(object):
    '''Text on screen.'''
    def __init__(self, text='', size_pt=16, antialias=1, color_rgb=rgb.snow, background_rgb=None):
        '''Initialize self.surface with consolas text and color.'''

        # make resource path agnostic to path kit-gui is launched from
        path = Path(__file__)
        here = path.parent
        consola = str(Path(here).joinpath('consola.ttf'))

        # create the font
        self.font = pygame.font.Font(consola, size_pt)

        # initialize text surface values
        self.text=text # display this text
        self.antialias=antialias # 0: off, 1: on
        self.color_rgb=color_rgb # rgb text foreground color
        self.background_rgb=background_rgb # rgb text background color, None: transparent

        # create the text surface
        self.update(text=self.text, color_rgb=self.color_rgb)

    def update(self, text=None, color_rgb=None):
        '''Update text and/or color on self.surface.'''
        # update text if given:
        if text is not None: self.text=text
        # update color if given:
        if color_rgb is not None: self.color_rgb=color_rgb
        # update the surface
        self.surface = self.font.render(
            self.text,
            self.antialias,
            self.color_rgb,
            self.background_rgb,
            )

class AutoExpose(object):
    '''Autoexpose information displayed on screen.'''
    def __init__(self):

        # initialize autoexpose results
        self.is_success =  True # kit.autoExposure().success
        self.num_tries = 1 # kit.autoExposure().iterations

        # initialize displayed text
        self.title = Text(
            text='Auto-expose:',
            color_rgb=rgb.gravel
            )
        self.hitmiss = Text(
            text=f'{"HIT TARGET" if self.is_success else "GAVE UP"}',
            color_rgb=rgb.darkgravel
            )
        self.iterations = Text(
            text=f'iterations: {self.num_tries}',
            color_rgb=rgb.darkgravel
            )

# initialize autoexpose result display
autoexpose = AutoExpose()


class Exposure(object):
    '''Exposure information displayed on screen.'''
    def __init__(self):

        # initialize value displayed on screen
        self.cycles = kit.getExposure().cycles

        # initialize displayed text
        self.title = Text(
            text=f'exposure:',
            color_rgb=rgb.gravel
            )
        self.ms_text = Text(
            text=f'{to_ms(self.cycles):.2f}ms',
            color_rgb=rgb.darkgravel
            )
        self.cycles_text = Text(
            text=f'{self.cycles} cycles',
            color_rgb=rgb.darkgravel
                )

# initialize exposure-time display
exposure = Exposure()

class PeakCounts(object):
    '''Peak counts information displayed on screen.'''
    def __init__(self):
        # initialize peak counts value
        self.value = 0

        # initialize displayed text
        self.text = Text(
            text=f'peak: {self.value}',
            size_pt=20,
            color_rgb=rgb.saltwatertaffy
            )

peak_counts = PeakCounts()

# Want to use counts values as pixel y-coordinate,
# but pixel 0,0 is the top-left, not the bottom-left.
# Flip the count values about the x-axis by making all values negative,
# and constrain the plot peak to the top of the screen by adding max_yval to all values.
# Add an extra offset to control where the top of the plot is.
max_data_length = 392 if binning else 784
plot_height = 300 # later call scale_data_to_fit(counts, plot_height)
margin = 20 # space in screen pixels between plot top and window top
xax_space = 40 # space below x-axis
yax_space = 40 # space left of y-axis
win.open_window( yax_space + max_data_length + 100, xax_space + plot_height + margin )
print(f"Display window size: {win.width}x{win.height}")
clock = pygs.Clock(framerate=50)

class VerticalLine(object):
    def __init__(self, position=0, color=rgb.tardis):
        self.position = position
        self.color = color
        self.ybot = plot_height+margin+xax_space
        self.ytop = margin

class PeakPixel(object):
    '''Indicate the peak pixel on screen.'''
    def __init__(self):
        self.value = 0
        self.text = Text(text=f'self.value', size_pt=14, color_rgb=rgb.tardis)
        self.line=VerticalLine()

# initialize vertical line and label of peak feature
peak_pixel = PeakPixel()

class Cursor(object):
    '''Vertical line to inspect pixel number.'''
    def __init__(self, position=yax_space+round(max_data_length/2), color=rgb.tardis):
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
        self.ybot = plot_height+margin+round(xax_space/2)
        self.ytop = margin
        # now pixel_number is set in the loop, line 510
        self.pixel_number = start_pixel # used to be this: yax_space+max_data_length-position
        self.text = Text(
            text=f'{self.pixel_number}',
            size_pt=14,
            color_rgb=self.color
            )
    def get_motions_pressed(self, event, key_pressed, key_mods):
        # empty the motion list
        self.motions = []

        # add motions to the list
        if pygs.user.pressed_right(event, key_pressed, key_mods): self.motions.append('right')
        if pygs.user.pressed_up   (event, key_pressed, key_mods): self.motions.append('up')
        if pygs.user.pressed_left (event, key_pressed, key_mods): self.motions.append('left')
        if pygs.user.pressed_down (event, key_pressed, key_mods): self.motions.append('down')
        if pygs.user.pressed_home (event, key_pressed, key_mods): self.motions.append('home')
        if pygs.user.pressed_end (event, key_pressed, key_mods): self.motions.append('end')
        '''joystick control: test with "Controller (XBOX 360 For Windows)"'''
        if event.type == pygame.JOYAXISMOTION:
            # right-hand stick for fine-grain left/right
            if round(joy.get_axis(4),1) == -1.0:
                self.motions.append('left')
            elif round(joy.get_axis(4),1) == 1.0:
                self.motions.append('right')
            # left-hand stick for coarse-grain left/right
            if round(joy.get_axis(0),1) == -1.0:
                self.motions.append('down')
            elif round(joy.get_axis(0),1) == 1.0:
                self.motions.append('up')
            # left-trigger to go home (left-most end of useful range)
            if round(joy.get_axis(2),1) == 1.0:
                self.motions.append('home')
            # right-trigger to go to end (right-most end of useful range)
            if round(joy.get_axis(2),1) == -1.0:
                self.motions.append('end')

    def move(self):
        big = 10
        for motion in self.motions:
            if motion == 'right':
                # self.pixel_number -= 1
                self.position += 1
            if motion == 'left':
                # self.pixel_number += 1
                self.position -= 1
            if motion == 'up':
                # self.pixel_number -= big
                self.position += big
            if motion == 'down':
                # self.pixel_number += big
                self.position -= big
            if motion == 'home':
                # self.pixel_number = stop_pixel
                self.position = yax_space+max_data_length-stop_pixel
            if motion == 'end':
                # self.pixel_number = start_pixel
                self.position = yax_space+max_data_length-start_pixel

        # ------------------
        # | VERY IMPORTANT |
        # ------------------
        # empty the motion list
        self.motions = []

# control data cursor with h,j,k,l or with a joystick
cursor = Cursor(position=yax_space+max_data_length-stop_pixel)

# add the last connected joystick
if pygame.joystick.get_count() > 0:
    joy = pygame.joystick.Joystick(pygame.joystick.get_count()-1)
    joy.init()

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
        cursor.get_motions_pressed(event, kp, km)
        quit = pygs.user.quit(event, kp, km)

        if event.type == pygame.JOYBUTTONDOWN:
            if joy.get_button(6) == 1:
                quit = True

        if ( pygs.user.pressed_X(event, kp, km)
             or
             event.type == pygame.JOYBUTTONDOWN and joy.get_button(3) == 1
           ): # increase exposure

            # read exposure to INCREASE, convert to milliseconds
            ms = to_ms(kit.getExposure().cycles)

            # round milliseconds to nearest single significant digit
            # use second-most significant when most sig digit == 0
            if str(ms)[0] == '0':
                ms = float(str(ms)[0:3])
            else:
                nsigdig = 1 # e.g., if exposure_ms = 123.45, want 100
                ms = int(round( ms, nsigdig-len(str(ms).split('.')[0])))

            # INCREMENT significant digit
            # use second-most significant when most sig digit == 0
            if (str(ms)[0] == '0'):
                ms = float(str(float(str(ms)[0:3])+0.1).format(".3f"))
            else:
                # increment first leading digit ('1:9')
                ms = int(str(int(str(ms)[0])+1)+str(ms)[1:])

            # set new exposure
            kit.setExposure( to_cycles(ms) )

            # get new exposure for reporting in GUI
            exposure.cycles = kit.getExposure().cycles

            # update GUI label "exposure"
            exposure.ms_text.update(text=f'{to_ms(exposure.cycles):.2f}ms', color_rgb=rgb.saltwatertaffy)
            exposure.cycles_text.update(text=f'{exposure.cycles} cycles', color_rgb=rgb.dirtyblonde)
            # grey out GUI labels "success" and "iterations"
            autoexpose.hitmiss.update(text=f'{"HIT TARGET" if autoexpose.is_success else "GAVE UP"}', color_rgb=rgb.darkgravel)
            autoexpose.iterations.update(text=f'iterations: {autoexpose.num_tries}', color_rgb=rgb.darkgravel)

        if ( pygs.user.pressed_x(event, kp, km)
             or
             event.type == pygame.JOYBUTTONDOWN and joy.get_button(2) == 1
           ):  # decrease exposure

            # read exposure to DECREASE, convert to milliseconds
            ms = to_ms(kit.getExposure().cycles)

            # round milliseconds to nearest single significant digit
            # use second-most significant when most sig digit == 0
            if str(ms)[0] == '0':
                ms = float(str(ms)[0:3])
            else:
                nsigdig = 1 # e.g., if exposure_ms = 123.45, want 100
                ms = int(round( ms, nsigdig-len(str(ms).split('.')[0])))

            # DECREMENT significant digit
            # use second-most significant when most sig digit == 0
            # also use second-most significant digit when round(ms) == 1ms
            if (str(ms)[0] == '0') or (ms == 1):
                ms = float(str(float(str(ms)[0:3])-0.1).format(".3f"))
            # carry when most significant digit == 1
            elif str(ms)[0] == '1':
                # decrement using first two leading digits ('10')
                ms = int(str(int(str(ms)[0:2])-1)+str(ms)[2:])
            else:
                # decrement first leading digit ('2:9')
                ms = int(str(int(str(ms)[0:1])-1)+str(ms)[1:])

            # set new exposure
            kit.setExposure( to_cycles(ms) )

            # get new exposure for reporting in GUI
            exposure.cycles = kit.getExposure().cycles

            # update GUI label "exposure"
            exposure.ms_text.update(text=f'{to_ms(exposure.cycles):.2f}ms', color_rgb=rgb.saltwatertaffy)
            exposure.cycles_text.update(text=f'{exposure.cycles} cycles', color_rgb=rgb.dirtyblonde)
            # grey out GUI labels "success" and "iterations"
            autoexpose.hitmiss.update(text=f'{"HIT TARGET" if autoexpose.is_success else "GAVE UP"}', color_rgb=rgb.darkgravel)
            autoexpose.iterations.update(text=f'iterations: {autoexpose.num_tries}', color_rgb=rgb.darkgravel)

        if ( pygs.user.pressed_spacebar(event, kp)
             or
             pygs.user.pressed_a(event, kp, km)
             or
             event.type == pygame.JOYBUTTONDOWN and joy.get_button(0) == 1
           ): # autoexpose

            # auto-expose
            reply = kit.autoExposure()

            # get algorithm results for reporting in GUI
            autoexpose.is_success = True if reply.success else False
            autoexpose.num_tries = reply.iterations

            # update GUI labels "success" and "iterations"
            autoexpose.hitmiss.update(text=f'{"HIT TARGET" if autoexpose.is_success else "GAVE UP"}', color_rgb=rgb.dirtyblonde)
            autoexpose.iterations.update(text=f'iterations: {autoexpose.num_tries}', color_rgb=rgb.dirtyblonde)

            # get new exposure for reporting in GUI
            exposure.cycles = kit.getExposure().cycles

            # update GUI label "exposure"
            exposure.ms_text.update(text=f'{to_ms(exposure.cycles):.2f}ms', color_rgb=rgb.saltwatertaffy)
            exposure.cycles_text.update(text=f'{exposure.cycles} cycles', color_rgb=rgb.dirtyblonde)

    '''---UPDATE PIXEL NUMBER LABEL---'''
    cursor.move()
    cursor.text.update(text=f'{cursor.pixel_number}')

    '''--- ACQUIRE SPECTRUM ---'''
    # capture one frame
    frame = kit.captureFrame()

    # Exception: 'NoneType' object has no attribute 'pixels'
    # This is rare but it causes the application to quit.
    # Instead of quitting, just ignore the dropped frame and
    # replot the previous value of `counts`.
    if frame is not None: counts = frame.pixels

    '''--- CREATE PLOT DATA ---'''
    # put short wavelengths on left side of plot
    # counts.reverse()
    # create x-axis: 1,2,...,391,392
    pixnum = list(range(1,len(counts)+1))
    # convert to screen pixels 0:391 + yax_space
    screen_pixels = [(p-1)+yax_space for p in pixnum]
    # put short wavelengths on left side of plot
    screen_pixels.reverse()

    # display peak
    peak_counts.value = max(counts[start_pixel-1:stop_pixel])
    peak_counts.text.update(text=f'peak: {peak_counts.value}')
    # TODO: correct this for offset-by-one and for mirrored x-axis
    # TODO: turn this into a single update call
    peak_pixel.line.position = screen_pixels[counts.index(peak_counts.value)] # screen pixel number
    peak_pixel.value = pixnum[counts.index(peak_counts.value)] # actual pixel number
    peak_pixel.text.update(text=f'{peak_pixel.value}')

    # update cursor pixel text using screen_pixels
    # todo: add a hard limit to the cursor, otherwise this will take us outside
    # the range of screen_pixels
    cursor.pixel_number = pixnum[screen_pixels.index(cursor.position)]

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
        (yax_space,margin), (yax_space+len(counts),margin) # start, end
        )
    # bottom
    pygame.draw.aaline(
        win.surface,
        rgb.darkgravel,
        (yax_space,plot_height+margin), (yax_space+len(counts),plot_height+margin) # start, end
        )

    # AutoExpose target level
    # example: target = 46420
    ae_y = round(plot_height + margin - plot_height/yrange * target)
    pygame.draw.aaline(
        win.surface,
        rgb.dress,
        (yax_space,ae_y), (yax_space+len(counts),ae_y) # start, end
        )
    # example: target = 46420 + 3277
    ae_y = round(plot_height + margin - plot_height/yrange * (target+tol))
    pygame.draw.aaline(
        win.surface,
        rgb.deepgravel,
        (yax_space,ae_y), (yax_space+len(counts),ae_y) # start, end
        )
    # example: target = 46420 - 3277
    ae_y = round(plot_height + margin - plot_height/yrange * (target-tol))
    pygame.draw.aaline(
        win.surface,
        rgb.deepgravel,
        (yax_space,ae_y), (yax_space+len(counts),ae_y) # start, end
        )
    max_dark = 4500
    ae_y = round(plot_height + margin - plot_height/yrange * max_dark)
    pygame.draw.aaline(
        win.surface,
        rgb.deepgravel,
        (yax_space,ae_y), (yax_space+len(counts),ae_y) # start, end
        )

    # Draw plot: meaningful data
    meaningful_data = zipped[start_pixel-1:stop_pixel]
    ignored_lower_data = zipped[0:start_pixel-1]
    ignored_upper_data = zipped[stop_pixel:]
    pygame.draw.aalines(
        win.surface,
        rgb.mediumgravel,
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
        rgb.saltwatertaffy,
        False, # if True, connect first and last points
        meaningful_data # XY plot data [(x0,y0), ... (xn,yn)]
        )

    # Draw pixel label
    win.surface.blit(cursor.text.surface, (cursor.position+2, win.height-xax_space))
    win.surface.blit(exposure.title.surface,    (yax_space+max_data_length-140, margin+110))
    win.surface.blit(exposure.ms_text.surface,     (yax_space+max_data_length-120, margin+130))
    win.surface.blit(exposure.cycles_text.surface, (yax_space+max_data_length-120, margin+150))
    win.surface.blit(autoexpose.title.surface,              (yax_space+10, margin+110))
    win.surface.blit(autoexpose.hitmiss.surface,         (yax_space+30, margin+130))
    win.surface.blit(autoexpose.iterations.surface,      (yax_space+30, margin+150))
    win.surface.blit(peak_counts.text.surface,            (yax_space+10, margin+190))
    win.surface.blit(peak_pixel.text.surface, (peak_pixel.line.position+2, win.height-round(xax_space/2)))
    # Draw vertical line through peak feature
    pygame.draw.aaline(
        win.surface,
        peak_pixel.line.color,
        (peak_pixel.line.position, peak_pixel.line.ybot), # start
        (peak_pixel.line.position, peak_pixel.line.ytop) # end
        )

    # Draw pixel label line
    pygame.draw.aaline(
        win.surface,
        cursor.color,
        (cursor.position, cursor.ybot), # start
        (cursor.position,cursor.ytop) # end
        )

    # Flip to new surface drawing
    pygame.display.flip()

