- [Windows Setup](#install-python-and-microspec-on-windows)
- [Linux Mint Setup](#install-python-and-microspec-on-linux-mint)
- [Check `microspec` is working](#check-microspec-works)

# Install Python and microspec on Windows

Overview:

- [enable "Load VCP"](#enable-load-vcp)
- [download](#download-windows-installer-for-python) and [install Python](#install-python-on-windows)
- [test the installation](#check-python-is-installed-on-windows)
- [upgrade `pip`](#upgrade-pip-on-windows)
- install Chromation package `microspec`:
    - [install in a virtual environment](#create-a-virtual-environment)
    - or [install to your base Python installation](#install-microspec-on-windows)

## Enable Load VCP

The dev-kit uses FTDI# FT221X for USB communication. You should
not need to download drivers from the FTDI site, but you do need
to tell Windows to `Load VCP` for this device.

Please see [enable "Load VCP"](DEV-KIT.md#windows-users-enable-load-vcp).

## Download Windows installer for Python

- Go to the latest official [Python download page for Windows](https://www.python.org/downloads/windows/)
- Download the executable installer for Windows
    - example for my Windows system:

    ```
    python-3.8.1-amd64.exe
    ```

## Install Python on Windows

- Run the installer
- At the final step of installation, customize to add `py`
  launcher:
    - Select option to **Add Python to environment variables**

## Check Python is installed on Windows

- `Win+x` then press `i` to open a Windows PowerShell
- run `python --version`

```powershell
PS C:\Users\mike> python --version
Python 3.8.1
```

Open the `python` *REPL*:

```powershell
PS C:\Users\mike> python
Python 3.8.1 (tags/v3.8.1:1b293b6, Dec 18 2019, 23:11:46) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
```

Exit Python:

```python
>>> exit()
```

## Upgrade pip on Windows

Check the version of pip:

```powershell
PS C:\Users\mike> pip --version
pip 19.2.3 from c:\users\mike\appdata\local\programs\python\python38\lib\site-packages\pip
(python 3.8)
```

Upgrade to the latest pip:

```powershell
PS C:\Users\mike> python -m pip install --upgrade pip
Collecting pip
  Downloading pip-20.2.2-py2.py3-none-any.whl (1.5 MB)
     |████████████████████████████████| 1.5 MB 1.6 MB/s
Installing collected packages: pip
  Attempting uninstall: pip
    Found existing installation: pip 20.2
    Uninstalling pip-20.2:
      Successfully uninstalled pip-20.2
Successfully installed pip-20.2.2
```

## Try `microspec` in a virtual environment

Create a virtual environment, activate the virtual environment,
install microspec with pip, then deactivate the virtual
environment.

If you do not want use a virtual environment, just skip to
section [Install microspec](#install-microspec).

### Create a virtual environment

Pick a name for the virtual environment. This example is a
throwaway environment, so I name it `test`.

First, make sure there is no folder in this directory named
`test`:

```powershell
PS C:\Users\mike> ls test
ls : Cannot find path 'C:\Users\mike\test' because it does not exist.
```

Create the virtual environment:

```powershell
PS C:\Users\mike> python -m venv test
```

Be patient, this will take a few seconds. Expect no output.

Check the virtual environment was created by repeating the `ls`
command (`up-arrow` to go backwards in PowerShell history):

```powershell
PS C:\Users\mike> ls test


    Directory: C:\Users\mike\test


Mode                LastWriteTime         Length Name
----                -------------         ------ ----
d-----        8/12/2020   5:42 PM                Include
d-----        8/12/2020   5:42 PM                Lib
d-----        8/12/2020   5:42 PM                Scripts
-a----        8/12/2020   5:42 PM            116 pyvenv.cfg
```

### Activate the virtual environment

Run the `activate` script. To run a script in PowerShell, simply
enter the path to the script.

```powershell
PS C:\Users\mike> .\test\Scripts\activate
(test) PS C:\Users\mike>
```

Notice the prompt is prefaced with the virtual environment name.

List the installed packages. Note that there are no additional
packages installed yet because this is a new virtual environment:

```powershell
(test) PS C:\Users\mike> pip list
Package    Version
---------- -------
pip        19.2.3
setuptools 41.2.0
WARNING: You are using pip version 19.2.3, however version 20.2.2 is available.
You should consider upgrading via the 'python -m pip install --upgrade pip' command.
```

If you plan to keep this virtual environment, upgrade its pip:

```powershell
(test) PS C:\Users\mike> python -m pip install --upgrade pip
```

### Deactivate the virtual environment

After installing and trying out `microspec` in the next section,
deactivate the virtual environment:

```powershell
(test) PS C:\Users\mike> deactivate
PS C:\Users\mike>
```

### Delete the virtual environment

If this virtual environment is just a throwaway test environment,
delete it to free up disk space:

```powershell
PS C:\Users\mike> rm .\test\

Confirm
The item at C:\Users\mike\test\ has children and the Recurse parameter was not
specified. If you continue, all children will be removed with the item. Are you
sure you want to continue?
[Y] Yes  [A] Yes to All  [N] No  [L] No to All  [S] Suspend  [?] Help
(default is "Y"):A
```

### Install microspec on Windows

```powershell
(test) PS C:\Users\mike> pip install microspec
Collecting microspec
  Using cached https://files.pythonhosted.org/packages/7e/7a/17ec4288fb192ef2d9cfe25c11f64744d7e2415c5b717bbb1f19b202f2d7/microspec-0.1.1a3-py3-none-any.whl
Collecting pyserial (from microspec)
  Using cached https://files.pythonhosted.org/packages/0d/e4/2a744dd9e3be04a0c0907414e2a01a7c88bb3915cbe3c8cc06e209f59c30/pyserial-3.4-py2.py3-none-any.whl
Installing collected packages: pyserial, microspec
Successfully installed microspec-0.1.1a3 pyserial-3.4
```

## Install dependencies for Chromation GUI applications

Chromation uses `pygame` to create GUI applications.

The dev-kit GUI is still under development. It is project
`microspecgui` on PyPI.

```powershell
(test) PS C:\Users\mike> pip install microspecgui
```

As a requirement to `microspecgui`, this command also installs
`pygame` version 1.9.6. The Windows build of Python contains all
of the non-Python dependencies necessary to install `pygame`.

# Install Python and microspec on Linux Mint

Overview:
- [Gain permission to access `/dev/ttyUSB0`](#gain-permission-to-access-ttyusb0)
- Either [use the installed Python](#python-install-option-1):
    - use the Python3 that comes with the Linux Mint distribution
    - install `pip` and `venv` with the Linux package manager
- Or [build Python from source](#python-install-option-2):
    - download latest Python source archive
    - build Python from source
        - this build includes `pip` and `venv`

At this point, some version of Python3 is installed with packages
`pip` and `venv`:

- [upgrade `pip`](#upgrade-pip)
- [install Chromation package `microspec`](#install-microspec-in-a-virtual-environment-on-Linux-Mint)

## Gain permission to access ttyUSB0

### Context

The dev-kit uses *FTDI# FT221X* for USB communication. You should
*not* need to download drivers from the FTDI site. Just connect
the dev-kit with a USB cable and it should be visible to the
operating system.

But you do need to grant the user permission to communicate with
the dev-kit over USB.

### Check hardware is visible

- connect the dev-kit with a USB cable
    - *expect:* dev-kit indicator LEDs light up
    - two yellow LEDs
    - three green LEDs
- display messages from the kernel ring buffer that contain
  string `FTDI`:

```bash
$ dmesg | grep FTDI
[101038.609497] usb 3-1: FTDI USB Serial Device converter now attached to ttyUSB0
```

Only members of the `dialout` group have permission to open port
`/dev/ttyUSB0`.

If this user is communicating with an FTDI device over USB
for the first time, you probably need to add this user to the
`dialout` group.

Show the current user:

```bash
$ echo $USER
```

Show which `groups` this user is part of:

```bash
$ groups $USER
```

Check if this user is part of the `dialout` group:

```bash
$ groups $USER | grep dialout
```

If the user is already part of `dialout`, a list of groups is
printed and `dialout` is highlighted.

If the user is not part of `dialout`, nothing is printed.

Add this user to `dialout`:

```bash
$ sudo adduser $USER dialout
```

Confirm the user is part of the `dialout` group:

```bash
$ groups $USER | grep dialout
```

Finally, logout and log back in (or simply restart the computer).
Even though the user is listed as a member of `dialout`, this
change does not take effect until the user logs back in.

## Python install option 1

Use the default Python that comes with Linux Mint.

`microspec` requires Python >=3.7. Check which version is
installed:

```bash
$ python3 -V
Python 3.6.9
```

At the time of this writing, Linux Mint is distributed with a
version that is less than Python 3.7, so I have to build Python
from source. See [Python install option
2](#python-install-option-2).

If the version is >= 3.7, then install `pip` and `venv`, detailed
below. If the packages are already installed, `apt` checks if
they are the latest versions.

### Install `pip`

```bash
$ sudo apt install python3-pip
...
Recommended packages:
  python3-setuptools python3-wheel
The following NEW packages will be installed:
  python3-pip
...
```

### Install `venv`

```bash
$ sudo apt install python3-venv
...
The following additional packages will be installed:
  python-pip-whl python3.6-venv
The following NEW packages will be installed:
  python-pip-whl python3-venv python3.6-venv
...
```

## Python install option 2

Build Python from source.

### Install the build dependencies

```bash
$ sudo apt install build-essential checkinstall
$ sudo apt install libreadline-gplv2-dev \
libncursesw5-dev \
libssl-dev \
libsqlite3-dev \
tk-dev \
libgdbm-dev \
libc6-dev \
libbz2-dev \
libffi-dev \
zlib1g-dev
```

### Download the compressed source distribution

Download the source to `/opt`.

*`/opt` is the folder for software that is not part of the
default Linux Mint installation.*

```bash
$ cd /opt
```

Go to `python.org` to check the latest Python version number.
Edit the link below. At the time of this writing, the latest
version is `3.8.5`.

```bash
$ sudo wget https://www.python.org/ftp/python/3.8.5/Python-3.8.5.tgz
```

### Extract the archive file

Extract the archive into folder Python-3.8.5:

```bash
$ sudo tar xzf Python-3.8.5.tgz
```

### Configure the build

Enter the source folder and run the configure script.

```bash
$ cd Python-3.8.5
$ sudo ./configure --enable-optimizations
```

This takes about a minute.

### Build and install Python

Use `altinstall` to avoid replacing the default /usr/bin/python.

```bash
$ sudo make altinstall
```

This takes about ten minutes.

### Check version numbers

`python3` still uses the default installation Python.

`python3.8` invokes this `alt` installation of the latest Python.

```bash
$ python3.8 --version
Python 3.8.5
```

## Upgrade pip

If Python was built from source, identify if by the version
number. I built Python 3.8.5, so I use `python3.8`:

```bash
$ python3.8 -m pip install --upgrade-pip
```

If using the default Python that comes with Linux Mint, identify
it as `python3`:

```bash
$ python3 -m pip install --upgrade-pip
```

*Each installation of Python has its own `pip` installation.*

## Install microspec in a virtual environment on Linux Mint

Create the virtual environment:

```bash
$ cd ~
$ python3 -m venv test
```

Remember to use the correct Python version number in the above
command:

- `python3` is the default installation (which is `python3.6` in my case)
- `python3.8` is the installation built from source

Activate the virtual environment:

```bash
$ . ~/test/bin/activate
(test) $ 
```

Now that I am in the virtual environment, `python` refers to the
correct version of Python.

For example, if I created the virtual environment with
`python3.8`, then `python` shows version `3.8.5`:

```bash
(test)$ python --version
Python 3.8.5
```

Likewise, `pip` is the correct version of `pip`. I do not need to
specify `pip3` or `pip3.8`.

If I plan to keep this virtual environment, take 10 seconds to
upgrade `pip`:

```bash
(test)$ python -m pip install --upgrade pip
```

Install `microspec`:

```bash
$ pip install microspec
```

Deactivate the virtual environment:

```bash
(test)$ deactivate
$ 
```

Delete the virtual environment:

```bash
$ sudo rm -r ~/test
```

## Install dependencies for Chromation GUI applications

Chromation uses `pygame` to create GUI applications.

Using the Python3 that comes with Linux Mint, I am able to
install `pygame` like any other Python package:

```bash
$ python3 -m pip install pygame
```

But the install fails for the Python3.8 that I built from source:

```bash
$ python3.8 -m pip install pygame
```

The solution is to install several build dependencies with the
Linux package manager. Without these dependencies, the above
command results in different errors depending on which
dependencies I am missing.

### Install `pygame` build dependencies

`pygame` is a Python wrapper around `SDL` and requires
`sdl-config`.

Install `sdl-config`:

```bash
$ sudo apt install libsdl1.2-dev
```

*The install collects additional packages that are not part of the
default Linux Mint distribution. Proceed with installing the
additional packages.*

Check `sdl-config` is installed:

```bash
$ sdl-config --version
1.2.15
```

Obtain all of the other dependencies for building pygame using
the list of build dependencies for the `python-pygame` package:

```bash
$ sudo apt-get build-dep python-pygame
```

The following dependencies are installed:

```
libflac-dev
libjbig-dev
libjpeg-dev
libjpeg-turbo8-dev
libjpeg8-dev
liblzma-dev
libmad0-dev
libmikmod-config
libmikmod-dev
libogg-dev
libportmidi-dev
libpython-all-dev
libsdl-image1.2-dev
libsdl-mixer1.2-dev
libsdl-ttf2.0-dev
libsmpeg-dev
libtiff-dev
libtiff5-dev
libtiffxx5
libvorbis-dev
libwebp-dev
python-all
pythonall-dev
sharutils
```

Note this is not the same as installing the Linux package
`python-pygame`. These are the packages needed for *building*
`python-pygame`. Luckily, these are also the packages for
`pip` to run pygame's `setup.py`.

Now finally pygame will install with the usual command:

```bash
$ python3.8 -m pip install pygame
```

Or from the virtual environment `test`:

```bash
(test) $ pip install pygame
```

Or when installing Chromation's GUI, `pygame` is installed as a
requirement:

```bash
(test) $ pip install microspecgui
```

## Setup for modifying microspeclib

Like any other Python package, if you simply want to use the
`microspeclib` package in an application, *it is not necessary to
run the `microspeclib` unit tests or to rebuild the `microspec`
documentation.*

For example, users can write system-level tests like the ones in
`dev-kit-2020/python/system-tests.py` and never run the
`microspeclib` unit tests.

But for users interested in modifying `microspeclib`, there are
**unit tests** and **documentation** to maintain.

This section explains how to set up the tools for this work.
*These tools only work on Linux and Mac.* The unit tests require
`socat`, only available on Linux and Mac. If you figure out how
to rebuild the documentation on Windows, please let us know.

### Clone project microspec

First, create a local clone of the project repository (installing
the `microspeclib` package with `pip install microspec` does not
download the project repository):

```bash
$ git clone https://github.com/microspectrometer/microspec
```

This creates folder `microspec` in the current directory.

### Install additional Python packages

The tests and documentation require additional Python packages.
Since typical users do not require these, they are not installed
with `microspec` by default.

Install the additional packages for dev work:

```bash
$ pip install microspec[dev]
```

*These additional packages are listed in the `extras_require` dict
in `setup.py`.*

### Install utility for emulating hardware

The `microspec` unit tests use an **emulator**. When writing an
application that uses `microspeclib`, it is helpful to have the
option of faking the dev-kit hardware.

The **emulator** depends on the `socat` utility, only available
on Linux and Mac.

Check if `socat` is installed:

```bash
$ socat -V
```

If not found, install `socat`:

```bash
$ sudo apt install socat
```

Check it installed:

```bash
$ socat -V
socat by Gerhard Rieger and contributors - see www.dest-unreach.org
socat version 1.7.3.2 on Apr 4 2018 10:06:49
...
```

## Overview of modifying the API

At a high-level, the flow to modify the API is:

- **alter the API calls** by editing `microspec/cfg/microspec.json`
- **run the unit tests** to make sure they still pass
- **edit the docstrings** in
  `src/microspeclib/internal/docstrings.py`
- **rebuild the documentation**

### Run the tests

From the top-level folder of `microspec`, run `pytest`:

```bash
$ cd microspec
$ pytest
```

The test ends with a summary:

- how many passed
- how many failed *and were expected to fail* (this means they passed)
- how many failed *but were expected to pass*
    - these are the *real* failures
    - if this number is zero, the unit tests all **pass**

Unlike the conventional unit tests in `dev-kit-2020/python/system-tests.py`, the `microspec` test suite is auto-generated.

Editing the `cfg/microspec.json` file is enough to generate new
unit tests. But it is sometimes necessary to modify the test
generator function `generateTest()` to catch new cases introduced
by the new functionality. This function is defined in:

- `microspec/tests/test_expert_interface.py`
- `microspec/tests/test_simple_interface.py`

### Rebuild the documentation

Since the command methods are created by meta-class factories,
they are not defined in code, so there are no docstrings to
edit.

`src/microspeclib/internal/docstrings.py` is where the developer
places the documentation that would normally go in the docstring.

Then enter the `microspec/doc` folder and run the Sphinx
Makefile to generate the new HTML documentation:

```bash
$ cd microspec/doc
$ make clean html
```

# Check microspec works

## Open the Windows Python REPL

Open the Python REPL from PowerShell.

- `Win+x` then press `i` to open a Windows PowerShell

Activate virtual environment `test`:

```powershell
PS C:\Users\mike> .\test\Scripts\activate
(test) PS C:\Users\mike> 
```

Start Python:

```powershell
(test) PS C:\Users\mike> python
```
```python
Python 3.8.1 (tags/v3.8.1:1b293b6, Dec 18 2019, 23:11:46) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>>
```

## Open the Linux Mint Python REPL

On Linux Mint, open the Python REPL from bash.

- `ctrl+alt+t` to open a bash terminal

Activate virtual environment `test`:

```bash
$ . ~/test/bin/activate
(test) $ 
```

Start Python:

```bash
(test) $ python
```
```python
Python 3.8.5 (default, Aug 12 2020, 00:28:27)
[GCC 7.5.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>>
```

## Test the API is installed and the dev-kit is visible

This first line imports the API. If the API is installed, expect
no response:

```python
>>> from microspeclib.simple import MicroSpecSimpleInterface
```

This next line opens communication with the dev-kit over USB. If
the dev-kit is connected over USB, expect no response:

```python
>>> kit = MicroSpecSimpleInterface(timeout=2.0)
```

## Troubleshoot failure to open communication

If the above command results in the error:

```python
    raise MicroSpecConnectionException("Cannot find CHROMATION device")
microspeclib.exceptions.MicroSpecConnectionException: Cannot find CHROMATION device
```

Check:

- is the dev-kit connected over USB?
- are the indicator LEDs lit?
- on *Windows*:
    - did you [enable "Load VCP"](DEV-KIT.md#windows-users-enable-load-vcp)?
- on *Linux Mint*:
    - did you [make `$USER` a member of group `dialout`](#gain-permission-to-access-ttyusb0)?

## Try an API command

If there was no error, continue with a quick example using the
API.

Command `getSensorConfig` gets the pixel configuration of the
spectrometer's photodiode array:

```python
>>> print(kit.getSensorConfig())
SensorGetSensorConfig(status=0, binning=1, gain=1, row_bitmap=31)
```

## Exit the REPL to close USB communication

There is no command to close USB communication. The serial
connection will automatically close when you exit the REPL.

## Execute the example as a script

Try executing the same example as a script:

```python
from microspeclib.simple import MicroSpecSimpleInterface
kit = MicroSpecSimpleInterface(timeout=2.0)
print(kit.getSensorConfig())
```

The serial connection automatically closes when the script exits,
*regardless of whether the script exits normally or due to an
exception.*

In other words, context management is already done for you. You
do *not* need to open communication inside a `with` block. If the
script terminates due to an exception, you do not need to reset
the hardware before running the script again.


