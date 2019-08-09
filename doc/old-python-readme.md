% Eval Kit `Serial Interface Example With Python 3`

# Preliminaries
## Firmware is a WIP
The eval kit firmware is still a work-in-progress. Our main focus is on the
quality of the spectometer chip itself, the evaluation cradle is secondary.

Do not use the eval kit in a system where it is expected to run indefinitely.
There are known bugs that cause the firmware to hang. That is why this doc is
full of Python examples instead of simply providing a Python package. We
need stable firmware before writing an API.

Do not build your codebase around the behavior of the eval kit. The eval kit is
changing. The general functionality will be the same, but it will be more
robust. Future versions will likely *not be compatibile* with the current serial
command protocol.

That being said, we use this same eval kit hardware for spectrometer wavelength
calibration and QC. So it is at least *usable*. It does not take much Python
code to get data from the spectrometer. If you are comfortable with Python, you
can quickly incorporate these examples in a small throwaway lib that gets the
job done.

## Requirements
- Chromation eval kit:
    - spectrometer chip soldered onto breakout board
    - readout circuit with ~mini-B USB connector~
    - readout circuit with micro-B USB connector
- Windows with a Python 3 installation
    - any Python environment should work
    - these instructions assume a simple Python REPL in the Windows PowerShell

### Not Using Windows
Not using Windows? Good for you. The Python snippets *should be* platform
independent. One possible exception is *opening* USB communication because USB
device names are platform dependent.

These snippets use `pyserial` for USB communication. `pyserial` is
cross-platform, and I think I've used it in these examples in a way that is
cross-platform. I search for the USB device by its serial number and let
`pyserial` handle looking up the device name.

But I only tested these examples on Windows. If you run into trouble opening the
USB port, `pyserial`, has good documentation and is open source. If the snippets
are not working on your OS and you'd like help, please email me at
mike@chromationspec.com, describe what OS/platform you are using, and I'll do my
best to help.

### Using Python from Windows PowerShell
#### Open a Python REPL (read-eval-print loop)
- open a Windows PowerShell:
    - `WinKey+x`
    - press `i` to select `Windows PowerShell`
- start the Python REPL
```powershell
> python
```
I start PowerShell code snippets start with `>` to indicate it is a command to
type at the PowerShell prompt.

### PowerShell Copy and Paste

#### Paste into PowerShell
In case you want to test a snippet by copy-and-paste:

- copy the snippet to your clipboard
- paste into the *Python REPL* in *PowerShell* with:
    - `Alt-Space`
    - press `e` to edit
    - press `p` to paste

#### Copy from PowerShell
In case PowerShell outputs something you want to paste somewhere:

- highlight the text with your mouse
- press `Enter`

Another way is to pipe output directly to the clipboard. For example, this
copies your `PATH` environment variable to the clipboard:

```powershell
> $env:PATH | clip
```

## Install Python package `pyserial`
- from PowerShell:
```powershell
> pip install pyserial
```

## Hardware Overview - Context for Serial Communication
### Printed Circuit Boards (PCBs)
The eval kit hardware consists of three PCBs:

1. *spectrometer breakout* board
    - the spectrometer chip is in a *QFN-48* package
    - the spectrometer chip has a linear array with 392 pixels
    - the breakout board has a *ZIF* connector for an 8-pin flat-flexible cable
2. *vis-spi-out* board ~*analog-to-SPI readout* board~:
    - this has four red/green LEDs to indicate state
    - this connects to two boards:
        - the *spectrometer breakout* with a flat-flexible cable
        - the ~*SPI-to-USB bridge*~ *usb-bridge* with a ~10-pin ribbon cable~
          16-pin header
3. *usb-bridge* ~*SPI-to-USB bridge*~ board for communicating with the kit
   hardware from your computer:
    - this has:
        - one red/green LED to indicate state
        - the `FTDI FT221X` for USB communication, i.e., serial communication
        - a `reset` button:
            - press `reset` if the system is unresponsive
            - resets the microcontrollers but does not reset the `FTDI FT221X`
            - when you press `reset` you do **not** need to close and re-open
              serial communication
            - if `reset` does not help, unplug/plug the USB cable to
              power-toggle the system
            - when you power-toggle, you **do** need to close and re-open serial
              communication

### Pixel Subset
The linear array has 392 pixels, but the spectrometer only uses a subset of
these pixels. The eval kit software uses this pixel subset for plotting and
saving data.

