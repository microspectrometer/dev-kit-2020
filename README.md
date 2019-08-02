# Status

## Old `external user notes` I wrote in initial Osram repo

### Context
I must have wrote this up imagining I was about to freeze the API. It talks
about a protocol (the SPI slave) that doesn't actually exist. We have a working
SPI slave, but the API is not ready for external use.

### Spectrometer Chip Digital Interface Protocol
- the digital interface makes it easy for client hardware to obtain raw spectra
  from the spectrometer chip
- the digital interface protocol describes how to:
    - request a frame of pixel data
    - detect when the data is ready
    - read the data out
#### A raw spectrum is a frame of dark-corrected pixel data
- each spectrum is a frame of pixel data
    - there are 784 pixels
    - starting from pixel 1 and reading to pixel 784, there are:
        - 13 optically black
        - 1 dummy between the black and active
        - 770 optically active
    - each pixel is a 16-bit value
- the pixel values are dark-corrected in hardware
    - the dark-correction is done prior to digital conversion
    - this is unlike most other spectrometers which require the user to perform
      a dark-correciton step by collecting a separate frame of dark data to
      subtract from all subsequent data
    - the dark reference voltage comes from the optically black pixels, so any
      changes to the dark voltage are accounted for at the time of pixel readout
    - this makes dark-correction more convenient since it is done for you and
      more accurate since it is based on the real-time dark voltage
- the frame of pixel data is *raw* only in the sense that it is not
  power-calibrated, i.e., the magnitude of each pixel response is weighted by:
    - the spectral responsivities of the photodiode array and photonic crystal
    - and as a second-order consideration, any non-linearities in the amplifier
      and ADC of the digital interface
        - at the time of this writing, 2018-09-05, there is saturation, either
          in the output stage of the linear photodiode array or in the ADC input
          stage on the digital interface PCB, that causes gain to have a
          nonlinear effect
        - the nonlinearity is only dependent on the amplitude of the output
          signal, e.g., gain through increased integration time, increased
          amplifier gain, or increased pixel height all manifest this
          nonlinearity
        - the digital interface is being revised to eliminate this saturation
          and restore the expected linearity

#### Context: how pixel data is collected
##### light inside the spectrometer chip
- the spectrometer chip is a linear photodiode array of pixels mated with an
  optical device
- the optical device consists of a photonic crystal and mirrors to structure the
  input light before it interacts with the photonic crystal
- the photonic crystal operation is similar to a diffraction grating in that it
  separates the input light into a continuous spectrum of wavelengths
- each pixel subtends some angular spread of the continuous spectrum, thus the
  continuous spectrum is binned into pixels at the plane where it is incident
  upon the linear photodiode array
##### control of the linear photodiode array
- the linear photodiode array has signal lines for:
    - controlling exposure time
    - clocking out the pixel voltages after exposure
- the pixel voltages are available one-at-a-time on an analog output pin during
  the high-time of the pixel clock
- the dark reference voltage is available at all times on an analog output pin
##### pixel data is collected by the digital interface
- the digital interface is idle until it receives a request for a frame of pixel
  data
- the digital interface collects a frame of pixel data by:
    - exposing the pixels for the desired exposure time
    - running the pixel clock to access all of the pixels for digital conversion
- for each pixel accessed, the digital interface:
    - takes the difference between the analog pixel and dark voltages
    - converts this differential voltage to a 16-bit value
    - stores that 16-bit value in memory
- after the entire frame is in memory, the digital interface signals it is ready
  to transmit the data
- the digital interface ignores any further commands send to it until it has
  transmitted the entire frame of data
- the digital interface begins *listening* again to incoming data from the
  client hardware during the last byte of pixel data transferred out
##### exposure time ranges from 200us to 13.1s
- exposure time:
    - defaults to 10ms
    - is easily changed by the client hardware (see next section)
    - can be set to any value between 200us and 13.1s in increments of 200us
    - 2018-09-05 update:
        - the version sent to Tyler works in 20us steps
        - the range of integration times is 20us to 1.31s
        - this was a mistake, but I did not have time to fix it
        - there was no reason to make this a priority:
            - the output runs into saturation issues when output is large
            - Tyler can also use the gain control to increase signal
            - in my tests with a 5mm LED, I never needed integration times
              beyond 200ms
        - when we ship Tyler the new spectrometers, we will send new PCBs with
          corrected firmware and a new LabVIEW application that sends the
          correct integration time value
#### Communication with the digital interface
- the spectrometer chip digital interface is a SPI slave
- see Spi-Commands.h for the list of commands recognized by the SPI slave
#### SPI overview
- one SPI master communicates with N SPI slaves
- SPI is synchronous meaning that the SPI master controls the speed of
  communication by controlling the SPI clock
- there are two uni-directional data lines: MOSI (master out slave in) and MISO
  (master in slave out)
- there are four possible modes for the relationship between clock and data
    - all SPI slaves expect to operate in one of these four modes
#### Special considerations for the spectrometer chip digital interface
##### SPI mode
- the SPI slave uses the mode known as `CPOL=0 CPHA=0` meaning that:
    - the SPI clock is idle low
    - the data lines are sampled on the rising edge of the clock
    - the next bit to transfer is set up on the data lines on the falling edge
      of the clock
- data is sent MSB (most significant bit) first
##### transfer bytes one at a time and synchronize between transfers
- SPI transfers are done on a byte-by-byte basis:
    - the SPI master starts a byte transfer by pulling Slave Select low
    - the SPI master runs the clock for a total of eight rising edges
    - the master ends the byte transfer by pulling Slave Select high
- therefore if multiple bytes are sent, Slave Select pulses high between every
  byte
    - pulsing Slave Select in between byte transfers adds redundancy in keeping
      the SPI master and slave synchronized
##### delay between transfers when writing consecutive bytes
- when the SPI master is writing consecutive bytes to the SPI slave, leave Slave
  Select high for 16.5us betweeen every byte
    - this guarantees the SPI slave has time to move the received byte into
      memory before receiving the next byte
    - 16.5us is a conservatively long time to wait, but the SPI master never
      needs to write more than a few bytes to the SPI slave for a given command,
      so there is no harm in using a conservatively long delay to guarantee the
      SPI slave is ready for the next byte
##### wait for data-ready signal when reading bytes
- when the SPI master is reading bytes from the SPI slave, wait for the SPI
  slave to signal the data is ready before starting each byte transfer
    - for example, when requesting a frame, the master waits for the slave to
      signal the data is ready
        - this initial wait is relatively long as the SPI slave has to conduct a
          frame readout with the spectrometer chip to buffer a frame of pixel
          data
    - after transferring the first byte of data, the master again waits for the
      slave to signal the data is ready, and so on for all 1568 bytes
        - this guarantees the slave always has the next byte of data ready in
          the SPI transmit buffer before the master reads the byte
##### checking the data-ready signal requires awareness of the tri-state
- the SPI slave signals data is ready by pulling MISO low for 4.5us
- the SPI master guarantees it correctly detects this signal using the following
  macro:
```c
#define MacroSpiResponseIsReady() MacroBitIsClear(Spi_pin, Spi_Miso)
#define MacroSpiMasterWaitForResponse() do { \
    while(  MacroSpiResponseIsReady() ); \
    while( !MacroSpiResponseIsReady() ); \
    while(  MacroSpiResponseIsReady() ); \
} while (0)
```
- this guarantees that
    - low is detected on `MISO` because the SPI slave pulled `MISO` low
        - and not because `MISO` is still recovering from a final low at the end
          of a SPI transmission
        - the first two while loops provide this guarantee
    - the SPI slave is ready for the next data transfer
        - the last while loop is the SPI master waiting for `MISO` to slowly
          pull high
        - the SPI slave has plenty of time to re-enable its SPI hardware module
          while this happens
        - this last while loop guarantees the SPI master does not attempt to
          communicate until the SPI slave is ready for another SPI transfer

#### Commands affect the state of the spectrometer chip digital interface
- the interface is normally in an idle state
- in this state it responds to the commands listed in `Spi-Commands.h`
- once the interface receives a command, it enters a state that must be carried
  to completion before it can hear new commands
    - if the SPI slave expects the command to be followed by additional data
      from the master, then the master must send that data for the SPI slave to
      return to its idle state
    - for example the command to set to exposure time must be followed by two
      bytes (and the SPI slave interprets these two bytes as the desired
      exposure time)
    - if the command requests data from the SPI slave, then the master must
      carry out the expected number of SPI transfers for the slave to shift out
      all of the requested data
    - for example, setting exposure time and requesting a frame both cause the
      SPI slave to send data back to the SPI master
- there is no command to abort a data frame once it has been requested

#### Set the exposure time
- set the exposure time by sending the byte `0x02`
- the `Spi` lib defines this as:
```c
uint8_t const cmd_set_exposure_time     = 0x02;
```
- follow the command with the exposure time as a 16-bit value
- for example, to set the exposure time to 40ms, send `0x02` `0x07` `0xD0`
    - 40ms is `2000` 20us ticks
    - TODO: revise this example for the 200us ticks
    - `2000` as two bytes is `7` for the *MSB* and `208` for the *LSB*
    - `7` in hexadecimal is `0x07` and `208` in hexadecimal is `0xD0`
- the spectrometer chip digital interface echoes back the 16-bit exposure time:
    - the spectrometer chip signals that it has data to send by pulling `MISO` low
      for 4.3us
    - the SPI master waits for this signal, then does two SPI transfers to read
      the two bytes
    - in the eval kit, the SPI master relays this values back to the USB host
      for the application user to verify that the spectrometer is using the
      desired exposure time
- client hardware *must* read this data:
    - the SPI slave ignores any further commands from the SPI master until the
      SPI master does two SPI transfers to shift out these two bytes
#### Request a frame of data
- request a frame by sending the byte `0x01`
- the `Spi` lib defines this as:
```c
uint8_t const cmd_send_lis_frame        = 0x01;
```
- wait 

## Bugs
- [ ] fix bug found on 2018-11-04:
    - spectrometer USB communication times out if only one row is selected and
      the auto-expose command is sent
    - probably a problem with the firmware, not the host PC application
    - also times out on integration times around 300ms with all rows selected

## Next step
- [ ] fix bugs
- [ ] failed to add parameters to auto-expose SPI command
    - this should have been straight-forward
    - the fact I could not get it to work suggests a problem with the SPI
      interface itself
    - I worked on this at the 11th hour in getting the California kits ready, so
      attack this again with more time and patience
- [ ] Provide a trigger signal for Tyler at Osram
    - triggers an LED driver
    - use the SDA and GND pin on the USB-Spi PCB
    - I looked up LED driver solutions for machine vision to answer the
      trigger-requirements question because Tyler has gone silent on this
    - I had two takeaways:
        1. the trigger signal is simply an LED on/off signal, i.e., sketch "B"
           in my email to Tyler
        2. driver delay time is negligible compared with the spectrometer
           integration time
        - conclusion:
            - the trigger signal goes high just before exposure starts
            - the trigger signal goes low just after exposure ends
    - waiting to hear back a confirmation from Tyler that this sounds good
    - if I do not hear back, I think it is safe to proceed with this design
- [ ] design an LED driver circuit with the LT3805 LDO or LT3932 buck to
  demostrate the trigger
- [ ] redesign readout board to have the trigger breakout
- [ ] convert Fluttershy into customer-ready kits:
    - remove dies to paint minor faces
    - paint over Nadia's original black paint job
    - reassemble with gorilla glue
    - place a machined light-block over the assembly
    - characterize with the monochromator and wide-angle, wide-bandwidth LEDs
- [ ] wavelength sweep characterization setup
- [x] revise firmware on simBrd and mBrd to use the correct number of bytes in a
  frame
    - summing mode on:  pixels are binned (392)
    - summing mode off: pixels are not binned (784)
    - [x] correct number of bytes in a frame is driven by the number of pixels
    - [x] revise firmware on mBrd to use the correct number of pixels during LIS
      readout
    - [x] revise firmware on mBrd to send the correct number of bytes in a frame
    - [x] revise firmware on simBrd to request the correct number of bytes in a
      frame
    - [x] host PC app reads the correct number of bytes, driven by the UI
      setting for pixel binning
    - [x] host PC app initializes the LIS config immediately after opening
      communication
        - ensures the host PC app and the two MCU all agree on the number of
          pixels in a frame before any frames are acquired
- [x] find sources of optical background in Fluttershy
    - started 2018-09-05, see email update
    - finished 2018-10-12:
        - reflection off minor faces of die creates structured stray light
          around pixel 300
        - the light input at the input aperture is not completely blocked by the
          black paint Nadia used and this creates a large smooth stray light
          hump at the low pixels
- [x] discover the cause of saturation to inform revising the readout circuit
    - I was loading the output of the LIS-770i with the poorly chosen capacitor
      value for the clock filter
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
- [x] Spi
    - [x] document Spi stuff in the `README`, just like you did for `FT1248`
    - [x] develop the Spi lib
    - [x] testing the SPI lib is a little more tricky:
        - both the SPI master and SPI slave need working code
        - this is why I need the debug LED on the `mBrd` working first
- [x] then next embedded test application:
    - control the mBrd debug LED through the simBrd using lib Usb and lib Spi

## Saturation Threshold Lower Than Expected
### data and notes are here
> `C:\chromation-dropbox\Dropbox\ChromationBD\customers\Osram\demo-collaboration\spectrometer-tests\2018-09-07-saturation-investigation\readme.md`
> `/cygdrive/c/chromation-dropbox/Dropbox/ChromationBD/customers/Osram/demo-collaboration/spectrometer-tests/2018-09-07-saturation-investigation/readme.md`
- NERDTree Bookmark: `fluttershy-saturation-investigation`
- online repo:
> `https://rainbots@bitbucket.org/rainbots/fluttershy-saturation-investigation.git`

## Optical background
### data and notes are here
> `C:\chromation-dropbox\Dropbox\ChromationBD\customers\Osram\demo-collaboration\spectrometer-tests\2018-09-06-optical-background`
> `/cygdrive/c/chromation-dropbox/Dropbox/ChromationBD/customers/Osram/demo-collaboration/spectrometer-tests/2018-09-06-optical-background`
- NERDTree Bookmark: `fluttershy-optical-background`
- online repo:
> `https://rainbots@bitbucket.org/rainbots/fluttershy-optical-background.git`

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

#### SpiMaster and SpiSlave
- master SPI on simBrd talks to slave SPI on mBrd

#### UartSpi
- USART module on `mBrd` is the SPI master
- For reading data from the *ADC* on the `mBrd`
- see PDF sketch `mcu-usart-in-spi-mode-for-adc-spi.pdf`
    - explains USART configuration for MSPIM

##### ADC
- LTC1864L

##### analog overview
- 16-bit SAR ADC with a differential input and a unipolar transfer function
    - Differential input means the analog voltage is the difference between
      inputs IN+ and IN-
        - IN- connects to the LIS-770i VREF (dark reference) output
        - the difference between IN+ and IN- is the dark-corrected pixel voltage
    - Unipolar transfer function means the allowed analog voltages range from 0V
      to 2.5V.
- ~use a 2.5V external reference~
    - original build uses 2.5V, then switched to 2.048V
- new design: use a 1.8V external reference

