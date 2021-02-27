[TOC]

# How to use the firmware documentation

All the useful firmware information is under [Files](files.html).
This browses the source code.

- click a folder to open
    - click the file *icon* to view its **source code**
    - or click a file *name* (if it's a hyperlink) to view its **documentation**

## Firmware organization

The firmware is divided into folders:

```
├── lib  <--------- reusable code
│   ├── src
│   └── test
├── usb-bridge <--- code specific to this circuit board
│   ├── src
│   └── test
└── vis-spi-out <-- code specific to this circuit board
    ├── src
    └── test
```

Each folder has `src` (source code) and `test` (unit tests).

## Library functions break with C convention

The first thing C programmers will notice is that the `lib`
source code does not follow convention:

- most functions in the `lib` folder are `inline`
- the function bodies are in the `.h` files
- the function signatures are in the `.c` files

This is for [performance reasons](index.html#autotoc_md10).

# Related dev-kit documentation

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

Jump to the `main()` `.c` file for the two microcontrollers:

- [usb-bridge/src/usb-bridge.c](usb-bridge_8c_source.html)
- [vis-spi-out/src/vis-spi-out.c](vis-spi-out_8c_source.html)

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

- REPL
    - The Python REPL is an easy way to quickly test commands.
      Details are in the
      [documentation](https://microspec-api.readthedocs.io/en/latest/microspec.commands.html).
- command-line
    - The [microspec](https://pypi.org/project/microspec/)
      package also provides a command-line utility to send
      commands without opening a Python REPL. This is useful when
      running single dev-kit commands from a text editor like Vim
      or Emacs that can execute shell commands.
- system tests
    - Run
      [system-tests.py](https://github.com/microspectrometer/dev-kit-2020/blob/master/python/microspeclib/system-tests.py)
      to quickly check that the firmware and Python API are in
      complete agreement about the serial communication protocol.
    - This script runs all the serial commands and checks for the
      expected responses from the dev-kit.
    - The script is organized into classes, one class per serial
      command. Each class method tests different arguments to the
      serial command.

## Protocol example

TODO: Add an example with the `usb-bridge` and `vis-spi-out`
firmware that corresponds to one of the commands in the JSON
file, and how the resulting call-and-response looks from Python.

# Why so much inline

The library code started out in the conventional way:

- functions defined in `.c` files
- function signatures declared in `.h` files
- functions broken into smaller functions for testability
  and function pointers everywhere to create seams for **function
  faking** in unit tests (redirecting the definition of the "production" code to
  a "stub" or "mock" version for test code)

The code was "clean" -- no macros, lots of seams. And it had
almost 100% test coverage.

Unfortunately, this style of programming is not compatible with
producing optimal assembly instructions for the ATmega328.
It results in system-breaking speed penalties.

The ATmega328 microcontroller clock is only 10MHz: *10 cycles
worth of instructions takes a microsecond*. At this clock speed,
**inefficient assembly code** causes noticeable slow-downs and makes
critical timing impossible, resulting in bad behavior that is
sometimes obvious and sometimes subtle.

The avr-gcc compiler was generating inefficient assembly code for
three reasons:

1. lots of call overhead because one function was broken into
   several functions for readability and unit-testability and an
   extra layer of indirection from function pointers that create
   the seams for unit-testability
2. slow instructions used instead of faster instructions because
   register addresses are unknown at compile time because code
   was spread across multiple compilation units for reusability
   and unit-testability
3. ISR overhead: too much time spent saving and restoring state
   on every interrupt because the ISR had too much code

The last item is "easy" to fix: modify the ISR code, count cycles
in the generated assembly, repeat until the time consumed by the
ISR is acceptable.

But the first two items are not easy to fix. One approach is
to take the carefully segregated code base and mush it back
together:

- put all the code in one C file
- eliminate all the function pointers
- throw away most of the unit tests

Compared with that, a small break with convention is OK.

To use the fast instructions, the compiler must know register
addresses at compile-time. Object files generated without these
values are forced to use the much slower, generic load and store
instructions.

How do I know what instructions should be used? I manually look
up the address of the register and figure it out. I also wrote
some shortcuts to cut and paste disassembly into the code.

*That explains why the code is full of comments like this:*

```c
     ClearBit(UartSpi_port, UartSpi_AdcConv);
     // ---Expected Assembly---
     // cbi  0x0b, 2 ; 11
```

*In more complicated cases, there are several lines of assembly.*

Pasting the disassembly into the code helped me check that the
compiler was still using the right instructions after a code
change. This is how I determined the correct steps to make
`inline` actually inline the code.

## Conventional C yields inefficient assembly

*The simplest example is the bit manipulation functions discussed
below. The same reasoning applies to all the library code.*

The [ReadWriteBits](ReadWriteBits_8h_source.html) library defines
bit manipulation functions (for readability and testability). In
the conventional approach, this source file is compiled without
knowing the register addresses that client code will pass to its
functions. But the register address determines which instructions
are allowed! Without knowing the address, the compiler has to use
the most generic instruction.

The bit manipulation function might be passed an I/O register in
the range `0x00 - 0x1F`. These registers allow *fast*
instructions like `sbi`, `cbi`, etc.

Or the bit manipulation function might be called on an I/O
register in the range `0x20 - 0x3F`. In this range the bit
manipulation has to be done with entire bytes using instructions
`in` and `out`.

Or the bit manipulation might be performed on dataspace,
`0x40 - 0x5F`, or extended I/O space, `0x60 - 0xFF` in SRAM, in
which case instructions like `ld` and `st` are used.

Again, the compiler does not know what register address the
function is called with, so it has to use the generic `ld` and
`st` instructions. The result is that calls to the bit
manipulation library take a massive speed penalty.

## Inlining yields efficient assembly

To avoid this, the `inline` copies the bit manipulation source
code into all the places it is called.

Now the code is part of the compilation unit that has the
register addresses and the compiler knows if it can use the
faster instructions. In addition to using faster instructions,
the function call overhead is eliminated. And the code still
retains its readability and reusability.

In order for the compiler to `inline`, the function body must be
visible in the compilation unit, therefore the function body goes
in the header.

The result is a glaring break with C convention. Most (all?) of
the library functions are defined in `.h` files, with the
function signatures declared in the `.c` file.

## Macro USE_FAKES

The only practical drawback to this approach is that it is
incompatible with using function pointers to replace functions
with fakes for unit tests.

Instead of function pointers, the code gets marked up with test
macros to handle the function faking. This comes up when:

- I want to write a unit test for a function
- but the function calls other functions that interact with
  hardware

In this scenario, the definition for the function-under-test gets
bracketed with `#ifdef USE_FAKES` statements to `#define` and
`#undef` the fakes like this:

```c
#ifdef USE_FAKES
#define LisExpose LisExpose_fake
#define LisReadout LisReadout_fake
#endif
uint16_t AutoExpose(void)
{
    ...
}
#ifdef USE_FAKES
#undef LisExpose
#undef LisReadout
#endif
```

