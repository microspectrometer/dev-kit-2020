% Chromation Spectrometer: Interface Information for Preliminary Datasheet
% Mike Gazes
% December 3rd, 2018

# Recommended Operating Conditions
 Symbol  Parameter           MIN   TYP   MAX     Units
-------- ----------         ----- ----- -----   -------
  V~DD~  Supply Voltage     --      3.0 --      V
  f~CLK~ Clock Frequency    15      50  200     kHz
  CLK~S~ Setup time         --      10  --      ns
  CLK~H~ Hold time          --      10  --      ns

Table: Recommended operating conditions

# Recommended Interface Design Parameters

TODO: correct the values in this table

Parameter           MIN    TYP   MAX   Units
----------         -----  ----- ----- -------
Resolution          10      16  --      bits
Voltage reference   1.8    2.048        V

Table: Recommended ADC configuration for the spectrometer interface

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

## Spectrometer Interface Block Diagram
The block diagram below shows the spectrometer signals referred to in the
following sequences to initialize, configure, and acquire a spectrum. The block
diagram also shows how the **spectrometer digital interface** converts the
spectrometer to a *SPI slave* device.

![Spectrometer Interface Block Diagram](img/spectrometer-interface-block-diagram.png)

The purpose of the **spectrometer digital interface** is to hide the operational
details of the `LIS-770i` and ADC, and instead provide the measurement system
with a standard interface. Chromation recommends presenting the spectrometer
interface to upstream devices as a *SPI slave*. A *SPI master* in the upstream
measurement system executes requests from the system host application by writing
high-level commands recognized by the *SPI slave*.


## Initialize the spectrometer after power-up
- TODO: initialize-spectrometer-signal-diagram

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
- TODO: configure-spectrometer-signal-diagram

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
- TODO: acquire-spectrum-signal-diagram

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