##### digital interface
- communicate with ADC over SPI to read the pixel voltages as 16-bit values:
    - ADC is the SPI slave
    - ATmega328 on `mBrd` is the SPI master
        - uses the USART in SPI master mode
        - Slave select (or chip select) is just a general purpose i/o
        - the clock is USART XCK
        - MISO is USART RxD
        - there is no MOSI connection: the ADC is a read-only device

#### Lis-io
- readout
- programmable setup

#### I2c
- talks to the i2c slave LED driver on the RGB LED board
- RGB LED board connection is available on both the mBrd and simBrd
- useful for dev and demos
- this lib was dropped because the LED driver part is obsolete
    - use these pins for a hardware LED driver trigger signal instead

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
### [x] Ft1248
- 28 unit tests
- 26 functions
    - 7 API functions for Usb
    - 19 plumbing functions
        - some legitimate plumbing, i.e., low-level functionality that is
          occassionally useful
        - some only exposed for tests
    - 0 private functions
        - I was learning how to mock while writing this lib, so I exposed
          everything for mocking!
- 0 embedded system tests (Usb is the API)
- [ ] TODO: decide if applications using lib `Usb` need to include lib `Ft1248`
    - final application definitely needs to include `Ft1248-Hardware.h`
    - but does it need to include `Ft1248.h`?
### [x] Usb
- 17 unit tests
- 5 API functions
- 0 private functions (Usb is the API for Ft1248)
- 6 embedded system tests
### [x] Spi
- 16 unit tests
- 17 functions
    - 5 API
    - 2 plumbing to wrap AVR asm macros
    - 10 private
        - not counting private implementations for public function pointers
- break down by Master and Slave
- SpiMaster
    - 11 unit tests
    - 10 embedded system tests
        - 8 manual embedded system tests
        - 2 automated embedded system tests
            - tests run on power-up
            - read tests on a USB host by reading all available bytes on the USB
              port
            - automated tests use lib Usb, so these only run on the `simBrd`
    - 2 API functions
- SpiSlave
    - 5 unit tests
    - 9 embedded system tests
        - 5 embedded system tests showing application examples
        - 4 embedded system tests
    - 3 API functions
    - 2 plumbing functions
        - one function to wrap each AVR asm macro
### [x] UartSpi
- 12 unit tests
- 21 functions
    - 2 API
    - 1 plumbing to wrap AVR asm delay loop
    - 18 private
        - not counting private implementations for public function pointers
### LisIo
- [ ] Lis-io
    - [x] initialization
    - [x] clock signal generation
    - [ ] frame readout
### ReadWriteBits
- [x] ReadWriteBits
    - 10 tests
    - 5 functions
### DebugLed
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
### DebugLeds
- [x] DebugLeds for four LEDs
    - *goal*:
        - indicate a four-bit error code redefinable on a per-test basis
        - combine with lib `DebugLed` into a single lib where you say how many
          LEDs there are
    - *minimum viable*:
        - [x] just use one LED!
        - [x] provide plumbing for all four LEDs!
### I2c
- [-] I2c
    - I am not writing this lib anymore
    - the part it was for communication with is now obsolete!
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

See the ATmega328 header for definitions:
`/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/iom328p.h`

Each library has a `lib-Hardware.h` file that includes `avr/io.h` which I think
then uses the `iom328p.h` file via the `-mmcu` compiler flag:

```make
#=====[ AVR ]=====
# atmega328_lib has the .o and .a lib files and the spec file.
atmega328_lib = '/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'

-mmcu=atmega328p -B ${atmega328_lib}
```

And for standard C libraries like `stdint` and `stdbool`, `avrgcc` defines its
own implementations. For example, here is `stdint.h`:

`/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/stdint.h`

The `_BV()` macro for the `bit-value of` is here:
```c
// /cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/sfr_defs.h
#define _BV(bit) (1 << (bit))
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))
```
Of course you cannot use `_BV()` in your libs because they are cross-compiled.

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

# TDD
The primary benefit of TDD is *workflow*. TDD is a sane way to split an
intimidating task into smaller tasks. Each task has a clear goal. Focus on the
small goal while ignoring the intimidating task. An initial straw man task list
gets the first few tasks queued up. As the tasks are developed into tests, the
details become clearer and the task list is revised. By expressing each small
task as code it is very clear:

- how to proceed on the task
- when the task is finished

At some point, you step back and look at the list of completed tasks, i.e., the
list of passing tests, and you realize the initial intimidating task is done.

My old workflow was pretty simple. The drawback is that I end up with code that
I am afraid to make changes to, which means at some point I am no longer able to
add functionality without doing a complete rewrite.

- start writing the application and check the control flow as I go by turning
  the debug LED red
- bootstrap the application: as it grows, I am able to check more stuff, e.g.,
  once there is USB communication, I have more ways to check that the code is
  actually doing what I think its doing
- keep doing this until I've written the whole application

My TDD workflow goes like this. I do not actively use this like a check list,
this is just the flow I naturally follow after practicing TDD for the past 6
months. The embedded part are very recent habits I've noticed forming in the
past week now that I've started working with the actual embedded targets:
`simBrd` and `mBrd`.

- write an embedded test application
    - this is a very small piece of embedded code that tries to do something,
      e.g., write one byte or turn the debug LED red when a condition happens
    - the only goal of this code is that it worksk
    - do not worry about anything else
    - proceed each line with a one-liner comment explaining what the code does,
      e.g., `Load the data and start the transmission.`
        - the need for these is obvious because the code it explains is cryptic
          register names and bit names that require referencing the datasheet to
          interpret
    - if there is a large block of code that works together, write a multi-line
      comment explaining how that block of code behaves under different
      conditions
- get the embedded test to do what it claims
- place the test code in a function that describes the test
- add a multi-line comment to this function explaining all the conditions
  necessary to run this test
- comment the test as passing with the date tested
- commit the passing embedded test
- now turn the embedded code into unit-tested lib code
    - the one-liner comments become the function names in the lib
    - the multiline block comments become the test cases
- write an initial list of tests
- pick a test to work on first
- write a function with the test as the name and a body that asserts failure
- get the test runner to compile and report the failure
- create the `Operate` section: usually this is just calling the function
- if the function does not exist yet, get the test runner to compile
- once it compiles, I am back to a failing test
- create the `Test` section: write the real test and erase the asserted failure
- once I start expressing the test in code, it is clear what I need in terms of
  test setup and teardown
- create the `Setup` and `Teardown` sections
- check that this test fails
- start editing the production code function body to make this test pass
- git commit the passing test
- think about a refactor:
    - if I am unhappy with the production code or the test code
    - and if I have an idea on how to make it better now
    - then refactor and commit the passing refactored code.
    - otherwise stick a `TODO` that says why I am unhappy
    - it will become clear how to refactor after there are more tests in place
- think about how this test affects the test list:
    - often a test like `foo returns true when bar` begs a twin test like `foo
      returns false when not bar`
    - getting the test to pass shows what functions are needed
- continue this until all of the functionality of the passing embedded code is
  captured
- the unit-tested lib code is the refactored version of the original embedded
  code
- in small pieces, replace the embedded code with the lib code and check that
  the embedded test still passes
- refactor the embedded code as much as possible: it should now be dead simple
  to understand when you come back to it in the future
- this will often result in great example code:
    - neat little functions that seem to application-specific to make them part
      of the lib, but will probably come in handy later when all of the libs are
      done and you are working on the final application
- commit the final, refactored, passing, embedded test
- if it is clear how to continue extending the lib code, go back to developing
  the lib, otherwise write some more embedded code

# Mockist TDD
## Mocks are not fakes
### 2017-07-19 Update: only mock when the test requires it
- if possible, test behavior without comparing call lists
- comparing call lists couples tests to implementation details
- even tests that stub functions to mock-up a scenario by controlling return
  values may not need to compare call lists
- tests require comparing call lists when:
    - there is no other way to check the behavior
    - the purpose of the test is to describe one path, e.g.: sad path or
      happy path

### This section was copied from repo `mock-object`
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
symbols within. The linker throws a **multiple definition error**.

The right way to write the test is to simply call the public function that calls
the DOFs. The code-under-test should not know/care the DOFs are called. The
purpose of the test is to see that the FUT makes the right calls. This is where
it is hard to write the higher-level lib before writing the lower-level lib. I
do not know what the API should be until I've refactored the lower-level lib
into a higher-level API. Similarly, the refactoring of the lower-level lib is
guided by the desire to keep the API simple, and the test of that simplicity is
the test of the higher-level lib.

### And having read this in the future

Mocking C generates a lot of supporting code. A strict isolate-everything
generates too much code. Save mocking for when it is necessary. It is OK for
tests to be coupled. The headache of *several tests suddenly failing* is smaller
than the headache of *ripping up the supporting mock code* when refactoring.
I'll take have occasionally confusing test output over barriers to refactoring.
Again, the result is *write the lower-level libs first*.

### And having read this even further in the future

There is a lot of *TDD cruft* on the internet. Anytime anyone claims something
must be done a certain way, search for an opposing opinion before taking the
advice. Here are the ones I have found to be misleading and subsequent internet
searches have validated my experience:

- *Use a framework*
    - frameworks are tools
    - use them when they help
    - do not use them if they get in the way
- *Do not write your own framework*
    - the important thing is to get things done
    - testing is important if it helps me get thing done
    - if I am not using a framework but I am testing, i.e., getting things done,
      then the framework is just extra work
    - if I can quickly add the functionality myself to get the job done, e.g.,
      using a global to stub a return value, then I should
    - if it is faster to use the framework, I should do that
- *All tests must be isolated -- isolate by mocking everything*
    - mocking comes at a price
    - it is the tool of last resort
    - if tests are expressible without mocks, do not mock

The industry wisdom is to *use mocking when appropriate*. There are tradeoffs to
mocking:

- function are exposed in the lib header file that would otherwise be private
- The number of files in the test code increases.
- Noise increases in the test code:
    - more files
    - more setup/teardown
    - more pieces that have to work together to form the actual test
- Noise increases in production code:
    - a function becomes a function pointer
    - this is noise if it is only in service of the test

Sometimes the act of mocking drives refactoring into a form that improves the
API. In this case I am intentionally mocking to figure out how to
refactor. I find this effect happens a lot from testing, not as often from
mocking.

My latest approach is to attempt to write the test without mocks first. I end up
mocking for one or all of these reasons:

- I want a function to return a certain value or a sequence of values.
    - That function needs to be mocked so I can stub it in the test and inject
      my return values through the global that is returned by the stub.
        - see [Hardware behavior is mocked](#markdown-header-hardware-behavior-is-mocked)
        - I actually do not *need* a `mock` object to do this.
        - But I still need to generate most of the *mocking* noise:
            - make a function pointer
            - create functions to stub and unstub
            - do the stubbing and unstubbing in `Setup` and `Teardown`
        - so I do just end up creating the full `mock` object with `;me`
- I want to check that the production code follows some conditional path under
  some condition.
    - e.g.:
        - some *error* causes the *sad path* to execute
        - there is **no way** to check this without logging some of the function
          calls and checking that they were actually called
    - The functions need to be mocked so I can log when they are called.
        - The mocking creates stubs that record themselves in the `mock` object
          when the *function under test* is called.
    - The functions need to be mocked so I can log the expectations set up by
      the test.
        - The mocking creates expectation functions that record themselves in
          the `mock` object when the test runs.
    - The `mock` object records both the function names and the function input
      arguments.
    - The test checks that the correct calls were made in the correct order with
      the correct input arguments.
- I am unhappy with the code and want to refactor but it is not clear how to do
  it.
    - Forget the existing code.
    - Make a mock that looks/feels exactly how I want it to.
    - Write test cases stating what I want under each condition and stub return
      values for each case.
    - Convert the stubbed function into actual production code.

### Hardware connections are faked not mocked
And dealing with harwdare I/O, i.e., registers and pin names, has nothing to do
with mocking. Hardware I/O is faked. Fake it by declaring all registers and pin
names as `extern` in the lib header. Unit-test code resolves the `extern` by
defining the registers and pin names using fake variables that match the
datatypes of the registers and pins. Application code resolves the `extern` by
including the vendor libraries and defining the registers and pin names using
the actual names in the vendor `include` files.

### Hardware behavior is mocked
If the hardware changes a register value under some condition, e.g. an interrupt
flag is set when something happens or something finishes, this is easy to write
unit tests for.

The lib code needs a level of indirection: encapsulate the act
of checking the register value inside a the function. This makes for better lib
code anyway.

Now that there is a function, there is a seam to insert a mock. Mock by stubbing
this function. The purpose of the stub is to give the test code control over the
values returned by the function. Here is an example:

```c
while( BitIsClear(Spi_spsr, Spi_InterruptFlag) );
```
- some context for the above:
    - a `SPI` transfer was started
    - this line of code sits in a loop until the transfer finishes
    - `Spi_spsr` is a register name declared in the lib header
    - `Spi_InterruptFlag` is a bit name declared in the lib header
    - the SPI hardware module sets this bit when the transfer is done

- the test code fakes the register name and pin name as explained in the
  previous section
- a mock is needed to take the place of the SPI hardware module and control when
  the bit is set

I want a unit test to check that the logic in this code is correct, i.e., in a
larger code block, I want to know that this line of code halts execution until
the SPI hardware module sets the bit.

Test this by mocking the SPI hardware. The first step is to encapsulate the bit
checking in a function:

```c
while( !SpiTransferIsDone() );
```

The production code implementation of the function looks like this:

```c
bool SpiTransferIsDone(void)
{
    return BitIsSet(Spi_spsr, Spi_InterruptFlag);
}
```

Now there is a function to stub to take control over the return value. To stub
the function, it must be a function pointer:

```c
bool (*SpiTransferIsDone)(void) = SpiTransferIsDone_Implementation;
static bool SpiTransferIsDone_Implementation(void)
{
    return BitIsSet(Spi_spsr, Spi_InterruptFlag);
}
bool (*SpiTransferIsDone)(void) = SpiTransferIsDone_Implementation;
```

Declare the function pointer `extern` in the lib header:
```c
extern bool (*SpiTransferIsDone)(void);
```

Declaring it `extern` lets the test code reassign it to the stubbed
implementation. Write `Stub` and `Unstub` functions:
```c
static bool (*SpiTransferIsDone_Saved)(void);
void Stub_SpiTransferIsDone(void) {
    SpiTransferIsDone_Saved = SpiTransferIsDone;
    SpiTransferIsDone = SpiTransferIsDone_Stubbed;
}
void Unstub_SpiTransferIsDone(void) {
    SpiTransferIsDone = SpiTransferIsDone_Saved;
}
```
The test calls the `Stub` function in `Setup` and the `Unstub` function in
`Teardown`. The `Stub` function replaces the production code function with its
stubbed version. The `Unstub` function puts the production code version back in.

Lastly, write the stubbed implementation:
```c
bool SpiTransferIsDone_SPIF_example[] = {true};  // default return values
bool *SpiTransferIsDone_StubbedReturnValue = SpiTransferIsDone_SPIF_example;
bool SpiTransferIsDone_Stubbed(void) {
    return *(SpiTransferIsDone_StubbedReturnValue++);
}
```

Declare the stubbed return value `extern` in the mock-lib header:
```c
bool SpiTransferIsDone_Stubbed(void);
extern bool *SpiTransferIsDone_StubbedReturnValue;
```
Declaring it `extern` lets the test code reassign it its own array of return
values that describe the test condition.

I am leaving out the code for recording the call. It is only an extra couple of
lines, but those lines use my `mock-c` library for creating `mock` objects. All
I care about here is taking control over the value returned by the function.
I do not need any `mock-c` functionality for that.

The stubbed return value is a global pointer to an array. The test code
describes the test condition by creating an array of return values, and it runs
the test under this condition by pointing the stubbed return value at this
array.

Each time the *function under test* calls the stub, the stub returns the next
value in the array. And *voila* what would have been an infinite while loop
checking non-existent hardware is instead mocked by the array of return values
simulating some number of *bit checks* until the hardware module sets the flag.

If the test code does not assign the value, the default is to simply use the
`example` value. Note this should not be relied on as a default. If the function
is only called once, the default is fine.

For example, the stub returns `true` the first time the function is called. That
exits the `while` loop and the function is not called again. But if the same
unit test *is* going to call the function again, e.g., a test that involves two
serial transfers, then the test must either reset the stubbed return value to
point at the beginning of the array or make the array long enough to work for
two calls, e.g., `{true, true}`. So it is good practice to always explicitly put
the array of return values in the test and *not* rely on the default *example*
value.

# Design patterns
I use two design patterns. I call them the `API pattern` and the `Hardware
Abstraction pattern`.

## API pattern
This is the standard C style of modular programming. Start with some
application code and separate details out to libs. The motivation is to make the
application code easier to reason about.

All of my simple APIs are function calls. A more complex API provides abstract
data types and function calls to work with instances of those abstract data
types.

The application code includes the lib header so that it can access the
API provided by the lib. The lib has functions and variables the application
does not need. These are kept private by keeping their declarations out of the
header, so the application cannot see these. The application uses functions and
variables with the same name without a name conflict when the application object
file is linked with the lib object file.

### The lib header is only declarations for the API or for testing
*The lib puts declarations in the header, but no definitions.* The declarations
are functions, function pointers declared extern, and abstract data types
(forward-declared structs).

#### function declarations
- **function declarations** are the API calls
- Example:
```c
uint8_t SpiSlaveRead(void);
```

#### function pointer extern declarations
- **function pointer extern declarations** expose lib private functions for
  testing
- Example:
```c
extern uint8_t (*ReadSpiStatusRegister)(void);
```
- the client application is not *supposed to* call this function, but it has to
  be exposed for testing: the best I can do is indicate this with commented
  headings to separate the porcelain from the plumbing
- in the above example, `test_Spi_MockUps.c` provides the recipe for
  `test_Spi.c` to reassign `ReadSpiStatusRegister` to a stubbed definition;
  `test_Spi.c` assigns it to the stub during test setup and reassigns it to the
  actual implementation during test teardown
- this way, `test_Spi.c` mocks `ReadSpiStatusRegister` while testing other code
  that calls it
- the mock enables the test to verify the function is called, was passed the
  correct inputs if any, and to control the value it returns if any

- **function pointer extern declarations** also enable tests to redirect API
  calls
- Example:
```c
extern void (*SpiSlaveSignalDataIsReady)(void);
```
- same idea as redirecting private function calls, except here it is an API call
  that needs mocking in the test suite
- not all API calls need mocking, so some are left as function declarations

#### abstract data type declarations
- **abstract data type declarations** are the API objects
- I don't have any in LIS-770i API, but the idea is the client uses the API to
  interace the object without knowing its internal structure which is hidden in
  the lib.c file. There's not enough memory on the microcontroller to store
  large data structures. Most of the memory is taken up by the LIS frame
  readout. But I do use an abstract data type in my `mock-c` lib.
- Example:
```c
// mock-c/include/Mock.h
typedef struct Mock_s Mock_s;
```
- the implementation of `Mock_s` is hidden, but API calls are provided for
  working with the `Mock_s`:
```c
Mock_s * Mock_new(void);
void Mock_destroy(Mock_s *self);
void RecordExpectedCall(Mock_s *self, RecordedCall *func_call);
void RecordActualCall(Mock_s *self, RecordedCall *func_call);
void RecordArg(RecordedCall *self, RecordedArg *input_arg);
void PrintAllCalls(Mock_s *self);  // Example
bool RanAsHoped(Mock_s *self);
char const * WhyDidItFail(Mock_s *self);
char const * ListAllCalls(Mock_s *self);  // alternate to WhyDidItFail()
```

### Lib header has definitions for `inline` and `#define`
Everything in the header is a delcaration, no definitions. But there are two
exceptions:

1. `inline` functions: To enable the compiler to inline a lib function in the
   client code, the definition must go in the lib header (and the declaration
   goes in the lib .c file to emit the symbol when the lib is compiled).
2. `#define` macros: The presence of a `#define` macro in one of my lib headers
   indicates a kludge fix.

## Hardware abstraction pattern
The lib depends on a value specific to the application: a pin name, register
name, or vendor-specific macro. Breaking this dependency makes the lib re-usable
with other microcontroller targets. More importantly, it makes the lib testable
on the development target.

### Scheme for variables: extern in lib header, defined in client
The lib header declares the variable `extern` and does not define it. The
variable is linked to the definition in the client when lib.o is linked to
the client.o. If client.o is the application, this is the actual hardware value.
If client.o is the test code, this is a fake value. This may not seem too
important for pin names, but register names it is essential. The address space
used on a microcontroller is completely forbidden to the user space on the
development computer that runs the unit tests.

Here are the details on how this plays out for a variable.

#### Variable definition goes in the client code
The definition goes in the application. To reduce noise in the application code,
this goes in its own header:
```c
// mBrd/src/Spi-Hardware.h
uint8_t const Spi_Ss    =   PB2;    // slave select driven by the master
```
which the application includes:
```c
// mBrd/src/mBrd.c
#include "Spi-Hardware.h"       // map SPI I/O to actual hardware
```

The lib test code follows the same structure. The definition goes in a fake
version of this hardware header:
```c
// lib/test/fake/Spi-Hardware.h
uint8_t const Spi_Ss    =   2;    // slave select driven by the master
```
which the test includes:
```c
// lib/test/test_Spi.c
#include "fake/Spi-Hardware.h"  // fake hardware dependencies in Spi.h
```

#### Variable `extern` declaration goes in the lib header
The lib code uses the variable defined in the client. The variable must be
defined `extern` in the lib so that the variable name in `test_lib.o` is linked
with the symbol defined in `client.o`.

```c
// lib/src/Spi.h
extern uint8_t const Spi_Ss;
```
and of course the lib.c can see this variable because it includes lib.h:
```c
// lib/src/Spi.c
#include "Spi.h"
```


### Scheme for macros: same as scheme for variables
The implementation looks a little different but the idea is exactly the same.
Unlike the variables, macros are not tied to a specific lib. For example, many
libs call the interrupt enable and disable macros or the delay macros. So the
macros go in their own header-only lib. And this headery-only lib doesn't
actually contain the macros, just declarations for pointers to functions that
are called in place of the macro:
```c
// lib/src/AvrAsmMacros.h
extern void (*GlobalInterruptEnable)(void);
extern void (*GlobalInterruptDisable)(void);
```

The macro is wrapped in a function in an `AvrAsmMacros.c` that is compiled and
linked with the application code:
```c
// simBrd/src/AvrAsmMacros.c
static void GlobalInterruptEnable_Implementation(void)
{ sei(); }
```

The test client is compiled and linked with a fake `AvrAsmMacros.c` that defines
an empty implementation to stub the wrapper:
```c
// lib/test/fake/AvrAsmMacros.c
static void GlobalInterruptEnable_Implementation(void) {}
```

Both source files define a function pointer assigned to the implementation:
```c
// simBrd/src/AvrAsmMacros.c
// lib/test/fake/AvrAsmMacros.c
void (*GlobalInterruptEnable)(void) = GlobalInterruptEnable_Implementation;
```

The function pointer is declared extern in a header-only lib. The header is
included in .c source files that call the function (in lieu of the macro). The
.c source files include libs, test code, and the application.

Here is the header again:
```c
// lib/src/AvrAsmMacros.h
extern void (*GlobalInterruptEnable)(void);
extern void (*GlobalInterruptDisable)(void);
```

The client code and lib code call the function pointer in place of the macro.
When the client is the application, the function call points to a definition
that calls the macro. When the client is the test code, the function call points
to the empty definition. The lib code does not define the function pointer --
calls to the function are resolved when the lib.o is linked to the client.o.

The last part that makes all this work is the build recipe:

```make
# mBrd/Makefile
avr-asm-macros := src/AvrAsmMacros.c
build/%.elf: ${obj_dev-libs} src/%.c ${avr-asm-macros}
```

```make
# simBrd/Makefile
avr-asm-macros := src/AvrAsmMacros.c
build/%.elf: ${obj_dev-libs} src/%.c ${avr-asm-macros}
```

And the Makefile for the lib test code is with the libs:
```make
# lib/Makefile
avr-asm-macros := test/fake/AvrAsmMacros.c
build/TestSuite.exe: ${test-runner} ${avr-asm-macros} ${obj_dev-libs} ${obj_libs-tested} \
${obj_libs-tested-with-mocks} ${obj_libs-mocked} ${obj_shared-lib_mock-c}
```

# Weird embedded stuff
## C to assembly
### Be careful when doing |= vs =
<https://www.avrfreaks.net/forum/resetting-just-one-interrupt-flag-tifr?skey=best%20way%20to%20write%20to%20tifr0>

interrupt flag registers are cleared by writing a one to them

use reg = value

where value is 1 for the flags to clear

do not use reg |= value because that will clear all of the flags!

### `cbi` and `sbi` only happen if you name the registers directly.

Here I am first looking to see that code was inlined as intended.

Next I am horrified at the size of the code that is being inlined, so I get to
the bottom of why `cbi` and `sbi` are not being used.

TLDR: the io reg are *not* memory addresses. Don't treat them like memory
addresses. Directly set the address equal to a value. Treating them like a
memory address still works, but it adds a lot of extra assembly.

#### inline is happening
example:
looking in the `.map` files, we can see the functions using the inlined rw bits
are taking up more space in `.text` memory. Note the address of all *before*
inlining is 0x10.
```map
// before inlining
 .text.DebugLedToggleColor
                0x00000000       0x10 build/DebugLed.o
 .text.DebugLedIsRed
                0x00000000       0x10 build/DebugLed.o
 .text.DebugLedIsGreen
                0x00000000       0x10 build/DebugLed.o
```
```map
// after inlining

 .text.DebugLedToggleColor
                0x00000000       0x22 build/DebugLed.o
 .text.DebugLedIsRed
                0x00000000       0x2c build/DebugLed.o
 .text.DebugLedIsGreen
                0x00000000       0x26 build/DebugLed.o
```

Now notice the address of `DebugLedTurnRed` in the `asm`. And notice that it
just loads its args on the stack frame and then does a `jmp` to `SetBit`.
```asm
// DebugLedTurnRed assembly code in simbrd.lst *before* inlining SetBit
00000100 <DebugLedTurnRed>:
void DebugLedTurnRed(void)
{
    SetBit(port_register_, debug_led_);
 100:	60 91 54 01 	lds	r22, 0x0154	; 0x800154 <__data_end>
 104:	80 91 57 01 	lds	r24, 0x0157	; 0x800157 <port_register_>
 108:	90 91 58 01 	lds	r25, 0x0158	; 0x800158 <port_register_+0x1>
 10c:	0c 94 53 00 	jmp	0xa6	; 0xa6 <SetBit>
```
Here is `SetBit`:
```asm
// SetBit assembly code in simbrd.lst *before* inlining SetBit
000000a6 <SetBit>:
    *port ^= (1<<bit);
}
  a6:	fc 01       	movw	r30, r24
  a8:	40 81       	ld	r20, Z
  aa:	21 e0       	ldi	r18, 0x01	; 1
  ac:	30 e0       	ldi	r19, 0x00	; 0
  ae:	01 c0       	rjmp	.+2      	; 0xb2 <SetBit+0xc>
  b0:	22 0f       	add	r18, r18
  b2:	6a 95       	dec	r22
  b4:	ea f7       	brpl	.-6      	; 0xb0 <SetBit+0xa>
  b6:	24 2b       	or	r18, r20
  b8:	20 83       	st	Z, r18
  ba:	08 95       	ret
```
Every function that calls `SetBit` is doing the `jmp`. Of course the functions
that use the macro version have the macro gobbled by the preprocessor, so I
don't get to see those. But there are enough versions around without the macro
that I can see the old `inline` in the C file did nothing, as expected based on
my new knowledge, and as suspected based on oscilloscope measurements.

Here is the code after inlining correctly:
```asm
// DebugLedTurnRed assembly code in simbrd.lst *after* inlining SetBit

void DebugLedTurnRed(void)
{
    SetBit(port_register_, debug_led_);
  ec:	e0 91 57 01 	lds	r30, 0x0157	; 0x800157 <port_register_>
  f0:	f0 91 58 01 	lds	r31, 0x0158	; 0x800158 <port_register_+0x1>
/* | 2019-03-04 WIP: inline version of ReadWriteBits | */
/* --------------------------------------------------- */

inline void SetBit(uint8_t volatile * const port, uint8_t const bit)
{
    *port |= 1<<bit;
  f4:	20 81       	ld	r18, Z
  f6:	81 e0       	ldi	r24, 0x01	; 1
  f8:	90 e0       	ldi	r25, 0x00	; 0
  fa:	00 90 54 01 	lds	r0, 0x0154	; 0x800154 <__data_end>
  fe:	02 c0       	rjmp	.+4      	; 0x104 <DebugLedTurnRed+0x18>
 100:	88 0f       	add	r24, r24
 102:	99 1f       	adc	r25, r25
 104:	0a 94       	dec	r0
 106:	e2 f7       	brpl	.-8      	; 0x100 <DebugLedTurnRed+0x14>
 108:	82 2b       	or	r24, r18
 10a:	80 83       	st	Z, r24
 10c:	08 95       	ret
```

#### why the `SetBit` is so long
The actual 10 lines or so that do the `SetBit` operation are almost identical
apart from the different working registers used. Knowing the operation is a
simple sbi or cbi on an i/o register makes it painful to see the disassembly in
this way.

But this makes the code readable.

If needed, go and manually modify the code for speed by directly setting the
register names equal to the desired value.
```asm
inline void DebugLedTurnRed2(void)
{
    PORTC |= (1<<PINC3);
 9a0:	43 9a       	sbi	0x08, 3	; 8
```
This shows how to write the inline call to get the single `sbi`. The same
happens whether the optimization level is `-Os` (size) or `-O3` (speed).

```c
SetBit(Ft1248_port, Ft1248_Sck);
```
Results in `sbi`:
```asm
 97a:	41 9a       	sbi	0x08, 1	; 8
```
The trouble with DebugLedTurnRed was just my dumbass original attempt at
hardware abstraction. I added one layer too many. DebugLed gets *initialized*
with registers and a pin. Stupid.
And I can continue putting functions in functions, as I'd wanted to.

```c
inline void ClockHigh(void)
{
    SetBit(Ft1248_port, Ft1248_Sck);
}
```
I still get:
```asm
 97a:	41 9a       	sbi	0x08, 1	; 8
```

What about a function pointer?

This is absolutely not what you want to do:
```c
static void ClockHigh_(void)
{
    SetBit(Ft1248_port, Ft1248_Sck);
}
inline void (*ClockHigh)(void) = ClockHigh_;
```
The compiler warns you too:
```
src/simBrd.c|171 col 15| warning: variable 'ClockHigh' declared 'inline'
||  inline void (*ClockHigh)(void) = ClockHigh_;
```
And the assembly:
```asm
    ClockHigh();
 97e:	e0 91 2a 01 	lds	r30, 0x012A	; 0x80012a <ClockHigh>
 982:	f0 91 2b 01 	lds	r31, 0x012B	; 0x80012b <ClockHigh+0x1>
 986:	09 95       	icall
```
Let's inline the function instead of the variable.
```c
inline void ClockHigh_(void)
{
    SetBit(Ft1248_port, Ft1248_Sck);
}
void ClockHigh_(void);
void (*ClockHigh)(void) = ClockHigh_;
```
The assembly is the same.
```asm
    ClockHigh();
 97e:	e0 91 2a 01 	lds	r30, 0x012A	; 0x80012a <ClockHigh>
 982:	f0 91 2b 01 	lds	r31, 0x012B	; 0x80012b <ClockHigh+0x1>
 986:	09 95       	icall
 ```
The actual assembler of `CLockHigh_` is good:
```asm
0000093a <ClockHigh_>:
 93a:	39 9a       	sbi	0x07, 1	; 7
 93c:	08 95       	ret
```
But it is inescapably a jump of some sort once the function pointer is
introduced.

The takeaways are:

1. Fix DebugLed.h to match the styles in the other libs. This is a good style
   that results in `sbi` and `cbi` calls.
2. Watchout for special registers with flags. These need to be *set equal* to a
   bit mask that clears the flag.
3. If you use a function pointer to create a test seam or some other code
   abstraction purpose, be aware you pay the price with speed because you cannot
   `inline` away the jump. Go through your code and eliminate this function
   pointer madness on functions that are low-level and called often in the loops
   of higher-level functions. Do this as you introduce the `inline` versions of
   things.

Here is another function style that does result in `sbi`:
```c
DebugLedTurnRed4(&PORTC, PINC3);
```


```c
inline void DebugLedTurnRed4(uint8_t volatile * port, uint8_t pin)
{
    *port |= (1<<pin);
 976:	43 9a       	sbi	0x08, 3	; 8
```

### `cbi` and `sbi` are only for registers `0x00` to `0x1F`
- `MacroDisableSpi()` is defined as `MacroClearBit(Spi_spcr, Spi_Enable)`
    - `0x2c` is the address of register `SPCR`
    - bit 6 is `Spi_Enable`
    - `cbi` is the instruction to clear a bit
- I expected a call to `MacroDisableSpi()` to turn into:
```asm
cbi 0x2c, 6
```
- But instead it turns into:
```asm
in      r24, 0x2c
andi    r24, 0xBF
out     0x2c, r24
```
- Why is this turned into so many instructions in this instance but not other
  instances?

### What happens
- `MacroClearBit` is defined as:
```c
#define MacroClearBit(ADDRESS,BIT)        (*ADDRESS &= (uint8_t)~(1<<BIT))
```
- so the definition does a logical `AND` with the negation of the bit mask
- and the `asm` literally does the `AND` with `1011 1111`

### Why it happens
- the compiler is unable to use `cbi` and `sbi` on these registers
- datasheet page 426 states:
> The CBI and SBI instructions work with registers 0x00 to 0x1F only.
- so the compiler does the bit set/clear as defined in the macro

### Complete example
- look at the disassembly to check this macro is defined correctly:
```C
#define MacroSpiSlaveSendBytes(byte_array, nbytes) do { \
    for (uint16_t byte_index = 0; byte_index < nbytes; byte_index++) \
    { \
        MacroWriteSpiDataRegister(byte_array[byte_index]); \
        MacroSpiSlaveSignalDataIsReady(); \
        while ( !MacroSpiTransferIsDone() ); \
    } \
} while (0)
```
- `MacroWriteSpiDataRegister(byte_array[byte_index]);`
- This turns into a 2-cycle `ld` and a 1-cycle `out`:
```asm
76a:	81 91       	ld	r24, Z+ ;
```
- `X`,`Y`, and `Z` are 16-bit pointers into *SRAM*
- `Z+` means load the value pointed to by `Z` and post-increment pointer `Z`
- `Z` is `byte_index`, so incrementing `Z` is part of the *for loop*
```asm
76c:	8e bd       	out	0x2e, r24	;
```
- `0x2e` is the address of register `SPDR`
- `MacroSpiSlaveSignalDataIsReady();`
    - defined as `MacroClearBit(Spi_port, Spi_Miso);`
```asm
76e:	2c 98       	cbi	0x05, 4;
```
- `0x05` is `PORTB`, `Spi_Miso` is bit `4`
- `MacroDisableSpi()`:
    - defined as: `MacroClearBit(Spi_spcr, Spi_Enable)`
```asm
770:	8c b5       	in	r24, 0x2c	; 44
772:	8f 7b       	andi	r24, 0xBF	; 191
774:	8c bd       	out	0x2c, r24	; 44
```
- `MacroEnableSpi();`
- keep going...

## Compiler optimization
> https://www.avrfreaks.net/comment/2029696#comment-2029696
> 
> Never, ever, ever use -O0. It is simply there as a test mode for the compiler
> developers so they can see the dreadful code that is fed into the optimiser.
> Loads of things (timed stuff) won't work if you build -O0 because the code is
> so slow and bloated it won't meet the timing requirements.
> 
> If you pick -O0 to debug because "it's easy" then you will end up debugging
> something that is quite unlike the -O1..3/s that you eventually release.
> 
> AS7 (since I told them back at the AS5 stage) now uses -O1 (not -O0) for
> "Debug" and -Os for "Release", these are probably sensible choices until -Og
> is more easily available as a choice for "Debug".
> 
> Of course the final release choice between -Os and -O3 kind of depends on the
> way in which you want it optimised (size or speed). User manual here:
> 
> https://gcc.gnu.org/onlinedocs/g...
> 
> As you'll read there the -On/-Os choices are really nothing more than a
> grouping of 10's of -fxxxx options, so if you want even more control you could
> build your own optimisation by some combination of -Ox and -fxxx selections.
> Though that's probably for "power users". The 123s choices are sensible
> groupings for most. Just go with -O3 for speed, -Os for size as a starting
> point.

- TLDR:
    - the AVR community compiles with either -O3 for speed or -Os for size
    - the different optimization levels just set various compiler flags
    - the idea is you are freeing the compiler from a literal interpretation of
      your code and letting it optimize either for size or for speed
    - try the different levels when compiling with avr-gcc:
        - in `CFLAGS` in the Makefile, just change the numbering of -O1 to:
            - -O2 and look for a speedup
            - -O3 and look for even more of a speedup
            - -Os and look for a size reduction
        - size: see what happens to the size of the .elf file
            - see section *Size of the final `.elf` files* below
            - I have not tried this with -Os
        - speed: see what instructions change in the disassembly
        - speed: look for a change in i/o timing on an oscilloscope
            - I did the latter by comparing the hardware-controlled PWM module
              output with a software-controlled pin tracking the PWM output pin
              and measuring the distance between the software-controlled pin
              rise/fall relative to the hardware-controlled pin rise/fall
            - I compared these measurements for different optimization levels:
                - observed no change
            - I compared these measurements for a high-level abstraction with
              true function calls vs the same high-level abstraction in terms of
              readability, but using function-like macros
                - the function calls are a huge slow down
                - the function-like macro maintains the readability, but the
                  code is as fast as it can be without switching to inline
                  assembly
                - I did not attempt inline assembly to look for additional
                  speedup
            - see my lesson learned in section:
            - *Abstracting for readability is a performance hit but there is a
              solution*

## Abstracting for readability is a performance hit but there is a solution
### Context
- I often write one-liner comments to explain the code that comes after
- I then eliminate these one-liner comments by turning that comment into a
  function that calls whatever code it was explaining.
- this makes the code readable:
    - the hard-to-read implementation code is now hidden
    - the same readable text is there, just in CamelCase
    - writing an idea in CamelCase helps me be terse to avoid ending up with
      something that too long to be readable
- this automates code honesty:
    - the compiler guarantees the function is called
        - as opposed to a comment, which the compiler ignores
        - if there is no automatic check that the comment is true, then there is
          no sane way to guarantee the comment is still in sync with the code it
          is explaining
    - tests check the behavior of that function
        - the tests gaurantee that the function does what it claims it does
- this eliminates separate documentation:
    - the test becomes the documentation
    - the test cases list all the behaviors
    - the tests provide multiple examples of calling the function and show the
      minimum viable on the state setup before the function is called
### Problem
- each time a function is called, this adds overhead in execution time
- this overhead is significant when the system has to respond to events within a
  time window that is on the order of a few clock cycles
    - example: fcpu = 10MHz, then a single-cycle instruction takes 0.1us
    - a function call adds the instruction to jump to the function and the
      instruction to return from the function
    - jumping to the function is an `rjmp` or an `icall`
        - `rjmp` takes 2 clocks
        - `icall` takes 3 clocks
    - returning is an `ret`
        - `ret` takes 4 clocks
    - so each call adds at least 6 clock cycles
    - calls add more if the call is passing arguments because those need to be
      pushed into registers that the subroutine will access
    - and a calculation result needs to be stored somewhere as well
    - loading and storing are the `ld` and `st` instructions which each take 2
      clocks
    - so something like setting a bit is at a minimum 12 clocks
    - but that is only 1.2us and while that kind of sucks, it is nowhere near
      the overhead I was observing -- I was seeing more like 5us to 6 us per
      high-level function call
### Solution
#### Do not make function calls
- without any function calls, the execution time was only about 5 clock cycles
- it is hard to imagine this getting any better
- even using interrupts incurs a *call and return* penalty of about 10 clock
  cycles
#### `inline` does not work
- ideally, `inline` is supposed to eliminate the function call and insert the
  function implementation code in its place
    - I could not get `inline` to have any affect
    - perhaps this is because my code is split across multiple files
    - so the object file with the `inline` function call does not have any way
      to get the code inserted in the client application object file that it is
      linked against
#### Use function-like macros
- to maintain readability, I made function-like macros that look and act just
  like the functions I want to inline, but the name has the prefix `Macro`
- to avoid macro pitfalls, I mirrored my existing unit tests with versions that
  test all the same behavior but swapping the `Macro` function-like macros in
  place of the functions
### Summary and Takeaway
- adding function calls slows down execution by adding tens of clock cycles
- this slow down is a problem when the function needs to respond to an event
  within a few clock cycles
- these function calls are added to the code as part of refactoring:
    - the refactoring steps turns the comment into a function call
    - the function call provides readable-and-automatically-verified code
- there is a solution for both fast-and-readable
    - it doesn't have to be slow-but-readable with calls or fast-but-unreadable
      without calls
- develop the code for readability and maintainability by refactoring as usual,
  introducing function calls
- where slow downs are problematic, mirror the function calls with function-like
  macros
    - use the same function name, but add the prefix `Macro`
- the existing unit tests guard against the pitfalls of function-like macros
    - create an identical set of unit tests that swap out the function-like
      macro for the original function
- in the slow-down-problem-areas, use the `Macro` version
    - this is a manual implementation of `inline` for instances when `inline`
      does not do the job

### disassembly instruction analysis
- my C code:
```c
void DemoMacroFastestRstResponseToClk(void)
{
    while (1) // PASS with best results: no ISR, use lowest-level code
    {
        while(MacroBitIsClear(&TIFR0, OCF0A)); // wait for clock rising edge
        MacroSetBit(Lis_port1, Lis_Rst);
        // delay bewteen Clk high and Rst high is 0.4 to 0.6us
        MacroSetBit(&TIFR0, OCF0A); // clear the flag
        while(MacroBitIsClear(&TIFR0, OCF0B)); // wait for clock falling edge
        MacroClearBit(Lis_port1, Lis_Rst);
        // delay bewteen Clk low and Rst low is 0.4 to 0.6us
        MacroSetBit(&TIFR0, OCF0B); // clear the flag
    }
}
```
- the assembly generated by *avr-gcc*:
```asm
00000286 <DemoMacroFastestRstResponseToClk>:
```
...
```
286:	a9 9b       	sbis	0x15, 1	; 21
288:	fe cf       	rjmp	.-4      	; 0x286 <DemoMacroFastestRstResponseToClk>
28a:	5e 9a       	sbi	0x0b, 6	; 11
28c:	a9 9a       	sbi	0x15, 1	; 21
28e:	aa 9b       	sbis	0x15, 2	; 21
290:	fe cf       	rjmp	.-4      	; 0x28e <DemoMacroFastestRstResponseToClk+0x8>
292:	5e 98       	cbi	0x0b, 6	; 11
294:	aa 9a       	sbi	0x15, 2	; 21
296:	f7 cf       	rjmp	.-18     	; 0x286 <DemoMacroFastestRstResponseToClk>
```
- `sbi 0x0b, 6 ; 11`
    - `sbi` [S]et [B]it in [I]/O register
    - takes 2 clock cycles
    - register address `0x0b (0x2b)` is register name `PORTD`
    - bit `6` is bit name `PD6`
        - I set `const uint8_t Lis_Rst` equal to macro `PD6` in `Lis-Hardware.h`
        - note there is no overhead penalty for this abstraction
        - the compiler takes the substitution to the end, using the bit number
          defined by the macro
- `sbi 0x15, 1 ; 21`
    - clear the OCF0A flag by setting the bit
    - takes 2 clock cycles
- `sbis 0x15, 1 ; 21`
    - `sbis` [S]kip if [B]it in [I]/O register is [S]et
    - register address `0x15 (0x35)` is register name `TIFR0`
    - bit `1` is bit name `OCF0A`
    - takes 1/2/3 clock cycles
- `rjmp .-4 ; 0x286`
    - `rjmp` is [R]elative [J]u[MP]
    - takes 2 clock cycles
    - in hand-written code this would jump to a label
    - here I guess the notation `.-4` means *relative to the location of the
      program counter after this line executes* go *back 4 bytes* in program
      memory
- after translating this segment of assembly, I think it is the fastest way
  to express the Rst pin following the Clk pin in software
## AVR asm macros
These are macros defined in `avr-libc`.

### Example
- For example:
    - `Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/interrupt.h` 
- defines these macros:
    - `sei()` - do the SEI instruction: *Global Interrupt Enable*
    - `cli()` - do the CLI instruction: *Global Interrupt Disable*

### I wrap these inside functions
I wrap these macros inside functions because:

- my function names are more readable:
    - for example, `sei()` is wrapped in `GlobalInterruptEnable`
- this allows me to put them in my lib code

Like any other lib code, I put these macros in my lib code to hide the details
from the application code.

- For example, when enabling a specific interrupt:
    - globally disable interrupts
    - clear the pending interrupt, if any, for the specific interrupt being
      enabled
    - *enable the specific interrupt* -- **this is the goal**
    - then globally re-enable interrupts
- the surrounding `cli()` and `sei()` are a precaution that no interrupts occur
  while I enable this interrupt
- clearing any pending interrupt is a precaution against enabling this interrupt
  and having it execute as soon as I return from this function

### Compiling lib code with AVR asm macros
Putting the macro in my lib code has a similar problem to putting a register or
pin name in my lib code. My test environment compilers, gcc and clang, do not
know what these macros are.

I cannot just include the `avr-libc` header that defines the macros. The macro
is inline assembly. The assembly recognized by avr-gcc is not the same assembly
recognized by gcc or clang. The inline assembly will compile fine for the avr
target, but the unit tests will not compile for the development environment.

My solution is similar to how I fake register and pin names.

#### Recall strategy for faking register and pin names

- register and pin names are just macros for numbers
    - e.g.:
```c
#define SPCR _SFR_IO8(0x2C)
#define SPR0 0
```
- I make a `hardware` header file specific to the embedded system target
- the `hardware` header file defines variables and assigns them to the
  register and pin names, just as a variable would be assigned to any literal
  value
- e.g.:
```c
// mBrd/src/Spi-Hardware.h
uint8_t volatile * const Spi_spcr   = &SPCR;  // SPI control register
uint8_t const Spi_ClockRateBit0     = SPR0;
```

This creates a seam for the *stub* or *fake*. It substitutes the macro with a
variable. I need a variable rather than a macro so that I can assign the value
appropriate to the translation unit it is in: the test runner's translation unit
stubs the value, and the application's translation unit assigns the
hardware-specific value. For example, use `uint8_t const` to substitute for a
macro defining an 8-bit register or a bit number. `const` gives me help from the
compiler if I accidentally try to write the variable after initialization. This
replaces `#define` which uses the pre-processor to achieve the same end goal.

While creating this seam I also abstract the name. I suffix the variables with
names reflecting which of my libs they belong to.

- this is just the old *no magic numbers* mantra
- I shift the name from the language domain or hardware domain into the solution
  domain

For example, while developing code, I care about what the pin is used for, e.g.,
the `Spi` lib, so I suffix it:
```c
// mBrd/src/Spi-Hardware.h
uint8_t const Spi_Ss    =   PB2;    // slave select driven by the master
```
I do not care that a pin is on `PORTB` -- that detail was important during PCB
layout and planning for the functionality of the embedded system, but I want to
forget that detail while developing code. My lib header only has the
solution-domain name:
```c
// lib/src/Spi.h
extern uint8_t const Spi_Ss;
```
The same hardware pin can even be used by more than one lib! The lib code is
truly abstracted from the hardware.

To make this scheme work:

- The lib header file:
    - declares those variables as `extern`
    - only has to match the datatype in the `hardware` header file
- My test code is otherwise ignorant of the `hardware` header file and defines
  those variables in a `fake` header folder to mirror how it is done in the
  application code:
```c
// lib/test/test_Spi.c
#include "Spi.h"                // lib under test
#include "fake/Spi-Hardware.h"  // fake hardware dependencies in Spi.h
```
- the `extern` is resolved here:
```c
// lib/test/fake/Spi-Hardware.h
uint8_t const Spi_Ss    =   2;    // slave select driven by the master
```
- My application code defines the hardware values in a separate `hardware` file
  to keep the code clean:
```c
// mBrd/src/Spi-Hardware.h
uint8_t const Spi_Ss    =   PB2;    // slave select driven by the master
```
- And the application code includes the lib header to access the lib and the
  hardware header to define the `extern` hardware abstractions in the lib:
```c
#include <Spi.h>                // Chromation spectrometer is a SPI slave
#include "Spi-Hardware.h"       // map SPI I/O to actual hardware
```


#### Solution for faking AVR asm macros
The solution is a little different because the asm macros are not just numbers,
they are inline assembly code. I cannot create the *faking seam* with a simple
`const` variable.

I create the *faking seam* by wrapping the macro in a function. Similar to
setting up a function for stubbing, I keep the macro wrapper as a private
implementation and expose the function pointer for the API.

Here is the implementation in the `.c` file for the avr targets:
```c
/* =====[ mBrd/AvrAsmMacros.c ]===== */
/* =====[ simBrd/AvrAsmMacros.c ]===== */
#include "AvrAsmMacros.h"
#include <avr/interrupt.h>      // defines macros `sei()` and `cli()`

// sei()
static void GlobalInterruptEnable_Implementation(void)
{ sei(); }
void (*GlobalInterruptEnable)(void) = GlobalInterruptEnable_Implementation;
```

Here is the fake implementation in the `.c` file for the unit test code:
```c
/* =====[ lib/test/fake/AvrAsmMacros.c ]===== */
#include "AvrAsmMacros.h"
/* =====[ Fake the AVR asm macros with empty definitions ]===== */
// sei()
static void GlobalInterruptEnable_Implementation(void) {}
void (*GlobalInterruptEnable)(void) = GlobalInterruptEnable_Implementation;
```

And here is the one lib header used by all clients:
```c
/* =====[ lib/src/AvrAsmMacros.h ]===== */
extern void (*GlobalInterruptEnable)(void);
```

- The avr target uses the inline assembly implementation.
- The unit tests use empty implementations.

I cannot unit test these functions because they do nothing. But I can compile
them without harm and I can fake their effects within the stack frame of a test
case.

## Interrupts
Interrupts are yet another macro to deal with.

- the macro looks like a function that is missing a return type
```c
ISR(SPI_STC_vect)
{
    Turn_led1_red_and_the_rest_green();
}
```
- `avr-gcc` does some useful hidden setup/teardown on entering and exiting the
  interrupt
    - suspend interrupts while this interrupt executes
    - call `reti()` at the end

But I do not like the disconnect between *interrupt enablers* in one part of the
application code and *what the interrupt does* being described in the
body of the interrupt service routine. This really sucks during embedded system
tests. I want the test to control the behavior of the ISR. Different tests need
different bodies for the ISR. And I want to do this without resorting to
conditionals.

I use the *command* design pattern:
```c
/* =====[ Move control over the SPI ISR into the test code ]===== */
typedef void (SPI_ISR_task)(void); SPI_ISR_task *DoTaskForThisTest;
//
ISR(SPI_STC_vect)
{
    DoTaskForThisTest(); // fptr assigned in test code
}
```
The *ISR* just knows it is doing a task. A task is any function that has no
input arguments and returns nothing. The task is selected in the testcase. For
example, here is a test case to check that the ISR is being called. If it is
called, then `debug_led1` turns red:
```c
void SPI_interrupt_routine_turns_debug_led1_red(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    DoTaskForThisTest = Turn_led1_red_and_the_rest_green;
    /* =====[ Operate ]===== */
    SpiEnableInterrupt();
    /* while(0); // exit loop immediately */
    while(1); // loop forever
    DebugLedsTurnAllRed();  // This should *never* be called.
    /* =====[ Test ]===== */
    // Program the SPI Master to send any byte on reset.
    // Visually confirm the debug LEDs are all green.
    // Flip `SW2` to `SPI`. Press the reset button.
    // Visually confirm debug LED 1 turns red.
}
```
The ISR behavior is defined by the test case! I can read what this test does
without ever looking at the ISR macro.

## Read a register and throw away the value
Sometimes it is necessary to read a register to clear a bit flag. The value in
the register is garbage, but reading it is the only way to clear the bit in some
status register. But now there is a *throw-away* value that needs to be dealt
with.

Here is an example:
```c
// Clear the SPI Interrupt Flag bit in the SPI Status Register.
// Implementation:
// Read registers SPSR and SPDR, in that order.
```
First attempt at an implementation:
```c
static void ClearPendingSpiInterrupt(void)
{
    uint8_t volatile throw_away;
    throw_away = *Spi_spsr; throw_away = *Spi_spdr;
}
```
Compiling this code with `gcc` generates a warning that the value is not used:
```make
|| src/Spi.c: In function ‘ClearPendingSpiInterrupt’:
src/Spi.c|92 col 22| warning: variable ‘throw_away’ set but not used [-Wunused-but-set-variable]
||      uint8_t volatile throw_away;
||                       ^~~~~~~~~~
```
It is tempting to write this as inline assembly, but the assembly recognized by
avr-gcc is not the same assembly recognized by gcc or clang. The inline assembly
will compile fine for the avr target, but the unit tests will not compile for
the development environment.

One workaround is to write a wrapper for inline assembly. The wrapper has an
empty implementation used by test code, and an inline assembly implementation
used by the avr-target code. I use this approach to place the `AVR asm` macros
in my lib code.

But there is a simple solution. It turns out the *assignment is not necessary*.
It is sufficient to do this:
```c
static void ClearPendingSpiInterrupt(void)
{
    *Spi_spsr; *Spi_spdr;
}
```
Unfortunately the
[success of this simple approach is compiler-dependent] (https://embeddedgurus.com/stack-overflow/2010/03/reading-a-register-for-its-side-effects-in-c-and-c/
on the compiler).

- [ ] TODO: check the disassembly from `avr-gcc` to confirm the register access
  happens.

If I am disappointed in the results of the `avr-gcc` output, I can use the
`AvrAsmMacros` trick.

## clobbering large arrays when making structs?
- the frame data is a big array:
    - approximately `770 pixels * 2 bytes per pixel = 1540`
- and transmitting large arrays over USB works fine
- but printing test results messes this up for reasons I do not understand
### observations
- if I precede a large array print with a test result print, the array is
  consistently clobbered starting at byte 1042
- if I follow a large array print with a test result print, the array is
  consistently clobbered starting at byte 1385
- if I print an array of 1540 bytes without printing any test results, the array
  prints correctly
### conclusions
- do not print test results
- in general, behavior is very unpredictable with this large array dancing
  around
- I think this is because the array is too large a percentage of the SRAM
- unfortunately I cannot switch it to PROGMEM because I need to write to it!
- I tried for 6 hours to understand the problem, time to move on
    - in that time I heavily characterized what would and wouldn't cause
      problems
    - in each case it was not predictable until I actually did the test
    - the main takeaway is that I need to be careful about even doing simple
      function calls
    - it would be better to offload the data as it comes in
    - I tried just printing out direct every byte, but that doesn't work, I
      don't know why. The SPI communication fails.
- I can print test results, I just cannot pass the `TestResult` struct around
    - fault if I use its members as string variables in `snprintf`
- I also must declare the *total* amount of memory snprintf will write to before
  calling snprintf
- Here is the final working example:
```c
    char test[94];  // assume 93 char is the biggest `TestResult` message
    char pcb_name[] = "simBrd";
    char pass_fail[] = "PASS";
    snprintf(test, 94,
            /* "\n%ssimBrd %stest " */
            "\n%s%s %stest "
            "Get_a_frame_from_slave_and_write_frame_to_USB_host"
            /* ":%sPASS%s\n", */
            ":%s%s%s\n",
        text_color_white, pcb_name,
        text_color_grey,  // test_name
        text_color_white, pass_fail,
        text_color_reset
        );
    uint16_t nb = strlen(test);
    UsbWrite((uint8_t *)test, nb);
```
- It really sucks that any of the steps to turn this into a function cause a
  fault
- I don't know why I can pass `pcb_name` and `pass_fail` as string variables,
  but I cannot do that for the `test_name` or the debug LED does not even turn
  on
- so I am stuck just copy-and-pasting this code wherever I need it
- even attempting to automate the color of pass/fail causes a little array
  clobbering, even if I am not passing the whole TestResult struct!

### wait but why
- why is printing coupled to the memory for the array?
    - this is probably an `snprintf` issue
    - I am calculating the message length at runtime
    - that is OK, but not for determining the maximum amount of memory
    - but I need to give the compiler a safe maximum to reserve, and then the
      program decides how much to actually use
- solving those problems was not enough
    - sending two frames is no problem
    - and reading from the slave is no problem
    - the problem is 100% due to attempting to print a test result either before or
      after printing the array
    - so what is special about my test-result printing call?
    - it uses `snprintf` -- possibly that is accessing memory it should not be
    - yep, `snprintf` causes the problem, but so does a straight call to `UsbWrite`
      with mixed up signedness of the pointers, as well as use of the `TestResult`
      struct in building a string to write with `snprintf`
- the issue is the SRAM is only 2K
    - I am consuming 1.54K just to hold an array
    - it does not take much to overrun into this array by accident
    - I'm losing SRAM to stack frames from all the function calls
    - I'm losing SRAM to data like the `TestResults` struct which holds a long
      string
    - so it does not take much to step onto the array
- the solution is to *not store the array*
    - the tradeoff here is speed
    - sending each byte as I get it is definitely slower
- I can probably speed up if I send the data out in half-frames
- alternatively, I can do an 8-bit version for now
- but yes, success, now I can go back to my printing routines with no problem
#### for future refactoring
- use `vsnprintf` instead?
- it's a good solution once I have my array-clobbering problem sorted, but this
  will not help with that
- plus I have to learn to write variadic functions...
> https://embeddedgurus.com/stack-overflow/2009/02/effective-c-tips-1-using-vsprintf/
> https://www.avrfreaks.net/forum/passing-valist-snprintf
- it is in `stdio.h` along with `snprintf`:
> `https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html`
> `https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gac92e8c42a044c8f50aad5c2c69e638e0`
```c
int vsnprintf(
        char * __s,
        size_t __n,
        const char *__fmt,
        va_list ap
        )
```
> Like vsprintf(), but instead of assuming s to be of infinite size, no more
> than n characters (including the trailing NUL character) will be converted to
> s.
> 
> Returns the number of characters that would have been written to s if there
> were enough space.

- The example from Nigel Jones:
```c
// char array creation happens here
void display_Write(
    uint8_t row,
    uint8_t column,
    char const * format,
    ...
    )
{
    va_list args;
    char buf[MAX_STR_LEN];

    va_start(args, format);
    vsprintf(buf, format, args); /* buf contains the formatted string */
    
    /* Send formatted string to display - hardware dependent */
    
    va_end(args);                /* Clean up. Do NOT omit */
}
// example client code
void display_Time(int hours, int minutes, int seconds)
{
    display_Write(3, 9, "%02d:%02d:%02d", hours, minutes, seconds);
}
```
## pointer signedness
```make
warning: pointer targets in initialization differ in signedness [-Wpointer-sign]
```
- this happens because `UsbWrite` deals in raw bytes, so it takes pointers to
  unsigned bytes:
```c
UsbWrite((uint8_t *)test, nb);
```
- but in the example above, `test` is a string, which is a pointer to a char
- even though `char` and `uint8_t` are identical, `avr-gcc` complains this is a
  sign change from `unsigned` to not `unsigned`
- the attempted `cast` in the above snipped does not eliminate the warning
  because I have CFLAGS `-Wall` and `-Wextra -pedantic`
- solution: add CFLAG `-Wno-pointer-sign` to eliminate the warning
```make
#=====[ Compiler and Linker flags ]=====
CFLAGS = -I../lib/src -Isrc \
	-g -Wall -Wextra -pedantic -Wno-pointer-sign\
	-O1 -ffunction-sections -fdata-sections -fshort-enums \
		-mmcu=atmega328p -B ${atmega328_lib}
```

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
### Hardware Specific Ft1248 Settings in ftprog
- Run `FT_Prog`
- Scan for devices
- locate the `UMFT221XA`
- Name it `Chromation Mod UsbSpi Bridge`
- Select `Hardware Specific -> Ft1248 Settings`
- *8-bit wide config used on the LIS-770i interface:*
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
## SPI dev tasklist
### This is the dev cycle
#### start with embedded tests to discover working code
- Example: this is how I started working on the SPI libraries
    - [x] configure user-defined pins in software
    - [x] setup SPI registers to use SPI pins for SPI on Master and Slave
    - [x] set SPI Master to run the SPI clock at `fosc/8 = 1.25MHz`
    - [x] setup slave to change debug LED when it receives a byte over SPI
    - [x] setup master to send a byte over SPI when it starts up
#### switch to unit tests to refactor working code
- look at the functions in the embedded tests:
    - one-liner comments describing what the next line does become the function
      names
    - each claim in the function documentation becomes a unit test
### Track progress in the current dev cycle
#### ! embedded tests to discover new code
- [x] SpiMaster
- [x] SpiSlave
    - [x] killed a few bugs:
        - [x] I was reading `PORT` instead of `PIN`
        - [x] I was reading a function name instead of the function value
            - i.e., I forgot the trailing `()`
        - [x] I was trying to use `MISO ` as general purpose I/O while the `SPI`
          hardware module was overriding the `PORT` value
    - [x] this test: sending bytes
    - next test: write Application versions of `Slave_receives_request`
    - make a version without interrupts and a version with interrupts
#### unit-tested working code into lib code and refactored
- [x] SpiMaster
- [x] SpiSlave
#### refactor embedded tests using new lib code
- [x] SpiMaster
- [x] SpiSlave
#### embedded tests of refactored lib code
- [x] embedded test of SpiMaster
- [x] embedded test of SpiSlave

## SPI dev overview
### SPI and Spi
- *SPI* is *Serial Peripheral Interface*
- I use *Spi* when referring to my lib
- the libs are `SpiMaster` and `SpiSlave`
- there is another *SPI* lib: `UartSpi` for communication with the *ADC* on the
  `mBrd`, see section [UART SPI](#markdown-header-uart-spi)

### Master and Slave
- *SPI master*: the MCU on the `simBrd`, a.k.a the *Ft1248 Master* MCU
- *SPI slave*:  the MCU on the `mBrd`

### SPI overview
- see datasheet
```powershell
PS> &$atmega328p_datasheet
```
- *SPI* is well-described in the *ATmega328P* datasheet, chapter 18 `SPI -
  Serial Peripheral Interface`
- see **Figure 18-2.** *SPI Master-slave Interconnection*
- this figure summarizes how *SPI* works:
    - the master has an **8-bit data** register
    - the slave  has an **8-bit data** register
    - by connecting `MOSI` to `MOSI` and `MISO` to `MISO`, these two 8-bit
      registers form a **16-bit circular buffer**
    - it is a *circular buffer* because each time the bits are shifted left, the
      old `MSB` is shifted back in as the new `LSB`
    - on each clock pulse:
        - the **master `MSB`** is shifted into the **slave `LSB`**
        - the **slave `MSB`** is shifted into the **master `LSB`**
    - the circular buffer is clocked 8 times to shift all 8 bits
    - the transmission is finished
    - whatever was in the *master* is now in the *slave*, and vice versa
- **takeaway**: both the master and slave send and receive on every
  transmission, regardless of which one is sending the *actual* data

### SPI communication for requesting a frame
#### SPI master requests a frame
- SPI master transmits `request_frame`:
    - `Spi_Ss` goes high to low
        - this places the `mBrd` MCU in *slave* mode
    - SPI master loads SPI data register with byte to `request_frame`
        - this initiates the transmission
    - the byte in the SPI data register is shifted out
    - `Spi_Ss` goes low to high
        - this releases the `mBrd` MCU from *slave* mode
- SPI slave parses the byte and recognizes it as `request_frame`
- SPI slave coordinates with the ADC and LIS to get a frame of data

#### SPI slave responds with the frame
- SPI slave collects the entire frame of data
- SPI slave loads the first byte of data into the SPI data register
- SPI slave pulls `Spi_Miso` low to signal that a frame is ready
- SPI master reads out the frame:
    - `Spi_Ss` goes high to low
        - this places the `mBrd` MCU in *slave* mode
    - SPI master does the read out:
        - SPI master initiates the transmission of each byte by writing to the
          SPI data register
        - it does not matter what the master writes to the SPI data register:
            - I can use this byte like an ACK/NAK:
                - e.g., master sends a byte meaning *stop sending*
            - but `Spi_Ss` already handles synchronization, so to stop a
              transmission all I have to do is pull `Spi_Ss` high
            - I cannot think of anything useful for the master to send
    - `Spi_Ss` goes low to high
        - this releases the `mBrd` MCU from *slave* mode
        - SPI slave considers the frame finished
- I am having timing problems.
- I can try an extra handshake:
    - slaves wait to be released
    - master releases slave
    - this is slaves queue to signal when data is ready
    - master waits for this signal
    - master reopens communication with slave
## Setup the SPI hardware
### Setup the SPI pins
- see `hardware-connection-and-schematics.pdf`
```powershell
PS> &$lis_hardware
```
- [x] create `SpiMaster-Hardware.h`
- [x] create `SpiSlave-Hardware.h`
    - pinout is identical to `SpiMaster-Hardware.h`

### Setup the SPI registers
- see datasheet
```powershell
PS> &$atmega328p_datasheet
```

#### SPI master pin overrides
pin         | direction
----------- | ---------
`Spi_Ss`    | user defined
`Spi_Miso`  | input
`Spi_Mosi`  | user defined
`Spi_Sck`   | user defined

#### SPI slave pin overrides
pin         | direction
----------- | ---------
`Spi_Ss`    | input
`Spi_Miso`  | user defined
`Spi_Mosi`  | input
`Spi_Sck`   | input

### SPI Errata
I discovered a weird-but-obvious-in-hindsight behavior that is not documented
anywhere. Below I ramble on about this for a while, but there is a more
significant takeaway in:
    - writing the logic for the SPI master to detect when the SPI slave sends
      the *data is ready* signal
    - characterizing the setup and hold times for when the SPI slave sends the
      *data is ready* signal and providing this information in the datasheet

#### Loss of MISO as general purpose IO when SPI is enabled
##### Context
The Chromation spectrometer is a SPI slave. Customer hardware is the SPI master.
When a master requests data, it takes time for the sensor to collect the data.
Since `MISO` is always an input on the SPI master, I decided to use `MISO` as
the signal to the master that the data is ready. The master requests data, the
slave gets the data, then pulls `MISO` low to signal it is ready for the master
to read the data.

##### Problem
After the SPI slave enables the SPI hardware module:
```c
SetBit(Spi_spcr, Spi_Enable);
```
`MISO` no longer behaves as general purpose I/O. `MISO` idles high (I think it
is pulled-up, not a hard high, but I have not tested this). This has no affect:
```c
ClearBit(Spi_port, Spi_Miso);
```
The pin continues to idle high and the SPI master thinks the slave is still
collecting data.

Of course, the pin must be configured as an output pin for the SPI hardware
module to use it as a slave output line. But what is not clearly stated anywhere
is the loss of control via the `PORT` register.

*As long as SPI is enabled, only the SPI hardware module has control over MISO*.

I cannot use `MISO` to signal to the master that data is ready.

##### Solution
Just kidding. It's an easy fix.

Follow the `MISO`-go-low instruction with a quick `SpiDisable()` to regain
control of the pin and `SpiEnable()` to give control back to the SPI hardware
module:
```c
ClearBit(Spi_port, Spi_Miso);    // signal the slave is ready
ClearBit(Spi_spcr, Spi_Enable);  // disable SPI -- now MISO goes low
SetBit  (Spi_spcr, Spi_Enable);  // enable SPI -- MISO is pulled back up
```
##### Well not really -- timing
The fix only works with the sluggish function call overhead. With that removed,
this doesn't work at all.
To make matters worse, I cannot measure the MISO signal on an oscilloscope. The
pin is not pulled up, so there is nothing to see. Attempting to measure on an
oscilloscope interferes with communication.

##### It makes sense in hindsight
It kind of makes sense that the SPI module works this way, but I'm still
annoyed this isn't spelled out in the datasheet.

I mean it is spelled out in `Figure 13-5. Alternate Port Functions`. But that
takes a real careful, thoughtful read of the datasheet. The `SPI` section
doesn't mention this, and that's what I'm ticked about.

Of course, the chip has to work this way. As `Figure 13-5` shows, a *MUX* is
needed to select whether control goes to general purpose or the alternate
function. And what signal controls the *MUX*? The act of *enabling/disabling*
the alternate function. A nice and simple system. I just wish the implication
was stated explicitly.

From a system-design point-of-view, this is definitely a feature, not a bug.
Disabling PORT control is a safeguard. In a multi-slave system, if SPI is always
enabled, the pin can only drive the MISO line when the slave is select. This
protects against the slave driving MISO while another slave is driving MISO. The
SPI protocol handles selecting one slave at a time, and the SPI hardware module
tri-states the MISO for inactive slaves.

### Detection logic for SPI data-ready signal includes tri-state
- the SPI slave uses MISO to signal when data is ready
- this happens in software
- this happens *outside* the SPI transfer
- therefore the SPI module is disabled to let the slave do this
- but disabling the SPI module is not the main problem
- the main problem is that the slave *does not* disable the SPI module until it
  wants to signal that it is ready
- [ ] TODO: so why didn't I just change that do teh SpiDisable() when the SPI
  slave recieves the `request_frame` command?
- if the transfer ends with a low, `MISO` is *slowly* pulled high, it is not
  driven high as it is during the transfer
- if the SPI master starts right away checking for a low on `MISO`, it will
  think this slowly rising `MISO` voltage *is* a low driven by the slave
- luckily, the pull-up at least ensures a monotonic rise
- the SPI master must first look for a high on `MISO`
- a high and a low are separated by some voltage, so by the time `MISO` is high
  enough to look high, it is *definitely* not going to look low from noise
  - there is no need for any debouncing
  - this is kind of like a Schmitt trigger providing hysteresis:
    - look for a high
    - after high, look for a low
    - by the time the signal is high, it well above the threshold for low
- the SPI master starts looking for `MISO` low after it sees `MISO` high
- to make sure the SPI slave is ready, the SPI master again waits for `MISO` to
  go high before continuing
- after the SPI slave pulls `MISO` high, it re-enables the SPI hardware module

#### what I implemented in the firmware sent to Osram 2018-08-28
```c
#define MacroSpiResponseIsReady() MacroBitIsClear(Spi_pin, Spi_Miso)
#define MacroSpiMasterWaitForResponse() do { \
    while(  MacroSpiResponseIsReady() ); \
    while( !MacroSpiResponseIsReady() ); \
    while(  MacroSpiResponseIsReady() ); \
} while (0)
```
- the first `while` loop:
    - loop and do nothing while `MISO` is low
    - exit the loop when `MISO` is high
    - this guarantees `MISO` is high before the SPI master looks for `MISO` low
- the second `while` loop:
    - loop and do nothing while `MISO` is high
    - exit the loop when `MISO` is low
    - this is the SPI slave signaling *data is ready*
- the third `while` loop:
    - loop and do nothing while `MISO` is low
    - exit the loop when `MISO` is high
    - this is the SPI master catching when the SPI slave is done sending the
      *data is ready* signal and is now going to re-enable the SPI slave
      hardware module
- but the SPI slave does not actively drive `MISO` high again:
```c
#define MacroSpiSlaveSignalDataIsReady() do { \
    MacroClearBit(Spi_port, Spi_Miso); \
    MacroDisableSpi(); \
    Delay3CpuCyclesPerTick(10); \
    MacroEnableSpi(); \
} while (0)
```
- and `MacroEnableSpi()` does not drive `MISO` high:
```c
#define MacroEnableSpi() MacroSetBit(Spi_spcr, Spi_Enable)
```
- the SPI slave does *not* pull `MISO` high first!
- it just re-enables SPI and lets `MISO` slowly rise up again
    - this guarantees the SPI slave has time to get ready
    - but I'd rather:
        - the slave ends *data is ready* by driving `MISO` high
        - then release `MISO` by re-enabling the SPI slave hardware module
    - in this scenario, the SPI master uses a fixed delay before starting
      communication
    - this will cut-down on unnecssary wait time
    - and it will make the overall system more deterministic by making it less
      dependent on the pull-up resistor value and the parasitic capacitance on
      the `MISO` pin

#### philosophizing
- this is a case where logic is not binary
- *not low* does not imply *high*
- the tri-state makes *not low* ambiguous
    - it could mean *high*
    - or it could mean *tri-state*
    - the abstraction of *negation* is not sufficient in this context
- since it is a SPI pin, *not low* means *tri-state*
    - the pin has a pull-up resistor
    - this gives predictable behavior:
        - if the transfer ends with `MISO` high
            - then *not ready* is `MISO` high
        - if the transfer ends with `MISO` low
            - then *not ready* is an exponential decay from 0V to 3.0V
        - in both cases, the SPI master first looks for `MISO` high
        - this ensures the `MISO` pin is in a high state
        - the SPI slave must then also set a minimum high time on `MISO` for
          setup, not just a minimum low time
        - and to make sure the SPI master does not start the transfer before the
          SPI slave re-enables communication, the SPI slave must also set a
          maximum time from `MISO` high to when it has renabled its SPI hardware
          module
            - this does not have to be a minimum because after `MISO` goes high,
              the SPI slave does nothing until the SPI master starts
              communication, so the SPI master has all the time it needs to
              check `MISO`
            - it is a maximum because the SPI master has no other way to know
              the SPI slave is ready
### TODO: implement bullet-proof logic to detect *data is ready* signal
- I did not fully implement the bullet-proof logic outlined above
- I look for `MISO` high
- then I look for `MISO` low
- then I delay to give the SPI slave time to re-enable the SPI hardware module
- so I have no protection against two scenarios:
    1. the SPI slave pulls `MISO` low before it ever has a chance to rise high
        - then the SPI master sits in an infinite loop waiting for `MISO` to go
          high
        - this never happens, but there is no guarantee I will not extend
          functionality and continue to avoid this
        - fix: the SPI slave first holds `MISO` high for a guaranteed minimum
          before pulling `MISO` low to signal data is ready
    2. the SPI master starts the transfer before the SPI slave re-enables the
       `SPI` hardware module
        - again, this never happens, but there is no guarantee it will never
          happen in the future as functionality grows
        - fix: the SPI slave has a maximum guaranteed time from `MISO` high
          until the SPI hardware module is enabled
        - if the SPI master waits for this amount of time after detecting `MISO`
          high, it is guaranteed the SPI slave is ready
- another protocol rule to implement is that the Chromation Digital Interface
  maintains control over `MISO` once the SPI master requests data
    - no other SPI slave may drive `MISO` until the SPI master reads the
      requested data
    - this is to avoid contention where another device is trying to signal the
      SPI master
    - this contention is easily avoided by the SPI master
    - the SPI master must not attempt to communicate with other SPI devices once
      it has requested data from the Chromation Digital Interface
    - this guarantees it is OK for the Chromation Digital Interface to use
      `MISO` to signal when data is ready
    - but the *data is ready* signal is highly deterministic, so if the SPI
      master *does* attempt to pipeline communication with other SPI slaves,
      e.g., requesting a frame of data with a long integration time, it is up to
      the SPI master to time everything so that the Chromation Digital Interface
      is *the only* SPI slave that drives `MISO` when the Chromation Digital
      Interface sends the *data is ready* signal
- [ ] add a 10k resistor on the next design of the Chromation Digital Interface
    - purpose: want the SPI MISO pull-up to be independent of the value of the
      internal pull-up resistor in the SPI slave hardware module

### TODO: code for bad actor SPI scenarios
- I have done nothing to handle the cases where the SPI master is a bad actor:
    - the SPI master sends a second data request before receiving the data from
      the previous data request
    - the SPI master sends commands to change exposure time or to change the LIS
      programming settings while the SPI slave is still acquiring data to
      fulfill a previous data request
- the SPI slave implementation sent to Osram on 2018-08-28 might already handle
  these cases:
    - see `Commands affect the state of the spectrometer chip digital interface`
    - TLDR:
        - the `SPI slave` ignores communications sent while it still has unread
          frame data
        - the `SPI slave` interprets data sent after commands as the values for
          those commands
    - [ ] but I still need to write tests to see what happens in these cases
        - show the effect is:
            - the slave simply ignores the bad communications
            - the master gets meaningless garbage back

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


## Old but useful: SPI master writes to SPI slave
- [x] continue copying in from
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

## Old notes that are probably not useful anymore
### Old Reference docs
> `C:\chromation-dropbox\Dropbox\design files and protocols\circuits\pcb design\eagle\projects\Chromation\20150807_SPI_Wand\doc\design on paper\Communication With FLIR Ex-Series Camera.txt`
> `/cygdrive/c/chromation-dropbox/Dropbox/design files and protocols/circuits/pcb design/eagle/projects/Chromation/20150807_SPI_Wand/doc/design on paper/Communication With FLIR Ex-Series Camera.txt`
> `C:\chromation-dropbox\Dropbox\design files and protocols\circuits\mcu\Atmel Studio\LIS-770i_Interface\20151020_LIS-770i_mBrd\src\main.c`
> `/cygdrive/c/chromation-dropbox/Dropbox/design files and protocols/circuits/mcu/Atmel Studio/LIS-770i_Interface/20151020_LIS-770i_mBrd/src/main.c`

### Standard SPI
- SPI master *always* initiates communication
- Every SPI communication is both a **read** and a **write**
    - master and slave form an 8-bit circular buffer
    - the 8-bits in the slave's transmit buffer get shifted into the master's
      receive buffer
    - the 8-bits in the master's transmit buffer get shifted into the slave's
      receive buffer
### SPI sensors
- many SPI sensors are slaves that only transmit (no receive buffer)
- transmit only scheme:
    - host transmit buffer is garbage
    - clock slave some number of times
    - clock drives the sensor's ADC
    - meaningful data received on last N clocks after the first M clocks.
- the `mBrd` is configurable so it must receive too
### Basic SPI
- SPI master loads a byte into its SPI data register
- SPI master tells its hardware peripheral to send the byte
- SPI slave hardware peripheral receives the byte
    - this is independent of the main program execution
- SPI slave has an interrupt that triggers when a byte is received
    - this interrupt notifies the CPU that a byte was received
    - this is the first time the SPI slave need even be aware that the SPI
      master has been communicating with it.

### SPI with Slave/Chip Select
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

# UART SPI
- lib `UartSpi`
- the `mBrd` MCU is the SPI master
- the `mBrd` ADC is the SPI slave

## SPI communication with LTC1864LADC
Tx a byte:
- Wait until bit UDREn in reg UCSRnA is high.
- Write two bytes to UDRn for a 16-bit transfer.
- Wait for an interrupt that is triggered when the UART transmission completes.
  Or, wait until bit RXCn in reg UCSRnA is high.

# LIS
- lib `Lis`

## one frame of data
- the ADC converts every pixel to a 16-bit value
- there are 784 pixels
    - the first 13 are optically dark
    - the 14th is a dummy
    - 15-784 are optically active
- 784 * 2 bytes = 1568 bytes
- hopefully the ATmega 2K SRAM can handle this
- if not, we can ignore the first 14 pixels
    - that makes it 1540 bytes
- there are probably more pixels we can ignore too
    - revisit the optical design to see which pixels are actually used

## power down
- see `Power Standby Mode`
- `Lis_Rst` is high
- `Lis_Clk` stops and idles low
- remains in power down mode until `Lis_Rst` is pulled low and `Lis_Clk`
  is restarted

## power up
- see `Power Up sequence`
- `Lis_Rst` is pulled low
- `Lis_Clk` starts
- this power up sequence resets the imager
- default programming is enabled:
    - select entire active pixel array
    - set output amplifier gain to 2.5x
    - turn off the summing mode

## programmable setup
- see `Programmable Setup Register`
- power up
- pull `Lis_PixSelect` high
    - [x] `LisInit()` idles `Lis_PixSelect` low
- first rising clock edge reads `Lis_Rst`
    - this sets the value of the *summing mode bit*
    - *Summing Mode = 0* - normal operation
        - pixels are 7.8um wide
    - *Summing Mode = 1* - every other pixel is added together during
      integration
        - the size of the pixel array is halved to 392 pixels
        - pixel width becomes 15.6um
        - only 392 clocks are needed to output the data
- 2nd and 3rd rising clock edges read `Lis_Rst`
    - sets the value of gain bits `G2` and `G1`
    - hold `Lis_Rst` high for 25 clock cycles after the gain bits are set
    - this completely contradicts what the datasheet says next
- 4th through 28th rising clock edges read `Lis_Rst`
    - sets the active pixel sub-arrays in the order P25 to P1
    - see *Table 5b: Selectable Pixel Array* to see which pixel is in which of
      the 25 pixel sub-arrays
        - a pixel has five segments in the *pixel height* direction
        - each segment is 62.5um tall
        - each sub-array is one segment tall
        - each sub-array spans 154 pixels
    - just keep reset high, this selects every segment of every pixel

## frame readout
- `Lis_Clk` is running
    - [x] `LisInit()` pulls `Lis_Rst` low
    - [x] `LisInit()` starts the `Lis_Clk`
        - in the future, `LisInit()` will not start the clock
        - this will be a separate command to leave power-down mode
        - but for now intialize in power up mode
- [ ] TODO: implement a power-down mode
    - [ ] provide a SPI command to enter/exit power-down mode
- Do I need to use the *ISR* to do things on clock rising/falling edges?
- No, this can be done without interrupts by polling flags and manually clearing
  flags. In fact it is slightly faster without interrupts because it cuts out
  the function call/return overhead of the ISR.
    - The output compare unit sets the output compare flag `OCF0A` or
      `OCF0B` when `TCNT0` equals `OCR0A` or `OCR0B`.
    - Clear the flag in software by writing a logical one to its I/O bit
      location.
    - I tested with and without interrupts and the fastest performance is to do
      this without interrupts. That is worth the extra step of manually clearing
      the flag. The result code is easier to read too.
    - Unfortunately, I also discovered the high-level code makes the response
      time to clock edges unacceptably slow. It is so slow that attempting to
      follow the clock edges on Rst makes Rst output a square wave that jitters
      between sometimes being half the frequency, sometimes the same frequency,
      because it only rarely catches adjacent clock edges.
- `Lis_Rst` is low
    - `LisInit()` has `Lis_Rst` idle high for power-saving
    - but once the device is in power up mode, `Lis_Rst` is low
- `Lis_Rst` controls the integration period
    - in the following, `Lis_Rst` is pulled high and low
    - the new value is clocked in on the very next rising edge of `Lis_Clk`
      after `Lis_Rst` has changed
    - [x] change the value of `Lis_Rst` just after a `Lis_Clk` falling edge
        - this avoids confusion over when the LIS shifts in the new value of
          `Lis_Rst`
        - at 50kHz, it is 10us from the clock falling edge to the clock rising
          edge
        - there is a 0.6us to 0.8us delay between the actual clock falling edge
          and the time it takes software to catch the fall and pull `Lis_Rst`
          high or low
        - so in effect, I am guaranteed to have `Lis_Rst` always transition
          about 9us just before the next `Lis_Clk` rising edge, and to never
          transition during a clock edge
- `ExposureStart()` - pull `Lis_Rst` high
    - resets all pixels
    - integration begins
- `Lis_Rst` goes low
    - integration ends
    - integration time is the number of clock cycles that `Lis_Rst` was high
    - [x] decide how to track the integration period:
        - check for N clock cycles in the PWM interrupt?
            - requires the ISR know about the state of the system
            - system can take new requests while the camera integrates
            - no -- ISR adds call/return overhead, slowing response time
        - or sit and poll the interrupt flag, incrementing a counter
            - localizes where the state needs to be known to precisely that
              place where it is used
            - entire system blocks until integration period ends
            - yes -- there is nothing else the system has to do
- Exposure time
    - LisRst is high for 20us * nticks + 2us
    - But the LIS is counting clock pulses, so exposure time is exactly 20us *
      nticks.
- `Lis_Sync` input is pulsed high
> Once RST is brought low a synchronization signal (SYNC) pulse is fired
> starting on the next falling clock edge. Pixel readout then begins on the next
> rising CLK edge after the SYNC signal goes low and continues for 784 clock
> cyles. The SYNC signal is one clock period wide. SYNC is again fired during
> the last pixel readout on the falling edge of CLK.
    - `ExposureStop()` - pull `Lis_Rst` low
        - call this just after a `Lis_Clk` falling edge
        - exposure officially stops on the next `Lis_Clk` rising edge
    - `WaitForSync()` - watch for `Lis_Sync` to go high, then low
        - call this just after calling `ExposureStop()`
        - `Lis_Sync` goes high on the next falling clock edge
        - `Lis_Sync` then goes low on the next falling clock edge
        - pixel readout starts on the very next clock rising edge
    - `PixelReadout()` - sample analog `VOUT` while the clock is high
        - video output may have disturbance during the falling clock edge
        - pixels 1 to 13 are dark
        - pixels 15 to 784 are optical

# Program Flash
## Quick Summary
- check hardware connection: `;mkp`
    - Expect device signature is `0x1E950F`
    - Expect fuse settings are `EXTENDED 0xFF, HIGH 0xD9, LOW 0xF7`
    - [ ] to do: program fuses from Vim
        - off the shelf, LOW is 0x62
        - for now just go into AtmelStudio to set the LOW to 0xF7
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

## Compiler options
> `https://www.microchip.com/webdoc/AVRLibcReferenceManual/using_tools_1using_sel_gcc_opts.html`

## Memory on the ATmega328
The ATmega328 uses a *Harvard architecture* meaning separate memory and buses
for program and data:

- **2KBytes** of *SRAM* hold data memory
- **32KBytes** of *Flash* hold program memory

- *SRAM* **is not** *Flash*
- *Flash* holds the program memory
- *SRAM* holds the data memory

Both the *SRAM* and *Flash* use indirect addressing because it is a large amount
of memory to navigate and the buses are only 8-bit. The **32** 8-bit *General
Purpose Registers* use direct addressing.

Static memory, stack memory, and the heap all use the *SRAM*. Static memory is
never a problem. The trouble is with *stack-heap collisions*.

### RAM layout and stack-heap collisions
> https://www.nongnu.org/avr-libc/user-manual/malloc.html

- *.data* are initialized variables
- *.bss* are uninitialized variables
- both are stored in *SRAM*

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


### Size of the final `.elf` files
- the avr-target executable is an `elf32-avr` with a `.elf` extension
- `avr-size` is a utility to report the size of a `.elf` by section:
```make
build/%.elf: ${obj_dev-libs} src/%.c ${avr-asm-macros}
	avr-gcc $^ -o $@ $(CFLAGS)  \
		-Wl,-Map="build/$*.map" -Wl,--gc-sections
	avr-size $@ > build/avr-size_simBrd.log
```
- example output of `avr-size`:
```
   text	   data	    bss	    dec	    hex	filename
    730	     34	      7	    771	    303	build/simBrd.elf
```
- about these values:
    - units are bytes
    - numbers are represented in base 10 except for the final `hex` column
    - the `dec` and `hex` columns are the total *Flash* memory used
- memory sections are described here:
> `https://www.microchip.com/webdoc/AVRLibcReferenceManual/mem_sections_1sec_dot_text.html`
- I think the following is a decent summary
#### `.text` memory section
- the `.text` contains the machine instructions
- the total amount of *Flash* memory consumed is the `.text` plus the `.data`
    - the `.data` is added because the *Flash* gets:
> a copy of the `.data` initializers
- the `ATmega328P` has `32KBytes` of *Flash*
- example: How much of the *Flash* did I use?
    - `.data`: 34
    - `.text`: 730
    - total: **764**
- so this small example application uses less than `1KByte`
- that is about 2.4% of the *Flash* memory.

#### `.data` memory section
- static data:
    - constants
    - initialized globals
    - initialized static variables
- `.data` and `.bss` are both data, so they both go in `SRAM`
- the total amount of *SRAM* consumed is the `.data` plus `.bss`
- the `ATmega328P` has `2KBytes` of *SRAM*
- example: How much of the *SRAM* did I use?
    - `.data`: 34
    - `.bss`: 7
    - total: **41**
- that is about 2.1% of the *SRAM*

#### `.bss` memory section
- uninitialized globals
- uninitialized static variables

#### `.dec` memory section
- this is just the sum of the other three sections
    - `.data`: 34
    - `.text`: 730
    - `.bss` : 7
    - total: **771**
- `dec` stands for decimal
- `hex` is the same total represented in base 16

## Cable connections and switch settings
- Power `simBrd` from hostPC with mini-B USB cable.
- Power ATMEL-ICE from hostPC with micro-B USB cable.
- Connect ATMEL-ICE to `simBrd` by connecting the 10-pin ribbon cable to the AVR
  port on the Atmel-ICE, and the 6-pin keyed female socket end of the 10-pin
  ribbon cable to the male shrouded header on the PCB.
- Set simBrd `SW2` to `ISP`
    - program the `simBrd`: `SW1` to `M.ISP`
        - **pneumonic**: *M for SPI Master*
    - program the `mBrd`:   `SW1` to `S.ISP`
        - **pneumonic**: *S for SPI Slave*
- Before invoking `;mfa` or `;fa` *make sure the `pwd` is correct*:
```vim
:pwd
```
- Switch to the `simBrd` directory before programming the `simBrd`:
```vim
:cd ../simBrd
```
- Switch to the `mBrd` directory before programming the `mBrd`:
```vim
:cd ../mBrd
```

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

## Look at the disassembly instructions
### read the .lst file
- the disassembly file has extension `.lst`
- [x] learn to read the .lst file

- all of the instructions are listed in the datasheet
- I pulled out a handful below that are used in the `.lst` snippets I attempt to
  translate

instr   | Operand   | Operation         | Flags     | Description
------- | --------- | ---------         | --------- | ---------
ADD     |   Rd, Rr  | Rd←Rd+Rr          | Z,C,N,V,H | Add two Registers
BRPL    |   k       | if(N=0), PC←PC+k+1| None      | Branch if Plus
CALL    |   k       | PC←k              | None      | Direct Subroutine Call
DEC     |   Rd      | Rd←Rd-1           | Z,N,V     | Decrement
ICALL   |           |                   | None      | Indirect Call to (Z)
LD      |   Rd, Z   | Rd←(Z)            | None      | Load Indirect
LDI     |   Rd, K   | Rd←K              | None      | Load Immediate
LDS     |   Rd, k   | Rd←(k)            | None      | Load Direct from SRAM
MOVW    |   Rd, Rr  | Rd+1:Rd←Rr+1:Rr   | None      | Copy Register Word
PUSH    |   Rr      | STACK←Rr          | None      | Push Register on Stack
RET     |           | PC←STACK          | None      | Subroutine Return
RJMP    |   k       | PC←PC+k+1         | None      | Relative Jump

```c
000000f0 <DebugLedTurnOn_Implementation>:
    DebugLedTurnGreen();
}

static void DebugLedTurnOn_Implementation(void)
{
    SetBit(ddr_register_, debug_led_);
  f0:	60 91 32 01 	lds	r22, 0x0132	; 0x800132 <__data_end>
  f4:	80 91 37 01 	lds	r24, 0x0137	; 0x800137 <ddr_register_>
  f8:	90 91 38 01 	lds	r25, 0x0138	; 0x800138 <ddr_register_+0x1>
  fc:	0e 94 53 00 	call	0xa6	; 0xa6 <SetBit>
 100:	08 95       	ret
```
- `0xf0` is the address of this function
- this function just sets up a function call
- the input arguments are stored in `SRAM`
- it loads these arguments from `SRAM` into some registers:
    - copy value from `SRAM` at address `0x0132` to `r22`
    - copy value from `SRAM` at address `0x0137` to `r24`
    - copy value from `SRAM` at address `0x0138` to `r25`
- then it calls `SetBit`:
    - `SetBit` is the subroutine at `PC` address `0xa6`
    - load the `PC` with `0xa6`
    - when the program counter returns here (address `0xfc`) the final step is
      to return from this function by popping the return address from the stack
      and loading the `PC` with that return address

- I don't know where the assembly for `SetBit` is
- this is all I could find:
```c
000000a6 <SetBit>:
    *port ^= (1<<bit);
}
```

```c
bool BitIsSet(uint8_t volatile * const port, uint8_t const bit) {
    return *port & (1<<bit);
}
  a6:	fc 01       	movw	r30, r24
  a8:	40 81       	ld	r20, Z
  aa:	21 e0       	ldi	r18, 0x01	; 1
  ac:	30 e0       	ldi	r19, 0x00	; 0
  ae:	02 c0       	rjmp	.+4      	; 0xb4 <SetBit+0xe>
  b0:	22 0f       	add	r18, r18
  b2:	33 1f       	adc	r19, r19
  b4:	6a 95       	dec	r22
  b6:	e2 f7       	brpl	.-8      	; 0xb0 <SetBit+0xa>
  b8:	24 2b       	or	r18, r20
  ba:	20 83       	st	Z, r18
  bc:	08 95       	ret
```

- Here is the function in question, `UsbWrite`:
```c
0000032a <UsbWrite>:
uint16_t UsbWrite(uint8_t *write_buffer, uint16_t nbytes)
{
```
- this first part just makes space to work:
    - save the values in the registers by pushing them onto the stack:
```c
 32a:	ef 92       	push	r14
 32c:	ff 92       	push	r15
 32e:	0f 93       	push	r16
 330:	1f 93       	push	r17
 332:	cf 93       	push	r28
 334:	df 93       	push	r29
```
- now copy the input arguments into the registers just freed up:
```c
 336:	8c 01       	movw	r16, r24
 338:	7b 01       	movw	r14, r22
```
- `FtSendCommand` is a function pointer:
```c
void (*FtSendCommand)(uint8_t) = FtSendCommand_Implementation;
```
- function pointers are treated differently from regular functions
- compare `FtSendCommand` and `SetBit`
- it is not called like `SetBit`
- `SetBit` is a `PC` address
- `SetBit` was called by loading the `PC` with that address
- the address of `FtSendCommand` is stored in `SRAM`
- `FtSendCommand` is called by loading a 16-bit address from `SRAM` into
  registers `r30` and `r31`
- these comprise the 16-bit `Z` register

```c
    uint16_t num_bytes_sent = 0;
    FtSendCommand(FtCmd_Write);
 33a:	e0 91 1c 01 	lds	r30, 0x011C	; 0x80011c <FtSendCommand>
 33e:	f0 91 1d 01 	lds	r31, 0x011D	; 0x80011d <FtSendCommand+0x1>
 342:	80 91 24 01 	lds	r24, 0x0124	; 0x800124 <FtCmd_Write>
 346:	09 95       	icall
    if (!FtBusTurnaround())
 348:	e0 91 1a 01 	lds	r30, 0x011A	; 0x80011a <FtBusTurnaround>
 34c:	f0 91 1b 01 	lds	r31, 0x011B	; 0x80011b <FtBusTurnaround+0x1>
 350:	09 95       	icall
 352:	81 11       	cpse	r24, r1
 354:	0d c0       	rjmp	.+26     	; 0x370 <UsbWrite+0x46>
    {
        DebugLedTurnRedToShowError();
 356:	e0 91 00 01 	lds	r30, 0x0100	; 0x800100 <DebugLedTurnRedToShowError>
 35a:	f0 91 01 01 	lds	r31, 0x0101	; 0x800101 <DebugLedTurnRedToShowError+0x1>
 35e:	09 95       	icall
        FtDeactivateInterface();
 360:	e0 91 14 01 	lds	r30, 0x0114	; 0x800114 <FtDeactivateInterface>
 364:	f0 91 15 01 	lds	r31, 0x0115	; 0x800115 <FtDeactivateInterface+0x1>
 368:	09 95       	icall
        return num_bytes_sent;
 36a:	c0 e0       	ldi	r28, 0x00	; 0
 36c:	d0 e0       	ldi	r29, 0x00	; 0
 36e:	15 c0       	rjmp	.+42     	; 0x39a <UsbWrite+0x70>
 370:	c0 e0       	ldi	r28, 0x00	; 0
 372:	d0 e0       	ldi	r29, 0x00	; 0
    }
    // TODO: rename `byte_sent` as `byte_sent_OK` or `byte_was_sent`
    bool byte_sent = true; bool finished = false;
```

- This is where the value is copied.
- `r16` is the input argument: the address of `write_buffer`
- so look at `FtWrite`, not `UsbWrite`
```c
    while (byte_sent && !finished)
    {
        byte_sent = FtWrite(write_buffer++);
 374:	e0 91 18 01 	lds	r30, 0x0118	; 0x800118 <FtWrite>
 378:	f0 91 19 01 	lds	r31, 0x0119	; 0x800119 <FtWrite+0x1>
 37c:	c8 01       	movw	r24, r16
 37e:	8c 0f       	add	r24, r28
 380:	9d 1f       	adc	r25, r29
 382:	09 95       	icall
        if (byte_sent) num_bytes_sent++;
 384:	88 23       	and	r24, r24
 386:	21 f0       	breq	.+8      	; 0x390 <UsbWrite+0x66>
 388:	21 96       	adiw	r28, 0x01	; 1
        FtDeactivateInterface();
        return num_bytes_sent;
    }
    // TODO: rename `byte_sent` as `byte_sent_OK` or `byte_was_sent`
    bool byte_sent = true; bool finished = false;
    while (byte_sent && !finished)
 38a:	ce 15       	cp	r28, r14
 38c:	df 05       	cpc	r29, r15
 38e:	90 f3       	brcs	.-28     	; 0x374 <UsbWrite+0x4a>
    {
        byte_sent = FtWrite(write_buffer++);
        if (byte_sent) num_bytes_sent++;
        finished = (num_bytes_sent >= nbytes);
    }
    FtDeactivateInterface();
 390:	e0 91 14 01 	lds	r30, 0x0114	; 0x800114 <FtDeactivateInterface>
 394:	f0 91 15 01 	lds	r31, 0x0115	; 0x800115 <FtDeactivateInterface+0x1>
 398:	09 95       	icall
    return num_bytes_sent;
}
 39a:	ce 01       	movw	r24, r28
 39c:	df 91       	pop	r29
 39e:	cf 91       	pop	r28
 3a0:	1f 91       	pop	r17
 3a2:	0f 91       	pop	r16
 3a4:	ff 90       	pop	r15
 3a6:	ef 90       	pop	r14
 3a8:	08 95       	ret
```

- looking at `FtWrite`:
```c
bool (*FtWrite)(uint8_t *write_buffer) = FtWrite_Implementation;
```
- the dissasembly for `FtWrite_Implementation`:
```c
000001be <FtWrite_Implementation>:
 1be:	cf 93       	push	r28
 1c0:	df 93       	push	r29
 1c2:	ec 01       	movw	r28, r24
 1c4:	e0 91 12 01 	lds	r30, 0x0112	; 0x800112 <FtPushData>
 1c8:	f0 91 13 01 	lds	r31, 0x0113	; 0x800113 <FtPushData+0x1>
 1cc:	09 95       	icall
 1ce:	e0 91 06 01 	lds	r30, 0x0106	; 0x800106 <FtWriteData>
 1d2:	f0 91 07 01 	lds	r31, 0x0107	; 0x800107 <FtWriteData+0x1>
```
- here is where the value of the byte in the `write_buffer` gets passed to
  `FtWriteData`:
```c
 1d6:	88 81       	ld	r24, Y
 1d8:	09 95       	icall
```
- what is register `Y`?
- `YL` is `r28`, `YH` is `r29`
- the input argument, the pointer `write_buffer`, is in `r24`
- so copy this to `r28`
- but how could the pointer, which is an address, fit is `r24`?

```c
 1da:	e0 91 0e 01 	lds	r30, 0x010E	; 0x80010e <FtPullData>
 1de:	f0 91 0f 01 	lds	r31, 0x010F	; 0x80010f <FtPullData+0x1>
 1e2:	09 95       	icall
 1e4:	e0 91 08 01 	lds	r30, 0x0108	; 0x800108 <FtIsBusOk>
 1e8:	f0 91 09 01 	lds	r31, 0x0109	; 0x800109 <FtIsBusOk+0x1>
 1ec:	09 95       	icall
 1ee:	df 91       	pop	r29
 1f0:	cf 91       	pop	r28
 1f2:	08 95       	ret

```
### view disassembly in `gdb`
- `gdb` will not run .elf files, so this is *not useful* for figuring out how
  `avr-gcc` is turning C into assembly instructions
- [-] view the disassembly from gcc using the `gdb` command `layout`:
```bash
(gdb) help layout
Change the layout of windows.
Usage: layout prev | next | <layout_name> 
Layout names are:
   src   : Displays source and command windows.
   asm   : Displays disassembly and command windows.
   split : Displays source, disassembly and command windows.
   regs  : Displays register window. If existing layout
           is source/command or assembly/command, the 
           register window is displayed. If the
           source/assembly/command (split) is displayed, 
           the register window is displayed with 
           the window that has current logical focus.
```

Use `layout split` to view source and assembly at the same time. Use the up/down
arrow keys to move through the source code. The source and assembly windows
stay in sync. Adding a breakpoint places a `b+` next to the line number for both
the source and assembly windows.

# USB Host Application
## New Python application
Moving forward, the USB Host is a Python 3 application.

See work here:
`/cygdrive/c/chromation-dropbox/Dropbox/sales/spect-py3-examples/`

Clean these notes:
    - Python 3 `pyserial` works
    - my error in the past was because I did not know how byte arrays worked in
      Python 3, nothing to do with `pyserial`
        - see short snippets here:
        <https://www.mkyong.com/python/python-3-convert-string-to-bytes/>
    - D2XX calls still work with `Load VCP` enabled
    - that means people have a choice
    - and the right choice on Python is `Load VCP` because:
        - `pyserial` does not have a d2xx api
        - the serial commands work just as well
            - whatever you need to do in D2XX you can do treating the device as
              a generic COM Port
    - I can leave my LabVIEW code using the D2XX API and use VCP when
      programming in Python with `pyserial`
    - `Load VCP` cannot access all the USB Description Strings that D2XX can
      access
        - `Product Description` is not visible
        - `Manufacturer` is not visible
        - but `Serial Number` is visible
    - There is a COM port variable named `manufacturer` but this actually comes
      from the `.inf` file installed with the USB driver
        - FTDI wants you to change this
        - this doc shows where the fields are in `FT_Prog`:
            <https://www.ftdichip.com/Support/Documents/AppNotes/AN_124_User_Guide_For_FT_PROG.pdf>
        - this doc explains what you're supposed to change:
            <https://www.ftdichip.com/Support/Documents/TechnicalNotes/TN_102_OEM_Technical_Support_Requirements_for_FTDI_Products.pdf>
        - but this would void the security certificate:

        > It is important to note that the device drivers in this example are
        > shown in Figures 2 and 3 as “Not digitally signed”. Any time there are
        > edits performed on any of the device driver files, it will invalidate
        > the WHQL signature. Re-certification by the OEM is possible. Refer to
        > the AN_101_WHQL_Certified_Driver_Process(FT_000063).

        - and that would sketch out customers
        - and since no one gives a fuck anyway, just leave it as is
        - `manufacturer` shows up as FTDI, not a big deal
        - this is just `eval kit hardware` -- if it *were* a consumer product,
          then we'd want to change this and do it right, paying for the security
          certificate mojo:
            <https://www.ftdichip.com/Support/Documents/AppNotes/AN_101_Submitting_Modified_FTDI_Drivers_for_Windows_Hardware_Certification.pdf>
    - I leave the `Product Description` as-is so that my LabVIEW code does not
      have to change: it still gets `ChromationSpect-0643-01`
    - but now I put CHROMATION123456 in FTDI `serial_number`
    - when `pyserial` scans the USB ports, it can `grep` (awesome!) for the
      serial number and return a `port` object
    - I can tell that `port` object to open up without ever having to use the
      `COM` string explicitly
    - this *should* make the find-and-open code cross-platform
    - other change in `FT_Prog` is the manufacturer
    - change manufacturer from `FTDI` to `CH`
        - does not impact `Load VCP` users, e.g., what you can see from
          `pyserial`
        - does not impact my LabVIEW code since I never look at the manufacturer
          field
        - but it helps with the 44 character limit:
        - Product Description + manufacturer + serial number cannot exceed 44
          characters
        - serial number is supposed to be 16 characters
CHROMATION123456
- very happy with script examples

    - serial commands are encapsulated in functions
    - functions are all very short thanks to generators and list comprehensions
    - even the final plotting code is relatively short, as plotting code goes
- up until 3AM figuring out a snappy live updating matplotlib
    - snag here with globals to revisit later

## LisSweep
- send this command to get a frame of data:
- `cmd_send_lis_frame`
- this is defined as 0x01 in `Spi-Commands.h` in the `mBrd/src`

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

## Dev kit redesign
- *2019-05-10: revisiting this now: everything below is addressed*
- TODO: update this with redesign
- old 2018-08-16 notes start here:
- PCBs for dev kit will change
- spi-master-simulant and spi-slave will be simpler
- spectrometer breakout board will have optimized form factor for
  optics/alignment/mechanical-design

### Notes from 2018-08-16 meeting with Nadia
- revisit this in the future when we are making decision about ordering the
  dev kit:
    - compare lead time tradeoff with design time tradeoff
    - keep in mind we want to have some minimum redesign, so there will be
      lead time for an initial test run no matter what
    - so a redesign to eliminate the reduce the lead time is always a net
      positive
- high-level goal:
    - reduce lead time and cost before placing dev kit order
    - reach out to support@macrofab.com with lead time questions
- goal: eliminate all through-hole parts
    - move FT221X to a custom smt design with USB connector
    - eliminate switches:
        - for SPI communication, just connect boards
        - for programming, just connect programmer
            - make a breakout board to go from programming header to ribbon
              cable
- goal: eliminate cable termination step
    - switch from 10-pin ribbon cable to 10-pin FFC
    - also necessary to eliminate through-hole parts
- look for smt test points
    - if smt test loops do not exist we can get creative

# Repo links
Link to this repo: https://bitbucket.org/rainbots/lis-770i/src/master/

Clone this repo:
```bash
git clone https://rainbots@bitbucket.org/rainbots/lis-770i.git
```
