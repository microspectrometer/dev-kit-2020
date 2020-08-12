# Context

## Python

Python is a high-level language with many **built-in** libraries.

Python has a *huge* open-source community that contributes
**additional** libraries.

For example, Chromation maintains `microspec`, an API for USB
communication with our spectrometer dev-kit.

Installing Python consists of installing Python itself, then
installing additional libraries your application depends on.

## Python Packages

Python libraries are called *packages*. Community-created
packages are distributed as *projects* on PyPI (the *Python
Package Index*).

See the [official terminology
here](https://packaging.python.org/glossary/).

I use the terms *project* and *package* interchangeably. In
practice, a *project* contains the *package* and the `setup.py`
file for installing the *package*. But a *project* may also
include applications, data, or other resources.

For example, installing the `microspec` *project* installs the
`microspeclib` *package*. Installing `microspec` also installs:

- API config file `microspec.json`
- command line utility `microspec_cmdline`
- a suite of unit tests
- example applications `microspec_api.py` and
  `microspec_lowlevel_api.py`

## Manage packages and virtual environments

The package manager `pip` and virtual environment manager `venv`
are the common tools for Python developers to use PyPI libraries in their projects.

These tools and concepts are introduced below. Example commands
are given in the `Windows` and `Linux` sections.

### pip

`pip` is the built-in *package manager*. It is Python's tool for
installing/updating/removing packages.

### venv

`venv` manages virtual environments.

I include `venv` in the installation steps because it is good
practice. But you do not need to use `venv`. If you do not have a
reason to use `venv`, do not use it. Skip steps where I create
and activate the `venv`.

Ignoring these `venv` steps means `pip` installs to the base
installation. That is OK. You will know when you run into a
problem that virtual environments solve.

#### venv details

`venv` is the built-in *virtual environment manager*.

A virtual environment is an isolated sandbox for
installing/updating/removing packages without affecting the base
Python installation or other virtual environments.

In other words, a *single Python* is installed but the *list of
installed packages* depend on which virtual environment is
active.

Note: the PYTHONPATH environment variable determines where Python
looks for *import packages* (i.e., to import code from installed
packages), but PYTHONPATH is not connected to any single virtual
environment. PYTHONPATH is the same value for all virtual
environments.

Users typically create a virtual environment for each project.
The downside to this is that all of the duplicate package
installations quickly add up as wasted disk space.

I use `venv` to create a throwaway virtual environment to test
installing a new package. I delete the virtual environment when
I'm done testing.

The flow for virtual environments is simple:

- *Create* a virtual environment.
    - This creates a folder with the virtual environment name.
- *Activate* the virtual environment to limit Python to the
  packages installed in that virtual environment.
    - The command line prompt is prefaced with the virtual
      environment name.
- *Deactivate* the virtual environment to return to the base
  installation's packages.
    - The command line prompt returns to normal.
- *Delete* the virtual environment by deleting its folder.

Some users do not install any packages in their base Python
installation folder and prefer to work exclusively with virtual
environments. But there is nothing wrong with installing packages
to the base Python installation.

A common use case for virtual environments is when package
`A` depends on a specific version of package `B` because updating
package `B` to the latest version breaks package `A`.

In this example, create a virtual environment for working on the
projects that use package `A`. In this virtual environment, the
old version of package `B` is installed, while the base Python
installation can safely use the latest and greatest version of
package `B`.

When the virtual environment is active, Python sees the older
package `B`. When the virtual environment is deactivated, Python sees the
new package `B`.

# Install Python and microspec on Windows

Download and install Python, test the installation, upgrade pip, and install
`microspec` to a virtual environment.

## Download

- Go to the latest official [Python download page for Windows](https://www.python.org/downloads/windows/)
- Download the executable installer for Windows
    - example for my Windows system:

    ```
    python-3.8.1-amd64.exe
    ```

## Install

- Run the installer
- At the final step of installation, customize to add `py`
  launcher:
    - Select option to **Add Python to environment variables**

## Check Python is installed

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

## Upgrade pip

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

## Install `microspec`

Create a virtual environment, activate the virtual environment,
install microspec with pip, then deactivate the virtual
environment.

### Create a virtual environment named `test`

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

### Install `microspec`

First note that there are no additional packages installed yet:

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

Install `microspec`:

```powershell
(test) PS C:\Users\mike> pip install microspec
Collecting microspec
  Using cached https://files.pythonhosted.org/packages/7e/7a/17ec4288fb192ef2d9cfe25c11f64744d7e2415c5b717bbb1f19b202f2d7/microspec-0.1.1a3-py3-none-any.whl
Collecting pyserial (from microspec)
  Using cached https://files.pythonhosted.org/packages/0d/e4/2a744dd9e3be04a0c0907414e2a01a7c88bb3915cbe3c8cc06e209f59c30/pyserial-3.4-py2.py3-none-any.whl
Installing collected packages: pyserial, microspec
Successfully installed microspec-0.1.1a3 pyserial-3.4
```

## Try `microsepec` at the REPL

Open the Python REPL:

```powershell
(test) PS C:\Users\mike> python
Python 3.8.1 (tags/v3.8.1:1b293b6, Dec 18 2019, 23:11:46) [MSC v.1916 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>>
```

A quick test of the API:

```python
>>> from microspeclib.simple import MicroSpecSimpleInterface
>>> kit = MicroSpecSimpleInterface(timeout=2.0)
```

There is no response to the above command when everything is
working.

If the above command results in the error:

```python
    raise MicroSpecConnectionException("Cannot find CHROMATION device")
microspeclib.exceptions.MicroSpecConnectionException: Cannot find CHROMATION device
```

Check:

- is the dev-kit connected over USB?
- are the indicator LEDs lit?
- did you [enable "Load VCP"](DEV-KIT.md#windows-users-enable-load-vcp)?

Continue a quick test of the API if there was no error. Get the
pixel configuration of the spectrometer's photodiode array:

```python
>>> print(kit.getSensorConfig())
SensorGetSensorConfig(status=0, binning=1, gain=1, row_bitmap=31)
```

There is no command to close communication. The serial connection
will automatically close when you exit the REPL.

# Install Python on Linux Mint

Download and install Python, test the installation, upgrade pip,
and install `microspec` to a virtual environment.