When a frame of data is requested, the eval kit hardware returns all pixels.
It is up to you to extract the useful subset using the datapoints provided for
the pixel-to-wavelength mapping.

[ ] This is illustrated in the Python code snippets.

### Pixel-to-Wavelength Mapping
The eval kit software creates a pixel-to-wavelength mapping to plot the spectrum
with wavelengths, rather than pixels, on the x-axis.

Chromation performs a wavelength calibration and provides you with seven
pixel-to-wavelength data points. Fit a polynomial to these data points to create
the pixel-to-wavelength mapping.

[ ] This is illustrated in the Python code snippets.

### Dark-correction
The pixel readings output by the eval kit are already dark-corrected. *You do
not need to take separate dark measurements.*

#### Details
This is *analog* dark-correction. The spectrometer's linear array provides
analog voltage outputs for the pixel and for the dark-reference. The ADC in the
eval kit hardware converts the difference between the pixel voltage and the
dark-reference voltage. The final dark-signal output by the spectrometer,
therefore, is always close to zero at all pixels, except in the case of *leaky*
pixels which appear as spikes. The spike amplitude is proportional to
the integration time (bigger spikes at longer exposures).

Adjust the actual dark level with the trimpot on the *vis-spi-out* board. Some
people prefer to see *a little* of the dark background. Some people prefer to
get the dark background *just below* zero counts to get a clean zero at all
pixels without signal.

The linear array has 392 pixels. Sometimes a couple of these pixels are faulty.
Faulty pixels have dark values that *increase* with integration time. Chromation
screens out linear arrays that have faulty pixels in the useful subset, but
passes linear arrays that have faulty pixels *outside* the useful subset.

The eval kit software only displays this useful range. Since you are writing
your own software, be aware that there may be pixels outside the useful subset
which have non-zero dark readings.

# Python 3 Code Snippets
## `>>>`
- Python code snippets start with `>>>` to differentiate between code input and
  stdout/stderr output, for example:
```python
>>> import serial.tools.list_ports as usb_ports
>>> for spectrometer in (port for port in usb_ports.grep('CHROMATION')):
>>>     spectrometer.serial_number.strip('CHROMATION')
...
'064301'
'074201'
```
- if the snippet has no output, e.g., a function definition, I don't bother with
  `>>>`, for example:
```python
def com_port(serial_number):
    return next(usb_ports.grep('CHROMATION'+serial_number)).device
```

## Open communication with the spectrometer
The code snippets use Python package `pyserial` for USB communication with the
eval kit.

The following snippet defines function `open_spectrometer`.

- `open_spectometer`:
    - takes the spectromter six-digit serial number as a string
    - opens USB communication
    - returns a `serial.serialwin32.Serial` object

The returned USB object is your handle for the rest of your program:

- writing bytes
- checking how many bytes are available to read in the buffer
- reading those bytes
- eventually closing communication

```python
import serial
import serial.tools.list_ports as usb_ports

def com_port(serial_number):
    return next(usb_ports.grep('CHROMATION'+serial_number)).device

def open_spectrometer(serial_number='012301'):
    usb=serial.Serial()
    usb.baudrate = 115200
    usb.port = com_port(serial_number)
    usb.open()
    return usb

```
Helper function `com_port` returns the `'COM'` string that identifies which
virtual COM port is assigned to your spectrometer.

Here is an example using `open_spectrometer` to open a spectrometer with serial
number `0643-01` and then close communication:

```python
usb = open_spectrometer('064301')
usb.close()

```

### Handling exceptions
This snippet adds exception handling to cover the possibility that the
spectrometer is not connected.

```python
import serial
import serial.tools.list_ports as usb_ports
import contextlib
import sys

def com_port(serial_number):
    with contextlib.suppress(StopIteration):
        return next(usb_ports.grep('CHROMATION'+serial_number)).device

def open_spectrometer(serial_number='012301'):
    usb=serial.Serial()
    usb.baudrate = 115200
    usb.port = com_port(serial_number)
    if usb.port is None:
        print(
            "Cannot `open_spectrometer('{}')`. Make sure it is connected."
            .format(serial_number),
            file=sys.stderr
            )
        return
    usb.open()
    return usb

```
- `next()` raises `StopIteration` if it cannot generate any further items
    - `StopIteration` is raised if there are no spectrometers attached with that
      serial number
    - use `contextlib.suppress` to prevent the `StopIteration` exception from
      terminating your program
