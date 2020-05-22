The firmware in this repo is the new protocol. It works with the
Python low-level API in private GitHub repo:

<https://github.com/microspectrometer/microspectrometer>

# Install Python API package
This will be available soon via `pip install microspectrometer`.

For now, clone this repo inside Python's `USERSITE` folder. Find
this folder from PowerShell:

```powershell
> python -m site --user-site
C:\Users\mike\AppData\Roaming\Python\Python38\site-packages
```

# Set PATH to example Python scripts
To run the example python scripts in the `python` folder, add the
path to `$env:PYTHONPATH` by adding this line to the PowerShell
`$PROFILE` (`Microsoft.PowerShell_profile.ps1`):

```powershell
$env:PYTHONPATH += "C:\cygwin64\home\mike\chromation\dev-kit-2020\python;"
```

*Modify the above to match the location where you cloned this
`dev-kit-2020` repo.*

Run the example GUI:

```powershell
> python -m kit-gui
```

# Firmware old and new repos

Old firmware is in private GitHub repo `dev-kit-2019`:

<https://github.com/sustainablelab/dev-kit-2019>

My local copy of the two firmware repos:

- `~/chromation/dev-kit-2019/` (old)
- `~/chromation/dev-kit-2020/` (new)

# Firmware and Python API communication protocol

The low-level protocol definitions are in `cfg/chromaspec.json`.

See firmware/README-firmware.md for more information.

# Python API repos

The `microspectrometer` repo contains protocol changes that are not
in Sean's `chromaspec-sean` repo:

<https://rainbots@bitbucket.org/eruciform/chromaspec-sean>

My local copy of the official Python API repo:

- `/home/mike/.local/lib/python3.7/site-packages/microspectrometer/`

My local copy of Sean's repo:

- `/home/mike/.local/lib/python3.7/site-packages/chromaspec-sean/`

