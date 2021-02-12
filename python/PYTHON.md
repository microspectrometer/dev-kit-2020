# Talk to the Chromation dev-kit with Python package microspec

Chromation's *Python project* `microspec` is available from PyPI
(the Python Package Index) at
<https://pypi.org/project/microspec/>.

The latest documentation for `microspec` is here: https://microspec.readthedocs.io

We are adding a higher-level API to `microspec` to simplify
application programming. It is already part of the latest version
of `microspec`, but we don't have any example scripts yet. The
documentation for the higher-level API is temporarily located here:
https://microspec-api.readthedocs.io/

There is also a Python GUI at
https://pypi.org/project/microspecgui/.

The GUI does not have much functionality yet, but it at least
gives a live plot with control over exposure time and a display
of the peak pixel. It's a good alternative when using the LabVIEW
GUI is inconvenient but you need visual feedback to align your
spectrometer. And unlike the LabVIEW GUI, the Python GUI works on
Windows, Mac, and Linux, and Raspberry Pi 3 or higher.

See [PYTHON-SETUP.md](PYTHON-SETUP.md) for steps starting on a
Windows or Linux system without Python.

## Overview of microspec

[microspec](https://pypi.org/project/microspec/) is a Python
package for USB communication with the spectrometer dev-kit.

Install `microspec` with `pip`:

```
$ pip install microspec
```

REPL Example:

```
>>> import microspec as usp
>>> kit = usp.Devkit()
>>> reply = kit.captureFrame()
>>> print(reply)
captureFrame_response(status='OK', num_pixels=392,
                      pixels=[...], frame={...})
```

Command-line example:

```
$ microspec-cmdline.exe captureframe
2021-02-11T22:56:17.133290,SensorCaptureFrame(status=0, num_pixels=392, pixels=[...])
```

### Inside the microspec package

The `microspec` package has a command line utility and two
modules:

- *utility* `microspec_cmdline.py`:
    - a simple command line utility to send single commands to
      the dev-kit without writing any Python
    - I use this utility for quick tests because it avoids the
      overhead of opening a REPL, importing the package, and
      instantiating the kit
        - for example, I send commands to the dev-kit directly
          from Vim (my text editor) by making a shell command
          shortcut that sends `microspec-cmdline.exe` followed by
          the dev-kit command and arguments
- *module* `microspec`:
    - a thin wrapper on `microspeclib` for the purpose
      of adding docstrings and simplifying the UI for sending
      commands to the dev-kit 
    - use this module if you just want to write Python
      applications that talk to the Chromation dev-kit
    - for example, the above REPL example is using `microspec`
    - [view source code on GitHub](https://github.com/microspectrometer/microspec/tree/master/src/microspec)
    - [view documentation on Read the Docs](https://microspec-api.readthedocs.io/en/latest)
- *module* `microspeclib`:
    - the real module that talks to the dev-kit hardware
    - each dev-kit command is a method call
    - the methods are auto-generated from the serial
      communication protocol defined in the [JSON config file](https://github.com/microspectrometer/microspec/blob/master/cfg/microspec.json)
    - so, use this if you are writing your own firmware (or want
      to modify the Chromation dev-kit firmware) and would rather
      modify a JSON file than write your own USB interface
    - for example, the command-line utility is a Python script
      that uses `microspeclib`
        - this means I can modify the JSON file (the serial
          communication protocol) and use the
          `microspec-cmdline.exe` utility to test that I've
          revised the firmware to match the change in the
          protocol -- I don't need to write any Python code to do
          this test
    - changing the JSON file changes the dev-kit commands in
      `microspeclib`, but breaks the `microspec` module (which is
      good because having to update `micropec` manually is my
      reminder to update the docstrings)
    - [view source code on GitHub](https://github.com/microspectrometer/microspec/tree/master/src/microspeclib)
    - [view documentation on Read the Docs](https://microspec.readthedocs.io/en/latest/)

The same example from earlier looks like this using `microspeclib`:

```python
>>> from microspeclib.simple import MicroSpecSimpleInterface
>>> kit = MicroSpecSimpleInterface()
>>> reply = kit.captureFrame()
>>> print(reply)
SensorCaptureFrame(status=0, num_pixels=392, pixels=[...])
```

## Using `microspec` to talk to the dev-kit

The `microspec` package make it easy to write Python applications
to control the dev-kit over USB **without** solving low-level USB
hardware issues such as:

- opening and closing USB communication
- searching the USB ports for the Chromation dev-kit
- checking when bytes are available to read

### A three-line example script

Here is a short example script using `microspeclib` that:

- finds the dev-kit
- opens communication
- sends a single command
- prints the response to stdout
- closes communication with the dev-kit

```python
from microspeclib.simple import MicroSpecSimpleInterface
kit = MicroSpecSimpleInterface(timeout=2.0)
print(kit.getSensorConfig())
```

Running the above 3-line script results in the following output:

```python
SensorGetSensorConfig(status=0, binning=1, gain=1, row_bitmap=31)
```

*In the above 3-line script, the command is `getSensorConfig`
which reads the spectrometer chip's internal analog
configuration.*


What is going on:

- the low-level USB work of locating and opening the USB device
  happens when creating an instance of the
  `MicroSpecSimpleInterface` class
    - example: to create an instance named `kit`:
    - `kit = MicroSpecSimpleInterface()`
- the USB device is automatically closed when the script ends
    - if the USB device is opened at the REPL, the device is
      automatically closed when exiting the REPL
- commands in the API are methods of the
  `MicroSpecSimpleInterface` instance
    - example: send command `foo` by calling `kit.foo()`

If multiple dev-kits are connected, name the dev-kit by serial
number when instantiating:

```python
from microspeclib.simple import MicroSpecSimpleInterface
kit = MicroSpecSimpleInterface(serial_number='091103',timeout=2.0)
print(kit.serial.serial_number)
```

Running the above 3-line script results in the following output
(the output depends on the kit's serial number):

```python
CHROMATION091103
```

If there is no dev-kit connected with serial number `091103` (or
if that dev-kit is already opened in another application), the
following exception is raised:

```python
MicroSpecConnectionException: Cannot find CHROMATION device
```


## Command Line API

`microspec_cmdline.py` runs a single command and prints the reply
to stdout, optionally in CSV format.

- command is case-insensitive
- pass command parameters as key=value, e.g., `led_num=0` or
`cycles=100`

For example, the following sequence of commands will:

- configure the detector with the recommended settings
- set exposure time to 100 cycles
- capture three frames spaced 1.5 seconds apart
- print the result in CSV format

```
microspec_cmdline.py setsensorconfig binning=true gain=gain1x row_bitmap=0x1f
microspec_cmdline.py setexposure cycles=100
microspec_cmdline.py captureframe -t 0.2 -r 3 -w 1.5 --csv
```

- `-t` *timeout*:
    - how long to wait for a response to each command
    - if the command timeouts (no response), the utility prints
      `None` and moves on
- `-`r *repeat*
    - run the command this many times
- `-w` *wait*:
    - how long to *wait* between repeated commands

All timeout and wait times are in fractional seconds.

### The command line utility is a `.exe` on Windows

On Windows, replace `microspec_cmdline.py` in the above lines
with `microspec_cmdline.exe`.

For example:

```powershell
> microspec-cmdline.exe captureframe -t 0.2 -r 3 -w 1.5 --csv
```


## Python examples

The repository has some examples:

- `one-command` runs one dev-kit command. The script docstring
  has a short summary of all the commands.
- `system-tests` runs every dev-kit command.

using `microspec`:
- [one-command.py](https://github.com/microspectrometer/dev-kit-2020/blob/master/python/microspec/one-command.py)

using `microspeclib`:
- [one-command.py](https://github.com/microspectrometer/dev-kit-2020/blob/master/python/microspeclib/one-command.py)
- [system-tests.py](https://github.com/microspectrometer/dev-kit-2020/blob/master/python/microspeclib/system-tests.py)

## Python GUI

*A GUI is necessary for real-time feedback when optically
aligning the spectrometer in a measurement setup.*

Chromation provides `microspecgui`. This is a Python GUI to
replace the legacy LabVIEW GUI. `microspecgui` uses the
`microspeclib` package for communication and SDL interface
`pygame` for display and user control.

Install:

```
$ pip install microspecgui
```

Run:

```
$ microspec-gui
```

On Windows:

```
> microspec-gui.exe
```

Use:

```
X       - decrease exposure
Shift+X - increase exposure
Space   - autoexpose
Ctrl+Q  - quit
```

## Open-source under MIT License

All dev-kit firmware, dev-kit design files, and Python code is
open-source under a standard MIT License. For example, users are
free to develop their own GUI reusing anything from the
`microspecgui` source code, or develop their own
hardware/firmware reusing any of the schematics, PCB layout, or
firmware provided by Chromation.

## Python API `microspec` is active

Chromation actively maintains `microspec`. We use this library
to create all of our applications, both for internal work and for
external example applications. If you make a modification you
want to share, or find a bug, or if you have a feature request,
please let us know.

## Modifying the communication protocol

If you'd like to reuse `microspeclib` for your own custom
communication protocol, the setup is described below, and is
described in much greater detail in
[PYTHON-SETUP.md](PYTHON-SETUP.md). Extra details on Linux Mint
setup are in PYTHON-SETUP.md in the section [Setup for modifying
microspeclib](PYTHON-SETUP.md#setup-for-modifying-microspeclib).

- clone the `microspec` repository from the project homepage

```bash
$ git clone https://github.com/microspectrometer/microspec.git
```
- create a virtual environment with `venv`
- enter the repository directory and run `pip install -e .`,
  this:
    - runs `setup.py`
    - creates symbolic links in the virtual environment
      installation folder
- later undo the installation with `pip uninstall microspec`, or
  by simply deleting the virtual environment

Install the packages for running the unit tests and rebuilding
documentation:

```bash
$ pip install microspec[dev]
```

Commands defined in the JSON file are automatically picked up by
the `microspec` unit tests to check that the hardware responds
with the expected response. The unit tests figure out the
expected response from the JSON file.

Enter the `microspec` local repository folder, then run the tests
with `pytest`:

```bash
$ pytest
```

### Documentation

The documentation automatically includes some basic information.
But it is still up to the user to add specific docstrings.

Since the command methods are created by meta-class factories,
they are not defined in code, so there are no docstrings to
edit. This is why I ended up creating the `microspec`
module to wrap `microspeclib`.

`src/microspeclib/internal/docstrings.py` is a place to put
documentation that would normally go in the method docstring.

Then enter the `microspec/doc` folder and run the Sphinx
Makefile to generate the new HTML documentation:

```bash
$ cd microspec/doc
$ make clean html
```

I build the documentation on Linux. I cannot get the build to run
on Windows. The `.rst` files use the `autodoc` Sphinx extension,
and something about the complicated path setup prevents `autodoc`
from finding the submodules in the `tests` folder.

[Back to Table of Contents](README.md#table-of-contents)
