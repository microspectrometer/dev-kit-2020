% Chromation Spectrometer: Interface Information for Preliminary Datasheet
% Mike Gazes
% December 3rd, 2018

# List of tasks

## fishy code to investigate later
- [ ] why am I calling `LisProgramSummingModeOff();` at the start of
  `WriteCfgToLis()` -- this should not work!
- [ ] 10x the max integration time by counting groups of 10 tics of the 50kHz
  CLK instead of single tics
- [ ] SPI communication troubles may be related to how the SPI Slave signals
  data is ready, see if there is a more robust way to implement this signalling,
  particularly only doing the SPI disable/enable when responding to the
  `request_frame` command

## table of operating conditions
- [x] setup and hold times
- [x] supply voltage
- [x] clock frequency

## general
- [ ] make up titles for the TODO images
- [ ] make the actual images called in the TODO images
- [ ] get pandoc to output my godamn table correctly
    - nope, there is *no* way to add vertical lines
    - so what is the point of extension `grid_tables`?

- [x] spell-check with Vim spelling:
    - `setlocal spell spelllang=en_us` enables spelling in this buffer
    - `[s` goes to next mis-spelled word
    - `zg` adds word to dictionary
    - `zG` adds word to temporary file for this buffer

--- actual writeup starts here ---


# Recommended Operating Conditions
 Symbol  Parameter           MIN   TYP   MAX     Units
-------- ----------         ----- ----- -----   -------
  VDD    Supply Voltage     --      3.0 --      V
  f~CLK~ Clock Frequency    15      50  200     kHz
  CLK~S~ Setup time         --      10  --      ns
  CLK~H~ Hold time          --      10  --      ns

Table: Recommended operating conditions

# Spectrometer Interface
The following describes how to interface the Chromation spectrometer. Since the
Chromation spectrometer is simply optical components mounted around the
`LIS-770i`, *the spectrometer interface is the `LIS-770i` interface*. The
interface description here is greatly simplified but still sufficient to operate
the spectrometer. See the `LIS-770i` datasheet for additional operating
conditions, waveforms, and details about other functionality not covered here,
in particular the `power-down` mode.

External hardware is required to drive the spectrometer to achieve its
higher-level function: *acquire a spectrum*. Chromation refers to this external
hardware as the **spectrometer digital interface**.

The purpose of the **spectrometer digital interface** is to hide the operational
details of the `LIS-770i` and ADC, and instead provide the measurement system
with a standard interface. Chromation recommends presenting the spectrometer
interface to upstream devices as a *SPI slave*. A *SPI master* in the upstream
measurement system executes requests from the system host application by writing
high-level commands recognized by the *SPI slave*.

## Spectrometer I/O
- TODO: block diagram of spectrometer with I/O grouped in a schematic
  representation
    - include ADC and MCU, label power supply value and VREF value
    - do not put part numbers except for linear pixel array, the part numbers
      only appear once in a reference design somewhere else
    - title: spectrometer interface block diagram

## Initialize the spectrometer after power-up
- drive `PIX_SELECT` *LOW*
    - `PIX_SELECT` idles *LOW*
- drive `RST` *LOW*
    - `RST` idles *LOW*
- drive `CLK` with a 50kHz PWM, 50% duty cycle
    - the choice of clock speed is determined by the ADC conversion settling
      time and the speed of communication with the ADC
    - 50kHz is slow enough for a 16-bit SAR ADC to convert pixel
      voltages and to read out the converted digital value

- NOTE: input pin values are *clocked in* on the *rising edges* of `CLK`

## Configure the spectrometer
The spectrometer's linear pixel array has configurable parameters:

- analog gain: 1x, 2.5x, 4x, 5x
- pixel height: 62.5um, 125um, 187.5um, 250um, 312.5um
- pixel pitch: 7.8um (array is 784 pixels), 15.6um (array is 392 pixels)

The linear pixel array does *not* have a known default configuration on
power-up. At a minimum, these parameters must be hard-coded in firmware and the
linear pixel array must be programmed each time the device is powered-up.

