% Python Interface
% Mike Gazes
% August 2, 2019

# Tools for Chromation development
These are the tools for someone at Chromation to work on the firmware and Python
libraries.

## Ctags and Cscope
Create your own `tags` and `cscope.out` database files using `ctags` and
`cscope`. These files are excluded from the repository because they are just
noise for the version control and they are easy to generate. If someone cannot
generate them, then they do not need them in the first place because it means
their IDE is not set up to take advantage of them.

Ctags and Cscope are separate utilities that address the same problem. I like
them both. Ctags is the simpler tool I reach for first.

### Python ctags
Create your own `tags` file for the `dev-kit` project. The Python code is in
`dev-kit/python`, so put the `tags` file there.

```bash
$ cd python/
$ ctags -R --languages=python --exclude="tests"
```

This creates a `tags` file in the `dev-kit/python` folder. `-R` recursively
dives into sub-folders in `dev-kit/python` and generates tags for all `.py`
files. `--exclude` ignores Python scripts in the `dev-kit/python/tests` folder.

Update the `tags` file with the same one-liner. Simply create a new `tags` file
to overwrite the original.

### Python cscope
Create your own `cscope.out` file for the `dev-kit` project.

Put it in the same place as the `tags` file. The `tags` and `cscope.out` files
do not depend on each other, but they serve the same purpose.

```bash
$ cd python/
```

#### Make `cscope.files`

Create `cscope.files`. This file lists all local `python` files in the project.
Creating a list of files is not required on a C project. It is usually used as a
way to limit which files are in the database. This is my way to get Cscope to
generate the database for Python files.
```bash
$ find . -name '*.py' > cscope.files
```
My Vim shortcut `;cfpy` makes `cscope.files`.

#### Setup up the cscope database and connect
Kill any existing cscope database connections.
```bash
$ cscope kill -1
```

Create the `cscope.out` database.
```bash
$ cscope -R -b
```
`-b` creates a new `cscope.out`
`-R` recursively look inside sub-folders while making database

```bash
$ cscope add cscope.out
```

My Vim shortcut ;cs does all of the above.

#### Update the cscope database and reconnect
Update the `cscope.out` file by simply creating a new `cscope.out` file to
overwrite the original. Then `reset` to *reconnect* the project with the new
`cscope.out`:

```bash
$ cscope -R -b
$ cscope reset
```

## Download firmware to flash memory

### Programmer and utilities
Install AtmelStudio7. This provides a utility called `atprogram.exe` and puts
this utility on the Windows PATH. The utility is installed here:

`C:\Program Files (x86)\Atmel\Studio\7.0\atbackend\atprogram.exe`

Invoke this utility to load flash *without* using the AtmelStudio GUI.

A physical programmer is required. Connect it to the shrouded 6-pin ISP
(in-system programming) header. Any Atmel programmer is OK. I have an
`atmelice`. The `avrismpk2` works just as well and is much cheaper.

Check the programmer is connected.
```bash
atprogram.exe --tool atmelice --interface isp --device atmega328p info
atprogram.exe --tool avrispmk2 --interface isp --device atmega328p info
```

Download the flash.
```bash
atprogram.exe --tool atmelice --interface isp \
--device atmega328p program --chiperase --verify --file build/usb-bridge.elf
```

Installing AtmelStudio also provides the utility `avr-size.exe`.

`C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe`

Invoke this utility to see how much memory is used by the flash file.
```bash
avr-size build/usb-bridge.elf
```

I use the AtmelStudio GUI interface to write the MCU fuses, though my intent is
to move this step into the Makefile, again using `atprogram.exe`.

### Makefiles
Each sub-project within `dev-kit` has its own Makefile. `cd` into the
sub-project and run the `make` command. For example, to download the flash to
the two MCUs:

```bash
$ cd firmware/usb-bridge/
$ make download_flash
```

My Vim shortcut is ;fa.