- with `StopIteration` suppressed, `com_port()` simply returns `None` if there
  are no spectrometers attached with that serial number
- `open_spectrometer()` handles this `usb.port==None` case accordingly
    - use `sys.stderr` for sending an error message to `stderror`
    - not implemented in this example: log the error message and safely shutdown
      the main program

Now a serial number typo results in an error message:
```python
>>> usb = open_spectrometer('06430Q')
Cannot `open_spectrometer('06430Q')`. Make sure it is connected.
```

### Handling multiple spectrometers
If you have multiple Chromation spectrometer eval kits connected, or if you just
want to double check what your serial number is, this snippet lists the serial
numbers of all connected Chromation devices:

```python
import serial.tools.list_ports as usb_ports
for spectrometer in (port for port in usb_ports.grep('CHROMATION')):
    spectrometer.serial_number.strip('CHROMATION')

...
'064301'
'075601'
```

## Manually test USB communication with red/green LED
The SPI-to-USB bridge has one red/green LED. When the kit is first connected,
this LED is green.

Send the command to `disable SPI communication`:
```python
usb.write(b'\x03')
```
Verify that the LED turns red.

Re-enable SPI communication with this command:
```python
usb.write(b'\x04')
```
Verify that the LED turns green.

## Configure the spectrometer's linear array
The Chromation spectrometer uses a configurable linear photodiode array. The
Chromation eval software loads the intended configuration after opening serial
communication.

Writing your own software in Python, you need to configure the spectrometer's
linear array yourself each time the device is powered-on or reset.

Send these four bytes, `0x07 0x06 0x0B 0x0C`, to configure the linear array:
```python
>>> usb.write(b'\x07\x06\x0B\x0C')
4
```
Put this in a function:
```python
def cfg_spectrometer(usb):
    usb.write(b'\x07\x06\x0B\x0C')
```
and simply call:
```python
cfg_spectrometer(usb)
```
The two inner LEDs on the readout circuit board turn red.

**The configuration is lost after power off or hardware reset.**

- Resend the configuration if you:
    - toggle power to the spectrometer
    - press the `reset` button on the hardware.

Closing and re-opening serial communication does not reset the microcontrollers,
so you do not *need* to resend configuration.

There is no harm in resending the configuration command, but be careful not to
lump multiple commands in a single `usb.write()`. The fragile protocol I wrote
on the eval kit microcontroller for parsing commands cannot handle multiple
commands sent in a single write. For example, if you send the four configure
bytes plus a fifth byte, e.g., the single-byte command that requests a frame,
the microcontroller does nothing. It does not configure the linear array or
acquire a frame.

## Read a frame of data
After configuration, there are 392 pixels. Each pixel reading is represented by
a 16-bit value, i.e., two bytes, therefore there are 784 bytes in one frame of
spectrometer data.

The Chromation eval kit acquires frames when requested. The command to request
and receive one frame is the single byte `0x01`:
```python
>>> usb.write(b'\x01')
1
```

The spectrometer kit responds with 784 bytes:
```python
>>> usb.inWaiting()
784
```

Read all the bytes:
```python
>>> usb.read_all()
b'U\x85\x00\x00\x00 ...
...
... \x00\x00\x00\x00\x00\x00\x00'
```

This outputs a big mess to the screen and is not *really* what you want.

Instead, read bytes two at a time. Each two-byte value is one 16-bit pixel
reading. The pixel reading is sent `byteorder='big'`, i.e., most-significant
byte first. The pixel readings are sent starting with pixel 1.

- Encapsulate all of this in a `read_frame()` function:
    - helper function `request_frame()` sends the request and waits until all
      784 bytes are received
    - helper function `pixel_reading()` describes how each pair of bytes is
      converted into a pixel reading

```python
def request_frame(usb, npixels):
    usb.write(b'\x01')
    while (usb.inWaiting()<npixels*2): pass

def pixel_reading(usb):
    return int.from_bytes(usb.read(2), byteorder='big', signed=False)

def read_frame(usb, npixels=392):
    request_frame(usb, npixels)
    return [pixel_reading(usb) for pixel in range(npixels)]
```

And acquire a frame by calling `read_frame()`:
```python
data = read_frame(usb)
```
Now three LEDs are green and only one is red.

