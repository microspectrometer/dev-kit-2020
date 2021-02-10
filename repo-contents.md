# Repository Contents

```
.
├── README.md
├── firmware
├── hardware
└── python
```

The dev-kit's hardware design files, firmware, and software are
all provided for use under an [MIT
license](https://en.wikipedia.org/wiki/MIT_License), which means
there is very little restriction on reuse of these files. See the
`LICENSE.md` file in each folder for details:

- [`firmware/LICENSE.md`](firmware/LICENSE.md)
- [`hardware/LICENSE.md`](hardware/LICENSE.md)
- [`hardware/design/CUVV-45-1-1-1-SMT/LICENSE.md`](hardware/design/CUVV-45-1-1-1-SMT/LICENSE.md)
- [`python/LICENSE.md`](python/LICENSE.md)

## Legal

The MIT license does *not* apply to Chromation's spectrometer
technology. Chromation's technology is protected in the United
States and foreign jurisdictions by [issued and pending
patents](https://www.chromation.com/patents.html). Chromation's
products are also protected by other intellectual property
rights, including copyright, mask work, trademark and trade
secret rights.

## dev-kit design and manufacturing files

The dev-kit design files are in `hardware`:

```
hardware
├── manufacture
│   ├── usb-bridge -- Gerbers and Drill
│   ├── vis-analog-out -- Gerbers and Drill
│   └── vis-spi-out -- Gerbers and Drill
└── design
    ├── CUVV-45-1-1-1-SMT -- Fusion360 CAD and documentation
    ├── usb-bridge -- EAGLE CAD files and docs
    ├── vis-analog-out -- EAGLE CAD files and docs
    └── vis-spi-out -- EAGLE CAD files and docs
```

See [`hardware/README.md`](hardware/README.md) for details.

## dev-kit firmware

The firmware is programmed in the Flash memory on the two dev-kit
microcontrollers. There is one microcontroller on PCB
`usb-bridge` and one microcontroller on PCB `vis-spi-out`.

The firmware is written in C.

```
firmware
├── README.md
├── Doxyfile -- Doxygen settings for firmware documentation
├── doxygen
│   └── html
│       └── index.html -- firmware documentation
├── Makefile
├── lib
│   ├── build
│   ├── Makefile
│   ├── src
│   └── test
├── usb-bridge
│   ├── build
│   ├── Makefile
│   ├── src
│   └── test
└── vis-spi-out
    ├── build
    ├── Makefile
    ├── src
    └── test
```

See [`firmware/README.md`](firmware/README.md) for details.

The firmware is documented with Doxygen. View the Doxygen in
[`firmware/doxygen/html/index.html`](firmware/doxygen/html/index.html).

## Example dev-kit interface applications written in Python

The example Python applications are in the `python` folder in the
`microspec` and `microspeclib` subfolders:

```
python
├── LICENSE.md
├── PYTHON-SETUP.md <--- how to install Python on Windows and Debian-based Linux (e.g., Mint and Raspbian)
├── PYTHON.md <--------- about `microspec` and `microspeclib`
├── README.md
├── microspec <--------- start here
│   └── one-command.py
└── microspeclib <------ lower-level interface (but still higher-level than serial port concerns)
    ├── one-command.py
    └── system-tests.py
```

`microspeclib` and `microspec` are Python packages Chromation
created to simplify application development.

*They are essentially the same package: `microspec` was created
to add a seam for method docstrings because all of the
`microspeclib` methods are auto-generated from a JSON file that
defines the serial communication protocol.*

Please [install microspec](python/PYTHON.md#setup-microspeclib)
before running these examples.

Examples coming soon:

- [ ] a simple application to capture a single frame and plot it
  in a web browser

Applications coming soon:

- [ ] a GUI interface in Python

### `one-command.py`

Use `one-command.py` to try a single command.

View the docstring with `pydoc`:

```bash
$ python -m pydoc one-command
```

Or open `one-command.py` in a text editor to read the docstring.

Example usage:

```bash
$ python one-command.py

Opened kit: CHROMATION125129...
Sent:
        Command 7: SensorGetSensorConfig
Received:
           Variable |   Value | Number of bytes
           -------- |   ----- | ---------------
             status |       0 | 1
            binning |       1 | 1
               gain |       1 | 1
         row_bitmap |      31 | 1

...Closed kit: CHROMATION125129
```

Edit the command on line 145 to try a different command.

The commands are summarized in the `one-command.py` docstring.
Full API documentation is available after installing Chromation's
`microspec` package.

### `systems-tests.py`

`system-tests.py` is a collection of unit tests.

Run the tests with `pytest` and `testdox`:

```
$ pytest --testdox system-tests.py
=============================== test session starts ===============================
platform win32 -- Python 3.8.1, pytest-5.3.5, py-1.8.1, pluggy-0.13.1
rootdir: C:\cygwin64\home\mike\chromation\dev-kit-2020\python
plugins: testdox-1.2.1
collected 49 items

system-tests.py

Get Bridge LED
 ✓ GetBridgeLED 0 replies led setting 0 if OFF
 ✓ GetBridgeLED 0 replies led setting 1 if GREEN
 ✓ GetBridgeLED 0 replies led setting 1 if RED
 ✓ GetBridgeLED 0 replies status OK
 ✓ GetBridgeLED 1 replies status ERROR

...

Auto Exposure
 ✓ AutoExposure replies status OK
                                                                             [100%]

=============================== 49 passed in 3.04s ================================
```


