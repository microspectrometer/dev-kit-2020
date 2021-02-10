Welcome to Chromation's dev-kit resources!

# How to read this

If you are reading this online at Chromation's microspectrometer
GitHub repository, please go to Chromation's GitHub pages site:

https://microspectrometer.github.io/dev-kit-2020/

It's the same docs, but the above link lets your browser run the
HTML portion of the docs.

# A quick overview

This is the **Chromation Spectrometer Development Kit**
(dev-kit):

![dev-kit with usb-bridge stacked on
top](img/photo/dev-kit-with-usb-bridge-not-rotated.jpg)

Use this kit to develop applications with the Chromation
spectrometer chip `CUVV-45-1-1-1-SMT`:

![render of CUVV-45-1-1-1-SMT](hardware/design/CUVV-45-1-1-1-SMT/render/CUVV-45-1-1-1-SMT-top.png)

The spectrometer chip has the standard interface for a 1D CMOS
image sensor:

- analog video out
- digital I/O to control exposure and pixel readout

The dev-kit mounts the spectrometer chip on a breakout board with
an 8-pin ZIF connector for connection to other circuit boards
with a flex cable:

![render of breakout board](hardware/design/vis-analog-out/render/pcb.png)

The dev-kit can be simply be plugged in and used as shipped, or
taken apart to access lower-level interfaces. This illustration
shows the dev-kit's hardware seams:

![dev-kit output options](img/dev-kit-output-options.png)

# Table of Contents

- [Getting started](getting-started.md)
- [Repository contents and license](repo-contents.md)
- [CUVV-45-1-1-1-SMT datasheet](hardware/design/CUVV-45-1-1-1-SMT/CUVV-45-1-1-1-SMT_technical-note-1.0.pdf)
- [Doxygen firmware documentation](firmware/doxygen/html/index.html)