`data` is a Python list:
```python
>>> type(data)
<class 'list'>
```
`data` has 392 elements:
```python
>>> len(data)
392
```
Display `data`:
```python
>>> data
[0, 0, 0, 0, 0, 0, 0, 0, ... 
...
... , 0, 0, 0, 0, 0, 0, 0, 0, 0]
```

I like `numpy`. Return an `ndarray` instead of a Python `list`:
```python
import numpy as np

def request_frame(usb, npixels):
    usb.write(b'\x01')
    while (usb.inWaiting()<npixels*2): pass

def pixel_reading(usb):
    return int.from_bytes(usb.read(2), byteorder='big', signed=False)

def read_frame(usb, npixels=392):
    request_frame(usb, npixels)
    return np.array([pixel_reading(usb) for pixel in range(npixels)])
```

Acquire a frame again by calling `read_frame()`:
```python
data = read_frame()
```
This time `data` is an `ndarray`:
```python
>>> type(data)
<class 'numpy.ndarray'>
```
`data` is a 1d array:
```python
>>> np.shape(data)
(392,)
```
`data` has 392 elements:
```python
>>> data.size
392
```
Each element is an integer:
```python
>>> data.dtype
dtype('int32')
```
Display `data`. This is the first time `data` is not a big giant mess, so I'll
actually display all of it below. This is *one* reason to use an `ndarray`
instead of a `list`.
```python
>>> data
array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
```

Now you can actually turn on a flashlight and try to get non-zero data:
```python
>>> read_frame(usb)
array([    0,     0,     0,     0,     0,     0,     0,     0,     0,
        ...
           0,     0,     0,     0,     0,     0,     0,     0,     0,
           0,     0,     0,     0,     0,     0,     0,     0,     0,
           0,     0,     0,     0,     0,     0,     0,     0,     0,
           0,     0,     0,     0,     0,     0,     0,     0,     0,
           0,     0,     0,     0,   474,  1062,  2398,  3960,  5804,
        7367,  7822,  8152,  9616, 10200, 11300, 10346, 10855, 10681,
        8297,  7541,  5360,  3180,  3442,  4402,  8371, 19196, 33346,
       37303, 21633,  9238,  2753,   188,     0,     0,     0,     0,
           0,     0,     0,     0,     0])
```
The `ndarray` auto-adjusts the printed column width to make all the numbers line
up.

Controlling integration time is essential for acquiring a spectrum. The eval kit
hardware has two ways to control integration time:

- auto expose
- set integration time

## Auto-expose
The command to auto-expose is `\x15`. The eval kit hardware responds with two
bytes representing its updated integration time value.

The eval kit hardware counts integration time in units of `tics`. Integration
time is quantized in `20µs tics`. The number of tics is the integration time in
seconds divided by 20µs.

For example, 23 ms is 1150 tics, or in hexadecimal, `0x047E` tics. So if
auto-expose sets integration time to 23 ms, the eval kit hardware sends the
bytes `0x04 0x7E`.

Starting with the number of tics sent by the eval kit hardware:

- integration time (ms) = number of tics divided by 50
    - example: 1150 tics * 0.02 ms/tic = 23 ms
- integration time (µs) = number of tics multiplied by 20
    - example: 1150 tics * 20 µs/tic =  23000 µs

```python
def to_ms(tics):
    return tics/50

def auto_expose(usb):
    usb.write(b'\x15')
    while(usb.inWaiting()<2): pass
    return to_ms(int.from_bytes(usb.read(2), byteorder='big', signed=False))
```

Auto-expose with no light input. Integration time increases to its maximum value
of 1000 ms:
```python
>>> auto_expose(usb)
1000.0
```

## Set integration time
The command to set integration time is `\x02` followed by two bytes indicating
the amount of time. The eval kit hardware responds with two bytes representing
its updated integration time value.

This examples sets the integration time to 23ms:
```python
>>> usb.write(b'\x02\x04\x7e')
3
```

Make this more readable with functions:
```python
def to_tics(ms):
    return ms*50

def to_ms(tics):
    return tics/50

def exposure(ms):
    return b''.join((b'\x02',to_tics(ms).to_bytes(2,byteorder='big', signed=False)))

def set_integration_time(usb,ms=1):
    usb.write(exposure(ms))
    while(usb.inWaiting()<2): pass
    return to_ms(int.from_bytes(usb.read(2), byteorder='big', signed=False))
```

Example setting integration time to 23 ms:
```python
>>> set_integration_time(usb, ms=23)
23.0
```

