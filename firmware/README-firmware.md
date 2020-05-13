# User's Guide -- setup Python API
Sean is working on a `pip install` installation method. For now,
here are the install steps without `pip install`.

## Install Python3.x
Install Python3.7 or later.

## Clone `chromaspec-sean`
Clone `chromaspec-sean` in the `USERSITE` folder.

### Find `USERSITE` and make it if it does not exist
`pip install` sets up a package in the site-packages folder where
Python looks for it. To use a package without `pip install`, I
use the `USERSITE` folder.

Open PowerShell and check the value of USERSITE:

```powershell
> python -m site --user-site
C:\Users\mike\AppData\Roaming\Python\Python38\site-packages
```

The repo does not have to go in USERSITE, but it is a good
default choice.

I do not install the repo here. For code-editing reasons, I
prefer to install to the USERSITE for my Cygwin Python
installation:

```bash
$ python3.7 -m site --user-site
/home/mike/.local/lib/python3.7/site-packages
```

I use Cygwin, but I run the Python applications using my Windows
Python installation. The Windows Python installation is executed
from Cygwin by specifying it at the bash command line as
`python.exe`.

I can call `python.exe` from Windows or from Cygwin. It does not
matter in this case. There are cases where attempting to run
`python.exe` from Cygwin fails because of incompatiblity between
Windows and POSIX path formatting.

I will continue to show examples from my actual setup below,
where the `USERSITE` is in my Cygwin directory. The examples
should work the same with any `USERSITE` path.

Create the `USERSITE` folder if it does not exist:

```powershell
> mkdir -p C:\Users\mike\AppData\Roaming\Python\Python38\site-packages
```

Again, in my case I created this USERSITE folder instead:

```bash
$ mkdir -p /home/mike/.local/lib/python3.7/site-packages
```

### Clone chromaspec-sean in USERSITE
Clone the repo there:

```bash
$ cd /home/mike/.local/lib/python3.7/site-packages
$ git clone https://rainbots@bitbucket.org/eruciform/chromaspec-sean.git
```

### Edit the `PYTHONPATH`
When looking for packages to import, Python uses the PYTHONPATH
environment variable *if it exists*.

Since I am using the Windows Python installation, I create the
PYTHONPATH environment variable using Windows conventions:

- backslashes
- semicolons separate paths

I add the following to my PowerShell profile:

```powershell
$pkg = "C:\cygwin64\home\mike\.local\lib\python3.7\site-packages"
$env:PYTHONPATH = "$pkg;"
$temp_sean = "chromaspec-sean\src"
$env:PYTHONPATH += "$pkg\$temp_sean;"
```

The trick here is that PowerShell runs the profile every time it
starts. The profile creates the PYTHONPATH environment variable.

Check the `PYTHONPATH` in PowerShell:

```powershell
> $env:PYTHONPATH.Split(";")
C:\cygwin64\home\mike\.local\lib\python3.7\site-packages
C:\cygwin64\home\mike\.local\lib\python3.7\site-packages\chromaspec-sean\src
```

Packages in USERSITE import just fine without explicitly adding
them to the PYTHONPATH. The issue with `chromaspec-sean` is that
the packages are not at the top-level.

`chromaspec-sean` contains more than just Python source code. The
actual package for import is in the `src` folder. The other
top-level folders contain executables and other things that Sean
created. This is all temporary anyway, so just deal with the
explicit path for now.

## Install Python package dependencies