### Flash programming documentation
Programming the MCU on both the `usb-bridge` PCB and the `vis-spi-out` PCB is
done by connecting the Atmel programmer to the `usb-bridge`. There is a switch
on the `usb-bridge` PCB to put it into `ISP` mode, and another switch to select
*which* PCB (`usb-bridge` or `vis-spi-out`) is being programmed.

See `doc/readme-flash.md` for the switch positions and for the MCU fuse
settings to program in AtmelStudio.

# USB communication as a COM port in Windows

## Windows and POSIX
On Windows, I can see the `pyserial` `port.hwid`:

```powershell
> python -m serial.tools.list_ports -v
COM3
    desc: Intel(R) Active Management Technology - SOL (COM3)
    hwid: PCI\VEN_8086&DEV_9C3D&SUBSYS_221817AA&REV_04\3&21436425&0&B3
COM7
    desc: USB Serial Port (COM7)
    hwid: USB VID:PID=0403:6015 SER=CHROMATION091113
2 ports found
```

But not on Cygwin:
```bash
$ python3 -m serial.tools.list_ports -v
/dev/ttyS2          
    desc: n/a
    hwid: n/a
/dev/ttyS6          
    desc: n/a
    hwid: n/a
2 ports found
```

I need an OSX box and a Linux box to test if `hwid` is a robust solution. I'd
imagine OSX and Linux are similar to the Cygwin POSIX environment, but I don't
want to solve this problem if it's just Cygwin, because I doubt any of our
customers are using Cygwin.

The issue with using the COM number is that it's a kludge. In my example above,
COM3 is S2 and COM7 is S6. It's not obvious if S2 or S6 is the kit. I have to
detach it, scan the ports again, then look at which port is missing. Also, a new
kit will get a new COM number. Say a new kit is COM12, then the POSIX port is
S11. If I leave the identifier as a COM port, I'm making the customer identify
their COM port. That sucks.

So if this problem is true for all POSIX and OSX environments, then I need to
use the `D2XX` library. The solution is just like making the `reset()` function
to manipulate the `CBUS` pin, but now it's to handle port scanning to get the
serial numbers.

- [ ] provide this function in a Chromation `pyspect.py` package
    - goal: customer's writing a USB Host application can `import pyspect` and
      call `pyspect.ports()` without ever consulting the *FTDI D2XX Programmer's
      Guide*. The function will do what `serial.tools.list_ports.grep()` does,
      but it uses d2xx.

## Context
The FTDI device on our `usb-bridge` is accessible from the USB Host in two ways:

- FTDI D2XX API
- generic COM port (VCP)

We use the FTDI D2XX API for LabVIEW programs. Moving forward, we are switching
from LabVIEW to Python. We are, therefore, switching from D2XX to the generic
COM port interface:

- I cannot find a stable `d2xx` python library solution
- the extra functionality offered by D2XX is not compelling enough to write our
  own `d2xx` library

### Use `pyserial`

- `pyserial` is the de facto standard Python serial library
- `pyserial` is compatible with different operating systems

### `pyserial` does not need a `COM` string on Windows
What about the annoyance of having to open a Windows Device Manager and
plug/unplug the USB device to figure out which device is on which COM port?

- `pyserial` has a `grep` function that takes the serial number and returns a
  `port` object
- `pyserial` can then open/close the `port` object without ever having to use
  the `COM` string explicitly

### `pyserial` does need a `tty` string on Cygwin
- the serial number is in the `hwid`
- [ ] troubleshoot why the `hwid` is empty on Cygwin
- for now, I run the Python logging script from PowerShell.

### VCP sees `Serial Number`
- the essential serial commands are available with VCP
- VCP cannot access *all* the USB Description Strings that D2XX can access:
    - `Product Description` is not visible
    - `Manufacturer` is not visible
- but VCP can access `Serial Number`
- `Serial Number` is all we need for identifying spectrometer dev kits on the
  USB Host

