#!/usr/bin/env python3
# -*- coding: utf-8 -*-
r"""Example using Sean's API

USAGE

    Run the script from Vim: `;sw` ([s]ource using [w]indows executable)
    This does :!python.exe ./%

    From PowerShell:
    PS> cd $dev_kit\python
    PS> python.exe .\use-api-example.py

    TLDR: alter the Windows PYTHONPATH and invoke python as `python.exe` to use
    the Windows Python3.7 installation. Do not alter PYTHONPATH via the
    `.bashrc`. Doing so will not impact running from PowerShell, but it will
    prevent running from Vim by messing up the sys.path entry for
    `chromaspeclib`.

PYTHON

    Note that `python.exe` runs the Windows Python37:
        /cygdrive/c/Python37/python.exe
    It does *not* run the Cygwin install of Python3.
    I cannot use the Cygwin or Vim Python3 because installations
    because Cygwin cannot see my USB ports.

REPO

    Throw-away copy of Sean's repo is here:

    /home/Mike/.local/lib/python3.6/site-packages/chromaspec-sean/

MODULE INSTALLATION

    Manually add the path to `chromaspec-sean/src/` to the PYTHONPATH.

    To run script from bash (on Mac/Linux, not Cygwin), edit the `~/.bashrc`

    ```.bashrc
    export PYTHONPATH=$PYTHONPATH:~/.local/lib/python3.6/site-packages/chromaspec-sean/src/
    ```

    Note that Cygwin cannot see USB ports. There is no reason to run this script
    using the Cygwin Python installation. Cygwin can call the Windows
    installation of Python (like it calls any other Windows executable).

    To run this script with the Windows Python installation, edit the PowerShell
    Profile:
    ```powershell
    $py3_user_site_packages = "C:\Users\Mike\AppData\Roaming\Python\Python37\site-packages"
    $chromaspeclib_path = "$py3_user_site_packages\chromaspec-sean\src"
    $env:PYTHONPATH = "$env:PYTHONPATH;$chromaspeclib_path"
    ```

    Check the PYTHONPATH:

    python -c "import sys; print('\n'.join(sys.path))"

    Running the above command from Cygwin, specify `python3` for the Cygwin
    Python3.6 or specify `python.exe` for the Windows Python3.7.

    Do not change PYTHONPATH for both environments. Doing so has no effect on
    Windows, but it messes up sys.path when calling Windows Python3.7 from
    Cygwin.

DETAILS: NO PIP YET

    `chromaspeclib` is not yet ready for `pip install`.
    I usually put these kinds of Python modules in my local site packages
    folder. This avoids editing the PYTHONPATH.

DETAILS: PYTHONPATH

    Module `chromaspeclib` is in `chromaspec-sean/src/`

    But `chromaspeclib` depends on the sibling folders `bin` and `cfg`, so these
    folders must be present. *Why can't every folder be in `chromaspeclib`?*

    While placing `chromaspec-sean` in the local site-packages folder does put
    `chromaspeclib` on the PYTHONPATH, because of this folder structure, the
    `json` dependency in the `cfg` folder is not visible.

    3.6.9 (default, Jul 21 2019, 14:33:59) 
    [GCC 7.4.0]

DETAILS: LOCAL SITE PACKAGES

    Find the local site packages destination in bash or Powershell this way:

    ```bash
    $ python3 -m site --user-site
    /home/Mike/.local/lib/python3.6/site-packages
    ```

    ```powershell
    PS> python -m site --user-site
    C:\Users\Mike\AppData\Roaming\Python\Python37\site-packages
    ```

    These folders do not exist. Create them the first time placing a module in
    the local site-packages.
"""

from chromaspeclib.internal.logger import debug, quiet
from chromaspeclib.simple import ChromaSpecSimpleInterface
# debug() to debug api, debug(True) to also debug all underlying stuff
# quiet() to shut it up again, quiet(True) to also shut up the internals
#xxx = ChromaSpecSimpleInterface( timeout=0.01, retry_timeout=0.001 )
xxx = ChromaSpecSimpleInterface()
print(xxx.getBridgeLED(led_num=0))
print(xxx.setBridgeLED(led_num=0, led_setting=1))
print(xxx.getBridgeLED(led_num=0))
print(xxx.setBridgeLED(led_num=0, led_setting=2))
print(xxx.getBridgeLED(led_num=0))

#xxx.flush()

if __name__ == '__main__':
    print("hi")

