This firmware runs on the Chromation dev-kit:

https://microspectrometer.github.io/dev-kit-2020/

The dev-kit has two ATmega328 microcontrollers and a spectrometer
chip.

This firmware is for the Flash memory in the two
microcontrollers. There is no firmware for the spectrometer
chip itself, but the chip does have programmable registers which
must be written to after power is turned on.

## Spectrometer chip programmable registers

The spectrometer chip has programmable registers that affect
pixel size and voltage gain. The dev-kit writes these registers
with Chromation's recommended values when the dev-kit is powered
on.

Most applications do not require changing these register values.
But in case there is a special use-case where it makes sense to
change these values, the Python API in `microspec` (defined in
[microspec.json](https://github.com/microspectrometer/microspec/blob/master/cfg/microspec.json))
includes command `setSensorConfig()` to write to these registers.

# Serial communication and the Python API

This firmware was created to work with PyPI project `microspec`.

```
$ pip install microspec
```

The Python code in `microspec` is auto-generated from the serial
communication protocol defined in the [microspec.json
file](https://github.com/microspectrometer/microspec/blob/master/cfg/microspec.json)

The firmware was written (manually) to comply with this protocol.

