# Latest

I started a Python developer guide: see
[GettingStarted.md](GettingStarted.md).

:rainbow: The firmware in this repo works with PyPI project
`microspec`.

# Repository contents

- firmware
- example Python scripts

## Firmware

The `firmware` folder contains the firmware for the Chromation
spectrometer dev-kit and some example Python scripts to get
started.

The Chromation spectrometer is a surface-mount PCB package
consisting of a linear photodiode array and optical components.
This spectrometer chip does not contain any firmware.

The firmware is on the two microcontrollers, one on each of the
stacked PCBs. The microcontroller on the bottom of the stack
provides a SPI interface to the Chromation spectrometer. The
microcontroller on the PCB stacked above provides a USB bridge
that turns the SPI interface into a USB interface.

The spectrometer chip is analog out, but it has programmable
registers that affect pixel size and voltage gain. The dev-kit
writes these registers with Chromation's recommended values when
the dev-kit is powered on. Developers typically do not need to
change these register values, but the Python API does expose the
dev-kit firmware commands that write to these registers, in case
developer's have a special use-case.

## Example Python scripts

The example scripts in the `python` folder communicate with the
firmware. These scripts require installing [PyPI project
microspec](https://pypi.org/project/microspec/).

PyPI project `microspec` contains:

- command-line executables for:
    - common measurement tasks
    - communication tests
- package `microspeclib`
    - low-level API to write Python applications that communicate
      with the dev-kit
    - this is the package required by the example Python scripts

## Install the Python API

To use the API, install with `pip`:

```
$ pip install microspec
```

Then import the API in a Python script or at a Python REPL:

```python
>>> from microspeclib.simple import MicroSpecSimpleInterface
```

To extend/customize/repurpose the API, clone the repository from
the project homepage:
<https://github.com/microspectrometer/microspec>.

If installing from PyPI is not an option:

- clone the repository
- create a virtual environment with `venv`
- enter the repository directory and run `pip install -e .`,
  this:
    - runs `setup.py`
    - creates symbolic links in the virtual environment
      installation folder
- undo the installation with `pip uninstall microspec`, or by
  simply deleting the virtual environment

## Run command line utilities

## Run example scripts
To run the example python scripts in the `dev-kit-2020/python`
folder, simply enter the `python` directory and run the script.

For example, to run `one-command.py`:

```powershell
> python .\one-command.py
```

### Set PYTHONPATH to include example Python scripts

To run the scripts from any directory, add the path to the
`python` folder to your `PYTHONPATH` environment variable.

For example, to run `one-command.py`:

```powershell
> python -m one-command
```

If you're a Mac/Linux user, you know how to edit your PYTHONPATH
environment variable.

If you're a Windows user, add the path to `$env:PYTHONPATH` by
adding this line to the PowerShell `$PROFILE`
(`Microsoft.PowerShell_profile.ps1`):

```powershell
$env:PYTHONPATH += "C:\cygwin64\home\mike\chromation\dev-kit-2020\python;"
```

- *Modify the above to match the location where you cloned this
`dev-kit-2020` repo.*
- *See next section if you need to create a PowerShell `$PROFILE`.*

Reopen PowerShell and check the PYTHONPATH to verify the above
path is listed:

```powershell
> $env:PYTHONPATH.Split(";")
```

*The above command only lists paths explicitly added by editing
environment variable PYTHONPATH.*

Alternatively, the following command lists *all* Python search
paths for module import:

```powershell
> python -c "import sys; print('\n'.join(sys.path))"
```

### Create a PowerShell Profile

If you are familiar with the Linux `.bashrc`, this is a similar
idea. The `.bashrc` runs whenever a bash shell is opened, so it
is a convenient place to edit the PYTHONPATH environment
variable. The user opens a bash shell to run Python, but before
the user can enter any commands, the `.bashrc` runs and
PYTHONPATH is modified.

Similarly, the `$PROFILE` runs when the *PowerShell* is opened.
Windows users run Python from the PowerShell, so the `$PROFILE`
is a convenient place to edit the PYTHONPATH environment
variable.

First create a WindowsPowerShell directory in $HOME:

```powershell
> mkdir C:\Users\{your-username}\Documents\WindowsPowerShell
```

*Enter the above by typing:*

- `mkdir $HOME/D`
- then pressing **Tab** to expand the `Documents` path
- then typing `WindowsPowerShell`

Then create the Profile:

```powershell
> New-Item -Path $PROFILE
```

Add this line (but with your repository path) to your Profile:

```
$env:PYTHONPATH += "C:\cygwin64\home\mike\chromation\dev-kit-2020\python;"
```

The `+=` appends the path to the `python` folder to the
PYTHONPATH. Windows paths use back-slashes. Lists of Windows
paths are separated by semicolons. The trailing semicolon after
`python` is to separate this path from the next path that gets
added to `PYTHONPATH`.

Any time you edit your Profile you can either close and
re-open PowerShell, or simply source the Profile in the current
session:

```
> .$PROFILE
```

## Run system tests

The `system-tests.py` script defines tests. It does not do
anything if run on its own. There are several options to run the
system tests.

My preferred method is to use pytest and testdox:

```powershell
> pip install pytest
> pip install pytest-testdox
> pytest --testdox system-tests.py
```

Of course you can also run the system tests with built-in
`unittest` module:

```powershell
> python -m unittest system-tests.py
```

These system tests are an alternative to reading the
documentation. Every possible command is called, using typical
parameter values for commands that take paramaters. Reading the
`--testdox` test output, it is easy to locate the code in
`system-tests.py` for any test case and use the code there as an
example of how to call that particular command.

## Run `microspec` unit tests

The system tests are not tests of the `microspec` package itself.
If you are planning on altering the API, you'll need to setup for
running the `microspec` unit tests. The unit tests require
`pytest` and `tabulate`.

To install these requirements, simply install `microspec[test]`:

```powershell
> pip install microspec[test]
```

If the `microspec` project is installed, enter directory
`share/microspeclib`.

For example, if your Python installation directory is
`C:\Users\mike\AppData\Roaming\Python\Python38`, then enter
directory:

```powershell
> cd C:\Users\mike\AppData\Roaming\Python\Python38\share\microspeclib
```

Alternatively, if the repository is cloned, simply enter the
directory of your local clone.

For non-Windows users, `microspec` includes an emulator that
requires utility `socat`. Install `socat` to run the unit tests
that use the emulator. The emulator lets you write unit tests
that fake the dev-kit hardware.

Whatever your operating system and however you choose to
install/use the `microspec` project, you are now setup to run
unit tests:

```powershell
> python -m pytest
```

There are two ways to run the unit tests:

1. without the dev-kit connected
2. with the dev-kit connected

First run the tests without the kit connected. This should run
very quickly and all tests should pass.

Tests marked as `xfailed` are passing tests. `xfailed` means
`expected to fail`, and with the hardware not connected, the test
suite know to expect certain tests to fail.

If you are not setup to use the emulator, all of the
emulator-based tests are `skipped`.

Next run the tests with the dev-kit connected. Again, all tests
should pass, but this should take a little longer to run since it
involves actual hardware i/o, and tests that call AutoExposure
and CaptureFrame run for as long as it takes to expose the pixles
for one or more frames. A trick to speed up these i/o calls is to
brightly illuminating the spectrometer while the tests run.

## Firmware and Python API communication protocol

After installing the `microspec` project, the low-level protocol
definitions are in `share/microspeclib/cfg/microspec.json`.

For example, if your Python installation directory is
`C:\Users\mike\AppData\Roaming\Python\Python38`, then the JSON
file located here:

```powershell
> C:\Users\mike\AppData\Roaming\Python\Python38\share\microspeclib\cfg\microspec.json
```

Alternatively, if the repository is cloned, the JSON file is
located here:

```powershell
> microspec\cfg\microspec.json
```

This JSON file drives the entire API. It defines:

- `global` byte values such as `StatusOK` and `StatusError`
- `protocol` byte sequences:
    - `command` the byte sequence for sending each command
    - `bridge` the expected byte sequence for responses from the
      USB Bridge
    - `sensor` the expected byte sequence for responses from the
      SPI-out Sensor Interface

To add new API calls or change existing calls, simply edit this
file, run the tests to make sure they still pass, then rebuild
the documentation.

*Extending the API does not require writing any new Python code.*

See firmware/README-dev.md for more information.


# For Chromation Internal Use Only

Below here needs editing before this goes public.


# Run the example GUI

The `dev-kit-2020` repository includes an example GUI:
`dev-kit-2020/python/kit-gui.py`

The GUI has two dependencies: `pygame` and `pygstuff`. After
installing dependencies (see next section), run the GUI from
PowerShell with this command:

```powershell
> python -m kit-gui
```

## Install `kit-gui` dependencies

### Install `pygame`

Install `pygame` from [PyPI](https://pypi.org/project/pygame/):

```powershell
> python -m pip install pygame
```

### Install `pygstuff`

Install `pygstuff` from [PyPI](https://pypi.org/project/pygstuff/):

```powershell
> python -m pip install pygame
```
Install repository `pygstuff` in `USER_SITE` (see later section
describing `USER_SITE`).

- `pygstuff` stands for `PYGame Stuff`
- `pygstuff` is:
    - a package of helper modules to simplify writing `pygame`
      applications
    - available on GitHub: https://github.com/sustainablelab/pygstuff
    - created and maintained by Mike Gazes
- `pygstuff` uses the standard MIT license

*Chromation does not own `pygstuff` and is not affiliated with it
anyway.*

Clone `pygstuff` in `USER_SITE`:

```bash
$ cd /cygdrive/c/Users/mike/AppData/Roaming/Python/Python38/site-packages/
$ git clone https://github.com/sustainablelab/pygstuff.git
```

# Python API repos

The `microspectrometer` repo contains protocol changes that are not
in Sean's `chromaspec-sean` repo:

<https://rainbots@bitbucket.org/eruciform/chromaspec-sean>

My local copy of the official Python API repo:

- `/home/mike/.local/lib/python3.7/site-packages/microspectrometer/`

My local copy of Sean's repo:

- `/home/mike/.local/lib/python3.7/site-packages/chromaspec-sean/`


# Install Python API package
The Python API package, `microspectrometer`, will be available
soon via `pip install microspectrometer`.

## Future
When [PyPI](https://pypi.org/) is ready, *replace the remainder
of this README with the following line:*

```powershell
> pip install microspectrometer
```

`pip install` takes care of:

- adding **modules** from `microspectrometer` to Python's
  **search path** for `import`
- adding **utilities** from `microspectrometer` to **run from the
  command line** with `python -m`
- installing **dependencies**

## Present

For now, clone repo `microspectrometer` inside Python's
`USER_SITE` folder.

*`USER_SITE` is one of many Python module search paths. It is a
convenient location to place user-defined packages to make them
available for use from any script, without touching the Python
installation's `lib/site-packages/` folder.*

### Create `USER_SITE`

Python searches many paths for module import. One of these paths
is the "user-specific site-packages directory,
[`USER_SITE`](https://docs.python.org/3.8/library/site.html#site.USER_SITE)"
(see [official python.org
documentation](https://docs.python.org/3.8/library/site.html)).

*The user-specific site-packages path is defined by the site
package, even if the path does not exist yet.*

Get the `USER_SITE` path. *From the PowerShell command line:*

```powershell
> python -m site --user-site
C:\Users\mike\AppData\Roaming\Python\Python38\site-packages
```

Create the path if it does not exist:

```powershell
> cd $HOME\AppData\Roaming\
> ls Python
ls : Cannot find path 'C:\Users\mike\AppData\Roaming\Python' because it does not exist.
> mkdir Python\Python38\site-packages
```

### Clone `microspectrometer` in `USER_SITE`
Clone the `microspectrometer` repository in the `USER_SITE`
folder:

```bash
$ cd /cygdrive/c/Users/mike/AppData/Roaming/Python/Python38/site-packages/
$ git clone https://github.com/microspectrometer/microspectrometer.git
```

## Install `microspectrometer` dependencies

- `python -m pip install pyserial`
- `python -m pip install wheel`
- `python -m pip install pytest`
- `python -m pip install psutil`
- `python -m pip install tabulate`
- `python -m pip install sphinx`
- `python -m pip install recommonmark`
- `python -m pip install m2r`
- `python -m pip install sphinxcontrib-argdoc`

## Test the `microspectrometer` package installation
Test the package is available for import by opening a REPL:

```powershell
> python
```

*TODO: rename package from `chromaspeclib` to
`microspectrometer`*

Try to import the package:

```python
>>> import chromaspeclib
>>>
```

No response means `import` worked.

`import` fails if Python cannot find a module named
`chromaspeclib`.

Expect the following error if the package is not installed in
`USER_SITE`:

```python
>>> import chromaspeclib
  File "<stdin>", line 1, in <module>
ModuleNotFoundError: No module named 'chromaspeclib'
```

## Set PYTHONPATH to include microspectrometer utilities
To run the command line utilities in the `microspectrometer`
package, add the path to `$env:PYTHONPATH` by adding this line to
the PowerShell `$PROFILE` (`Microsoft.PowerShell_profile.ps1`):

```powershell
$env:PYTHONPATH += "C:\Users\mike\AppData\Roaming\Python\Python38\site-packages\microspectrometer\src\chromaspeclib;"
```

*Modify the above to match the location of your Python
installation's `USER_SITE` folder. The utilities are in
`src/chromaspeclib/`.*