Making these parameters configurable by the upstream measurement system may be
beneficial in some context, particularly the analog gain. The spectrometer
performance specifications only apply for the configuration intended by
Chromation. This configuration matches the optical design of the Chromation
spectrometer.

This is the intended configuration:

- analog gain: 1x
- pixel height: 312.5um
- pixel pitch: 15.6um (the array contains 392 pixels)

The following programming sequence configures the parameters of the linear pixel
array parameters to match the intended configuration. The programming sequence
consists of shifting in 28 bits. Bits are shifted in on the rising edge of
`CLK`. The sequence starts with bit 1 and ends with bit 28.

- start the programming sequence:
    - drive `PIX_SELECT` *HIGH* on a falling edge of `CLK`
- bit 1 *HIGH* programs the pixel pitch for 15.6um
    - drive `RST` *HIGH*
    - wait for the next falling edge of `CLK`
- bits 2 and 3 *LOW* set the analog gain to 1x
    - drive `RST` *LOW*
    - wait for the next falling edge of `CLK`
    - drive `RST` *LOW*
    - wait for the next falling edge of `CLK`
- the next 25 bits are *HIGH* to select the 312.5um pixel height
    - drive `RST` *HIGH*
    - wait for the next falling edge of `CLK`
    - repeat 24 more times
- stop the programming sequence:
    - drive `RST` *LOW*
    - drive `PIX_SELECT` *LOW*

## Acquire a spectrum
The linear pixel array has internal switching logic to execute the spectrometer
**exposure** and **readout** sequence, but like most other linear pixel arrays,
the `LIS-770i` requires external logic-level signals to drive this internal
switching. The `LIS-770i` also requires an external ADC to convert its analog
output.

### Expose the spectrometer pixels
- drive `RST` *HIGH* to start exposure
    - Chromation recommends waiting for the falling edge of `CLK` to drive `RST`
      *HIGH*
    - pixel exposure starts when the *HIGH* on `RST` is clocked in on the next
      rising edge of `CLK`
- wait the desired integration time by counting falling edges of `CLK`
    - for a 50kHz `CLK`, falling edges represent 20us ticks
    - for example, waiting 500 ticks is a 10ms integration time
- drive `RST` *LOW* to stop exposure
    - again, the *LOW* is clocked in on the next rising edge of `CLK`

### Immediately readout the pixel voltages
- wait for a pulse on `SYNC`:
    - after `RST` is driven *LOW*, spectrometer output pin `SYNC` goes *HIGH* on
      the next falling edge of `CLK`
    - `SYNC` goes *LOW* on the next falling edge of `CLK` (`SYNC` is *HIGH* for
      one clock period)
- pixel readout begins on the next rising `CLK` edge after `SYNC` goes *LOW*
- start each ADC conversion on each rising edge of `CLK`
- finish each ADC conversion before the falling edge of `CLK`
    - the falling edge of `CLK` may couple into the pixel output voltage, so it
      is preferable to finish the ADC conversion before the `CLK` falling edge
    - and it is good practice in general to avoid transients during an ADC
      conversion
    - this is easily achieved using a 50kHz clock
    - in fact, using *Linear Technology #LTC1864L* as the ADC, both the
      conversion and readout of the converted samples fits within the `CLK` high
      time
- on the last pixel readout, `SYNC` pulses *HIGH* again for one clock period,
  starting on the falling edge of `CLK`


--- actual writeup ends here ---

# Everything after here is to be worked in later as needed
The rest of this provides useful context and examples, but it is not clear yet
where any of this belongs, so I am finishing tasks on the preceding section
first

# Complete measurement system
## Chromation example of an upstream SPI interface
- the main high-level command for the *SPI slave digital interface* is *acquire
  a spectrum measurement*:
    - run the *expose* and *readout* sequence
    - during the *readout*, acquire and store the pixel values
