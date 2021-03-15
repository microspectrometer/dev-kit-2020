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

### S13131-512 signals are similar to the LIS-770i

We are lucky with the LIS-770i and S13131-512. They have very
similar functionality in the analog and digital pins. No change
is required to the `vis-spi-out` PCB.

### Six signals on LIS-770i have perfect matches on S13131

For the six signals that have perfect matches, I route the six
S13131 signals to those same six ZIF pins:

- Power and Ground are the same
- VIDEO is the same
- CLK is the same
- RST on LIS-770i is ST on S13131
- SYNC on LIS-770i is EOS on S13131

### Remaining signals that don't match are not needed

For the remainder, we just get lucky:

- analog pins:
    - The VREF pin is particular to the LIS-770i, but the
      `vis-spi-out` analog front end expects a VREF, so I make my
      own VREF on the S13131 breakout board.
    - The Vcp pin on S13131 just needs an external capacitor for
      the internal voltage boost circuit. Vcp does not connect to
      the ZIF.
- digital pins:
    - The PIXSELECT pin is particular to the LIS-770i, so this
      ZIF pin is left as NC (not-connected) on the S13131
      breakout board.
    - the TRIG pin on S13131 does not help us because it is just
      CLK inverse; the same microcontroller that generates CLK
      also does readout, so it's simpler to look at CLK
      internally than it is to trigger off of the TRIG signal

## Describe exposure

Look at the datasheet and describe the relationship between the
CLK signal and the microcontroller output that controls exposure
time.

### S13131 exposure signals

- ST idles LOW
- ST goes HIGH to start exposure
    - Bring ST HIGH some time prior to a CLK rising edge (easiest
      is to bring ST HIGH just after a CLK falling edge)
    - Integration time officially starts on the 3rd CLK rising
      edge with ST HIGH
        - the time period from that 3rd rising edge to the 4th
          rising edge marks the first cycle of exposure time
- ST goes LOW to end exposure
    - Bring ST LOW some time prior to a CLK rising edge (against,
      easiest is to do this just after a CLK falling edge)
    - Integration time officially stops on the 9th CLK rising
      edge with ST LOW

This implies that the **minimum** integration time is **9
clocks**. At 50kHz, each clock is 20µs, so **minimum**
integration time is 180µs.

Example showing minimum exposure time:

```
     ┌─── Count all rising edges of CLK starting with first ST HIGH
     │ ┌─ Count rising edges of CLK where ST is LOW
     ↓ ↓
    (x,y)
   Exposure clocks -------> 1,0   2,0   3,0   4,1   5,2   6,3   7,4   8,5   9,6   10,7  11,8  12,9
           ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐
    CLK    │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │
         ──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └
                             ↑           ↑     ↑  ↑                                            ↑
        ST HIGH clocked-in: ─┘           │     │  │                                            │
                        exposure START: ─┘     │  │                            exposure STOP: ─┘
                           ST LOW clocked-in: ─┘  │                   on 9th CLK rising-edge
                                                  │                              with ST LOW
                           ┌─────────────────┐    │
    ST       IDLE LOW      │                 │    ↓
         ──────────────────┘                 └─────────────────────────────────────────────────────
                           ↑                 ↑
                           │                 │
       Wait for CLK LOW    │                 │ 
         to pull ST HIGH: ─┘                 │
                          Wait for CLK LOW   │
                            to pull ST LOW: ─┘
```

In general, integration time is:

- The number of full clocks where ST is HIGH (ignoring high
  time prior to the third CLK rising edge that clocks in ST
  HIGH and ignoring high time after the last CLK rising edge
  that clocks in ST HIGH)
- Plus, nine clock cycles
- -> Integration time is **ST HIGH plus six clocks**
- -> Minimum **ST HIGH** time is two clocks (three rising edges)
- -> Minimum integration time is nine clocks (nine rising
  edges)


## Describe readout

Look at the datasheet and describe the relationship between:

- the CLK signal and the microcontroller I/O involved in readout
- the CLK signal and the VIDEO signal:
    - want to know when the microcontroller should trigger a
      conversion on the ADC
    - want to know if we have enough time to do the ADC
      conversion before the next pixel is ready
    - want to know when the first pixel is ready for readout
    - want to know when readout of all pixels is done

### ADC conversion

