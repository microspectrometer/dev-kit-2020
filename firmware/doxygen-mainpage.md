# How to use this documentation

All the useful firmware information is under [Files](files.html).

# Additional documentation

The firmware runs on the Chromation dev-kit. Documentation for
the dev-kit is here:

https://microspectrometer.github.io/dev-kit-2020/

All dev-kit hardware and firmware files are open-source and are
available from the GitHub repository:

https://github.com/microspectrometer/dev-kit-2020

# Chromation dev-kit hardware

The dev-kit has two ATmega328 microcontrollers and a spectrometer
chip.

The firmware is for the Flash memory in those two
microcontrollers:

- the microcontroller on the `vis-spi-out` PCB handles
  spectrometer configuration and readout
- the microcontroller on the `usb-bridge` PCB simply passes
  messages between the application on the host computer (e.g., a
  Python script) and the 5-wire SPI interface on the
  `vis-spi-out` PCB

There is no firmware for the spectrometer chip itself, but the
chip does have programmable registers.

## Spectrometer chip programmable registers

The programmable registers are part of the LIS-770i, the CMOS
image sensor used in the spectrometer chip.
These registers initialize in an unknown state and must be
written to after the spectrometer chip is powered on.

All registers are programmed at once by sending the LIS-770i a
28-bit word. The value of this 28-bit word determines:

- pixel binning (choosing between 7.8µm and 15.6µm pitch)
- voltage gain
- which pixels are active

See details in [Lis.h](LisConfig_8h.html).

The dev-kit firmware writes these registers with recommended
default values when the dev-kit is powered on. The recommended
defaults are to turn on pixel binning, set gain to 1x, and turn
on all pixels.

The [microspec](https://pypi.org/project/microspec/) Python API includes command `setSensorConfig()`
for applications to write to these registers, but most
applications **do not require** changing these register values:

- the recommended value for pixel binning is based on the optical
  design of the spectrometer
- the recommended 1x gain is based on the dev-kit's analog
  front-end (i.e., the choice of ADC and ADC reference voltage)
  -- given this design, increasing the gain reduces dynamic range
  without improving SNR

# Serial communication and the Python API

## Protocol is in a JSON file

The serial communication protocol is defined in the JSON file
[microspec.json](https://github.com/microspectrometer/microspec/blob/master/cfg/microspec.json):

- The firmware follows the
  [microspec.json](https://github.com/microspectrometer/microspec/blob/master/cfg/microspec.json)
  serial communication protocol.
- PyPI project [microspec](https://pypi.org/project/microspec/)
  defines a USB interface to the dev-kit. This interface is a
  `class` that is auto-generated from
  [microspec.json](https://github.com/microspectrometer/microspec/blob/master/cfg/microspec.json).

## Develop and test

Install Python and the
[microspec](https://pypi.org/project/microspec/) package to
develop and test the firmware:

```
$ pip install microspec
```

The Python REPL is an easy way to quickly test commands. Details
are in the
[documentation](https://microspec-api.readthedocs.io/en/latest/microspec.commands.html).
The [microspec](https://pypi.org/project/microspec/) package also
provides a command-line utility to send commands without opening
a Python REPL. This is useful when running single dev-kit
commands from a text editor like Vim or Emacs.

To quickly check that the firmware and Python API are in complete
agreement about the serial communication protocol, run
[system-tests.py](https://github.com/microspectrometer/dev-kit-2020/blob/master/python/microspeclib/system-tests.py).
This script runs all the serial commands and checks for the
expected responses from the dev-kit. The script is organized into
classes, one class per serial command. The methods in each class
are the different scenarios for testing the serial command (the
different arguments the command might be called with).

## Protocol example

TODO: Add an example with the `usb-bridge` and `vis-spi-out`
firmware that corresponds to one of the commands in the JSON
file, and how the resulting call-and-response looks from Python.