### Leave the default USB identifiers in the FTDI `.inf` file
There is a COM port variable named `manufacturer` but this actually comes from
the `.inf` file installed with the USB driver. FTDI wants you to change this.
This FTDI doc shows where the fields are in `FT_Prog`:

<https://www.ftdichip.com/Support/Documents/AppNotes/AN_124_User_Guide_For_FT_PROG.pdf>

And this doc explains what you're supposed to change:

<https://www.ftdichip.com/Support/Documents/TechnicalNotes/TN_102_OEM_Technical_Support_Requirements_for_FTDI_Products.pdf>

But changing these values voids the security certificate:

> It is important to note that the device drivers in this example are
> shown in Figures 2 and 3 as “Not digitally signed”. Any time there are
> edits performed on any of the device driver files, it will invalidate
> the WHQL signature. Re-certification by the OEM is possible. Refer to
> the `AN_101_WHQL_Certified_Driver_Process(FT_000063)`.

- and that would sketch out customers
- and since no one gives a fuck anyway, just leave it as-is:
    - `manufacturer` shows up as FTDI, not a big deal

This is just `eval kit hardware`. If it *were* a consumer product, then we'd
want to change this and do it right.

Doing it right requires paying for the security certificate mojo:

<https://www.ftdichip.com/Support/Documents/AppNotes/AN_101_Submitting_Modified_FTDI_Drivers_for_Windows_Hardware_Certification.pdf>

## Set the serial number in `FT_Prog`
- I leave the `Product Description` as-is so that my LabVIEW code does not
  have to change: it still gets `ChromationSpect-0643-01`
- but now I put `CHROMATION123456` in FTDI `serial_number`
- when `pyserial` scans the USB ports, it can `grep` (awesome!) for the
  serial number and return a `port` object
- I can tell that `port` object to open up without ever having to use the
  `COM` string explicitly
- this *should* make the find-and-open code cross-platform
- other change in `FT_Prog` is the manufacturer
- change manufacturer from `FTDI` to `CH`
    - does not impact `Load VCP` users, e.g., what you can see from
      `pyserial`
    - does not impact my LabVIEW code since I never look at the manufacturer
      field
    - but it helps with the 44 character limit:
    - Product Description + manufacturer + serial number cannot exceed 44
      characters
    - serial number is supposed to be 16 characters: `CHROMATION123456`

## Make FTDI device visible as a generic COM port
- enable `Load VCP` in Windows Device Manager for FTDI bridge IC
- see `dev-kit/doc/load-vcp.md`
- relevant parts are below:

### Change the driver
- connect Chromation eval kit to my computer via USB
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

### Control `CBUS` pin without looking at the `D2XX` documentation
A `D2XX` call is still necessary for the USB Host application to send a hard
reset. The eval kit uses `CBUS`. This is very hardware-specific, so `pyserial`
has no API for manipulating the `CBUS` pin.

- [ ] provide this function in a Chromation `pyspect.py` package
    - goal: customer's writing a USB Host application can `import pyspect` and
      call `pyspect.reset()` without ever consulting the *FTDI D2XX Programmer's
      Guide*

# Install dependencies
## Install `pip` if it is not already installed
```bash
$ sudo apt install python3-pip
$ pip3 install --upgrade pip
```

## Show `pip` version number
```bash
$ python3 -m pip -V
pip 19.0.3 from /usr/lib/python3.6/site-packages/pip (python 3.6)
```

or:
```bash
$ pip3 -V
pip 19.0.3 from /usr/lib/python3.6/site-packages/pip (python 3.6)
```

show help:
```bash
$ pip3 -h

Usage:
  pip3 <command> [options]

Commands:
  install                     Install packages.
...
```

