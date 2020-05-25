## For Chromation Internal Use Only

This is for internal use only. Edit before making public.

# Contents
:rainbow: The firmware in this repo is the **new protocol**. It
works with Python package `microspectrometer`, Chromation's
low-level API for communicating with the dev-kit.

For now, the Python package is only available from the private
GitHub repo:

<https://github.com/microspectrometer/microspectrometer>

# Firmware old and new repos

Old firmware is in private GitHub repo `dev-kit-2019`:

<https://github.com/sustainablelab/dev-kit-2019>

My local copy of the two firmware repos:

- `~/chromation/dev-kit-2019/` (old)
- `~/chromation/dev-kit-2020/` (new)

# Firmware and Python API communication protocol

The low-level protocol definitions are in
`microspectrometer/cfg/chromaspec.json`.

See firmware/README-dev.md for more information.

# Set PYTHONPATH to include example Python scripts
To run the example python scripts in the `dev-kit-2020/python`
folder, add the path to `$env:PYTHONPATH` by adding this line to
the PowerShell `$PROFILE` (`Microsoft.PowerShell_profile.ps1`):

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

## Create a PowerShell Profile

If you are familiar with the Linux `.bashrc`, this is a similar
idea.

The `$PROFILE` runs when the PowerShell is opened. It is a
convenient way to edit the Python PATH environment variable for a
use-case like this where the goal is to run Python scripts from
the PowerShell command line.

Create a WindowsPowerShell directory in $HOME:

```powershell
> mkdir C:\Users\{your-username}\Documents\WindowsPowerShell
```

*Enter the above by typing:*

- `mkdir $HOME/D`
- then pressing **Tab** to expand the `Documents` path
- then typing `WindowsPowerShell`

Create the Profile:

```powershell
> New-Item -Path $PROFILE
```

# Run the example GUI

The `dev-kit-2020` repository includes an example GUI:
`dev-kit-2020/python/kit-gui.py`

The GUI has two dependencies: `pygame` and `pygs`. After
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

### Install `pygs`

Install repository `pygs` in `USER_SITE` (see later section
describing `USER_SITE`).

- `pygs` stands for `PYGame Stuff`
- `pygs` is:
    - a package of helper modules to simplify writing `pygame`
      applications
    - available on GitHub: https://github.com/sustainablelab/pygs
    - created and maintained by Mike Gazes
- `pygs` uses the standard MIT license

*Chromation does not own `pygs` and is not affiliated with it
anyway.*

Clone `pygs` in `USER_SITE`:

```bash
$ cd /cygdrive/c/Users/mike/AppData/Roaming/Python/Python38/site-packages/
$ git clone https://github.com/sustainablelab/pygs.git
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