- other high-level tasks are:
    - configure the `LIS-770i`:
        - set the programmable gain
        - select the active pixels
        - turn pixel summing on or off
    - auto-expose:
        - determine the optimal exposure time for the peak counts in the
          measured signal to reach the target value

## Acquire a spectrum measurement
- from the perspective of the `LIS-770i`, a *spectrum measurement* is just a
  *frame of data*: sequential output of the voltages on each pixel immediately
  after exposure
- to collect one frame of data from the `LIS-770i`, the SPI slave:
    - clocks the LIS for the desired exposure period
    - clocks out the pixel voltages while communicating with the ADC to
      convert the voltages to 16-bit values
    - stores the 16-bit value for each pixel
        - Chromation recommends configuring the `LIS-770i` as 392 pixels
        - therefore the digital interface needs 784 bytes of storage for one
          measurement
        - the photonic crystal does not illuminate the entire pixel array, so
          the actual storage requirement is less
        - the `LIS-770i` can also be configured as 784 pixels (not recommended),
          requiring 1568 bytes to store one measurement
- the ADC is a SPI slave to the digital interface, so in the context of
  converting voltages during pixel readout, the digital interface is a SPI
  master
- the digital interface signals the SPI-to-Host Bridge when readout is finished

## Convert spectrometer analog output to digital
- during readout, the digital interface converts the analog pixel voltage to
  counts
- the digital interface uses a 16-bit SAR ADC with *differential input* and
  *unipolar transfer function*
    - *differential input*: the ADC has *two* analog inputs and it converts the
      voltage difference between them
    - *unipolar transfer function*: the allowed analog voltage ranges from `0V`
      to the ADC external voltage reference
    - the ADC voltage reference is `2.048V`

## Dark-correct analog output in hardware before conversion
- the `LIS-770i` offers a dark-voltage output to allow dark-correcting the pixel
  voltages *before* conversion to digital counts
- the digital interface takes advantage of this analog dark-correction by
  connecting the dark-voltage to the negative terminal at the ADC differential
  input:
    - `ADC.IN+` connects to LIS-770i analog output `VOUT`
        - `VOUT` is the pixel voltage output by the LIS-770i
        - the specific pixel available at `VOUT` is controlled during readout
    - `ADC.IN-` connects to LIS-770i analog output `VREF`
        - `VREF` is the *dark voltage reference* output by the LIS-770i

The difference

~~~
ADC.IN+ - ADC.IN-
~~~

is the *dark-corrected pixel voltage*.

# Sketch outline of Chromation spectrometer `interface` for datasheet

