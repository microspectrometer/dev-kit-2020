# Table of Contents
- [Repo links](#markdown-header-repo-links)
- [Status](#markdown-header-status)
    - [lib organization](#markdown-header-lib-organization)
    - [estimate progress](#markdown-header-estimate-progress)
    - [track progress by lib](#markdown-header-track-progress-by-lib)
- [Internal deadline](#markdown-header-internal-deadline)
    - [Deliverables for August](#markdown-header-deliverables-for-august)
    - [Deadline](#markdown-header-deadline)
- [LIS-770i project code organization](#markdown-header-lis-770i-project-code-organization)
- [Abstract memory-mapped-io for tests](#markdown-header-abstract-memory-mapped-io-for-tests)
- [Mockist TDD](#markdown-header-mockist-tdd)
- [FT1248](#markdown-header-ft1248)
    - [How FT1248 relates to USB](#markdown-header-how-ft1248-relates-to-usb)
    - [FT1248 overview](#markdown-header-ft1248-overview)
    - [FT1248 protocol and Ft1248 C library](#markdown-header-ft1248-protocol-and-ft1248-c-library)
    - [FT1248 setup](#markdown-header-ft1248-setup)
    - [FT1248 starts communication with a combined command and bus-width byte](#markdown-header-ft1248-starts-communication-with-a-combined-command-and-bus-width-byte)
    - [How the MCU manages USB communication using FT1248 ](#markdown-header-how-the-mcu-manages-usb-communication-using-ft1248-)
        - [Check if there is any unread data from the USB host](#markdown-header-check-if-there-is-any-unread-data-from-the-usb-host)
        - [Read the data from the USB host](#markdown-header-read-the-data-from-the-usb-host)
    - FT1248 reference
        - [FT1248 format of combined command and bus-width byte](#markdown-header-ft1248-format-of-combined-command-and-bus-width-byte)
        - [FT1248 combined command and bus-width byte for an 8-bit bus](#markdown-header-ft1248-combined-command-and-bus-width-byte-for-an-8-bit-bus)
- [SPI](#markdown-header-spi)
- [Program Flash](#markdown-header-program-flash)
    - [Cable connections and switch settings](#markdown-header-cable-connections-and-switch-settings)
    - [Verify programming communication link](#markdown-header-verify-programming-communication-link)
    - [Programming flash with `make`](#markdown-header-programming-flash-with-`make`)

---e-n-d---

# Status
## Next step
- [x] embedded test of debug LEDs on the `mBrd`
    - the debug LED demonstrates I can program the `mBrd`
- [x] write a lib to control these four debug LEDs
    - the debug LED is a way to run tests on the `mBrd`
    - the easiest thing to do is:
        - eliminate `static uint8_t debug_led_` as a value passed in by
          `DebugLedInit()`
        - instead make this a value passed into the actual calls, for example:
        - `DebugLedTurnOn()` becomes `DebugLedTurnOn(uint8_t pin_number)` where
          the `pin_number` is defined in `DebugLed-Hardware.h`.
        - for the existing `simBrd` code, this changes to
          `DebugLedTurnOn(`debug_led`) since the hardware only defines one debug
          LED.
        - do not go back and change existing code now
            - it is a little messy
        - make a new version of the DebugLed lib just for the mBrd
        - call it DebugLeds
- [x] embedded test of lib DebugLeds on the `mBrd`
- [ ] Spi
    - [ ] document Spi stuff in the `README`, just like you did for `FT1248`
    - [ ] develop the Spi lib
    - [ ] testing the SPI lib is a little more tricky:
        - both the SPI master and SPI slave need working code
        - this is why I need the debug LED on the `mBrd` working first
- [ ] then next embedded test application:
    - control the mBrd debug LED through the simBrd using lib Usb and lib Spi

## Latest improvements
### lib USB passed its embedded system tests
- [x] PASS embedded system tests of lib Usb
    - This was a major hurdle to overcome.
    - There is Python code for the USB host to receive bytes from the embedded
      system.
    - The FT1248 hardware settings are proven. They are different from the
      Earhart FT1248 settings.
- This `simBrd.c` file will probably become a `test-simBrd-Usb.c` file.
- [x] The final test is to echo bytes
    - [x] create `EchoByte()`
        - this combines `UsbWrite` and `UsbRead`
    - [x] `EchoByte()` works for read buffers large enough to read the entire rx
      buffer in one call
    - [x] `EchoByte()` works when the read buffers is only 1-byte in length
        - in this case, each `UsbRead` terminates after reading a single byte
          because the read buffer is full
        - after writing the byte, `UsbRead` is called again because the rx
          buffer still has data
        - my implementation of `UsbRead()` is robust: it terminates after
          reading all bytes or after running out of space in its read buffer
        - this made writing `EchoByte()` shockingly easy

## lib organization
### libs for communication
#### Ft1248
- communication between the MCU and the USB bridge
- split into porcelain and plumbing
- porcelain calls are for interfacing with a higher-level lib
- plumbing calls eliminate comment-noise in the porcelain calls
- lib Usb only touches the porcelain

#### Usb
- encapsulates Ft1248
- user never needs to know about Ft1248 communication at the firmware level:
    - use writes a USB host application with the FTDI d2xx lib
    - these libraries have nothing to do with Ft1248 communication
    - my goal was to keep the interface as simple as possible
    - write the USB host application looking at the embedded `main` code:
        - `main` code makes calls to Usb lib
        - Ft1248 lib calls are hidden
- nothing to do with USB protocol
    - all USB protocol is done by the USB bridge
    - so no `open`, `close`, or `device-name` functionality
        - that all happens at the application level on the USB host
- but I call this lib *Usb* because that is the connecting cable
    - Usb is a bad name because this has nothing to do with USB
    - but it is the natural name for the USB host application developer to
      look for when searching the embedded code to develop their USB
      communication code
    - what else could I call it? Ft1248 naturally divided into three levels
      of abstraction:
        - bottom: each bare-metal bit-push
            - example: `FtActivateInterface` to *pull SS low*
        - middle: coordinated bit-pushes that achieve a single purpose and
          so are wrapped into a single function
            - example: `FtRead` to read one byte from the receive buffer
        - top: combining and looping those many functions into three simple
          abstractions: ready, read, write
            - example: `UsbRead` to read all bytes from the receive buffer

#### Spi
- master SPI on simBrd talks to slave SPI on mBrd

#### Adc-spi
- USART module on mBrd is the SPI master

#### Lis-io
- readout
- programmable setup

#### I2c
- talks to the i2c slave LED driver on the RGB LED board
- RGB LED board connection is available on both the mBrd and simBrd
- useful for dev and demos

### other libs

#### DebugLed
- controls the DebugLed
- simBrd has one debug LED
- mBrd has four debug LEDs

#### ReadWriteBits
- reads and writes bits given a register name and a bit number
- abstracts one level above the bit-mask operations
- eliminates comment-noise in the low-level libs

## estimate progress
- estimate percentage finished: `18%`

### estimate details:
- *number of tests written* is a crude progress indicator
- for the purposes of estimating:
- assume 50 tests per module
    - this is a bit of an over-estimate
    - it is crude fudge factor to allow for system tests and embedded tests
- treat Usb and Ft1248 as one module split into two libs
- ignore ReadWriteBits because it is done
- ignore DebugLed because it works for one LED
- number of module used in estimate: `5`
- estimated number of tests total: `250`
- number of tests written so far: `57`
- number of tests written that are part of the estimated total: `45`

### system tests
- *system tests* are done as needed and become part of the test suite
- these are part of the automated test suite
- uses hardware fakes
- no mocks:
    - high-level lib makes call to *actual* low-level lib

### embedded tests
#### overview
- *embedded tests* are done each time a lib is finished and there is a new
  testable something
- these are manual test applications
- runs on actual hardware
- testing requires reprogramming the MCU and manually interacting with the
  hardware:
    - look at debug LED
    - read response on screen in serial communication script
- the embedded test code is structured like a series of tests even though there
  is no test runner. I am the test runner.
- This means I can revisit past embedded tests, I don't just see it pass and
  then delete it.
    - example: revisiting passing embedded tests helped me identify the correct
      `FT1248 Hardware Settings`
#### comparison of embedded tests and lib unit tests
##### manual vs automated
- lib unit tests are automated, so they are easy to run
- embedded tests are manual, making them a pain in the ass to check
- all lib unit tests are run anytime the lib code base changes to confirm
  that the code base still behaves as promised
- once an embedded test passes, it is only run again when trying to debug some
  unexpected behavior by revisiting past behaviors to sanity check that they
  have not broken
##### code examples vs code as the specification doc
- lib unit tests read as a specification of the lib functionality
- an embedded test reads as a contract: run this test to see this
  behavior
- embedded tests sometimes read as checks on the lib functionality, but
  not always
    - example of a check on lib functionality:
      `UsbWrite_took_the_happy_path_if_debug_led_is_green`
    - in actual applications, the debug LED really will be green if `UsbWrite`
      follows its happy path
- sometimes the test name is just me doing something to make the test result
  observable
- in this case, the test is not confirming any lib functionality, it is just
  illustrating a code snippet with some behavior under some use case and
  describing how to verify that the correct behavior occurs
    - example: `Turn_debug_led_red_when_there_is_a_byte_to_read`
    - in actual applications, the debug LED will not turn red when there is a
      byte to read
    - but turning the LED red made it easy to check that the embedded system is
      able to detect when there is a byte to read

## track progress by lib
- [x] Ft1248
    - 28 tests
    - 17 functions
- [x] Usb
    - 17 tests
    - 4 functions
- [ ] Spi
- [ ] Adc-spi
- [ ] Lis-io
- [ ] I2c
- [x] ReadWriteBits
    - 10 tests
    - 5 functions
- [x] DebugLed for one LED
    - 2 tests
    - 9 functions
    - TODO: rewrite this test code
        - simplify hardware interface and eliminate dumb initialization
        - pin down all the behaviors of this lib
            - split `init` into two tests:
                - `init turns led on`
                - `init turns led green`
            - add tests for the plumbing behavior
- [ ] DebugLed for four LEDs
    - *goal*:
        - indicate a four-bit error code redefinable on a per-test basis
    - *minimum viable*:
        - this is done, just use one LED!
    - *just beyond minimum viable*:
        - treat as a single LED
        - but design the interface to be open for extension

# Internal deadline
## Context
- This was originally a writeup for Kulite.
    - Kulite went with a TSL1401 prototype instead, so they never saw this.
- But the time breakdown is still useful internally:
    - use the 16-day estimate to finish embedded work
    - final deliverable describes minimum viable to further internal goals
- The crossed-out dates are from the original estimate.
    - I assumed my first working day back from Sensors Expo would be `7/2`
    - But after Sensors Expo follow-up, meetings, July 4th, and Kurt's birthday
      on July 6th, my first day back on this project ended up being `7/9`

## Deliverables for August
- High-resolution spectrometer prototype using LIS-770i and n=7 die with
  custom-machined housing to mate with 20mm x 20mm LIS-770i daughterboard and an
  attachment for a #8-32 set-screw for mounting on an optical post.
- Wavelength map.
- Instructions on acquiring a single frame of data over USB.

## Deadline
[x] Estimated deadline for deliverable prototype to ~~ship to Kulite~~ use
internally: ~~`8/14`~~ `8/17`

- broken into two deadlines
    - Embedded development
    - measurement interface

### embedded development
[x] Estimated deadline for embedded development: `8/7`

#### 2018-07-19 updated estimate for embedded development
- total number of tests: `205`
- find total labor time: `205 tests * 15 min/test = 3075 minutes = 51.25 hours`
    - estimate number of actual days: `62.5 hours / 4 hours/day = 12.8125 days`
- plot days on calendar to find finished date
    - start the clock `7/19`
    - count forward 13 days to `8/7`
#### 2018-07-09 estimate for embedded development
- total number of tests: `250`
    - estimate tests per module: `50`
    - estimate number of modules: `5`
    - total number of tests: `50 tests/module * 5 modules = 250`
- labor time and actual time are different
- convert labor time to actual time using productive percentage of day: `50%`
    - this percentage accounts for any-time not spent on this project
    - e.g., meetings, PCB design, eval kits
- estimate actual labor time per day: `8 hours/day * 50% = 4 hours/day`
- find total labor time: `250 tests * 15 min/test = 3750 minutes = 62.5 hours`
    - estimate number of actual days: `62.5 hours / 4 hours/day = 15.625 days`
- plot days on calendar to find finished date
    - start the clock the week after SensorsExpo: first day is ~~`7/2`~~ `7/9`
    - count forward 16 days to ~~`7/25`~~ `7/31`

### measurement interface
[x] Estimate deadline for measurement interface:`8/16`
#### 2018-07-19 estimate for measurement interface
- goal is to finish in time for board meeting
- start date: `Deadline for Embedded development`
- that gives me 7 days
- let the time limit set the goals
- Estimate deadline for measurement interface: `8/16`

---
- old goal: modified monochromator setup for generating a map
- revised goal: plot live data in LabVIEW

#### 2018-07-09 estimate for measurement interface
- working from a time limit: two weeks
    - let the time limit set the goals
- start date: `Deadline for Embedded development`
- estimate deadline for measurement interface: ~~`8/8`~~ `8/14`

---
- goal: modified monochromator setup for generating a map
    - main hurdle:
        - modify LabVIEW communication for the new firmware
    - approach:
        - this is throw-away code to ~~meet the Kulite deliverable~~ demo to PVP
        - Python interface is longer term
    - Tanya is handling machining of custom housing
        - this mounts to the LIS-770i daughterboard
    - laser cut base plates to hold the readout electronics:
        - mBrd
        - simBrd
    - no MacroFab orders are required to meet the goal
        - we have several LIS-770i daughterboards
        - we have two of each of the readout boards
    - but I do need to order MacroFab daughterboards to try the *QuikPak* part

### Estimating time per test
#### 2018-07-10 velocity is 1 hour
```python
import datetime as dt
start = dt.datetime.strptime("11:26:01",  "%H:%M:%S")
stop  = dt.datetime.strptime("15:14:32", "%H:%M:%S")
total = stop - start
mean = (total.seconds- 1800)/9
```
I subtracted 1800 seconds to account for a 30-minute lunch break.
*The mean time per test development is 3970 seconds, or
1 hour and 7 minutes.*
- This is much longer than previously. It is two effects:
    - these tests are on a high-level lib `Usb`
    - the high-level lib requires mocking of functions I already had defined
      which requires doing some manual operations
    - this was my first day *back* after Sensors Expo
    - also, some work went into another test that I am not counting in this
      total because the test was not finished -- I realized it was better to
      implement the functionality in these simpler tests first
- Even with those caveats, I should readjust my prediction with a more
  conservative velocity.

#### 2018-06-20 velocity is 15 minutes
- look at git log: what is the average time per test?
    - Vim `gL` logs timestamped commits to `git-commits.md`
    - 2018-06-20 11:48:45 -0400  79 minutes ago FtDeactivateInterface pulls SS high.
    - 2018-06-20 11:43:11 -0400  84 minutes ago FtReadData returns the value on MIOSIO.
    - 2018-06-20 11:27:07 -0400     2 hours ago FtRead reads bytes from MIOSIO.
    - 2018-06-20 10:56:00 -0400     2 hours ago FtIsBusOk returns false if MISO is high.
    - 2018-06-20 10:53:49 -0400     2 hours ago FtIsBusOk returns true if MISO is low.
    - 2018-06-20 10:33:57 -0400     3 hours ago TDD FtBusTurnaround.
    - 2018-06-20 10:12:03 -0400     3 hours ago TDD FtLetSlaveDriveBus.
    - 2018-06-20 10:02:23 -0400     3 hours ago TDD FtPullData.
    - 2018-06-20 09:56:22 -0400     3 hours ago TDD FtLetMasterDriveBus.
    - 2018-06-20 09:40:11 -0400     3 hours ago Document how to abstract memory-mapped io.
- count the earliest commit here as commit 0
    - I don't know how long it took to do the work for this commit
    - I am just using its timestamp to measure how long it took before the next
      commit
    - then there are 9 commits
    - each is a test that I wrote and made pass
    - the total time spanned is 
```python
import datetime as dt
start = dt.datetime.strptime("9:40:11",  "%H:%M:%S")
stop  = dt.datetime.strptime("11:48:45", "%H:%M:%S")
total = stop - start
mean = total.seconds/9
```
*The mean time per test development is 857 seconds, or 14.3 minutes.*

Display the total time in HH:MM:SS by simply printing `total`:
```python
print(total)
```
This yields `2:08:34` (2 hours, 8 mnutes, 34 seconds).

Access the total time in seconds with `total.seconds`:
```python
print(total.seconds)
```
This yields `7714` seconds.

# LIS-770i project code organization
Separate project code into sub-folders by board name:

- simBrd
- mBrd

Libraries are shared:

- libraries are not in a project folder
- the libraries are developed in their own folder called `lib`
- the projects link against the object files in `lib`

# Abstract memory-mapped-io for tests
*The `avr` headers and libraries set up the memory-mapped I/O registers as if
their name is a dereferenced pointer.*

- Use I/O register names and pin names specific to the embedded project instead
  of names specific to the AVR microcontroller.
- Declare these names as `extern` in the lib header.
- Define these `extern` symbols in a file named `lib-Hardware.h`, e.g., lib
  `Ft1248` has a file named `Ft1248-Hardware.h`
- there are at least two versions of this hardware file
    - both have the same name, but they are in different folders
    - one is in the sub-folder for the PCB:
        - e.g., PCB `simBrd` has a subfolder named `simBrd`:
        - `LIS-770i/lib/src/simBrd/Ft1248-Hardware.h`
    - the other is in the `test` folder as a `fake`:
        - `LIS-770i/lib/test/fake/Ft1248-Hardware.h`

## Faking io registers

=====[ `LIS-770i/lib/src/Ft1248.h` ]=====
```c
extern uint8_t volatile * const Ft1248_port;
```
`Ft1248_port` is the name the `Ft1248` lib uses for some memory-mapped IO. What
is the actual address? `Ft1248` lib does not care. It up to the client of
`Ft1248` lib to set the address.

### test code as a client
The test code that tests the `Ft1248` lib is a client of `Ft1248` lib. Here is
some test code to test one of the low-level functions in `Ft1248` lib.

=====[ `LIS-770i/lib/test/test_Ft1248.c` ]=====
```c
void FtOutputByte_outputs_a_byte_on_port_MIOSIO(void)
{
    //TEST_FAIL_MESSAGE("Implement test.");
    //=====[ Setup ]=====
    uint8_t byte_to_write = 0xAB;
    uint8_t expected_miosio = byte_to_write;
    //=====[ Operate ]=====
    FtOutputByte(byte_to_write);
    //=====[ Test ]=====
    uint8_t actual_miosio = *FtMiosio_port; // value in miosio port
    TEST_ASSERT_EQUAL_HEX8( expected_miosio, actual_miosio );
}
```
This test code calls `FtOutputByte()` and `FtOutputByte()` writes to the address
of the `FtMiosio_port`.

The test code runs on my computer. My computer does not have the same addresses
used by the MCU. Those addresses point somewhere in kernel space on my computer.
If I try to access an address in kernel space, the test executable has a
segmentation fault.

Instead, the test code uses an arbitrary address chosen by the compiler. The
client sets up that address by including this fake version of the
`Ft1248-Hardware.h` header file:

=====[ `LIS-770i/lib/test/fake/Ft1248-Hardware.h` ]=====
```c
static uint8_t volatile value_in_fake_Ft1248_port;
uint8_t volatile * const Ft1248_port    =   &value_in_fake_Ft1248_port;
```

The header creates a dummy value of the right number of bytes. Since this is
faking an 8-bit microcontroller, it's just one byte. The symbol with the
`extern` declaration in the lib header file, `Ft1248_port`, is defined here to
point to the address of this dummy value.

### embedded system as a client
The embedded system is another client of the `Ft1248` lib. This is the
production code. It needs to use the actual address of the `Ft1248_port`
specific to the microcontroller target and specific to the PCB the
microcontroller is soldered to.

The embedded system on the `simBrd` PCB includes this version of the
`Ft1248-Hardware.h` header:

=====[ LIS-770i/lib/src/simBrd/Ft1248-Hardware.h ]=====
```c
#include <avr/io.h>  // defines IO addresses and pin names
uint8_t volatile * const Ft1248_port    =   &PORTC; // output (Port out)
```
The `simBrd` uses an `Atmega328P` microcontroller. When I designed the `simBrd`
PCB, I decided `PORTC` would handle the `Ft1248` control signals.

The `avr/io.h` file defines the numeric address of `PORTC` for the `ATmega328P`
microcontroller. `PORTC` is the pnemonic the vendor provides. The pnemonic is
handy for referencing this register in the `ATmega328P` datasheet. And many AVR
microcontrollers have a `PORTC` with similar functionality, even if the specific
address is different on each part.

## Thought process on faking io this way
### run time injection is stupid complicated
My first solution to testing code with IO dependencies was to use *run-time
injection*. See repo `mock-object` for an example. This means that register IO
addresses and pin numbers are loaded into memory at run-time instead of
compile-time.

My libraries have functions that operate on memory-mapped IO, but the library
object files do not specify the addresses and pin numbers, only the datatype
(byte size) of each address. It was up to the client of the library to call an
`init()` function to load the specific values. Those values came from a hardware
header that specified those details. Depending on which hardware header was
included in the client code, the same library functions operated on different
pin numbers for different PCBs, or even different addresses depending on if the
client runs on the embedded target or as a test on a computer.

The only benefit of run-time injection is to allow a single run-time to
instantiate a lib multiple times. But my libs do not define objects. I
do not need that level of complexity. All of my libs only have a single
instance. The instance is created by the compiler and placed in static
memory, and it stays in memory for as long the microcontroller is powered.
There is no reason to make this memory management manual. Since my libs do not
describe objects, there is no benefit to using run-time injection.

Loading the addresses in at runtime has three drawbacks.

- The main drawback is that test fixturing is complicated. Setup and Teardown
  need to load and unload these addresses.
- Similarly, the lib `init()` function is noisy: a huge argument list loads all
  the memory-mapped io data, and the actual initialization functionality is
  buried at the end.
- The last drawback is that I/O data takes up static memory.

### optimizing for sanity and simplicity
Simple is always good. That was the problem with run-time injection. So I went
back to the `just use vendor macros` idea and made the smallest adjustments I
could to address my problems.

The method I used before I embraced TDD is to simply define all I/O dependencies
at compile-time by using the vendor's macro names. This is straight-forward, but
then the code cannot be tested because it only runs on the target
microcontroller. The code is also just less readable.

A small improvement on this is to define project-specific macros to alias the
vendor macro names. I used to do this, purely to make the code easier to read,
but I never tried running tests that used this method. I guess the project code
is tested under this method by including a header that defines the macros using
dummy values instead of the vendor values. This approach still does not solve
the testing problem when it comes to lib code. Library code cannot be developed
separately: the lib code gets compiled with either the vendor macros or the
dummy macros. Depending on the executable target, the libs have to be
recompiled.

A final small improvement on this is to use `const` variables instead of macros.
Library code declares memory-mapped I/O as `extern` in its header file. The same
library object file works with both the test-development target and the embedded
targets because the `extern` definitions are resolved when the final executable
is linked. Each project creates `*lib*-Hardware.h` file that provides the
definition promised by the `extern` declaration in the `*lib*.h` file. These
hardware definition files use the vendor-specific macros. Test clients provide
their own fake hardware files that let the compiler choose the address instead
of using the vendor-specific macro.


### my old notes on this from mock-object
*The rest of this section was copied from repo `mock-object` with slight edits
to match the info above.*

Embedded design has *hardware dependencies*. The hardware dependency is a
memory-mapped I/O register. The dependency is *faked* by creating a variable
that occupies the same number of bytes as the memory-mapped I/O register. The
variable is created in static memory. For me, that means getting an address
starting with `0x1004` such as `0x100403120`. The *ATmega328P* is an 8-bit MCU,
so the fake registers are 8-bit unsigned integers: `uint8_t`.

The `avr` headers and libraries set up the memory-mapped I/O registers as if
their name is a dereferenced pointer. For example, `DDRC = 0xFF` sets the value
in register `DDRC` to `0xFF`. This is equivalent to creating a pointer to a fake
register and dereferencing it to write to the fake register:
```c
uint8_t volatile * const fake_LedRegister;
*fake_LedRegister = 0xFF;
```
- The fake register is *volatile* because the compiler should never assume it
  knows the value. The value might be altered by an interrupt.
- The pointer to the fake register is *const* because `fake_LedRegister` will
  never point to anything else in this executable, just as `DDRC` is the
  permanent alias to a specific address on this microcontroller.

*Faking enables testing on a development system.* It is much faster to compile
and test on the development system than on the target embedded system.

Faking forces writing the project code base in a way that is independent of the
hardware. The hardware details are stored in static memory and injected at
link time when the `extern` declarations in the hardware library's header
file are resolved by linking against a client that includes a specific
`lib-Hardware.h` header.

Now the question is whether the compiler is smart enough to see that the
register addresses are `const` and hardcode them directly as if they were
macros. I should investigate this.

# Mockist TDD
## Mocks are not fakes
- *2017-07-19 Update: only mock when the test requires it*
    - if possible, test behavior without comparing call lists
    - comparing call lists couples tests to implementation details
    - even tests that stub functions to mock-up a scenario by controlling return
      values may not need to compare call lists
    - tests require comparing call lists when:
        - there is no other way to check the behavior
        - the purpose of the test is to describe one path, e.g.: sad path or
          happy path

---
- *This section was copied from repo `mock-object`*
    - take any *mock-heavy* attitudes with a grain of salt `>*.*<`
---
This register faking is not mocking. The fake register is just another
dependency. Mocking is a way to pretend that the dependency exists without it
actually existing. This is done with expectations and stubs.

The test code generates a list of expectations for which functions are called,
the values passed in, and the values returned. This list represents the
developer's intended outcomes for a single path through the code under test.
That single path is the specific case handled by this test. Each test handles a
single path.

All of the functions called by the code under test are stubbed. Create stubbed
implementations of these depended-on-functions. The stubs record how they were
called and they return values specific to the test case. The test ends by
comparing the list of expectations with the list populated by the stubs.

The stubbed implementations are written specifically for a given test case. If
the stub just records that it was called and does nothing else, then the same
stubbed implementation could be used for all test cases. Either way, the setUp
for each test redirects the function names to point at the stubbed
implementations, and the tearDown redirects the function names to point back at
the production code.

Stubbing requires the DOFs are public. This brings up an interesting point about
how test development drives interface design.

I ran into a problem where I could not test that a DOF was called. The DOF was
private, and the only way to check the function was doing what I wanted it to
was to check the DOF was called. I already tested the DOF by writing a
test-wrapper. But now I was calling a function that called the DOF. It doesn't
call the test-wrapper. I didn't want to make the function public just to be able
to stub it from the test code for a client lib.

The other option was to check the value of the register manipulated by the
function. But this requires accessing hardware. I can fake the hardware, but
that is not the right solution. That is testing at the wrong level. I already
know the DOF works. What I am really supposed to be checking is whether the
correct functions are called.

In fact, attempting to spy by faking the hardware does not even work! If I
include the header for the fake registers, I get a linker error because now I
have two separate test object files that both include the header of fake
registers. Since the test object files are compiled separately, the #include
guards do not prevent them from both including the header and defining the
symbols within. The linker throws a multiple definition error.

The right way to write the test is to simply call the public function that calls
the DOFs. The code-under-test should not know/care the DOFs are called. The
purpose of the test is to see that the FUT makes the right calls. This is where
it is hard to write the higher-level lib before writing the lower-level lib. I
do not know what the API should be until I've refactored the lower-level lib
into a higher-level API. Similarly, the refactoring of the lower-level lib is
guided by the desire to keep the API simple, and the test of that simplicity is
the test of the higher-level lib.

*And having read this in the future, here are some more thoughts.*

Mocking C generates a lot of supporting code. A strict isolate-everything
generates too much code. Save mocking for when it is necessary. It is OK for
tests to be coupled. The headache of *several tests suddenly failing* is smaller
than the headache of *ripping up the supporting mock code* when refactoring.
I'll take have occasionally confusing test output over barriers to refactoring.
Again, the result is *write the lower-level libs first*.

# FT1248

## How FT1248 relates to USB
There is one FT1248 master, usually a microcontroller (MCU), communicating with
one or more FT1248 slaves. Each slave is a USB-bridge IC made by FTDI. Multiple
slaves means the FT1248 master can communicate with multiple USB hosts. In my
setup, there is only one USB host. The USB host is a Windows PC.

Even though the MCU is the FT1248 master, from the perspective of USB
communication, the MCU is a *USB device*, while the USB host is, well, the USB
host. A USB host initiates communication with its device. But at the lower level
of USB implementation, the FT1248 master initiates communication with the
USB-bridge! How does this play out? Who is actually initiating communication?

The USB host initiates the top-level conversation with the USB device. Always.
The USB device is a combination of USB-bridge IC and MCU. And viewing the USB
device as the combined effect of those two chips is the key to explaining the
seeming contradiction:

### USB

USB Host (the one that initiates)    |   USB Device (the one that responds)
:----------------------------------: | :-----------------------------------:
Windows PC                           |   USB-bridge + MCU

### FT1248

FT1248 Master (the one that initiates)  |   FT1248 Slave (the one that responds)
:-------------------------------------: | :------------------------------------:
MCU                                     |   USB-bridge

### The USB-bridge abstracts the USB hardware interface
Taking a page from the last *SOLID* principle, *Dependency Injection*, the
USB-bridge is an interface that abstracts the details of USB communication.
The interface lets the USB-host and the MCU both think they are the one to
initiate communication, and even lets them use different communication
protocols!

The MCU does not know it is talking to a USB host, i.e., I *do not
know how USB works*, and yet I wrote a library called `UsbDriver`! The USB host
*does know* it is talking to an FTDI device, because it uses the `d2xx.dll`
library and its associated FTDI API. But *I can forget the details of FT1248
communication* when I write the USB Host application.

### USB host and FT1248 master both initiate communication with the USB-bridge
The USB host writes to the USB device. The USB host trusts that the USB device
will respond, but like with any device, it *has to wait* for that response. It
cannot clock or drive the USB device. The USB-bridge IC tells the USB host when
a response arrives.

After the host writes to the USB device, the data sent is sitting in the
USB-bridge receive buffer. The USB-bridge signals to the MCU that there is data
in the receive buffer. Because the MCU is the FT1248 master, *it is responsible
for polling this receive buffer signal*. When it sees there is data in the
receive buffer, it initiates communication with the USB-bridge to read that
data.

If the USB host sent a command requesting data from the USB device, that means
the USB host is now waiting on a response. Because my USB device (the USB-Bridge
plus MCU) responds *quickly*, I write the USB host application to simply block
until it receives the transmission. The USB Host application continues to
*register* user events like a button press and provide some feedback to the user
that the button was pressed. But the application does not act on those commands
until it receives all of the expected data from the USB device.

### Contract between the USB host application and the FT1248 master firmware
As the FT1248 master, the MCU is in charge of delivering the data to the
USB-bridge. The USB host expects this data to be delivered promptly and
correctly. This is a *contract* between the USB host and the USB device.
When the USB host requests data, the MCU must respond promptly and with the
requested amount of data. This is the contract. The application on the USB host
only runs smoothly if the MCU firmware upholds its side of the contract.

The USB host initiates all USB communication, but it has no way to enforce the
contract. What if the USB device *does not* respond **promptly** and
**correctly**?

If the USB device does not respond **promptly**, the host can set a timeout. I
do this when I talk to the FTDI chip in our monochromator. The response time can
vary quite a bit, so I need to allow for this while guarding against
communication completely hanging.

There are several ways to ensure the USB device responds **correctly**:

- the USB host application requests a specific amount of data
- the USB host application reads until there is no more data in the USB buffer
- the data from the USB device always ends with a byte sequence that indicates
  the end of a transmission
- the data from the USB device has a header that identifies it as the response
  to a specific request from the host

These are not mutually exclusive. A good strategy would use some combination. I
think up until now, I've mostly relied on the *expect a certain number of bytes*
method, and that has actually never failed! I'll continue doing that. I set up
the communication to include a header, but I never actually implemented unique
header IDs, I just ignore the initial sequence of bytes. I might have also added
a sanity check that the USB host checks to see the the receive buffer is empty.
The point is it should not be difficult to make a robust communication protocol
given all of these options. It may even be possible to make a composable
protocol. Decide on the allowed options beforehand. State at the beginning of
each data frame which options are used. Program the USB host to compose a
`DetectEndOfFrame` function on-the-fly based on the data at the start of the
frame.

## FT1248 overview
The FT1248 interface consists of:

- **clock** and **slave select** signals driven by the FT1248 master
- a **data line** driven by the FT1248 slave
- 1, 2, 4, or 8 **bi-directional data lines** driven by the master or the slave,
  depending on the command issued by the master at the start of communication
    - The *Earhart* used **one** *bi-directional data line*.
    - The *LIS USB interface board* uses **eight** *bi-directional data lines*.

The FT1248 interface has *two states*: **inactive** and **active**.

In the **inactive state**, there is *no clock*. All communication is via the
signal on *two data lines driven by the slave*. One is the dedicated slave data
line, `MISO` *master-in-slave-out*. The other is the *LSB* of the
*bi-directional data port*, `MIOSIO[0]` *master-io-slave-io-bit-0*.

`MISO` indicates *if there is data in the receive buffer*. The master polls
`MISO`. If there is data in the receive buffer, the *master initiates a read*.

`MIOSIO[0]` indicates *if there is room in the transmit buffer*. If the *master
wishes to initiate a write*, it polls `MIOSIO[0]`. Once the transmit buffer has
room, the master initiates a write.

The **transmit** and **receive buffers** are each **1kByte**. If there is *at
least one byte of data* in the receive buffer, the slave *indicates there is
data to read*. If there is *at least one empty byte* in the transmit buffer, the
slave *indicates there is room to write*.

In the **active state** the master is clocking the slave to transfer data
between master and slave. On a read, the slave writes data to the bus. On a
write, the master writes data to the bus.

Aside: the FT1248 master has nothing to do with the data once it is written to
the slave's transmit buffer. At that point it is between the USB Host and the
USB-bridge to negotiate reading the data out to the USB Host application.

---

The **active state** is further divided into a **command phase** and a **data
phase**.

In the **command phase**, the master sends a byte that tells the slave whether
it is *reading* (from the receive buffer) or *writing* (to the transmit buffer),
and how many pins (1, 2, 4, or 8) to use for the data transfer.

Following the *command phase* there are one or more **data phases** during which
the actual data is transferred. Once data transfer starts, the interface remains
in the *active* state, transferring data according to the command, until the
master returns the interface to the *inactive* state.

### Bus-turnaround
After the *command phase* and before the first *data phase*, there is a
**bus-turnaround**. During bus-turnaround, if the command was a *write*, the
slave uses `MISO` to *indicate whether the transmit buffer is full*. If the
command was a *read*, the slave uses `MISO` to *indicate whether the receive
buffer is empty*. 

There is a subtle difference in how the *transmit buffer is full* signal is
communicated during the *active* and *inactive* states. During the inactive
state, the slave uses `MIOSIO[0]` to indicate when the *transmit buffer is
full*, and `MISO` to indicate when there is *data in the receive buffer*. During
the active state, the slave uses `MISO` for both *transmit buffer full* and
*receive buffer empty*. This happens during the *bus-turnaround*. Which signal
is communicated on `MISO` depends on whether the command was a *write* (goes
with *transmit buffer full*) or a *read* (goes with *receive buffer empty*). And
during the actual *data phase*, the slave is essentially doing the same thing on
`MISO`, but the signals are called `ACK` and `NAK`. Again, the thing being
*acknowledge* or *not acknowledge* depends on whether the command was a *read*
or a *write*.

### ACK and NAK
During the *data phase*, the slave uses `MISO` to indicate either an
**`ACK`** or a **`NAK`**.

If the command is a *write*, the `ACK` means the transmit buffer has at least
one more byte of space. On the rising clock edge, the master pushes its data,
and the slave pushes its `ACK` or `NAK`. On the falling clock edge, the slave
pulls in the data, and the master reads the `ACK` or `NAK` to determine whether
that byte was successfully written.

If the command is a *read*, the `ACK` means the receive buffer has at least one
more byte of data to read. On the rising clock edge, the slave pushes the byte
of data along with the `ACK` signal indicating that this is valid data. If the
slave sends the `NAK` signal, it means this byte is whatever garbage you get
from reading an empty buffer.

Anytime the FT1248 master receives a `NAK` it returns the interface to the
inactive state. 

## FT1248 protocol and Ft1248 C library
- `FT1248` is the FTDI protocol.
- `Ft1248` is my C library that implements the `FT1248` protocol.
- I implemented `FT1248` entirely in software:
    - the Earhart implementation used the SPI hardware module on the ATtiny84
      because it is 1-bit wide
    - the LIS-770i interface implementation is done in software because it is
      8-bitwide

The following summarizes 8-bit wide `FT1248` using the lib `Ft1248` function
names.

### Polling in the inactive state
In the inactive state, the master is always checking `MISO` to see if there is
data to read:

    FtHasDataToRead

These are the other names I considered: `!FtIsRxBufferEmpty`, `!FtIsRxEmpty`,
`FtIsDataAvailable`, `FtIsReadyToRead`.

After reading data from the USB host, the master is back in the inactive state.
If the data from the USB host was a request for the master to transmit data, the
master first checks `MIOSIO[0]` to see if the transmit buffer has room for new
data:

    FtHasRoomToWrite

These are the other names I considered: `!FtIsTxBufferFull`,
`FtTxBufferHasRoom`, `!FtIsTxFull`, `FtIsReadyToSend`

### Activate the interface
Whether the master wishes to do a write or a read, the master first activates
the interface by pulling `!SS!` low:

    FtActivateInterface

The master clocks the interface to push data onto the bus by pulling `SCK` high:

    FtPushData

The master has to wait to output on the `MIOSIO` port until after pulling `SCK`
high. The slave releases `MIOSIO[0]` on the rising edge of `SCK`. The master can
take as long as it needs to push the data and then pull `SCK` low. The slave
will not read the data until the falling edge of `SCK`.

---

### Read Command
After activating the interface and raising `SCK` to signal pushing data, the
master outputs the *read* command on the `MIOSIO` port:

    FtLetMasterDriveBus
    FtReadCommand

The master clocks the slave with a falling edge, telling it to pull the command
from the `MIOSIO` port:

    FtPullData

### Bus-turnaround after the read command
The master releases the `MIOSIO` port:

    FtLetSlaveDriveBus

The master clocks the slave with a rising edge, telling it to drive `MISO` with
the `RxBufferEmpty` signal:

    FtPushData

The master clocks the slave with a falling edge, indicating that it is going to
pull data from `MISO`:

    FtPullData

The master checks `MISO` to see if it is OK to continue with the data transfer:

    FtIsBusOk

Note this is actually the same as `FtHasDataToRead`, but I am using a new name
because the master is going to call this same function whether the command is a
*read* or a *write*.
During the `bus-turnaround`, the slave uses `MISO` for both *receive buffer
empty* and *transmit buffer full*.

- If it is *not* OK to continue with the data transfer, `FtIsBusOk` returns
  *false*. The low-level command immediately before this was `FtPullData` which
  pulls `SCK` low. This is good. `SCK` should be low before deactivating the
  interface. The master terminates the Ft1248 sesssion by deactivating the
  interface: `FtDeactivateInterface` pulls `!SS!` high again to return to the
  *inactive state*.

- If it *is* OK to continue with the data transfer, the master clocks out the
  data from the slave.

First a rising clock edge to tell the slave to push data onto
the bus and to drive `MISO` with an `ACK` or `NAK`:

    FtPushData

Then a falling clock edge to tell the slave that it is pulling the slave's data
from the bus:

    FtPullData

The master checks `MISO` for an `ACK` to see if this is a valid byte of data:

    FtIsBusOk

Then reads the data from the MIOSIO port:

    FtReadData

The master continues the **push-pull-isOk-readData** loop as long as it wishes
or until it receives a `NAK`.

When the master is done, it does a final pull:

    FtPullData
    FtIsBusOk
    FtReadData

And it leaves `SCK` low.

Lastly, the master pulls `!SS!` high again to return to the *inactive state*:

    FtDeactivateInterface

### Write Command
After activating the interface and raising `SCK` to signal pushing data, the
master outputs the *write* command on the `MIOSIO` port:

    FtLetMasterDriveBus
    FtWriteCommand

The master clocks the slave with a falling edge, telling it to pull the command
from the `MIOSIO` port:

    FtPullData

### Bus-turnaround after the write command
The master releases the `MIOSIO` port:

    FtLetSlaveDriveBus

The master clocks the slave with a rising edge, telling it to drive `MISO` with
the `TxBufferFull` signal:

    FtPushData

The master clocks the slave with a falling edge, indicating that it is going to
pull data from `MISO`:

    FtPullData

The master checks `MISO` to see if it is OK to continue with the data transfer:

    FtIsBusOk

Note this is *not* the same as `FtHasRoomToWrite`. That function checks
`MIOSIO[0]`, this one checks `MISO`.
During the `bus-turnaround`, the slave uses `MISO` for both *receive buffer
empty* and *transmit buffer full*.

The master clocks the data into the slave's transmit buffer. First a rising
clock edge to tell the slave to drive `MISO` with an `ACK` or `NAK`:

    FtPushData

The master pushes its data onto `MIOSIO`:

    FtWriteData

Then a falling clock edge to tell the slave to pull the data from the bus:

    FtPullData

The master checks `MISO` for an `ACK` to see if the data successfully entered
the transmit buffer:

    FtIsBusOk

The master continues the **push-pull-isOk-readData** loop as long as it wishes
or until it receives a `NAK`.

When the master is done, it does a final pull to tell the slave to read the last
byte into the transmit buffer:

    FtPullData

It checks that the buffer had room to write that last byte:

    FtIsBusOk

And it leaves `SCK` low.

The master then releases the `MIOSIO` port:

    FtLetSlaveDriveBus

Lastly, the master pulls `!SS!` high again to return to the *inactive state*:

    FtDeactivateInterface

## FT1248 setup

- `SCK`     : clock signal driven by the master
- `!SS!`    : slave select signal driven by the master
- `MISO`    : master-in-slave-out, *always* driven by the slave
- `MIOSIO`  : eight bi-directional lines

The eight bi-directional lines are driven by the master during a write, and
driven by the slave during a read.

The first bi-directional line, `MIOSIO[0]`, has an additional function. The
slave only drives this line during the inactive state. If LOW, the
transmit buffer is full. During the active state, the slave indicates a full
transmit buffer using `MISO`.

=====[ Summary of MCU pin configuration to be an FT1248 master ]=====
```c
// MISO   is an       input pin
// MIOSIO is an 8-bit input port
// SCK  is an output signal that idles LOW.
// !SS! is an output signal that idles HIGH.

// MISO is always an input.
clr_bit(FT1248_DDR,FT1248_MISO);    // cfg miso as an input pin
set_bit(FT1248_PORT,FT1248_MISO);   // enable pull-up on miso

// MIOSIO is always an input port while in the inactive state.
// MIOSIO is an input in the active state, except during the command phase
// and during the data phase for a write.
MIOSIO_DDR = 0x00;                  // cfg miosio0:7 as an input port
MIOSIO_PORT = 0XFF;                 // enable pull-ups on miosio

// SCK remains low while in the inactive state.
// The first clock edge in the active state is a rising edge.
clr_bit(FT1248_PORT,FT1248_SCK);    // Drive SCK low.
set_bit(FT1248_DDR,FT1248_SCK);     // Make SCK an output.

// !SS! controls whether the interface is in the active or inactive stage.
// !SS! remains high while in the inactive state.
// !SS! goes low to initiate the active state.
set_bit(FT1248_PORT,FT1248_SS);     // Drive !SS! high.
set_bit(FT1248_DDR,FT1248_SS);      // Make !SS! an output.
```

## FT1248 starts communication with a combined command and bus-width byte
FT1248 communication is initiated by the FT1248 master (the MCU). The FT1248
slave (the FTDI chip) signals when there is data in the receive buffer from the
USB host, but it is up to the master to decide when to read the data.

The master initiates communication by sending a single byte that is a
complicated combination of two nibbles:

- one nibble represents a command: 0 for write, 1 for read
- the other nibble represents the bus-width.

This is described on *p.6 of FTDI application note #167*.

See [FT1248 format of combined command and bus-width byte][ft1248-cmd-bw-byte].
[ft1248-cmd-bw-byte]: #markdown-header-ft1248-format-of-combined-command-and-bus-width-byte

## How the MCU manages USB communication using FT1248 
### Check if there is any unread data from the USB host
```c
!chk_bit(FT1248_PIN,FT1248_MISO);   // true:    there is unread data from the
                                    //          USB host in the receive buffer
```
### Read the data from the USB host
Activate the interface by pulling `!SS!` low:
```c
clr_bit(FT1248_PORT,FT1248_SS);     // Pull !SS! low to signal the FT1248 slave
                                    // (the FTDI chip) to release MIOSIO[0].
```
Ft1248 function:

    FtActivateInterface_SsLow

Clock the interface to push data onto the bus by pulling `SCK` high:
```c
set_bit(FT1248_PORT,FT1248_SCK);    // Pull SCK high to signal the FT1248
                                    // master (the MCU) and slave (the FTDI
                                    // chip) to shift their data onto the bus.
```
Ft1248 function:

    FtPushData_SckHigh

Output the command to read by outputting byte `0xC6` on the `MIOSIO` port:
```c
MIOSIO_DDR = 0xFF;                  // First the master drives the data lines.
MIOSIO_PORT = FT1248_CMD_READ8;     // The master outputs the read command
                                    // and specifies an 8-bit-wide bus.
```
Ft1248 function:

    FtOutputTheReadCommand

---
## FT1248 reference
### Hardware Specific Ft1248 Settings
- Run `FT_Prog`
- Scan for devices
- locate the `UMFT221XA`
- Select `Hardware Specific -> Ft1248 Settings`
- 8-bit wide config used on the LIS-770i interface:
    - Clock Polarity High: unchecked
    - Bit Order LSB: *checked*
    - Flow Ctrl not selected: *checked*
- Note this is different from the previous Chromation embedded system using
  FTDI USB bridge ICs:
    - Earhart 1-bit wide config:
        - Clock Polarity High: *checked*
        - Bit Order LSB: unchecked
        - Flow Ctrl not selected: *checked*

#### Flow Ctrl
- if `Flow Ctrl not selected` is unchecked, then the Ft1248 slave does not use
  MISO and MIOSIO[0] to send Rx and Tx buffer status signals
- read this as `Use flow control when the slave is inactive`

#### Bit Order
- I discovered this by trial and error
- the bit order is LSB
- send the byte `0x01`
    - if `Bit Order LSB` is checked
    - then the USB host reads this as `0x01`
    - if `Bit Order LSB` is unchecked
    - then the USB host read this as `0x80`
    - this is exactly what you expect when flipping the bit order

#### Clock Polarity
- when unchecked, clock polarity works as described all throughout the FTDI
  application note and all throughout this README
- unchecked means the CPOL is 0 which means SCK idles low
- since the clock phase CPHA is always 1, this means SCK goes high to signal
  pushing data onto the bus and SCK goes low to signal pulling data from the bus

- I was pretty sure Clock Polarity High should be unchecked
    - I confirmed this by trying it both ways
    - behavior is very strange when clock polarity is checked

- See FTDI Application Note `AN_167`
> When `CPOL` is 1, the idle state of the clock is high.
> When `CPOL` is 0, the idle state of the clock is low.
> There are 4 possible modes which are determined by Clock Polarity (`CPOL`) and
> Clock Phase (`CPHA`) signals. For the FT1248 slave, only 2 of these 4 modes are
> supported. `CPHA` will always be set to 1 in the FT1248 slave because data is
> available or driven on to MIOSIO wires on the first clock edge after `SS_n` is
> active and is therefore sampled on the trailing edge of the first clock pulse.

### FT1248 format of combined command and bus-width byte
The *bus-width* (*BW*) and command are sent in a byte on a subset of the
`MIOSIO[7:0]` pins over one or more clock cycles.

For example, if *BW* is 1-bit:

- bit[7..0] is clocked out on MIOSIO[0]
- takes eight clock cycles

And the opposite example, if *BW* is 8-bit:

- `bit[7..0]` is clocked out on `MIOSIO[7:0]`
- takes one clock cycle

On the **first falling edge** of `SCK`, the FTDI chip gets the **bus-width ID**
and therefore knows whether it's clocking out more bits, or reading the entire
`CMD[3..0]` on the `MIOSIO` pins.

Note that in the Earhart design, `CPOL = 1` (`SCK` idles `HIGH`), so it would be
the first rising edge of `SCK` when the FTDI chip reads the bus-width.

In the design for the LIS-770i interface, `CPOL = 0` (`SCK` idles `LOW`).

- command/bus-width byte format:
    - bit[7] = X
    - bit[6] = CMD[0]
    - bit[5] = CMD[1]
    - bit[4] = 0 if BW=8, X otherwise
    - bit[3] = CMD[2]
    - bit[2] = 0 if BW=4, X otherwise
    - bit[1] = 0 if BW=2, X otherwise
    - bit[0] = CMD[3]
    - if bits[4,2,1] are all X, the BW=1

- if BW=8, all eight pins are used:
    - MIOSIO[7] = X
    - MIOSIO[6] = CMD[0]
    - MIOSIO[5] = CMD[1]
    - MIOSIO[4] = 0 (BW=8)
    - MIOSIO[3] = CMD[2]
    - MIOSIO[2] = X
    - MIOSIO[1] = X
    - MIOSIO[0] = CMD[3]
- if BW=4, only MIOSIO[3:0] are used:
    - MIOSIO[7] = X
    - MIOSIO[6] = X
    - MIOSIO[5] = X
    - MIOSIO[4] = X
    - on 1st clock:
        - MIOSIO[3] = CMD[2]
        - MIOSIO[2] = 0 if BW=4
        - MIOSIO[1] = X
        - MIOSIO[0] = CMD[3]
    - on 2nd clock:
        - MIOSIO[3] = X
        - MIOSIO[2] = CMD[0]
        - MIOSIO[1] = CMD[1]
        - MIOSIO[0] = X

list of commands:


    write                 0x0
    read                  0x1
    write buffer flush    0x4

### FT1248 combined command and bus-width byte for an 8-bit bus
Here are the bytes to output on the MIOSIO port to specify 8-bit-wide read and
write:
```c
                                    // BW:8 (0)────────┐
                                    //                 │
                                    // CMD-nibble:  -01▾ 2--3
#define FT1248_CMD_WRITE8   0x86    //              1000 0110
#define FT1248_CMD_READ8    0xC6    //              1100 0110
                                    // Bit numbers: 7654 3210
```
There are four bits for the *command* nibble and four for the *bus-width*
nibble. A **low** on a particular bus-width bit indicates the bus width. The
other bits in the bus-width nibble should not matter, but to play it safe, the
other bits in the bus-width nibble are pulled high.

# SPI
- lib Spi
- the Ft1248 Master MCU is the SPI master
- the motherboard MCU is the SPI slave
## Reference docs
> `C:\chromation-dropbox\Dropbox\design files and protocols\circuits\pcb design\eagle\projects\Chromation\20150807_SPI_Wand\doc\design on paper\Communication With FLIR Ex-Series Camera.txt`
> `/cygdrive/c/chromation-dropbox/Dropbox/design files and protocols/circuits/pcb design/eagle/projects/Chromation/20150807_SPI_Wand/doc/design on paper/Communication With FLIR Ex-Series Camera.txt`
> `C:\chromation-dropbox\Dropbox\design files and protocols\circuits\mcu\Atmel Studio\LIS-770i_Interface\20151020_LIS-770i_mBrd\src\main.c`
> `/cygdrive/c/chromation-dropbox/Dropbox/design files and protocols/circuits/mcu/Atmel Studio/LIS-770i_Interface/20151020_LIS-770i_mBrd/src/main.c`
## Setup the SPI pins
- see `hardware-connection-and-schematics.pdf`
```powershell
PS> &$lis_hardware
```
- [x] create `SpiMaster-Hardware.h`
- [x] create `SpiSlave-Hardware.h`
    - pinout is identical to `SpiMaster-Hardware.h`
## Setup the SPI registers
- see datasheet
```powershell
PS> &$atmega328p_datasheet
```
- chapter 18 `SPI - Serial Peripheral Interface`
- [x] configure user-defined pins in software
- [x] setup SPI registers to use SPI pins for SPI on Master and Slave
- [x] set SPI Master to run the SPI clock at `fosc/8 = 1.25MHz`
- [ ] setup slave to change debug LED when it receives a byte over SPI
- [ ] setup master to send a byte over SPI when it starts up

### SPI master pin overrides
pin         | direction
----------- | ---------
`Spi_Ss`    | user defined
`Spi_Miso`  | input
`Spi_Mosi`  | user defined
`Spi_Sck`   | user defined

### SPI slave pin overrides
pin         | direction
----------- | ---------
`Spi_Ss`    | input
`Spi_Miso`  | user defined
`Spi_Mosi`  | input
`Spi_Sck`   | input


## SPI communication for requesting a frame
- SPI master requests a frame:
    - `Spi_Ss` goes high to low
        - this places the `mBrd` MCU in *slave* mode
    - SPI master transmits `request_frame`
    - `Spi_Ss` goes low to high
        - this releases the `mBrd` MCU from *slave* mode
- SPI slave parses the request
- SPI slave pulls `Spi_Miso` low when a frame is ready
- SPI master reads out the frame:
    - `Spi_Ss` goes high to low
        - this places the `mBrd` MCU in *slave* mode
    - SPI master does the read out
    - `Spi_Ss` goes low to high
        - this releases the `mBrd` MCU from *slave* mode
## SPI communication with LTC1864LADC
```c
// Configure USART to clock ADC SCK at fosc/2 = 5MHz
    // UBBRn=0 -> XCK clock frequency = fosc/2
    
// Cfg XCK as an output.
    // DDR_XCKn = 1

// Enable SPI mode.
    // UMSELn1:0 = [1 1]

// Configure clock polarity (CPOL) and clock phase (CPHA).
    // CPOL = 0 -> clock idles low -> first edge is rising.
    // CPOL = 1 -> clock idles high -> first edge is falling.
    // CPHA = 0 -> sample first
    // CHPA = 1 -> load first
    // Use CPOL = 1, CPHA = 1:
        // UCPOLn = 1
        // UCPHAn = 1
        // clock idles high
        // load data on first edge (falling edge)
        // sample data on rising edges

// Configure bit-order
    // UDORDn = 
    
// Tx a byte.
//  1.  Wait until bit UDREn in reg UCSRnA is high.
//  2.  Write two bytes to UDRn for a 16-bit transfer.
//  3.  Wait for an interrupt that is triggered when the UART
//      transmission completes. Or, wait until bit RXCn in reg
//      UCSRnA is high.
```
## Standard SPI
- SPI master *always* initiates communication
- Every SPI communication is both a **read** and a **write**
    - master and slave form an 8-bit circular buffer
    - the 8-bits in the slave's transmit buffer get shifted into the master's
      receive buffer
    - the 8-bits in the master's transmit buffer get shifted into the slave's
      receive buffer
## SPI sensors
- many SPI sensors are slaves that only transmit (no receive buffer)
- transmit only scheme:
    - host transmit buffer is garbage
    - clock slave some number of times
    - clock drives the sensor's ADC
    - meaningful data received on last N clocks after the first M clocks.
- the `mBrd` is configurable so it must receive too
## Basic SPI
- SPI master loads a byte into its SPI data register
- SPI master tells its hardware peripheral to send the byte
- SPI slave hardware peripheral receives the byte
    - this is independent of the main program execution
- SPI slave has an interrupt that triggers when a byte is received
    - this interrupt notifies the CPU that a byte was received
    - this is the first time the SPI slave need even be aware that the SPI
      master has been communicating with it.

## SPI with Slave/Chip Select
- There is a fourth SPI signal called Slave Select or Chip Select.
    - SPI master MCU `Spi_Ss` pin can by any general purpose I/O on the
      `ATmega328`
    - SPI slave MCU `Spi_Ss` is a specific pin that ties into the SPI hardware
      peripheral
- When the `simBrd` MCU wants to talk to the `mBrd`, it pulls `Spi_Ss` low to put
  the `mBrd` MCU's SPI hardware peripheral into a state where it is ready to
  receive communication.
    - If the `simBrd` MCU transimts data without pulling `Spi_Ss` low, the
      `mBrd` MCU's SPI hardware peripheral ignores the communication.
    - This allows the `simBrd` MCU to have multiple SPI slaves on the same SPI
      bus (every slave's SCK connected to the master's SCK, etc.), but only
      communicate with one slave at a time. The SPI master has a separate
      `Spi_Ss` pin for each slave.

## SPI simBrd and mBrd hardware
- the `SPI` pin connections on the `mBrd` are exactly the same as on the
  `simBrd`
```c
//  =====[ I/O Register Address ]=====          =====[ Register's Purpose ]=====
uint8_t volatile * const Spi_ddr     =   &DDRB;  // data direction in/out
uint8_t volatile * const Spi_port    =   &PORTB; // output (Port out)
uint8_t volatile * const Spi_pin     =   &PINB;  // input  (Port in)
// =====[ Spi Pin Connection On simBrd and mBrd ]=====
uint8_t const Spi_Ss     =   PB2;    // Slave select
uint8_t const Spi_Mosi   =   PB3;    // master data line: master-out, slave-in
uint8_t const Spi_Miso   =   PB4;    // slave data line: master-in, slave-out
uint8_t const Spi_Sck    =   PB5;    // SPI clock
```
- `simBrd` and `mBrd` are both ATmega328P on 10MHz oscillators
- `mBrd` is the SPI slave
    - `mBrd` uses SPI hardware peripheral
    - `mBrd` configures an interrupt for byte received over SPI
- `simBrd` is the SPI master
    - `simBrd` uses USART hardware peripheral
- SPI master SCK shall not exceed 2MHz.
- SPI master is an ATmega328P with a 10MHz oscillator.
- SPI master shall use SCK = fosc/8 = 1.25MHz.
- `simBrd` MCU `SCK` clocks `mBrd` MCU `SCK` at `fosc/8 = 1.25MHz`
- `mBrd` is SPI master to ADC
    - its `USART` clocks ADC SCK at `fosc/2 = 5MHz`


## SPI master writes to SPI slave
- [ ] continue copying in from
> `/cygdrive/c/chromation-dropbox/Dropbox/design files and protocols/circuits/pcb design/eagle/projects/Chromation/20150807_SPI_Wand/doc/design on paper/Communication With FLIR Ex-Series Camera.txt`
- `simBrd` transmits to `mBrd`:
    - `simBrd` pull `mBrd` `!SS` low
    - `simBrd` sends byte
    - `mBrd` receives byte in SPI hardware buffer
    - when one byte is received, hardware triggers interrupt and `mBrd` reads
      byte from SPI buffer
    - `simBrd` pulls `mBrd` `!SS` high (allowing slave to use its MISO pin as a
      GPIO)

- `mBrd` transmits to `simBrd`:
    - example: `simBrd` is waiting to receive a frame of data
    - `simBrd` is polling MISO
        - MISO goes low when the frame is ready
        - (check that control of MISO is automatically given to the SPI hardware
          module when the `mBrd` is put into slave mode)
    - `simBrd` pulls `!SS` low
        - `mBrd` is automatically placed into SPI slave mode
        - MISO output is controlled by SPI hardware
    - `simBrd` loads a byte of garbage into its transmit buffer
        - this starts a transmission
        - eight bits of garbage are sent to the slave
        - the slave transmits eight bits of frame data at the same time
        - the slave needs to prepare the next byte as soon as it knows the
          previous one has been sent
    - `simBrd` keeps `!SS` low
    - `simBrd` reads the byte received from the slave
    - `simBrd` loads another byte of garbage into its transmit buffer
        - this start another transmission
    - `simBrd` continues this until it has read a full frame (784 pixels * 2
      bytes per pixel = 1568 bytes)
    - `simBrd` checks the frame to make sure it was received correctly
        - not sure how to do this -- want to do a checksum?
    - `simBrd` requests a new frame from the `mBrd`


# UART SPI
- lib UartSpi
- the motherboard MCU is the SPI master
- the ADC is the SPI slave
# Program Flash
## Quick Summary
- check hardware connection: `;mkp`
    - Expect device signature is `0x1E950F`
    - Expect fuse settings are `EXTENDED 0xFF, HIGH 0xD9, LOW 0xF7`
- check build recipe: `;mna`
    - Expect build output is `simBrd.elf` if programming the `simBrd` MCU
- build and check size: `;mka`
    - builds the `Makefile` in the active folder
    - `;nr` refresh the build folder
    - open `avr-size_simBrd.log`
    - `make` reports `Nothing to be done for avr-target` if the `.elf` has not
      changed
    - clean the build to force a rebuild
- clean the build `;mc`
- download flash: `;fa`
- build and download flash: `;mfa`
    - downloads flash whether or not the build needed to be rebuilt
## Memory on the ATmega328
The ATmega328 uses a *Harvard architecture* meaning separate memory and buses
for program and data:

- **2KBytes** of *SRAM* hold data memory
- **32KBytes** of *Flash* hold program memory

Both the *SRAM* and *Flash* use indirect addressing because it is a large amount
of memory to navigate and the buses are only 8-bit. The **32** 8-bit *General
Purpose Registers* use direct addressing.

Static memory, stack memory, and the heap all use the *SRAM*. Static memory is
never a problem. The trouble is with *stack-heap collisions*.

## RAM layout and stack-heap collisions
https://www.nongnu.org/avr-libc/user-manual/malloc.html

- *.data* are initialized variables
- *.bss* are uninitialized variables

These are *static-mem* variables. The standard RAM layout is to place *.data*
then *.bss* at the beginning. Then the *heap* starts.

The *stack-mem* starts at the end of RAM and grows towards the beginning.

The trouble is the *heap* and *stack* can both require arbitrarily large amounts
of memory.
Besides allocating memory dynamically, the *heap* causes trouble if
there is a memory that is allocted but never freed, or if the memory is freed
but RAM because fragmented such that the freed memory is never reallocated.
The *stack* grows as stack frames stack up from nested function calls. The size
of each stack frame depends on how much data is local to that function.

It is hard to predict when the *heap* and the *stack* might collide.

## Cable connections and switch settings
- Set simBrd SW2 to "ISP"
    - program the simBrd: SW1 to "M.ISP" *M for SPI Master*
    - program the mBrd:   SW1 to "S.ISP" *S for SPI Slave*
- Power simBrd from hostPC with mini-B USB cable.
- Power ATMEL-ICE from hostPC with micro-B USB cable.
- Connect ATMEL-ICE to simBrd by connecting the 10-pin ribbon cable to the AVR
  port on the Atmel-ICE, and the 6-pin keyed female socket end of the 10-pin
  ribbon cable to the male shrouded header on the PCB.

## Verify programming communication link
### Correct output
- Expect device signature: `0x1E950F`
- Expect fuse settings: `EXTENDED 0xFF, HIGH 0xD9, LOW 0xF7`

### Do not invoke from command line
- you can invoke from PowerShell like this:
```
PS> atprogram -t atmelice -i isp -d atmega328p info
```
- invoke from Vim instead with the `;mkp` shortcut

### Invoke from Vim
- `cd` to directory that has the `Makefile` with the `avr` targets
```vim
;mkp
```
## Do not manually program flash from the command line
- don't bother, use `make` instead
### Verify a file is loaded from the command line
- TODO: make a Vim shortcut for this
- From PowerShell:
    - Copy the full path to the source.elf to the clipboard.
    - Create a variable:
```
PS> $source = {paste-path-to-source.elf}
PS> atprogram -t atmelice -i isp -d atmega328p verify -fl -f $source
```

## Programming flash with `make`
- each sub-folder in `LIS-770i` has a `Makefile`
    - `lib`
    - `mBrd`
    - `simBrd`
- `cd` to the directory with the right `Makefile`
    - the `Makefile` in `simBrd` and `mBrd` has *avr targets*
    - these are invoked by the following shortcuts
        - `;mkp` - query device
        - `;mkv` - measure voltage
        - `;mna` - show build recipe
        - `;mka` - build
        - `;fa`  - download flash
        - `;mfa` - build and download flash
        - `;f-Space` - close log windows
    - most are safe, but downloading flash with `;fa` will program the wrong
      .elf file if the *pwd* is the wrong sub-folder
    - the `Makefile` in `lib` does not have *avr targets*
        - it is safe to accidentally be in this folder
        - this results in messages like this:
```make
(1 of 1): make: *** No rule to make target 'test_programmer_is_connected'.  Stop.
```
### Diagnostics
#### ;mkp returns MCU device signature and fuse settings
- Expect device signature: `0x1E950F`
- Expect fuse settings: `EXTENDED 0xFF, HIGH 0xD9, LOW 0xF7`
```vim
nnoremap <leader>mkp :make test_programmer_is_connected<CR>
```
- `test_programmer_is_connected` is a target defined in the `Makefile`:
- =====[ `LIS-770i/simBrd/Makefile` ]=====
```make
.PHONY: test_programmer_is_connected
test_programmer_is_connected:
	atprogram.exe --tool atmelice --interface isp --device atmega328p info
```
#### ;mkv returns the target voltage
- Expect `2.94V`
```vim
nnoremap <leader>mkv :make display_target_voltage<CR>
```
- `display_target_voltage` is a target defined in the `Makefile`:
- =====[ `LIS-770i/simBrd/Makefile` ]=====
```make
.PHONY: display_target_voltage
display_target_voltage:
	atprogram.exe --tool atmelice --interface isp --device atmega328p parameters --voltage
```
### Build and Download
#### ;mna returns the .elf build recipe with variables substituted
```vim
nnoremap <leader>mna :make avr-target compiler=avr-gcc -n<CR>
```
#### ;mka builds the .elf
```vim
nnoremap <leader>mka :call CloseTestResults()<CR>
            \:make avr-target compiler=avr-gcc<CR><CR><CR>
            \:call MakeQuickfix()<CR>
```
#### ;fa programs the flash
- `;fa` to download flash
```vim
nnoremap <leader>fa :call DownloadFlash()<CR>
function! DownloadFlash()
    execute "make download_flash"
endfunction
```
#### ;mfa builds and programs the flash
- `;mfa` to build for the MCU and download flash
- there is something wrong with this Vim script
    - the first time I ran it I just saw a bunch of errors
    - the second time I ran it it erased my entire README.md file!
    - the trouble must have been here:
```vim
call MakeQuickfixAndDownloadFlash
```

- but the important stuff works
```vim
nnoremap <leader>mfa :call CloseLogfileWindows()<CR>
            \:make avr-target compiler=avr-gcc<CR><CR><CR>
            \:call MakeQuickfixAndDownloadFlash()<CR><CR>
```
#### ;f-Space closes any open atprogram log windows and quickfix window
- I rewrote this Vim script while fixing `;mfa`
- it is completely safe
- invoke it from any window
- as long as the *pwd* is correct, it does its job
- otherwise, it does nothing

# USB Host Application
## 2017 eval kit `Example Python Interface`
### Use legacy Python environment
- the `s.write()` calls in this example code requires my legacy Python
  environment:
    - Python2.7
    - OS: Windows
- invoke this legacy Python environment from PowerShell:
```powershell
PS> &$python2_os_Windows
```

### needs py27 to write unicode
- this calls `python.exe` in my Windows installation of `py27`:
> `C:\Anaconda3\envs\py27\python.exe`
- all other calls work as expected on `py3` on Windows

### needs Windows to acces the `COM` port
- `Windows` is required to use the `COM` port to identify the serial port.
- a `POSIX` OS complains that `COM` is not a file or directory
- here is the error when attempting to open `Cygwin` (a POSIX OS)
> File "/usr/lib/python3.6/site-packages/serial/serialposix.py", line 268, in
> open
```python
raise SerialException(
    msg.errno,
    "could not open port {}: {}".format(self._port, msg)
    )
```
> serial.serialutil.SerialException: [Errno 2] could not open port COM6: [Errno
> 2] no such file or directory: 'COM6'

### Configure the FT221XA for VCP
- set the FT221XA to use the COM port:
    - run `Device Manager`
    - Find `Usb Serial Converter`
    - right-clik, select `Properties`
    - click on the `Advanced` tab
    - check the box `Load VCP`
    - this uses `VCP` instead of `D2XX`

### Run at the Python REPL
- copy the following Python lines into the clipboard with Vim:
    - `V` to select by line, highlight all lines
    - `"+y` yank into the clipboard
- I added `^M` to the end of each line of script
    - when this is pasted into the *Python REPL* it behaves as if pressing
      `<CR>` at the end of the line
    - I added these `<CR>` keypresses in Vim using `i_Ctrl-B<CR>`
- paste into the *Python REPL* in *PowerShell* with `Alt-Space e p`

#### open serial port
```python
import serial
s=serial.Serial()
s.baudrate = 9600
s.port = 'COM12'
s.open()
```
- note that `pyserial` has improved handling of Windows port names since the
  last time someone used the Chromation `Example Python Interface`
    - `port` takes the Windows `COM` port number as a string
    - `port` no longer takes an `int`

#### write a byte
```python
s.write('\x00')
```
#### Earhart command sequence to get a frame
```python
s.write('\x00\x04')
s.write('\x00\x80')
s.write('\x00\x05')
s.write('\x01\xF4')
s.inWaiting()
```
- expect the response `4L` meaning there are four bytes waiting in the input
  buffer

# Repo links
Link to this repo: https://bitbucket.org/rainbots/lis-770i/src/master/

Clone this repo:
```bash
git clone https://rainbots@bitbucket.org/rainbots/lis-770i.git
```