## Gui
Install matplotlib:

```powershell
> pip install matplotlib
```

Upgrade an existing installation:
```powershell
> python -m pip install -U matplotlib
```

Use Python to find your `matplotlibrc` file:
```python
import matplotlib
matplotlib.matplotlib_fname()
'/usr/lib/python3.6/site-packages/matplotlib/mpl-data/matplotlibrc'
```
Or use `bash`:
```bash
$ find /usr/lib/python* -name 'matplotlibrc'
/usr/lib/python3.6/site-packages/matplotlib/mpl-data/matplotlibrc
```

Set the `backend` to `TkAgg`:
```python
# search for `backend` in your `matplotlibrc` file
#
backend      : TkAgg
```

Open a running plot of the spectrometer output:
```python
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def spectra():
    npixels = 392
    pixels = np.array([p+1 for p in range(npixels)])
    values = read_frame(usb, npixels)
    x = np.array([pixel for pixel in pixels if is_useful(pixel)])
    y = np.array([value for pixel,value in zip(pixels,values) if is_useful(pixel)])
    yield x,y

def update_plot(data):
    spectrum_plot.set_data(data)

serial_number = '064301'
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
ax1.set_xlabel('Pixel Number')
ax1.set_xlim(
    max(n for n in range(npixels+1) if is_useful(n)),
    min(n for n in range(npixels+1) if is_useful(n))
    )
ax1.set_ylim(0,30000)
ax1.set_ylabel('Counts (a.u.)')
ax1.set_title('Spectrometer {} Raw Data'.format(serial_number))
ax1.grid()
spectrum_plot, = ax1.plot([],[],'bo--',linewidth=1,markersize=3)
animation.FuncAnimation(fig=fig, func=update_plot, frames=spectra, interval=50)
plt.show()

```python
import matplotlib.pyplot as plt
import matplotlib.animation as animation
fig=plt.figure()
ax1 = fig.add_subplot(1,1,1)
plt.xlabel('Pixel Number')
plt.ylabel('Counts (a.u.)')
serial_number = '064301'
plt.title('Spectrometer {} Raw Data'.format(serial_number))
npixels = 392
pixels = np.array([p+1 for p in range(npixels)])
values = read_frame(usb)
def is_useful(pixel):
    return 300<=pixel<=392

x = np.array([pixel for pixel in pixels if is_useful(pixel)])
y = np.array([value for pixel,value in zip(pixels,values) if is_useful(pixel)])
plt.plot(
    x,
    y,
    'bo--',
    linewidth=1,
    markersize=3
    )
plt.show()