show specific help:
```bash
$ pip3 install -h

Usage:
  pip3 install [options] <requirement specifier> [package-index-options] ...
  pip3 install [options] -r <requirements file> [package-index-options] ...
  pip3 install [options] [-e] <vcs project url> ...
  pip3 install [options] [-e] <local project path> ...
  pip3 install [options] <archive url/path> ...

Description:
  Install packages from:

  - PyPI (and other indexes) using requirement specifiers.
  - VCS project urls.
  - Local project directories.
  - Local or remote source archives.

  pip also supports installing from "requirements files," which provide
  an easy way to specify a whole environment to be installed.
...
```

## List all python packages installed in the current environment
```bash
$ python3 -m pip list
Package           Version
----------------- --------
certifi           2019.3.9
...
virtualenvwrapper 4.8.2
yarg              0.1.9
You are using pip version 19.0.3, however version 19.2.1 is available.
You should consider upgrading via the 'pip install --upgrade pip' command.
```

## Upgrade `pip`
```bash
$ pip3 install --upgrade pip
Collecting pip
  Downloading
  https://files.pythonhosted.org/packages/.../pip-19.2.1-py2.py3-none-any.whl
  (1.4MB)
    100% |████████████████████████████████| 1.4MB 2.8MB/s
Installing collected packages: pip
  Found existing installation: pip 19.0.3
    Uninstalling pip-19.0.3:
      Successfully uninstalled pip-19.0.3
Successfully installed pip-19.2.1
```

## Check if `pyserial` is installed
```bash
$ python3
Python 3.6.8 (default, Feb 14 2019, 22:09:48)
[GCC 7.4.0] on cygwin
Type "help", "copyright", "credits" or "license" for more information.
```

```python
>>> import serial
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ModuleNotFoundError: No module named 'serial'

>>> exit()
```

## Install `pyserial`
```bash
$ pip3 install pyserial
Collecting pyserial
  Using cached https://files.pythonhosted.org/packages/.../pyserial-3.4-py2.py3-none-any.whl
Installing collected packages: pyserial
Successfully installed pyserial-3.4
```

## See where `pyserial` installed
```bash
$ pip3 show pyserial
Name: pyserial
Version: 3.4
Summary: Python Serial Port Extension
Home-page: https://github.com/pyserial/pyserial
Author: Chris Liechti
Author-email: cliechti@gmx.net
License: BSD
Location: /usr/lib/python3.6/site-packages
Requires: 
Required-by: 
```

```powershell
$ pip show pyserial
Name: pyserial
Version: 3.4
Summary: Python Serial Port Extension
Home-page: https://github.com/pyserial/pyserial
Author: Chris Liechti
Author-email: cliechti@gmx.net
License: BSD
Location: c:\python37\lib\site-packages
Requires: 
Required-by: 
```

## Use `pyserial` to *see* the dev-kit from bash
This method uses the `list_ports` utility provided with `pyserial`.

```bash
$ python3 -m serial.tools.list_ports
```

- `list_ports.py` is a command-line utility in the `pyserial` lib
- `python -m` searches `sys.path` for a module to execute as a script
- `python -m serial` searches for module `pyserial`
- `python -m serial.tools.list_ports` searches specifically for `list_ports.py`
  and executes its contents as the `__main__` module
- see <https://docs.python.org/3.6/using/cmdline.html#cmdoption-m>

An alternative way to run `list_ports` is to navigate to the `pyserial` folder
and run it like any other Python script.

```bash
$ cd /usr/lib/python3.6/site-packages/serial/tools/
$ python3 ./list_ports.py
/dev/ttyS2
/dev/ttyS6
2 ports found
```

Verbose list:
```bash
$ python3 ./list_ports.py -v
/dev/ttyS2
    desc: n/a
    hwid: n/a
/dev/ttyS6
    desc: n/a
    hwid: n/a
2 ports found
```

Only list CHROMATION devices:
```bash
$ python3 ./list_ports.py CHROMATION -v
Filtered list with regexp: 'CHROMATION'
no ports found
```

