Lots of things were against us today, but I sorted it all out:

Bottom line is I'm able to run the simple example script you
wrote using my Windows installation of Python3.7. Yay!

I get around the USB port problem in Cygwin by telling Cygwin to
use the Windows installation of Python3.7. This is called
`python.exe` so I run the script with `python.exe` (Windows
install) instead of `python3` (Cygwin install).

The work you did on my PYTHONPATH was still helpful to guide me
to the solution:

1) Alter the Windows PYTHONPATH and invoke python as `python.exe`
to use the Windows Python3.7 installation.

2) Do not append to PYTHONPATH in the `.bashrc`. This writes to
the same `PYTHONPATH` environment variable Windows uses. The
problem is Windows uses a `;` separator and `bash` uses a `:`
separator between paths. So appending `:whatever` to PYTHONPATH
ends up garbling the Windows-flavored path, meaning that when I
call `python.exe` from Cygwin, the path to `chromaspeclib` is
messed up.

I figured out why we could not alter `sys.path` when we initially
added `PYTHONPATH` to the `.bashrc`: there was a newline after
the word `export` in the `.bashrc`! I got rid of the newline and all was good.

The newline was probably inserted because I configured Vim to
auto line-wrap. :)

I figured out why I couldn't open communication with my dev kit
from PowerShell:

- my kit did not have `Load VCP` enabled!
- you'll run into this to on Windows
- to enable `Load VCP`
    - Windows: Device Manager:
        - right-click on USB Serial Converter
        - select Properties
        - go to tab "Advanced"
        - check "Load VCP"
