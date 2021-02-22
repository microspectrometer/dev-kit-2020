Steps to add a new sensor to the dev-kit codebase.

*Instead of maintaining separate projects for the LIS and S13131,
there is a single firmware folder. When make is invoked, the
optional `sensor=` argument controls which version of the firmware
is built.*

# Hardware changes

## Match the ZIF pins

Designing the sensor breakout board, map the ZIF pins to match as
close as possible.

*If the sensors are very different, the `vis-spi-out` board needs
to change as well.*

We are lucky with the LIS-770i and S13131-512. They have very
similar functionality in the analog and digital pins. No change
is required to the `vis-spi-out` PCB.

## Signals on LIS-770i and S13131

For the six signals that have perfect matches, I route the six
S13131 signals to those same six ZIF pins:

- Power and Ground are the same
- VIDEO is the same
- CLK is the same
- RST on LIS-770i is ST on S13131
- SYNC on LIS-770i is EOS on S13131

For the remainder, we just get lucky:

- analog pins:
    - The VREF pin is particular to the LIS-770i, but the
      `vis-spi-out` analog front end expects a VREF, so I make my
      own VREF on the S13131 breakout board.
    - The Vcp pin on S13131 does not connect to the ZIF.

- digital pins:
    - The PIXSELECT pin is particular to the LIS-770i, so this
      ZIF pin is left as NC (not-connected) on the S13131
      breakout board.
    - the TRIG pin on S13131 does not help us because it is just
      CLK inverse; the same microcontroller that generates CLK
      also does readout, so it's simpler to look at CLK
      internally than it is to trigger off of the TRIG signal

# Firmware changes

In the following I use the generic `sensor` in place of the
actual sensor.

*For the S13131 work, `sensor` is `S13131`.*

## lib/src/sensor.h

Define `sensor_` types for registers addresses, pin numbers, and
bit numbers.

*Example:*

```c
// sensor_ ──────────────────────┐
//                               ↓
typedef uint8_t volatile * const s13131_ptr; // reg address type
```

TODO(mike): `_ptr` is a poor choice of name, change this to `_reg`

Name the registers, pins, and bit numbers. Declare registers,
pins, and bit numbers as `extern`.

*Example:*

```c
extern s13131_ptr S13131_ddr;
```

## vis-spi-out/src/sensor-Hardware.h

*`vis-spi-out` is the PCB design. This is where the hardware is.
The hardware definitions, therefore, happen here.*

`vis-spi-out.c` uses the sensor lib by including `Hardware.h`.

*Sensor-related hardware values need to be defined in the
compilation unit with `vis-spi-out.c`.*

Include the `sensor-Hardware.h` file in
`vis-spi-out/src/Hardware.h`.

*Example:*

```c
// vis-spi-out/src/Hardware.h
# include "S13131-Hardware.h"
```

In `S13131-Hardware.h`:

- assign registers, pins, and bits to actual hardware I/O values
  defined in AVR header `iom328p.h`
    - use types defined in `lib/src/sensor.h`
    - use register, pin, and bit names declared in `lib/src/sensor.h`

*Example:*

```c
s13131_ptr S13131_ddr    = &DDRD;
```

## lib/test/sensor-HardwareFake.h

*`lib/test/` is a test environment that fakes testing on the
actual PCB. For testing purposes, this is where the hardware is.
The hardware definitions, therefore, happen here.*

*The hardware definitions are fake. The I/O register addresses
are assigned to arbitrary addresses by the operating system. The
read/write behavior of data in these fake I/O registers is
the same, but the registers don't do anything (e.g., the fake
DDRD register doesn't control the data-direction of an actual
pin). The fake registers are just dummy memory on the computer
running the test.*

`test_runner.c` includes fakes for the hardware values by
including `lib/test/HardwareFake.h`

*Example:*

```c
// vis-spi-out/test/test_runner.c
#include "HardwareFake.h"
```

Include `lib/test/sensor-HardwareFake.h` in
`lib/test/HardwareFake.h`

*Example:*

```c
// lib/test/HardwareFake.h
#include "S13131-HardwareFake.h"
```

In `lib/test/sensor-HardwareFake.h`:

- allocate static memory for fake versions of the hardware registers
- assign registers to these fakes
- assign pins and bits to fake hardware i/o values

*Example:*

```c
static uint8_t volatile fake_ddr;
s13131_ptr S13131_ddr = &fake_ddr;
s13131_pin S13131_Clk = 5; // PWM
```

## lib/test/HardwareFake.h

Include `sensor-HardwareFake.h`.

*Example:*

```c
#include "S13131-HardwareFake.h"
```

# Build changes

## Review of the Makefile flow

There are three Makefiles at the top-level:

- `uservars.mk` sets paths to pre-compiled dependencies
  from Microchip (formerly Atmel)
    - this is pulled out into its own file to make it clear where
      the user has to go to set this path
- `common.mk` has all the actual build recipes
- `Makefile` is just a bunch of help docs

The "entry-point" for `make` happens in the sub-folders for the
two PCBs: `firmware/vis-spi-out/` and `firmware/usb-bridge/`.

Users enter either `vis-spi-out` or `usb-bridge` (depending on
which firmware they are building) and run the `Makefile` **in
that folder**.

The `vis-spi-out/Makefile` and `usb-bridge/Makefile`:

- set a bunch of build variables used in `common.mk`
- end with `include ../common.mk`
    - the make target (e.g., build for AVR, build unit tests for
      the host computer, program the flash memory, etc.) finds
      its recipe in `../common.mk`
        - e.g., `make flash` has recipe `flash:` in
          `../common.mk`

The point of this setup is that `../common.mk` runs with the
definitions from the board's `Makefile`. For example,
`vis-spi-out/Makefile` sets `$(board-name)` equal to
`vis-spi-out`.

## make sensor=

The `vis-spi-out/Makefile` set a variable named `sensor`.

Identify the `sensor` when invoking make:

```bash
$ make print-vars sensor=S13131

sensor:
- S13131
...
```

If `sensor` is left out, the default value is `sensor=LIS`:

```bash
$ make print-vars

sensor:
- LIS
...
```

## Default value for sensor

Set `LIS` as the default sensor in `vis-spi-out/Makefile`:

```make
# Name which sensor is attached to the vis-spi-out PCB
sensor ?= LIS
```

## A macro is defined by sensor=

Modify the build recipes to use `sensor` to define a macro:

```make
-D$(sensor)
```

*Example:*

If `sensor` is `LIS`, then the above line becomes:

```make
-DLIS
```

That gets used in a recipe like this:

```make
build/${board-name}.o: src/${board-name}.c src/Hardware.h ${Hardware} ${lib_headers} ../lib/src/StatusCode.h ../lib/src/StatusCodes.h ../lib/src/LisConfig.h ../lib/src/LisConfigs.h
	${compiler} -D$(sensor) $(CFLAGS) -c $< -o $@
                 ↑
                 |
-DLIS ───────────┘
```

Now the macro `LIS` is "defined" when the compiler parses the
source code.

## Source code is bracketed with ifdef sensor macros

Source code checks if the `$(sensor)` macro is defined to decide
which code to include.

*Example:*

```c
// vis-spi-out/src/Hardware.h
#ifdef LIS
#include "Lis-Hardware.h"
#endif
#ifdef S13131
#include "S13131-Hardware.h"
#endif
```

If the build is invoked with `make flash sensor=S13131` then the
build ignores the LIS-770i code and uses the S13131 code.