## Use `pyserial` to *see* the dev-kit from PowerShell
I can see the Chromation serial number running `pyserial` under Windows.
```powershell
> python .\list_ports.py -v
COM3                
    desc: Intel(R) Active Management Technology - SOL (COM3)
    hwid: PCI\VEN_8086&DEV_9C3D&SUBSYS_221817AA&REV_04\3&21436425&0&B3
COM7                
    desc: USB Serial Port (COM7)
    hwid: USB VID:PID=0403:6015 SER=CHROMATION091113
2 ports found
```

## Trouble with Cygwin
I cannot see the Chromation serial number running `pyserial` under Cygwin. This
is because I cannot see the `hwid` in Cygwin. Why?

# Examples at the REPL
## Use `pyserial` to *see* the dev-kit from the Python REPL

### Cygwin: USB at the REPL
```python
>>> import serial.tools.list_ports as usb_ports
>>> ports = usb_ports.grep('')
>>> print('\n'.join([port.device for port in ports]))
/dev/ttyS2
/dev/ttyS6
```

`include_links` defaults to `False`, but making it `True` does not help:
```python
>>> ports = usb_ports.grep('tty', include_links=True)
>>> print('\n'.join([port.hwid for port in ports]))
n/a
```

So I can *see* the kit. It is `ttyS6`. But I cannot find it by serial number.

I modified the `main.py` logging example to open using `/dev/ttyS6`.

### PowerShell: USB at the REPL
```powershell
> python
```

```python

#### Import the `list_ports` utility to use its functions at the REPL.
>>> import serial.tools.list_ports as usb_ports

#### Show the COM ports
>>> ports = usb_ports.grep('')
>>> print('\n'.join([str(port) for port in ports]))
COM3 - Intel(R) Active Management Technology - SOL (COM3)
COM7 - USB Serial Port (COM7)

#### Show the hwid
>>> ports = usb_ports.grep('')
>>> print('\n'.join([str(port.hwid) for port in ports]))
PCI\VEN_8086&DEV_9C3D&SUBSYS_221817AA&REV_04\3&21436425&0&B3
USB VID:PID=0403:6015 SER=CHROMATION091113
```

`usb_ports.grep()` returns a generator, so it needs to be recreated every time
because the subsequent list comprehension walks the generator until it has
nothing more to generate.

```python
#### Only show for Chromation
>>> ports = usb_ports.grep('CHROMATION')
>>> print('\n'.join([port.hwid for port in ports]))
USB VID:PID=0403:6015 SER=CHROMATION091113

#### Only show the Chromation spectrometer serial number
>>> ports = usb_ports.grep('CHROMATION')
>>> print('\n'.join([port.serial_number for port in ports]))
CHROMATION091113

#### Only show the number part of the Chromation spectrometer serial number
>>> ports = usb_ports.grep('CHROMATION')
>>> print('\n'.join([port.serial_number.strip('CHROMATION') for port in ports]))
091113
```

# Python application examples
- see `old-python-readme.md`
- right now I have two scripts:
    - `main.py` is based on the *new* firmware
        - send commands
        - log responses
        - output a log file that reads like a system-level test suite
    - `live-plot-using-old-fragile-protocol.py` is based on the *old* firmware
        - the code in here has to be entered at the REPL
        - but it does get a live plot running using `matplotlib`
        - it's just proof that this method is possible
            - but it is not code to build on
            - it's something I did in one shot late one night (12-3AM)
            - the good: it is very short
            - the bad: it relies on global state
- these scripts are *way* bigger than they should be because nothing is broken
  out to libs yet
- each script I write has its own low-level serial wrappers on `pyserial`
    - [ ] move wrappers to their own module
- serial commands are encapsulated in functions
    - [ ] move serial commands to their own module
- functions are all very short thanks to generators and list comprehensions
    - [ ] write up a reference on generators and list comprehensions
    - assume customers are *not* familiar with these language features
    - up until 3AM figuring out a snappy live updating matplotlib

# Run the logging script

```bash
$ cd $dev_kit
$ ./python/main.py
```

