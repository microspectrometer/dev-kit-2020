# Introduction

![dev-kit with usb-bridge stacked on
top](img/photo/dev-kit-with-usb-bridge-not-rotated.jpg)

Reasons to use Chromation's Development kit:

- evaluate Chromation's spectrometer technology
- design a product using Chromation's spectrometer chip


## Dev-kit setup and usage

Please see additional information about setting up and using:

- the dev-kit hardware
    - see [DEV-KIT.md](DEV-KIT.md#usb-out)
- the dev-kit Python interface
    - see [PYTHON.md](python/PYTHON.md)

*Windows users:* please [enable "Load VCP"](DEV-KIT.md#windows-users-enable-load-vcp).

See [PYTHON-SETUP.md](python/PYTHON-SETUP.md) for steps starting on a
Windows or Linux system without Python.

## Dev-kit circuit boards

Each interface has its own circuit board. So the dev-kit divides into **three printed circuit boards** (PCBs):

- `vis-analog-out`
    - breakout board for Chromation `CUVV-45-1-1-1-SMT`
    - direct analog interface to the spectrometer chip
    - converts the QFN-48 chip to ZIF-8
- `vis-spi-out`
    - SPI interface to Chromation `CUVV-45-1-1-1-SMT`
- `usb-bridge`
    - converts SPI interface to USB interface

### Circuit board names

The names of the circuit boards are the names used in the
[hardware](hardware/) and [firmware](firmware/) files.

#### `vis` refers to the CMOS image sensor 300-1100nm wavelength range

*The `vis` prefix is short for **visible** spectrum, as opposed to
**UV** (ultra-violet) or **IR** (infrared).*

The Chromation spectrometer chip uses a CMOS image sensor. The
image sensor peaks in the **visible** part of the wavelength
range, so the breakout and readout boards are prefixed with the
name `vis`.

*The visible wavelength range is 400-700nm. This is not what
`vis` refers to.*

Chromation's `vis` spectrometer spans the full wavelength
sensitivity range of standard CMOS image sensors, roughly
300-1100nm. This range includes **UV** and **NIR**
(near-infrared), but CMOS image sensor sensitivity is weak at the
tail ends of this range. The exact wavelength range is provided
with the spectrometer's wavelength calibration data.