- `python -m pip install pyserial`
- `python -m pip install pytest`
- `python -m pip install psutil`
- `python -m pip install tabulate`
- `python -m pip install sphinx`
- `python -m pip install wheel` (I added this to Sean's list)
- `python -m pip install recommonmark`
- `python -m pip install m2r`
- `python -m pip install sphinxcontrib-argdoc`

## Check the installation

```powershell
> cd $pkg\chromaspec-sean\
> pytest
======================================================================== test session starts =========================================================================
platform win32 -- Python 3.8.1, pytest-5.3.5, py-1.8.1, pluggy-0.13.1
rootdir: C:\cygwin64\home\mike\.local\lib\python3.7\site-packages\chromaspec-sean
plugins: testdox-1.2.1
collected 147 items

tests\test_bytesio_stream.py ..................                                                                                                                 [ 12%]
tests\test_emulated_stream.py ..................ssssssssssssssssssss                                                                                            [ 38%]
tests\test_emulator.py ..........                                                                                                                               [ 44%]
tests\test_expert_interface_emulator.py ssssssssss                                                                                                              [ 51%]
tests\test_expert_interface_hardware.py xxxxxxxxxx                                                                                                              [ 58%]
tests\test_json.py ....                                                                                                                                         [ 61%]
tests\test_payload.py ..................................                                                                                                        [ 84%]
tests\test_simple_interface_emulator.py ssssssssss                                                                                                              [ 91%]
tests\test_simple_interface_hardware.py xxxxxxxxxx                                                                                                              [ 97%]
tests\test_util.py ...                                                                                                                                          [100%]

============================================================= 87 passed, 40 skipped, 20 xfailed in 1.24s =============================================================
```

Expect the same result (87 passed, 40 skipped, 20 xfailed) when
running from Cygwin bash. I think the 20 failed tests are because
I do not have a dev-kit with working firmware connected. I'd try
connecting it now, but as of May 2020, I haven't written the
firmware yet.

## Documentation

Read with `pydoc`:

```powershell
> python -m pydoc chromaspeclib
> python -m pydoc chromaspeclib.simple
> python -m pydoc chromaspeclib.expert
```

Read the expert interface:

```powershell
> python -m pydoc chromaspeclib.expert.ChromaSpecExpertInterface
Help on class ChromaSpecExpertInterface in chromaspeclib.expert:
```

Or read the HTML:

```powershell
> cd $pkg\chromaspec-sean\doc\build\html\
> explorer index.html
```

## Load VCP

- On Windows, sometimes VCP is not set. Go to Device Manager:
    - Right-click on USB Serial Converter
    - Select Properties
    - Go to tab "Advanced"
    - Check "Load VCP"

I tried to automate this in the FTDI EEPROM.

- under `Hardware Specific`, `Port A`, `Driver`
- change `D2XX Direct` to `Virtual COM Port`

This did not work. The device is still not visible to pyserial.

## Quick script to test VCP

Make a quick test script to check that the hardware is visible:

`test-hardware-is-visible.py`:

```python
'''Test hardware is visible.'''

from chromaspeclib.simple import ChromaSpecSimpleInterface
si = ChromaSpecSimpleInterface(timeout=0.1)
print(si)
```

```powershell
> python test-hardware-is-visible.py
<chromaspeclib.simple.ChromaSpecSimpleInterface object at 0x000001B5F0AB42B0>
```

If the dev-kit is not connected or VCP is not enabled, this test
produces error `Cannot find CHROMATION device`:

```powershell
> python test-hardware-is-visible.py
2020-04-28 22:20:37,772:stream.py:__init__:193: Cannot find CHROMATION device
Traceback (most recent call last):
  File "C:\cygwin64\home\mike\chromation\dev-kit-mike\firmware\python\test.py", line 6, in <module>
    si = ChromaSpecSimpleInterface(timeout=0.1)
  File "C:\cygwin64\home\mike\.local\lib\python3.7\site-packages\chromaspec-sean\src\chromaspeclib\expert.py", line 24, in __init__
    super().__init__(serial_number=serial_number,
  File "C:\cygwin64\home\mike\.local\lib\python3.7\site-packages\chromaspec-sean\src\chromaspeclib\internal\stream.py", line 194, in __init__
    raise ChromaSpecConnectionException("Cannot find CHROMATION device")
chromaspeclib.exceptions.ChromaSpecConnectionException: Cannot find CHROMATION device
```

# User's Guide -- use Python API
As of May 2020: I've just started using the Python API. Sean's
`cmdline.py` utility lets me send any single API command and look
at the response. This is very convenient. I am just doing quick
system tests, so I have no reason to write my own scripts at this
point. The `cmdline.py` utility does everything I need.

See section `system test from the command line` below.

# Developer's Guide -- feedback on Sean's API

## Notes on these notes
From hereon, I refer to paths in Sean's repo with `.` meaning the
repo name.

Capitalization does not matter, but I capitalize (contrary to the
documentation examples) to match with `./cfg/chromaspec.json`.
Sean asked for feedback on the API in the chromaspeclib package
and the supporting utilities and documentation. I am using the
API to write system tests as I develop the firmware.

## I dogfood Sean's API by doing system tests

I am using the command line utilities as a convenience for quick
system testing, i.e., testing that is not code, just me trying
something at the command line to check that firmware changes have
the expected result.

I system test as I code the firmware to point me in the right
direction *before* I spend time programming system tests.

# Developer's Guide -- system test from the command line

```bash
python.exe -m cmdline COMMAND KEY=VALUE
```

I made Vim shortcuts start with `pt` (pneumonic is `p`ython `t`est):

```vim
nnoremap <leader>pt<Space> :!python.exe -m cmdline 
nnoremap <leader>pt0 :!python.exe -m cmdline Null
nnoremap <leader>pt1 :!python.exe -m cmdline GetBridgeLED led_num=0
nnoremap <leader>pt2 :!python.exe -m cmdline SetBridgeLED led_num=255 led_setting=255
```

## System test examples from the command line
My examples work from PowerShell and Cygwin bash.

### Finding bugs on 2020-04-28

This opens communication:

```python
si = ChromaSpecSimpleInterface()
```

But this throws an error:

```python
si = ChromaSpecSimpleInterface(serial_number='CHROMATION091103')
```

Error traces back to line 177 of internal/stream.py:

```python
self.serial.port = list_ports.grep(serial_number).device
```

Full error message:

```
  File "<stdin>", line 1, in <module>
  File "C:\cygwin64\home\mike\.local\lib\python3.7\site-packages\chromaspec-sean\src\chromaspeclib\expert.py", line 24, in __init__
    super().__init__(serial_number=serial_number,
  File "C:\cygwin64\home\mike\.local\lib\python3.7\site-packages\chromaspec-sean\src\chromaspeclib\internal\stream.py", line 177, in __init__
    self.serial.port = list_ports.grep(serial_number).device
AttributeError: 'generator' object has no attribute 'device'

'generator' object has no attribute 'device'
```

Did Sean mean this?

```python
from serial.tools import list_ports
next(list_ports.grep('CHROMATION091103')).device
```

Or this?

```python
from serial.tools import list_ports
list(list_ports.grep('CHROMATION091103'))[0].device
```

### Hello world between API and `usb-bridge`

Here is the loop on `usb-bridge`:

```c
void loop(void)
{
    while (UsbRxbufferIsEmpty()); // loop until cmd received
    uint8_t cmd = 0; // initialize cmd as "Null"
    UsbReadByte(&cmd); // read cmd
    switch(cmd) // look up cmd
    {
        case 0: NullCommand(); BiColorLedGreen(status_led); break;
        default: BiColorLedRed(status_led); break;  // sbi	0x08, 3
    }
}
```

Here is the command line:

```powershell
PS C:\Users\mike> python -m cmdline Null
2020-04-29T18:34:46.495822,BridgeNull()

PS C:\Users\mike> python -m cmdline GetBridgeLed led_num=1
2020-04-29T18:34:57.172661,None

PS C:\Users\mike> python -m cmdline Null
2020-04-29T18:35:01.499843,BridgeNull()
```

Behavior is good:

- **PASS**: the PCB powers up with its LED green
- **PASS**: the first Null has no obvious effect
- the second command, GetBridgeLED led_num=1, sends bytes 0x01
  0x01
- this simplistic loop() should see these as two commands with
  byte value 0x01
- so both 0x01 *should go to the default case: turn the LED red*
- **PASS**: the LED turns red
- *sending the final Null should turn the LED back to green*
- **PASS**: the LED turns green

If I have no better use for the LED, it can indicate whether the
most recent command was recognized.

### system test GetBridgeLED

GetBridgeLED gets `led_setting=1` and `led_setting=2`

- `led_setting=1` means *Status LED is GREEN*
- `led_setting=2` means *Status LED is RED*

Here is the `main()` loop when I ran this system test:

```c
void loop(void)
{
    while (UsbRxbufferIsEmpty()); // loop until cmd received
    uint8_t cmd = 0; // initialize cmd as "Null"
    UsbReadByte(&cmd); // read cmd
    switch(cmd) // look up cmd
    {
        // Turn status LED red if cmd is not recognized.
        default:                    BiColorLedRed(status_led);   break;
        // Null gets no response, but turns status LED green for now.
        case 0: NullCommand();      BiColorLedGreen(status_led); break;
        case 1: GetBridgeLED(); break;
    }
}
```

And here is the system test.

START: power-on, *Status LED is GREEN.*

Send `Null` just for sake of it:

```bash
$ python.exe -m cmdline Null
2020-05-03T19:10:51.044753,BridgeNull()
```

*Status LED is GREEN.*

Send `GetBridgeLED`. Should return `led_setting=1`.

```bash
$ python.exe -m cmdline GetBridgeLed led_num=0
2020-05-03T19:10:56.366519,BridgeGetBridgeLED(status=0, led_setting=1)
```

**PASS**:
    - Response is `led_setting=1`
    - *Status LED is GREEN*


Send `SetBridgeLED led_num=255 led_setting=255`.

This sends bytes `0x02`, `0xFF`, `0xFF`. None of these bytes are
recognized, so:

- expect response is `None` and Status LED turns RED:
    - `None` means the firmware does not send a response back
    - expect `None` because I did not program a response to
      `SetBridgeLED` yet
    - all three bytes are handled as the `default` case of the
      `switch` block
    - the `default` case turns Status LED RED

```bash
$ python.exe -m cmdline SetBridgeLED led_num=255 led_setting=255
2020-05-03T19:11:05.782671,None
```

- **PASS**:
    - Response is `None`
    - *Status LED is RED*


Send `GetBridgeLED`. Should return `led_setting=2`.

```bash
$ python.exe -m cmdline GetBridgeLed led_num=0
2020-05-03T19:11:10.128380,BridgeGetBridgeLED(status=0, led_setting=2)
```

**PASS**:
    - Response is `led_setting=2`
    - *Status LED is RED*

Send `Null` to reset LED to GREEN:

```bash
$ python.exe -m cmdline Null
2020-05-03T19:11:16.836114,BridgeNull()
```

*Status LED is GREEN.*

Send `GetBridgeLED`. Should return `led_setting=1` again.

```bash
$ python.exe -m cmdline GetBridgeLed led_num=0
2020-05-03T19:11:19.115907,BridgeGetBridgeLED(status=0, led_setting=1)
```

**PASS**:
    - Response is `led_setting=1`
    - *Status LED is GREEN*

### system test SetBridgeLED

Here is the `main()` loop when I ran this system test:

```c
void loop(void)
{
    while (UsbRxbufferIsEmpty()); // loop until cmd received
    uint8_t cmd = 0; // initialize cmd as "Null"
    UsbReadByte(&cmd); // read cmd
    switch(cmd) // look up cmd
    {
        // Turn status LED red if cmd is not recognized.
        default:                    BiColorLedRed(status_led);   break;
        // Null gets no response, but turns status LED green for now.
        case 0: NullCommand();      BiColorLedGreen(status_led); break;
        case 1: GetBridgeLED(); break;
        case 2: SetBridgeLED(); break;
        //  case 8: SetSensorConfig(); break;
        // default: ReplyCommandInvalid(); break;
    }
}
```

Send command to turn off the status LED:

```bash
$ python.exe -m cmdline SetBridgeLED led_num=0 led_setting=0
2020-05-03T21:14:01.364061,BridgeSetBridgeLED(status=0)
```

**PASS**: status LED turns off

Check GetBridgeLED returns `led_setting=0`:

```bash
$ python.exe -m cmdline GetBridgeLed led_num=0
2020-05-03T21:14:05.208334,BridgeGetBridgeLED(status=0, led_setting=0)
```

Send command to turn the status LED green:

```bash
$ python.exe -m cmdline SetBridgeLED led_num=0 led_setting=1
```

**PASS**: status LED turns green

Check GetBridgeLED returns `led_setting=1`:

```bash
$ python.exe -m cmdline GetBridgeLed led_num=0
2020-05-03T21:14:14.974757,BridgeGetBridgeLED(status=0, led_setting=1)
```

Send command to turn the status LED red:

```bash
$ python.exe -m cmdline SetBridgeLED led_num=0 led_setting=2
2020-05-03T21:14:18.531435,BridgeSetBridgeLED(status=0)
```

**PASS**: status LED turns red

Check GetBridgeLED returns `led_setting=2`:

```bash
$ python.exe -m cmdline GetBridgeLed led_num=0
2020-05-03T21:14:21.380145,BridgeGetBridgeLED(status=0, led_setting=2)
```

### system test `GetSensorLED`

Both LEDs on sensor board are green. Expect `led_setting=1` for
both.

```bash
$ python.exe -m cmdline GetSensorLED led_num=0
2020-05-07T06:02:50.588743,SensorGetSensorLED(status=0, led_setting=1)
```

```bash
$ python.exe -m cmdline GetSensorLED led_num=1
2020-05-07T06:04:34.546605,SensorGetSensorLED(status=0, led_setting=1)
```

Sensor board only has led 0 and 1. Expect `status=1` (1 is ERROR)
and `led_setting=255` (255 is PADDING) if I pass `led_num=2`.

```bash
$ python.exe -m cmdline GetSensorLED led_num=2
2020-05-07T06:06:24.061009,SensorGetSensorLED(status=1, led_setting=255)
```

## TODO list for Sean
- [ ] TODO: Sean add documentation for the `serial_number` and
  `device` parameters of `ChromaSpecSimpleInterface`
    - [x] Sean is aware
    - document input is a string
    - document an example string: `CHROMATION091103`
- [ ] TODO: Sean add list of commands in documentation or, better
  still, in the help for `chromaspec_cmdline.py`
    - [x] Sean is aware
    - this is actually just a wrapper for python to execute
      `cmdline.py` as a module
    - cd into `chromaspec-sean/src/chromaspeclib/`
    - execute as `python -m cmdline.py command key=val`
    - `python cmdline.py` also works
    - `python cmdline.py GetBridgeLED led_num=0`
- [ ] TODO: Sean add documentation explaining how to read the
  response
    - [x] Sean is aware
    - nothing in the Sphinx HTML or the pydoc docstrings explains
      what the responses are
    - `python cmdline.py GetBridgeLED led_num=0`
    - has this response:
    - `2020-04-29T19:02:37.952994,None` (from Cygwin bash)
    - `2020-04-29T14:02:15.856620,None` (from PowerShell)
    - the correct time is 2:02PM
    - I'm not sure why running from Cygwin bash adds five hours
      to the time -- the `date` utility works just fine and
      prints the correct time, and I'm invoking the Windows
      Python from Cygwin bash, so what gives?
- [ ] TODO: Sean: unexpected end of docstring in
  ChromaSpecSimpleInterface.sendAndReceive()
    - [x] Sean is aware
    - ends with phrase "but a reply that contains"

# Developer's Guide -- navigate code base

Here is how I navigate the firmware code base.

## Search code within Vim

### cscope

The Vim package from Cygwin is built with `cscope`.

*I'd imagine most Vim builds have `cscope` because Vim tightly
integrates `cscope`.*

See the Vim help:

```vim
:help cscope
```

See `cscope` configuration in my `~/.vim/vimrc`:

```vim
if has("cscope")
    set nocscopetag
    set cscopequickfix=s-,d-,c-,t-,e-,i-,a-
```

Make a cscope database in the `./firmware` folder.

```bash
$ cd ./firmware/
$ cscope -R -b
```

This creates/overwrites the `cscope.out` file in `./firmware`.
I update the `cscope.out` file regularly. I made shortcut `;cu`.
The shortcut updates my tags file as well.

In Vim, updated connection to the new `cscope.out` file:

```vim
: cscope reset
```

The first time `cscope.out` is made, create the connection and
kill any existing connections. There is a bit to do here. I made
a `;cs` shortcut for this. See details in my `~/.vim/vimrc`:

```
function! CscopeCreateConnection()
```


Search with cscope using `:cscope find`.

- 's' symbol: find all references to `symbol_name`

```vim
:cscope find s symbol_name
```

As a Vim shortcut:

```vim
nnoremap <C-\>s :cs find s <cword><CR>
```

- 'g' global: find the global definition of `<cword>` under the cursor

```vim
:cscope find g symbol_name
```

```vim
nnoremap <C-\>g :cs find g <cword><CR>
```

- 'c' calls: find all calls to the function name under cursor
- 'd' dependencies: find functions called by function under cursor
- 't' text: find all instances of the text under cursor
- 'e' egrep: egrep search for the word under cursor
- 'f' file: open the filename under cursor
- 'i' includes: find files that include the filename under cursor

Then use the Vim quickfix window to navigate through the `cscope`
results.

```vim
:copen
```

#### cscope has no exclude option

Instead of an exclude create a `cscope.files` file that lists the
files to include. Use double-quotes for file names that include
spaces.

Or use `-inamefile` to specify an alternate file with a list of
file names to include. Use `-i-` to get the file list from
standard input. Use `find` to generate the file list from a
regexp.

### tags

Make a tags file in the `./firmware` folder.

```bash
$ cd ./firmware/
$ ctags --c-kinds=+l --exclude=Makefile -R .
```

- `--c-kinds=+l` includes local variables in the `cscope.out` file
- `-R` recursive
- `.` start in the current folder

Update this file with the same command to overwrite it.

I use Vim shortcut `;cu` which also updates the `cscope.out`
file.

### vimgrep

```vim
:vimgreap /search_term/ files_to_search
```

Example:

```vim
:vimgrep /SetBit(Flag_SpiFlags/ lib/**/*.[ch]
```

Find all occurrences of `SetBit(Flag_SpiFlags` in every `.c` and
`.h` file in the `lib/` folder, including sub-folders.

Open a quickfix window and navigate the results.

```vim
:copen
```

Similar to using `cscope`.

Use `cscope` to when the symbol name is known but its
purpose/definition/dependencies are unknown.

This is the scenario:

- I am looking at a specific line of code
- I see a symbol I want to know more about

Use `vimgrep` when the symbol name itself is not known yet.

This is the scenario:

- I want to determine a variable naming convention
- or I just want to check if a certain variable name is already
  used

Another example: search the active Vim file for a certain word:

```vim
:vimgrep /cscope/ %
```

This is superior to the usual Vim search (with `/`) because
`:copen` displays all of the lines where the search term occurs.

And this is useful when a cscope database does not exist.

# Developer's Guide -- update documentation

Here is how I update documentation.

There is one top level Doxyfile

- `./Doxyfile`

Create a doxygen folder at any level to document with Doxygen

- `./doxygen`
- `./firmware/doxygen`

Optionally, add a second `Doxyfile`:

- `./firmware/Doxyfile`

I decided to add a second `Doxyfile` to have different page
titles and main page files for the `./doxygen` and
`./firmware/doxygen`.

## Doxyfile customization

Generate the default Doxyfile:

```bash
$ doxygen -g
```

The `./Doxyfile` is the default Doxyfile with the following
changes:

```
PROJECT_NAME           = "dev-kit"
PROJECT_BRIEF          = "Python-compatible Chromation Spectrometer dev-kit"
OUTPUT_DIRECTORY       = ./doxygen
RECURSIVE              = YES
OPTIMIZE_OUTPUT_FOR_C  = YES
EXTRACT_STATIC         = YES
OPTIMIZE_OUTPUT_JAVA   = NO
EXCLUDE                = doc/ \
                        hardware-drawings/ \
                        labview/ \
                        system-design-sketches/ \
                        temp/ \
                        firmware/temp/ \
                        firmware/AssemblyExample.md \
                        firmware/Earhart-snippet.c \
                        firmware/README-firmware.md
EXTRACT_ALL            = NO
WARN_IF_UNDOCUMENTED   = YES
USE_MDFILE_AS_MAINPAGE = README.md
HTML_EXTRA_STYLESHEET  = "doxygen/doxygen-dark-theme/custom.css" \
                         "doxygen/doxygen-dark-theme/custom_dark_theme.css"
```

The `./firmware/Doxyfile` is the `./Doxyfile` with the following
changes:

```
PROJECT_NAME           = "firmware"
PROJECT_NUMBER         = "v0"
PROJECT_BRIEF          = "Protocol compatible with chromaspec-sean"
EXCLUDE                = doc/ \
                        temp/
USE_MDFILE_AS_MAINPAGE = README-firmware.md
```

- `PROJECT_NAME`
    - one-word project name
    - appears as title on every page
- `PROJECT_BRIEF`
    - optional one-liner
    - appears at the top of every page
- `RECURSIVE`
    - `YES`: look inside project folders for source code
- `EXTRACT_STATIC`
    - include documentation for `static` functions
- `OPTIMIZE_OUTPUT_JAVA`
    - `YES`: if project only contains Python code or Java code
- `USE_MDFILE_AS_MAINPAGE`
    - `README.md`: use the `README.md` file in the same folder
      that as the `doxygen` folder
    - this means the `doxygen` for the top-level and the
      `doxygen` for `./firmware` have their own `README.md` files
      which appear as the main page

Use Vim shortcuts `;DU` to create/update doxygen and `;DV` to
view in the browser.

`;DU` is equivalent to `bash` command:

```bash
$ doxygen ./firmware/Doxyfile
```

`;DV` is equivalent to `bash` command:

```bash
$ explorer "$(cygpath -wa ./firmware/doxygen/html/index.html)"
```

## Create a doxygen folder before running doxygen

The repo *does not contain* a `doxygen` folder after the initial
clone. Create empty `doxygen` folders before running `;DU`.

```bash
$ mkdir ./doxygen # top-level documentation
$ mkdir ./firmware/doxygen # documentation for firmware only
```

The importance of making a `doxygen` folder is that my Doxyfile
looks for this folder:

```
OUTPUT_DIRECTORY       = ./doxygen
```

If a `doxygen` folder does not exist in the `pwd`, doxygen does
not generate documentation.

## Minimum Doxyfile and doxygen folder to proceed

At a minimum, have `./Doxyfile` and `./firmware/doxygen/`. Then
the `bash` command to make the documentation is simply:

```bash
$ doxygen Doxyfile
```

## Top-level and firmware-level versions of the documentation

I created the top-level `./doxygen` to get a top-level version of
the documentation that excluded details in the `firmware`
folder's markdown files. This is for the wider Chromation
audience.

I created the `./doxygen/Doxyfile` to make a distinct `main page`
and `title` for the `./firmware/doxygen` firmware-specific
version of the documentation. This is for me during development
of the firmware.

## Vim shortcut to make doxygen docstrings from unit test results
My shortcut `;xdc` takes unit test results and formats them as a
Doxygen docstring. The pneumonic is `x` for transform, `d` for
doxygen docstring, and `c` for C. Shortcut `;xdp` makes Python
docstrings.

Place the cursor in the Vim buffer with the test results `.md`
file. `;xdc` puts the docstring in the default register. Navigate
to the top of the function definition and paste.

## `;xdc` (automated) Doxygen example

`;mktgc` generated the following test result output:

```
test/test_runner.c:45:ReadLedState_returns_OFF_if_LED_is_off:PASS
test/test_runner.c:46:ReadLedState_returns_GREEN_if_LED_is_on_and_green:PASS
test/test_runner.c:47:ReadLedState_returns_RED_if_LED_is_on_and_red:PASS
```

With the cursor in the markdown file showing, the output, `;xdc`
copied the doxygen docstring to the clipboard.

Withe the cursor on the opening `{` of the function body
definition, pasting inserts the docstring:

```c
inline uint8_t ReadLedState(void) // -> led_state
{
    /** ReadLedState behavior:\n 
      * - returns OFF if LED is off\n 
      * - returns GREEN if LED is on and green\n 
      * - returns RED if LED is on and red\n 
      * - see led_state in StatusCodes.h
      * */
```

The final line of docstring is added by hand.

Doxygen automatically turns the file name `StatusCodes.h` into a
link.

## Manual Doxygen example

Here is an example documenting source file `StatusCode.h`. The
documentation is accessed in the HTML from:

```
Files
    File List
        lib
            src
                StatusCode.h
```

Following this tree opens the `StatusCode.h File Reference`.

The documentation below appears in the HTML under the heading
`Detailed Description` at the end of `StatusCode.h File
Reference`.

```c
/** \file
 * *See `cfg/chromaspec.json` in the Python API repository.*
 * - Section `"global"` defines the **protocol byte codes**.
 *   - Firmware header file StatusCodes.h duplicates these `"global"`
 *   definitions.
 * - Section `"protocol":"command"` defines the commands and
 *   command parameters **sent by the USB host.**
 * - Section `"protocol":"serial"` defines the **expected
 *   response** from the **USB board** `usb-bridge`.
 * - Section `"protocol":"sensor"` defines the **expected
 *   response** from the **sensor board** `vis-spi-out`.
 * .
 * **STATUS CODES**
 * - JSON section `"globals"` defines the **status codes**: `OK`,
 *   `ERROR`, `INVALID_CMD`
 * - the **status code** is **byte 0** of the *response* from the
 *   **USB board** or **sensor board**
 * - the **USB board** and the **sensor board** reply with the
 *   same status codes
 * - the **status code** describes the status of the
 *   communication *from the point of view of the sender*.
 * .
 * **INDICATOR LED STATES**
 * - Section `"globals"` defines the **LED states**: `OFF`, `GREEN`, or `RED`
 * - **LED state** is:
 *   - **byte 2** of the command to `set` the indicator LED
 *   - **byte 1** of the reply to `get` the indicator LED
 * .
 * */
```

The syntax is the usual markdown. Note:

- two-space indentation to indicate the level of the `-`
  bullet symbol
- `.` marks the end of an indented list section
    - place `.` at the level of the indent to end
- `\n` are unnecessary in this case thanks to the `.`

# Developer's Guide -- build tests

Unit tests depend on `glib` for data structures used by my
function mocking library, `mock-c`.

And unit tests depend on `mock-c`. Right now I hard-code a path
to the source code and builds of the object files. Anyone in
Chromation on Dropbox automatically picks these files up.

TODO:

- make `mock-c` into a static library
- include the path to `mock-c` in `uservars.mk`
    - path becomes a user-variable to define, along with the DFP
      path

## glib for unit tests
Unit tests use `glib`. The `Makefile` depends on it.

The `Makefile` has CFLAGS:

```make
-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
```

The `Makefile` has LFLAGS:

```make
-lglib-2.0 -lintl -L/usr/lib/glib-2.0
```

Install Cygwin packages:

- `libglib2.0-devel`
- `libglib2.0-doc`

Check if the packages are already installed:

```bash
$ apt-cyg listall libglib2.0
libglib2.0-devel
libglib2.0-doc
libglib2.0_0
```

*Yes, they are installed.*

```bash
$ apt-cyg listall libglib2.0
libglib2.0_0
```

*No, they are not installed.*

Install from the command line with `apt-cyg`:

```bash
$ apt-cyg install libglib2.0-devel libglib2.0-doc
```

# Developer's Guide -- analyze code

## avr-size with ;as and ;ds

```vim
nnoremap <leader>as :call PasteAvrSize("build/vis-spi-out.elf")<CR>
```

Context: pwd is `./firmware/vis-spi-out/`.

Get size of `.elf` and paste it.

Hard-coded to look for `build/vis-spi-out.elf`.

`;ds` to paste size with timestamp.

```vim
nnoremap <leader>ds :call PasteBlock_DateAndAvrSize()<CR>
```

Again, this is hard-coded with the file to get the size of. To
get `vis-spi-out.elf` or `usb-bridge.elf`, comment accordingly in
the Vimscript function definition:

```vim
    " call PasteAvrSize("build/vis-spi-out.elf")
    call PasteAvrSize("build/usb-bridge.elf")
```

## Add `avr/include` to Vim path for `gf` on headers

Append the `avr/include` folder to the Vim path:

```vim
set path+=/cygdrive/c/Program\\\ Files\\\ (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include
```

Now searches that use the Vim path, such as `gf`, look in this
folder.

Usage: put cursor on header file like this:

```c
#include <avr/io.h> // includes iom328p.h for hardware i/o values
```

With cursor on any letter in header name, e.g., cursor on `a`,
`gf` goes to the file.

## ;io to open iom328p.h

```vim
let avr_include='/cygdrive/c/Program\ Files\ (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include'
nnoremap <expr> <leader>io ':split ' . avr_include . '/avr/iom328p.h<CR>:set readonly<CR>'
```

## `;yn` toggle test Yep/Nope

Usage: put cursor on line of test suite and `;yn` to toggle
whether the test runs

## Analyze assembly files

Put cursor in a file with `.avra` (assembly), usually a subset of
the assembly output to analyze.

- `;avrb` - clean AVR assembly and calculate cycles
- `;avrc` - just clean AVR assembly
- `;avrt` - just calculate AVR cycles

- TODO: document how to set these up
    - [ ] eliminate need to create a bash alias to the Python
      script, in fact, invoke the Windows Python and the
      script name
    - this way, Dropbox `Online only` sync method makes this work
      by downloading the Python script on demand

# TASKS

# dev
- [x] setup chromaspec-sean
- [x] use chromaspec-sean to open communication
    - chromaspeclib.simple wraps chromaspeclib.expert
    - see python/test-open.py
- [x] prepare Python for system test
    - use `python -m cmdline COMMAND KEY=VALUE`
- [ ] prepare `usb-bridge` for a system test
    - [x] get tests to build
    - [ ] write setup code
        - [x] find setup code in `dev-kit-ref`
        - [x] UsbInit
            - threw it into setup() for now
            - [ ] clean this up as a unit-tested UsbInit
            - works:
                - reads bytes off the FT221X
                - distinguishes between 0 and not 0
                - able to send Null so far, haven't set up
                  anything else
            - [x] send the commands to control the bridge LEDs
                - [x] system test GetBridgeLED
                - [x] system test SetBridgeLED
        - [x] SpiMasterInit
            - has the iom32p.h file changed? PORTB2 instead of
              PB2? No, hasn't changed... So when was PB2 ever
              defined?
            - [x] create SpiMaster lib
            - [ ] rename bit index type from lib_pin to lib_bit
                - pin is confusing with PIN register
                - pin is also confusing with numbered things like
                  LED0 -- bit makes it more obvious that LED0 is
                  high level, so bit for LED0 is not necessarily
                  bit 0
            - [x] pull common SPI functions out of SpiSlave.h
              into a common SPI lib
                - should I have just made SpiM Spi also? yes
                - is there *ANY* case where spim and spis
                  registers or bits would conflict? no
                - if not, then do it, make them all spis and move
                  them to a common lib: Spi
            - [x] write `GetSensorLED` in UsbCmd.h
                - need more lib functions:
                    - [x] SpiMasterXfrByte
            - once this is setup, I can send commands to the
              sensor
            - control the sensor LEDs
                - [x] GetSensorLED system test
                - [ ] SetSensorLED
                    - [x] vis-spi-out
                    - [ ] usb-bridge
            - then read/write the sensor configuration
    - [ ] write main loop switchcase
- [ ] do system test of command `SetSensorConfig`
    - system test:
    - mark as PASS/FAIL:
    - :`vis-spi-out` responds `OK` to a valid config
    - :`vis-spi-out` responds `ERROR` to an invalid config

## tabled
- [x] is there a way to reset the FT221X?
    - **NO**
    - *physical power cycle with USB hub switch*
    - Issue (why I want to reset the FT221X):
    - if I send data and do not read it, the data stays in the
      FT221X
    - the only way to clear the FT221X buffer is to power cycle
        - `FT_PROG` cycle does not do it
        - the physical reset button on the PCB does not do it
        - Atmel-Ice `atprogram` reset command does not do it
    - TODO: maybe there is a D2XX command that clears the FT221X buffer

# Build for dev-kit-mike

## clean Makefiles

- [ ] pull common code into top-level Makefile `common.mk`
- [ ] add help targets
- [ ] setup test framework
- [ ] turn test framework and `libs` into static libraries

## setup tools

- [ ] avra Python scripts
- [x] avr-size with ;as (run avr-size and paste output)
- [x] ctags, cscope with ;cs ;cu
- [x] Doxygen with ;DU ;DV ;xdc
- [x] glib (used by unit tests)
- [x] Add avr/include to Vim path

## firmware

- [ ] block diagrams starting with `main.c`
- [ ] why is UartSpi-Hardware.h in the `vis-spi-out.c` folder?
    - `vis-spi-out.c` does not include this file
    - who does?
    - [ ] how do I grep for `#include "UartSpi-Hardware.h` across
      the whole project?

# DONE
## protocol defined in json

- [x] find the copy of the `chromaspec.json` file I was editing
  to track my changes to the protocol
    - paste in `dev-kit-mike/firmware` folder

## dev - troubleshoot linker error for faked function
- linker error trying to link `FtBusTurnaround`:
    - All the faked functions in lib `Usb` have undefined
      references and symbols
- example:
    - undefined reference to `_FtClockDatabus_fake`
    - undefined symbol `_FtClockDatabus_fake`
- see function faking checklist in TiddlyWiki
    - `chromation-notebook`
    - tiddler `Fake a function`
- `_FtClockDatabus_fake` is the fake of inline
  function `_FtClockDatabus`
- [x] yes, the declaration exists in `Usb_faked.h`
- `void _FtClockDatabus_fake(uint8_t);`
- [x] yes, the definition exists in `Usb_faked.c`
- [x] yes, conditionally include the fake declarations in
  `Usb.h`
- [x] aha, I need to add to Makefile list of faked libs
- `lib_faked := Usb`
- that was the missing bit! I did not have `Usb` in the list of
  faked libs