## Table of Contents
- [Spectrum Measurement System Overview](#markdown-header-spectrum-measurement-system-overview)
    - [Spectrometer Overview](#markdown-header-spectrometer-overview)
        - [Components](#markdown-header-components)
        - [Measurement](#markdown-header-measurement)
    - [Digital Interface Overview](#markdown-header-digital-interface-overview)
        - [Acquire a spectrum measurement](#markdown-header-acquire-a-spectrum-measurement)
        - [Convert spectrometer analog output to digital](#markdown-header-convert-spectrometer-analog-output-to-digital)
        - [Dark-correct analog output in hardware before conversion](#markdown-header-dark-correct-analog-output-in-hardware-before-conversion)
    - [SPI-to-Host Bridge Overview](#markdown-header-spi-to-host-bridge-overview)
    - [System Host Overview](#markdown-header-system-host-overview)
        - [Dark-Correction Is Not Required](#markdown-header-dark-correction-is-not-required)
- [Details](#markdown-header-details)
    - [Recommended configuration of the LIS-770i](#markdown-header-recommended-configuration-of-the-lis-770i)
    - [Communication protocols in the measurement system](#markdown-header-communication-protocols-in-the-measurement-system)
        - [USB and FT1248](#markdown-header-usb-and-ft1248)
        - [Hardware Specific FT1248 Settings in `FT_Prog`](#markdown-header-hardware-specific-ft1248-settings-in-`ft_prog`)

---e-n-d---

# Spectrum Measurement System Overview
- A complete spectrum measurement system consists of:
    - spectrometer
    - SPI slave digital interface to communicate with the spectrometer
    - SPI master to bridge the digital interface and other slave devices to a
      system host
    - the system host application to request measurements and present the data
- TODO: diagram of complete system
- Chromation produces the spectrometer in a surface mount package compatible
  with standard reflow profiles
- Chromation offers the spectrometer as an IC package or soldered to a breakout
  PCB with an 8-pin flat-flexible cable:
    - TODO: photo of narrow QFN-48 package for PCB assembly by the customer
    - TODO: photo of QFN-48 package assembled on a small PCB breakout to an
      8-pin flat-flexible cable
- Chromation also offers a complete measurement system for evaluating the
  spectrometer:
    - mounted spectrometer
    - SPI slave digital interface
    - SPI master bridge
    - system host application for basic spectrum measurements
    - TODO: photo of complete system
    - TODO: screenshot of evaluation app

## Spectrometer Overview

### Components
The main components are the *photonic crystal* and the *linear pixel array*.

- TODO: diagram showing photonic crystal and linear pixel array

#### photonic crystal
- the *photonic crystal* is manufactured by Chromation
- light input to the spectrometer is spectrally separated by the *photonic
  crystal*
- the strong spectral separation of the *photonic crystal* provides high
  spectral resolution in a small form factor
- TODO: photo of photonic crystal

#### linear pixel array
- the spectrometer uses linear pixel array *Dynamax#* `LIS-770i`
- the *linear pixel array* measures the spectrally-separated light output by the
  photonic crystal
- each pixel collects a narrow wavelength bin from the spectrally-separated
  light
- the pixel's location in the linear array determines which narrow wavelength
  bin it collects
    - the system that interfaces the spectrometer handles conversion from pixel
      number to wavelength
    - the conversion maps each pixel to the center wavelength in that pixel's
      wavelength bin
    - Chromation currently provides this pixel-to-wavelength mapping data for
      each spectrometer

### Measurement
Every measurement is a *pixel exposure* immediately followed by a *pixel
readout*. After completing one measurement the
`LIS-770i` can sit idle arbitrarily long before taking the next measurement. 

#### pixel exposure
- the duration of *pixel exposure* is commonly referred to as the *integration
  time*
    - *integration time* varies from several milliseconds to several seconds
      depending on the measurement illumination power
- the `LIS-770i` has electronic switches to control the duration of *pixel
  exposure* to the light from the photonic crystal
- the pixels *accumulate charge* while exposed, resulting in a final voltage
  stored on each pixel at the end of the exposure period
- these pixel voltages represent the amount of light measured by each pixel,
  i.e., the amount of light in that pixel's wavelength bin

#### pixel readout
- immediately after the exposure ends, the pixel voltages are output
  sequentially for digital conversion by an external ADC
- the `LIS-770i` has electronic switches to control which pixel is connected to
  the `VOUT` pin

## SPI-to-Host Bridge Overview
- the SPI-to-host bridge is a SPI master for controlling the spectrometer and
  any other slave devices like an LED driver for light-source-triggered
  measurements
- the SPI master relays communication between the system host app and the
  spectrometer digital interface
- for light-source-triggered measurements, the SPI master coordinates the
  light-source and spectrometer:
    - enable the light source immediately before requesting a frame from the
      spectrometer digital interface
    - disable the light source before reading the frame from the spectrometer
      digital interface

## System Host Overview
- each frame of readout is a spectrum measurement
- the SPI master relays the measured spectrum to a system host for display,
  saving/logging, averaging, and for more complex data-processing
- the simplest data-processing is pixel-to-wavelength conversion
- examples of more complex data-processing:
    - power-normalizing the spectrum
    - converting from wavelength to color space or some application space
    - matching spectra to a database
    - displaying a go/no-go output
    - combining sample and baseline measurements as in an absorbance measurement

### Dark-Correction Is Not Required
- spectrometer data typically requires digital dark-correction by the system
  host before processing:
    - a frame of dark-data is collected at the same integration time as the
      measurement
    - the system host subtracts the dark-frame from the measurement
- the Chromation spectrometer does not require the system host to dark-correct
  if the spectrometer digital interface subtracts the `LIS-770i` dark-voltage
  from the `LIS-770i` pixel voltage in analog hardware
- Chromation recommends the spectrometer digital interface perform an analog
  dark-correction by taking a differential measurement at the ADC input, so each
  measurement received by the system host is already dark-corrected

# Details
- TODO: put the specifics of frame readout here
    - describe in terms of the actual LIS pins and the signals needed
    - do not put specifics for the ADC
    - pull this info from section: `LIS-770i frame readout`

### SPI communication for requesting a frame
#### SPI master requests a frame
- SPI master *selects* the *spectrometer digital interface* microcontroller by
  driving the `SS` (*slave select*) pin *LOW*
    - the *spectrometer digital interface* microcontroller is now in *SPI slave* mode
- SPI master loads SPI data register with byte to `request_frame`
    - this initiates the SPI transmission
- the byte in the SPI data register is shifted out
- SPI master drives `SS` *HIGH*
    - this releases the *spectrometer digital interface* microcontroller from *slave* mode
- SPI slave parses the byte and recognizes it as `request_frame`
- SPI slave coordinates with the ADC and LIS to get a frame of data

#### SPI slave responds with the frame
##### SPI slave prepares the frame of data
- SPI slave collects the entire frame of data
- SPI slave loads the first byte of data into the SPI data register
##### SPI slave signals the frame is ready
- if the *spectrometer digital interface* is on the same PCB as the SPI-to-Host
  bridge, dedicate a pin on the microcontroller to send the *frame is ready* signal
- Chromation has the *spectrometer digital interface* and the SPI-to-Host bridge
  on separate PCBs, requiring a cable to connect them
- to avoid wasting a connector pin on this *frame is read* signal, Chromation
  uses the SPI `MISO` pin to signal when the frame is ready
- SPI slave drives `MISO` *LOW* to signal that a frame is ready
- this is a deviation from SPI protocol because the SPI slave is driving `MISO`
  while its SPI bus is supposed to be idle
- this deviation is safe as long as the SPI master follows a contract:
    - when the SPI master sends `request_frame`, it waits for the frame
    - the SPI master does not open SPI communication with any other slaves until
      it gets the requested frame
- the other potential pitfall with this deviation is how the microcontroller SPI hardware
  implements driving the `MISO` pin
    - usually the SPI slave output is implemented for use with a pull-up
      resistor: the `MISO` pin is driven *LOW* and pulled-up *HIGH*
    - therefore, immediately after SPI communication ends, if the last bit out
      on `MISO` was a *LOW*, `MISO` slowly rises *HIGH* via its pull-up, it does
      not immediately swing *HIGH*
    - if the SPI master sent `request_frame` and is not waiting for `MISO` *LOW*
      as the signal that the frame is ready, it will mistake this slowly-rising
      `MISO` signal as `MISO` being ready
    - the solution is for the SPI slave to disable its SPI hardware immediately
      after recieving `request_frame` so it can actively drive `MISO` *HIGH*,
      and then re-enable the SPI hardware
    - the SPI master waits to see `MISO` go *HIGH* before it starts checking
      that `MISO` is *LOW*, signaling the frame is ready
##### SPI master reads out the frame
- drive `SS` *LOW*
    - this places the *spectrometer digital interface* microcontroller in *slave* mode
- SPI master initiates the transmission of each byte by writing to the SPI data
  register
    - it does not matter what the master writes to the SPI data register
- drive `SS` *HIGH*
    - this releases the SPI slave microcontroller from *slave* mode
    - SPI slave considers the frame finished


## Recommended configuration of the LIS-770i
- pixel binning: on
- use all five rows
- analog gain: 1x

## Communication protocols in the measurement system
### USB and FT1248
- the system app runs on a *USB* Host
- the SPI-to-Host Bridge has an FTDI USB-Bridge IC and uses the FTDI *FT1248*
  protocol to communicate with the USB Host:
    - the *SPI-to-Host Bridge* is the *FT1248 master*, meaning that it initiates
      FT1248 communication
    - the *USB Host* is the *FT1248 slave*
        - when the USB Host writes to the SPI-to-Host Bridge, the data sits in
          a `1K Rx buffer` in the FTDI USB-Bridge IC
        - the SPI-to-Host Bridge sees the buffer contains a message and, being
          the FT1248 master, it initiates FT1248 communication to read the
          message
        - similarly, when the SPI-to-Host Bridge writes to the USB Host, the
          data sits in a `1K Tx buffer` in the FTDI USB-Bridge IC
        - the USB Host receives the data when it is ready
    - the system app uses the FTDI `d2xx.dll` library to read and write the
      SPI-to-Host Bridge
        - the library contains many useful functions for setting up a serial
          communication library:
            - enumerating the connected devices,
            - checking the number of bytes available,
            - opening devices by name, closing devices
            - writing/reading bytes to/from an open device

# Ignore anything beyond here - -these are details to include later or not
## Digital Interface is SPI master to its ADC
- communicate with ADC over SPI to read the pixel voltages as 16-bit values:
    - ADC is the SPI slave
    - ATmega328 in *spectrometer digital interface* is the SPI master
        - uses the USART in SPI master mode
        - Slave select (or chip select) is just a general purpose i/o
        - the clock is USART XCK
        - MISO is USART RxD
        - there is no MOSI connection: the ADC is a read-only device
## FT1248 reference
### Hardware Specific FT1248 Settings in `FT_Prog`
- Run `FT_Prog`
- Scan for devices
- locate the `ChromationSpect-NNNN-NN` (`NNNN-NN` is the serial number)
- Select `Hardware Specific -> Ft1248 Settings`
- *8-bit wide configuration used on the LIS-770i interface:*
    - Clock Polarity High: unchecked
    - Bit Order LSB: *checked*
    - Flow Ctrl not selected: *checked*
#### Flow Ctrl
- if `Flow Ctrl not selected` is unchecked, then the Ft1248 slave does not use
  MISO and MIOSIO[0] to send Rx and Tx buffer status signals
- read this as `Use flow control when the slave is inactive`

#### Bit Order
- the bit order is LSB
- example: send the byte `0x01`
    - if `Bit Order LSB` is checked
    - then the USB host reads this as `0x01`
    - if `Bit Order LSB` is unchecked
    - then the USB host read this as `0x80`

#### Clock Polarity
- unchecked means the CPOL is 0 which means SCK idles *LOW*
- since the clock phase CPHA is always 1, this means SCK goes *HIGH* to signal
  pushing data onto the bus and SCK goes *LOW* to signal pulling data from the bus

- See FTDI Application Note `AN_167`
> When `CPOL` is 1, the idle state of the clock is *HIGH*.
> When `CPOL` is 0, the idle state of the clock is *LOW*.
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

The Chromation *spectrometer digital interface* uses `CPOL = 0` (`SCK` idles
`LOW`).

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
                                    // BW:8 (0)---------
                                    //                 |
                                    // CMD-nibble:  -01| 2--3
#define FT1248_CMD_WRITE8   0x86    //              1000 0110
#define FT1248_CMD_READ8    0xC6    //              1100 0110
                                    // Bit numbers: 7654 3210
```
There are four bits for the *command* nibble and four for the *bus-width*
nibble. A *LOW* on a particular bus-width bit indicates the bus width. The
other bits in the bus-width nibble should not matter, but to play it safe, the
other bits in the bus-width nibble are pulled *HIGH*.

## one frame of LIS-770i data
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
## LIS-770i power down
- see `Power Standby Mode`
- `Lis_Rst` is *HIGH*
- `Lis_Clk` stops and idles *LOW*
- remains in power down mode until `Lis_Rst` is pulled *LOW* and `Lis_Clk`
  is restarted
## LIS-770i - power up
- see `Power Up sequence`
- `Lis_Rst` is pulled *LOW*
- `Lis_Clk` starts
- this power up sequence resets the imager
- default programming is enabled:
    - select entire active pixel array
    - set output amplifier gain to 2.5x
    - turn off the summing mode
## LIS-770i programmable setup
- see `Programmable Setup Register`
- power up
- pull `Lis_PixSelect` *HIGH*
    - [x] `LisInit()` idles `Lis_PixSelect` *LOW*
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
    - hold `Lis_Rst` *HIGH* for 25 clock cycles after the gain bits are set
    - this completely contradicts what the datasheet says next
- 4th through 28th rising clock edges read `Lis_Rst`
    - sets the active pixel sub-arrays in the order P25 to P1
    - see *Table 5b: Selectable Pixel Array* to see which pixel is in which of
      the 25 pixel sub-arrays
        - a pixel has five segments in the *pixel height* direction
        - each segment is 62.5um tall
        - each sub-array is one segment tall
        - each sub-array spans 154 pixels
    - just keep reset *HIGH*, this selects every segment of every pixel

## LIS-770i frame readout
- `Lis_Clk` is running
    - [x] `LisInit()` pulls `Lis_Rst` *LOW*
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
- `Lis_Rst` is *LOW*
    - `LisInit()` has `Lis_Rst` idle *HIGH* for power-saving
    - but once the device is in power up mode, `Lis_Rst` is *LOW*
- `Lis_Rst` controls the integration period
    - in the following, `Lis_Rst` is pulled *HIGH* and *LOW*
    - the new value is clocked in on the very next rising edge of `Lis_Clk`
      after `Lis_Rst` has changed
    - [x] change the value of `Lis_Rst` just after a `Lis_Clk` falling edge
        - this avoids confusion over when the LIS shifts in the new value of
          `Lis_Rst`
        - at 50kHz, it is 10us from the clock falling edge to the clock rising
          edge
        - there is a 0.6us to 0.8us delay between the actual clock falling edge
          and the time it takes software to catch the fall and pull `Lis_Rst`
          *HIGH* or *LOW*
        - so in effect, I am guaranteed to have `Lis_Rst` always transition
          about 9us just before the next `Lis_Clk` rising edge, and to never
          transition during a clock edge
- `ExposureStart()` - pull `Lis_Rst` *HIGH*
    - resets all pixels
    - integration begins
- `Lis_Rst` goes *LOW*
    - integration ends
    - integration time is the number of clock cycles that `Lis_Rst` was *HIGH*
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
    - LisRst is *HIGH* for 20us * nticks + 2us
    - But the LIS is counting clock pulses, so exposure time is exactly 20us *
      nticks.
- `Lis_Sync` input is pulsed *HIGH*
> Once RST is brought *LOW* a synchronization signal (SYNC) pulse is fired
> starting on the next falling clock edge. Pixel readout then begins on the next
> rising CLK edge after the SYNC signal goes *LOW* and continues for 784 clock
> cycles. The SYNC signal is one clock period wide. SYNC is again fired during
> the last pixel readout on the falling edge of CLK.
    - `ExposureStop()` - pull `Lis_Rst` *LOW*
        - call this just after a `Lis_Clk` falling edge
        - exposure officially stops on the next `Lis_Clk` rising edge
    - `WaitForSync()` - watch for `Lis_Sync` to go *HIGH*, then *LOW*
        - call this just after calling `ExposureStop()`
        - `Lis_Sync` goes *HIGH* on the next falling clock edge
        - `Lis_Sync` then goes *LOW* on the next falling clock edge
        - pixel readout starts on the very next clock rising edge
    - `PixelReadout()` - sample analog `VOUT` while the clock is *HIGH*
        - video output may have disturbance during the falling clock edge
        - pixels 1 to 13 are dark
        - pixels 15 to 784 are optical