- `$dev_kit` is defined in my `~/.bashrc`:
```bash
export CHROMATION_HOME=/cygdrive/c/chromation-dropbox/Dropbox
dev_kit=$CHROMATION_HOME'/c/dev-kit'
```

# Example valid spectrometer configurations

These are examples of valid parameters
to send via `cfg_spectrometer()`.
The examples are shown two ways:

- as an array of four bytes
- as a single 32-bit word in hexadecimal


----------------------------------------
1x gain, binning on, select all rows:

	0x0ffffff9

	b'\x0F\xFF\xFF\xF9'

	uint8_t const valid_cfg[] = {0x0F, 0xFF, 0xFF, 0xF9};

----------------------------------------
1x gain, binning on, select only row 1:

	0x00842109

	b'\x00\x84\x21\x09'

	uint8_t const valid_cfg[] = {0x00, 0x84, 0x21, 0x09};

----------------------------------------
1x gain, binning on, select only row 2:

	0x01084211

	b'\x01\x08\x42\x11'

	uint8_t const valid_cfg[] = {0x01, 0x08, 0x42, 0x11};

----------------------------------------
1x gain, binning on, select only row 3:

	0x02108421

	b'\x02\x10\x84\x21'

	uint8_t const valid_cfg[] = {0x02, 0x10, 0x84, 0x21};

----------------------------------------
1x gain, binning on, select only row 4:

	0x04210841

	b'\x04\x21\x08\x41'

	uint8_t const valid_cfg[] = {0x04, 0x21, 0x08, 0x41};

----------------------------------------
1x gain, binning on, select only row 5:

	0x08421081

	b'\x08\x42\x10\x81'

	uint8_t const valid_cfg[] = {0x08, 0x42, 0x10, 0x81};

----------------------------------------
1x gain, binning on, select rows 1, 2, and 3:

	0x039ce739

	b'\x03\x9C\xE7\x39'

	uint8_t const valid_cfg[] = {0x03, 0x9C, 0xE7, 0x39};

# Pixel masks

These are pixel masks for checking
if pixels are on/off in a valid configuration.


----------------------------------------
all-pixels:

	0x0ffffff8

	b'\x0F\xFF\xFF\xF8'

	uint8_t const pixel_mask[] = {0x0F, 0xFF, 0xFF, 0xF8};

----------------------------------------
row-1:

	0x00842108

	b'\x00\x84\x21\x08'

	uint8_t const pixel_mask[] = {0x00, 0x84, 0x21, 0x08};

----------------------------------------
row-2:

	0x01084210

	b'\x01\x08\x42\x10'

	uint8_t const pixel_mask[] = {0x01, 0x08, 0x42, 0x10};

----------------------------------------
row-3:

	0x02108420

	b'\x02\x10\x84\x20'

	uint8_t const pixel_mask[] = {0x02, 0x10, 0x84, 0x20};

----------------------------------------
row-4:

	0x04210840

	b'\x04\x21\x08\x40'

	uint8_t const pixel_mask[] = {0x04, 0x21, 0x08, 0x40};

----------------------------------------
row-5:

	0x08421080

	b'\x08\x42\x10\x80'

	uint8_t const pixel_mask[] = {0x08, 0x42, 0x10, 0x80};

# Invalid configurations for unit tests

These are invalid configurations checking
if the embedded hardware is screening these out correctly.


----------------------------------------
bad-cfg-1:

	0x0f83e0f9

	b'\x0F\x83\xE0\xF9'

	uint8_t const invalid_cfg[] = {0x0F, 0x83, 0xE0, 0xF9};

----------------------------------------
bad-cfg-2:

	0x0fffe001

	b'\x0F\xFF\xE0\x01'

	uint8_t const invalid_cfg[] = {0x0F, 0xFF, 0xE0, 0x01};

----------------------------------------
bad-cfg-3:

	0x0003fff9

	b'\x00\x03\xFF\xF9'

	uint8_t const invalid_cfg[] = {0x00, 0x03, 0xFF, 0xF9};