```

## old stuff

### write a byte
```python
s.write('\x00')
```
#### Earhart command sequence to get a frame
```python
s.write('\x00\x04')
s.write('\x00\x80')
s.write('\x00\x05')
s.write('\x01\xF4')
s.inWaiting()
```
- expect the response `4L` meaning there are four bytes waiting in the input
  buffer

# Info for Python 2 is below. Do not use Python 2.

## ~~Open a Python 2 REPL~~

### Context
- the `s.write()` calls in the example use the `pyserial` package
- ~~the `s.write()` calls only work under Python 2.7~~
    - UPDATE: there is a simple fix to make this work in Python 3

### Instructions for Python 2 Setup

#### Install Python 2.7
- there are *many* ways to install Python 2.7
- here is the recommended option for Windows:
- <https://www.python.org/downloads/windows/>
- and the link specifically for Python 2.7.15 on 64-bit Windows:
- <https://www.python.org/ftp/python/2.7.15/python-2.7.15.amd64.msi>
- another popular choice is *Anaconda*
- the following assumes the recommended Windows installation (not *Anaconda*)

#### Need `python.exe` on your `PATH`
- the default installation places `python.exe` in `C:\Python27`
- add the path to `python.exe` to your Windows `PATH` environment variable:
  `$env:PATH`
    - most installation methods do this for you
    - but it is likely that you already have Python 3 installed because `Python
      2.7 will reach the end of its life on January 1st, 2020`
    - you do **not** want this legacy installation to interfere with your
      existing `python` and `pip` commands
    - in this case:
        - edit `PATH` manually
        - create aliases to run `python2` and `pip2` that do not interfere with
          your existing `python` and `pip` commands

#### Edit `PATH` manually and create aliases `python2` and `pip2`
- *Anaconda* is a popular choice for installing multiple versions of Python
  and controlling which is the active version
- a simple alternative to *Anaconda* is to edit your `PATH` variable
  directly in Windows PowerShell
- open a Windows PowerShell:
    - `WinKey+x`
    - press `i` to select `Windows PowerShell`
- in the snippets below, the `>` indicates the PowerShell prompt
- before editing `PATH`, print it out
- this snippet prints `PATH` to screen with one *path* per line:

```powershell
> (Get-ChildItem env:Path).value.split(";")
```

- the following snippet appends *paths* to your `PATH` and creates aliases so
  that the command `python2` runs `python-2.7.15` and `pip2` runs the `pip` for
  this `Python2` installation
- the change to `PATH` is temporary
- *temporarily* edit your `PATH` by running this snippet in PowerShell

```powershell
> $py2_win_path = "C:\Python27"
> $pip2_win_path = "C:\Python27\Scripts"
> $env:PATH = "$env:PATH;$py2_win_path;$pip2_win_path"
> new-item alias:python2 -value "C:\Python27\python.exe" | out-null
```

- test that the new paths work:
    - `python2` should print the version and open the `REPL`:

    ```powershell
    > python2

    Python 2.7.15 (v2.7.15:ca079a3ea3, Apr 30 2018, 16:30:26) [MSC v.1500 64 bit on win32
    Type "help", "copyright", "credits" or "license" for more information.
    ```
    - exit the `REPL`:
    ```powershell
    >>> exit()
    ```

    - `pip2 -V` should print the version of `pip`:

    ```powershell
    > pip2 -V

    pip 9.0.3 from c:\python27\lib\site-packages (python 2.7)
    ```

#### Update pip2
- this is a *very old* version of pip
- update it to the latest version:

```powershell
> python2 -m pip install --upgrade pip
```

- note the command is `python2 -m pip` and **not** `python2 -m pip2`
- the `python2` aliases to the Python 2 installation
- we are asking Python 2 to run *its pip module*, which it will find in its
  own local *Scripts* folder
- there is no script named `pip2`, so `python2 -m pip2` just returns `No
  module named pip2`

- check the `pip2` version again:

```powershell
> pip2 -V

pip 19.0.3 from c:\python27\lib\site-packages\pip (python 2.7)
```

#### Make `PATH` edits and aliases permanent with a PowerShell script
The `PATH` edits and the aliases are lost when you close this PowerShell
session. Make these permanent by adding them to your PowerShell profile script.

- create your profile script at this path:
`C:\Users\{user-name}\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1`

PowerShell runs this script when it starts up, but first you have to enable
PowerShell to run scripts. Windows disables running scripts by default for
security reasons.

- The recommended way to override this is to run PowerShell as an Administrator:
    - `WinKey+x`
    - press `a` to select `Windows PowerShell (Admin)`
- set the execution policy to `remotesigned`:
```powershell
Set-ExecutionPolicy RemoteSigned
```
- and exit out of the `Admin` shell

### Install package `pyserial` in Python 2 installation
- from PowerShell:
```powershell
> pip2 install pyserial
```

### Install package `pyft232` to use d2xx for CBUS access
I did this on my Python 3 installation.

`pyft232` depends on `pyusb` and `pyserial`. Upgrade those
installations with `-U`.

- from PowerShell:
```powershell
> python -m pip install -U pyusb
```
- check the versions in the Python REPL:
```python
>>> import usb # pyusb
>>> usb.version_info
(1, 0, 2)
>>> import serial # pyserial
>>> serial.__version__
'3.3'
```

Install `pyft232`:
```powershell
> pip install pyft232
Successfully installed pyft232-0.6
```

### `pyft232` only for reset via CBUS
See <https://github.com/lsgunth/pyft232/issues/8>.

There is no `bytes_available` or `in_waiting` to check the buffer. I
have to write my own using the `D2XX` API and following the examples
of other `D2XX` implementations in `pyft232`. Instead, I'll just
`usb.close()` to end the `pyserial` session, open via `ft232`, do the
CBUS reset, then close this and reopen the `pyserial` session.

So:
```python
usb.close() # end the pyserial session
import ctypes
d2xx = ctypes.windll.ftd2xx
import ft232
usbd2xx = ft232.Ft232(serial_number, baudrate=115200)
d2xx.FT_SetBitMode(usb.handle, 0x80, 0x20) # reset system
d2xx.FT_SetBitMode(usb.handle, 0x00, 0x20) # reset system
# Make sure the system is running again.
inputs = ctypes.c_char()
status = d2xx.FT_GetBitMode(usb.handle, c.byref(inputs))
if status != 0:
    print("D2XX error in attempting FT_GetBitMode. Cannot read system
    state. Please check PCB LEDs are on (green or red) before
    continuing, otherwise manually toggle power to the system.")
else:
    state = ord(inputs.value) & 0x08
    print("System is {}.".format(system[state]))
if state == 0x08:
    print("System is stuck in reset. The LEDs are off. Please
    manually toggle power to the system.")
else:
    print("Ready to restore system and continue measurements.")
```
Even the CBUS access does not quite work in `pyft232`, but there is
enough code there that it makes a great example for how to access the
`d2xx` .dll and provide the info it is looking for.

Accessing the `.dll` is just:
```python
import ctypes
d2xx = ctypes.windll.ftd2xx
```
And then `d2xx` API calls are available:
```python
d2xx.FT_SetBitMode(usb.handle, 0x80, 0x20) # reset system
```
Since this is a `.dll`, this limits the Python-controlled reset to
Windows. Not sure what the dynamic library from FTDI is called on
other operating systems, so I'm not sure how to tell Python about
`d2xx`. Maybe there's a `ctypes.somethingelse.ftd2xx`.

# Deprecated guide for selecting `Load VCP`
The `D2XX` API works even with `Load VCP` enabled. The hardware eval kits,
therefore, are sent to customers with `Load VCP` enabled. Customers no longer
need to know how to enable `Load VCP` to communicate with the eval kit as a
`COM` port.

## Configure FTDI Driver for VCP
### Context
The Chromation eval software uses the FTDI `D2XX` driver. On Windows, this
is the `ftd2xx.dll` library. The `ftd2xx` API provides functions for
identifying the Chromation eval kit by its `Product Description`.


The Python code snippets in this doc use Python package `pyserial`. On Windows,
this package only accesses serial devices listed as COM ports.

Before you can communicate with the eval kit using Python package `pyserial`,
you need to Load `VCP`.

- the eval hardware uses an `FTDI FT221X` for USB communication
- the eval hardware ships configured to communicate using the `D2XX` driver
- the Python example in this doc uses `VCP` instead of `D2XX`
- to run the Python example *instead of* the Chromation eval software, set the
  FT221X to `Load VCP`
- you can switch back to `D2XX` by unselecting `Load VCP` anytime and toggling
  power

### Instructions
#### Change the driver
- connect Chromation eval kit to your computer via USB
- run `Device Manager`:
    - `WinKey+x`
    - press `m` to select `Device Manager`
- expand `Universal Serial Bus Controllers`
- find `Usb Serial Converter`
- right-click, select `Properties`
- click on the `Advanced` tab
- check the box `Load VCP`
- this uses `VCP` instead of `D2XX`
- disconnect and reconnect the Chromation eval kit USB cable

### Find the `COM` number

```powershell
> python -m serial.tools.list_ports
COM3
COM5
```

### open the serial port
```python
>>> import serial
>>> usb=serial.Serial()
>>> usb.baudrate = 115200
>>> usb.port = 'COM12'
>>> usb.open()
```

### Better using generator `serial.tools.list_ports.grep`
Use a `generator expression` to yield all ports with 'CHROMATION' in
their `serial_number`, and iterate through those ports printing the serial
numbers.
```python
import serial.tools.list_ports as usb_ports
for spectrometer in (port for port in usb_ports.grep('CHROMATION')):
    spectrometer.serial_number.strip('CHROMATION')

...
'064301'
'074201'
```
There are two Chromation spectrometers connected. Return the `COM` port for
serial number `064301`:
```python
>>> next(usb_ports.grep('CHROMATION064301')).device
'COM8'
```


2 ports found
- expand `Ports (COM & LPT)`
- find `USB Serial Port (COM5)`
    - the `COM` will probably not `COM5`
    - to make sure this `COM` is the Chromation eval kit:
        - unplug the USB cable
        - the device list automatically refreshes
        - if this `COM` item disappears, it was the spectrometer
        - reconnect the USB cable
        - the device list automatically refreshes
        - if this `COM` item appears again, it was *definitely* the spectrometer
    - make note of the `COM` number, whatever it is
    - use your `COM` number wherever the example code below refers to `COM12`