- ADC pin CONV goes high for about 4.5µs
    - I think this is the time for the sample and hold
    - target time is 4.66µs
    - a blocking delay loop delays for a total of 45 10MHz ticks
    - that is 4.5µs
    - plus there's some small overhead for the loop
- ADC pin CONV goes low to enable SDO to shift out bits
    - I think the successive approximation happens as the bits
      are being shifted out
- two dummy bytes are written to the UART SPI module to clock out
  the 16 bits of the ADC reading

So the whole ADC sample-convert-and-readout is:

- about 5µs for sample-and-hold
- plus 16 clocks of the 5MHz UART SPI clock for convert and readout

5MHz is a 0.2µs period, so 16 periods is 3.2µs. The total time
then is about 8.2µs.

Since CLK is 50kHz, one clock period is 20µs, so the time from
the CLK falling edge to the next CLK rising edge is 10µs. And
even if the ADC conversion took longer, it would be OK because
nothing sensitive is happening on the CLK rising edge. As long as
the SPI clock stops (i.e., the ADC readout is done) by the next
CLK falling edge (and it definitely is), then there is no issue
with the readout of the previous pixel interfering with the
sampling of the next pixel.

### S13131 readout signals

#### Use CLK instead of TRIG

- TRIG makes sense in a logic circuit (high-speed application)
- TRIG is just extra work in a program (that's us)
    - microcontroller generates CLK
    - microcontroller can detect and respond to a CLK falling
      edge by polling
    - this is much faster than using a pin interrupt to catch
      rising edges of TRIG (because of ISR overhead)

#### Sample a pixel

- Sample pixel voltage on the falling edge of CLK
    - Sample on rising edges of TRIG
    - TRIG is the inverse of CLK
- CLK falling edge marks VIDEO valid
    - VIDEO is guaranteed to be stable at this point
- VIDEO shifts to next pixel on CLK rising edge
    - CLK rising edge marks VIDEO invalid

#### Start readout

- Define TRIG rising edge 1:
    - Count 1 is the first TRIG rising edge with ST LOW
- Sample first pixel on TRIG rising edge 14
- Translate this to CLK falling edges:
    - Start counting CLK falling edges with ST LOW (after ST LOW
      has been "clocked in" with a CLK rising edge)
    - Sample first pixel on 14th CLK falling edge

#### Finish readout EOS tabled

The microcontroller has no problem counting from 1 to 512 to tell
when the pixel readout finishes. And there is no concern that the
S13131 might clock out some other number of pixels.

The main reason to check an End-of-scan signal is to verify that
the microcontroller starts its pixel count at the correct clock
edge.

Eventually, I will check EOS and code CaptureFrame to send an
error message if the EOS signal came at an unexpected clock edge.
But I table that functionality for now. So EOS is wired up and
set up in firmware as an input pin, but I'm not doing anything
with it.

## Describe relationship between exposure and readout 

For example, on the LIS and S13131, readout must start when the
exposure finishes. In other words, there is no option to expose
and then wait some arbitrary time before reading out the pixel
data. The internal logic in the detector is designed to
immediately clock out the pixel voltages some fixed number of
clocks after exposure ends.

### S13131 relationship between exposure and readout

Exposure ends on the 9th CLK rising edge with ST LOW. Sample
first pixel on the 14th CLK falling edge with ST LOW.

Therefore, readout always starts 13.5 clocks after ST LOW.

### Example expose and readout

Here is an example showing ST HIGH for 3 clocks (4 CLK rising
edges). This corresponds to an integration time of 10 clocks (200
µs).

```
   Pixel count ---------------------------------------------------------------------------------------------------------------------> 001---002-││511---512---
                                                                                                                                      ┌──┌──┌──┌││┌──┌──┌──┌──
                                                                                                                                      ↓  │--│  ││││  │--│  │--
14th TRIG rising-edge (CLK falling-edge) with ST LOW: ────────────────────────────────────────────────────────────────────────────────┐  │--│  ││││  │--│  │--
First TRIG rising-edge (CLK falling-edge) with ST LOW: ─┐                                                                             │  │--│  ││││  │--│  │--
                                                        ↓                                                                             ↓  │--│  ││││  │--│  │--
   Clocks waiting for readout to start ---------------> 1     2     3     4     5     6     7     8     9     10    11    12    13    14 │--│  ││││  │--│  │--
   Exposure clocks --------> 1,0   2,0   3,0   4,0   5,1   6,2   7,3   8,4   9,5   10,6  11,7  12,8  13,9     ↓     ↓     ↓     ↓     ↓  ↓  ↓  ↓││↓  ↓  ↓  ↓  
           ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌──┐  ┌││┐  ┌──┐  ┌──
    CLK    │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  ││││  │  │  │  
         ──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘  └──┘││└──┘  └──┘  
                             ↑           ↑           ↑  ↑                                            ↑
        ST HIGH clocked-in: ─┘           │           │  │                                            │
                        exposure START: ─┘           │  │                            exposure STOP: ─┘
                                 ST LOW clocked-in: ─┘  │                   on 9th CLK rising-edge
                                                        │                              with ST LOW
                           ┌───────────────────────┐    │
    ST       IDLE LOW      │                       │    ↓
         ──────────────────┘                       └────────────────────────────────────────────────────────
                           ↑                       ↑
                           │                       │
       Wait for CLK LOW    │    Wait for CLK LOW   │
         to pull ST HIGH: ─┘      to pull ST LOW: ─┘
```

**Always wait for CLK LOW as the cue to change ST.**

It doesn't *really* matter if ST changes states just after a CLK
LOW or a CLK HIGH. But it's more robust to code transitioning
just after a CLK LOW.

This guarantees ST does not change until well-past the minimum
hold time after CLK goes HIGH. The CLK frequency has to increase
a lot before that gets dicy. There is no danger of CLK going too
fast given the speed limit imposed by the ADC conversion rate.

(Transitioning ST just after CLK goes HIGH, on the other hand,
ties the code to the MCU clock frequency -- now I'd have to look
closely at the number of MCU clock cycles consumed between
detecting CLK HIGH and changing ST to make sure I'm not changing
ST too soon.)

- "Pixel count"
    - indicates when pixels are ready for sampling
    - `number` indicates VIDEO is valid
        - number is the literal pixel number being sampled by the ADC
        - sample-and-hold begins on the clock falling edge
    - `---` indicates VIDEO is invalid:
        - ADC needs to finish sampling previous pixel by this time
        - ADC is not sampling next pixel yet
        - invalid period begins on the clock rising edge
- "Exposure clocks"
    - counts the number of CLK rising edges
    - two numbers separated by a comma:
        - first number counts rising clock edges with `ST` HIGH
            - on 3rd edge, integration time starts
        - second number counts rising clock edges with `ST` LOW
            - on 9th edge, integration time ends

- "Clocks waiting for readout to start"
    - counts the number of CLK falling edges



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

## LisConfigs.h

The LIS-770i has a header to define the user-programmable values
for its internal registers. The Configs define the allowed
values. Manually check this list matches the values in the JSON
file.

The S13131 has no configurable registers, so there is no Configs
file.

The vis-spi-out application for the LIS and the test runner for
LIS unit tests include the LisConfigs header.

*Examples:*

```c
// vis-spi-out/src/vis-spi-out.c
#include "LisConfigs.h"

// vis-spi-out/test/test_runner.c
#include "LisConfigs.h"

// lib/test/test_runner.c
#include "LisConfigs.h"
```

LisConfigs includes "LisConfig.h" (note no "s"). This file
defines LIS CONFIG datatypes and declares all the variable names
that are defined in "LisConfigs.h".

Why two files?

The compilation unit with `main()` includes `LisConfigs.h`.

Files that might end up outside the `main` compilation unit avoid
a multiple definition linker error by including `LisConfig.h`
*instead of* `LisConfigs.h`. This is just to make the code
testable. For the final AVR elf, make sure that any file that
includes `LisConfig.h` is in the same compilation unit as the
`main` C file.

For example, `LisConfigs.h` defines:

- `BINNING_ON=0x01`
- `GAIN_1X=0x01`
- `ALL_ROWS_ACTIVE=0x1F`

And when files that only include `LisConfig.h` are in the `main`
compilation unit, the assembly is correct like this:

```avra
    binning = BINNING_ON;
     1cc:	81 e0       	ldi	r24, 0x01	; 1
     1ce:	80 93 1a 01 	sts	0x011A, r24	; 0x80011a <binning>
    gain = GAIN_1X;
     1d2:	80 93 34 01 	sts	0x0134, r24	; 0x800134 <gain>
    active_rows = ALL_ROWS_ACTIVE;
     1d6:	8f e1       	ldi	r24, 0x1F	; 31
     1d8:	80 93 33 01 	sts	0x0133, r24	; 0x800133 <active_rows>
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
