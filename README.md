# Introduction

This is the Chromation Spectrometer Development Kit (dev-kit) for
Chromation spectrometer chip `CUVV-45-1-1-1-SMT`:

![dev-kit with usb-bridge stacked on
top](img/photo/dev-kit-with-usb-bridge-not-rotated.jpg)

The dev-kit helps:

- evaluate Chromation's spectrometer technology
- design a product using Chromation's spectrometer chip

## Dev-kit interfaces

The Chromation dev-kit divides into **three interfaces**:

1. USB out
2. SPI out
3. Analog out

![dev-kit output options](img/dev-kit-output-options.png)

## Dev-kit circuit boards

The dev-kit divides into **three printed circuit boards** (PCBs):

- `vis-analog-out`
    - breakout board for Chromation `CUVV-45-1-1-1-SMT`
    - direct analog interface to the spectrometer chip
    - converts the QFN-48 chip to ZIF-8
- `vis-spi-out`
    - SPI interface to Chromation `CUVV-45-1-1-1-SMT`
- `usb-bridge`
    - converts SPI interface to USB interface

## Dev-kit setup and usage

Please see additional information about setting up and using:

- the dev-kit hardware
    - see [DEV-KIT.md](DEV-KIT.md#usb-out)
- the dev-kit Python interface
    - see [PYTHON.md](python/PYTHON.md)

*Windows users:* please [enable "Load VCP"](DEV-KIT.md#windows-users-enable-load-vcp).

See [PYTHON-SETUP.md](python/PYTHON-SETUP.md) for steps starting on a
Windows or Linux system without Python.

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

The example Python applications are in `python`:

```bash
python
├── LICENSE.md
├── one-command.py
└── system-tests.py
```

Please [install microspec](python/PYTHON.md#setup-microspeclib) before running these examples.

*The example applications use Chromation's `microspeclib` package.*

More applications coming soon:

- [ ] a Python version of the GUI
- [ ] a simple application to capture a single frame and plot it
  in a web browser

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


