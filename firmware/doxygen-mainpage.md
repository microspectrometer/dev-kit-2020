# Firmware and Python API

This firmware works with PyPI project `microspec`.

```
$ pip install microspec
```

The firmware follows the protocol defined in the [microspec.json
file](https://github.com/microspectrometer/microspec/blob/master/cfg/microspec.json)

# Chromation Spectrometer dev-kit

The Chromation spectrometer is a surface-mount PCB package
consisting of a linear photodiode array and optical components.
This spectrometer chip does not contain any firmware.

The firmware is on the two microcontrollers in the dev-kit. The
dev-kit has a stack of two PCBs, with one microcontroller on each.

The microcontroller on the lower PCB provides a SPI interface to
the Chromation spectrometer. The microcontroller on the upper PCB
converts the SPI interface into a USB interface.

The dev-kit helps users evaluate the spectrometer chip. Also, all
of the dev-kit files, including this firmware, are available to
users under the MIT license, meaning reuse in commercial and
non-commercial projects with very little limitation.

## Programmable registers

The spectrometer chip has programmable registers that affect
pixel size and voltage gain. The dev-kit writes these registers
with Chromation's recommended values when the dev-kit is powered
on.

Developers typically do not need to change these register values.
The Python API in `microspec` (defined in
[microspec.json](https://github.com/microspectrometer/microspec/blob/master/cfg/microspec.json))
includes command `setSensorConfig()` to write to these registers,
in case developers have a special use-case.

