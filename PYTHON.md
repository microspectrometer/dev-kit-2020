# Interface dev-kit with microspec

Chromation's *Python project* `microspec` is available from PyPI
(the Python Package Index) at
<https://pypi.org/project/microspec/>.

## Overview of microspec

*Python project* `microspec` provides:

- *Python package* `microspeclib`:
    - a Python API to talk to the dev-kit hardware
- `microspec_cmdline.py`:
    - a simple command line utility for collecting data
    - i.e., use `microspec_cmdline` to take measurements without
      needing to first develop your own Python application

`microspec` also provides developer tools to modify
`microspeclib` to work with your own firmware (instead of
Chromation's dev-kit firmware). See the [project homepage on
GitHub](https://github.com/microspectrometer/microspec) for
details.

## Setup microspeclib

Install `microspec`:

```bash
$ pip install microspec
```

Now you can use *Python package* `microspeclib` in a script or at
the REPL:

```python
from microspeclib.simple import MicroSpecSimpleInterface
```

And you can run the command line utility:

```bash
$ microspec_cmdline.py setexposure cycles=100
$ microspec_cmdline.py captureframe -t 0.2 -r 3 -w 1.5 --csv
```

On Windows, replace `microspec_cmdline.py` with
`microspec_cmdline.exe`.

The API and command line utility are explained more below.

## Use microspeclib

Chromation provides the `microspeclib` package so users can
quickly put together Python applications to control the dev-kit
over USB **without** solving low-level USB hardware issues such
as:

- opening and closing USB communication
- searching the USB ports for the Chromation dev-kit
- checking when bytes are available to read

### A three-line example script

Here is a short example script that:

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

If there is no dev-kit connected with serial number `091103`, the
following exception is raised:

```python
MicroSpecConnectionException: Cannot find CHROMATION device
```


## Command Line API

The `microspec_cmdline.py` executable will run a single command
and print the reply to stdout, optionally in CSV format.

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

In addition to the example scripts in `microspec`, the dev-kit
[firmware
repository](https://github.com/microspectrometer/dev-kit-2020)
includes a simple example Python application using `microspec`
and two example scripts to supplement the `microspec`
documentation.

## Python GUI

*A GUI is necessary for real-time feedback when optically
aligning the spectrometer in a measurement setup.*

Chromation provides `microspecgui`. This is a Python GUI to
replace the legacy LabVIEW GUI. `microspecgui` uses the
`microspeclib` package for communication and SDL interface
`pygame` for display and user control.

Users install the GUI from PyPI with the usual command:

```
$ pip install microspecgui
```

This makes `microspecgui` available from the command line as a
command that starts up the Python GUI:

```
$ microspecgui
```

## Open-source under MIT License

All dev-kit firmware, dev-kit design files, and Python code is
open-source under a standard MIT License. For example, users are
free to develop their own GUI reusing anything from the
`microspecgui` source code, or develop their own
hardware/firmware reusing any of the schematics, PCB layout, or
firmware provided by Chromation.

## Python API `microspeclib` is active

Chromation actively maintains `microspeclib`. We use this library
to create all of our applications, both for internal work and for
external example applications. If you make a modification you
want to share, or find a bug, or if you have a feature request,
please let us know.

## Modifying the Python API

Chromation encourages using the `microspeclib` Python API even
when building your own hardware/firmware.

The `microspeclib` API is easy to customize/extend for users to
create new interfaces. The commands defined in `microspeclib` are
auto-generated from a JSON file. It is not necessary to write new
API code.

*[This single JSON
file](https://github.com/microspectrometer/microspec/blob/master/cfg/microspec.json)
defines the entire protocol:*

- commands understood by the dev-kit firmware
- command parameters expected by the dev-kit firmware
- responses expected from the dev-kit firmware

Whether users modify the dev-kit firmware or replace the dev-kit
hardware entirely, the `microspeclib` is a solid platform for
defining a protocol and auto-generating a Python interface that
follows the protocol.

Users interested in developing with `microspec` at this level
should install in `--editable` mode:

- clone the `microspec` repository from the project homepage
- create a virtual environment with `venv`
- enter the repository directory and run `pip install -e .`,
  this:
    - runs `setup.py`
    - creates symbolic links in the virtual environment
      installation folder
- later undo the installation with `pip uninstall microspec`, or
  by simply deleting the virtual environment

### Example modifying the API

For example, a user might want to modify the autoexpose algorithm
to choose the final exposure time from a list of allowed exposure
times.

Besides editing the firmware to modify the autoexpose behavior,
the user needs a new configuration command to load the list of
allowed exposure times, e.g., `setAutoExposeTimes()`.

Once this new command and its expected response are defined in
the JSON file (and the firmware is reprogrammed to respond to
this command), *the command is ready-to-use in applications*.
The `setAutoExposeTimes()` method exists without editing any
Python code in `microspeclib`!

Of course it is also good practice to unit test and document. See
the `microspec` project homepage for details. Here is a quick
overview of unit testing and documentation.

### Unit testing

Commands defined in the JSON file are automatically picked up by
the `microspec` unit tests to check that the hardware responds
with the expected response (the unit tests figure out the
expected response from the JSON file).

It is still up to the user to run the unit tests.

1. Install the required packages for running the unit tests:

```bash
$ pip install microspec[test]
```

2. Enter the `microspec` local repository folder, then run the
   tests with `pytest`:

```bash
$ python -m pytest
```

### Documentation

The documentation automatically includes some basic information.
But it is still up to the user to add specific docstrings and to
rebuild the Sphinx documentation to update the HTML version of
the documentation.

1. Install the required packages for building the documentation:

```bash
$ pip install microspec[dev]
```

2. Edit the docstring.

Documentation is the one case where the developer needs to
manually edit a Python file when adding new API commands.

Since the command methods are created by meta-class factories,
they are not defined in code, so there are no docstrings to
edit.

`src/microspeclib/internal/docstrings.py` is where the developer
places the documentation that would normally go in the docstring.

3. Enter the `microspec/doc` folder, then run the Sphinx Makefile:

```bash
$ make clean html
```


