# Design notes for usb-bridge

![render](render/pcb.png)

This is not an explanation of the design, just documentation on
some design decisions.

# Power

## FT221X makes its own 3.3V supply internally

- convert 5V to 3.3V in FT221X
    - tie `3V3OUT` to `VCCIO`:
        - use the internal 3.3V regulator for the `FT221X` I/O
          logic levels
        - eliminates pull-ups on the FT221X data lines:
            - if VCCIO is less than 3.3V, external pull-ups are
              needed on data lines
            - schematic for `UMFT221X` shows *12* `47k` pull-ups
            - avoid adding these pull-ups by using a 3.3V `VCCIO`
            - connect `VCCIO` directly to `3V3OUT`
    - tie `RESET#` to `3V3OUT` as well
    - do not power other devices from the FT221X `3V3OUT`
        - use an external voltage regulator to generate a 3.3V
          supply from the 5V USB power

## Power everything besides FT221X from single 3.3V regulator

- use separate ADP3303, do not use FT221X `3V3OUT`
- expect power consumption is barely noticeable in terms of
  regulator package heating:
    - MCU active at 10MHz and 3.3V:
        - a little below 4 mA
        - 4 mA * 3.3 V = 13.2 mW
    - each debug LED:
        - (3.3 V - 2.2 V) / 0.3 kΩ = 3.7 mA
    - detector:
        - 10mW max (5mW typ)
        - 10 / 3.3 = 3 mA
    - ADC:
        - 1.22mW
        - 1.22 / 3.3 = 0.4 mA
    - `3.3 V` current draw on `usb-bridge`:
        - 4 (MCU) + 3.7 (LED) = `7.7 mA`
    - `3.3 V` current draw on `vis-spi-out`:
        - 4 (MCU) + 4*3.7 (LED) + 0.4 (ADC) + 3 (LIS) = `22.2 mA`
    - estimate about `30 mA` total current load on the `3.3 V`
      voltage regulator
    - multiply by the voltage drop: `5 V - 3.3 V = 1.7 V`
    - power consumption is `0.051 W`
    - thermal resistance is 96°C/W
    - expect temperature to rise about 5°C
    - if ambient is 25°C, the voltage regulator runs at 30°C
    - that's fine
    - and that is based on the maximum power values

# MCU Crystal Capacitor Calcs
- Calculate capacitance `C1` = `C2` = C
- `2*(C_load - C_stray)` = C
- assume `C_stray` = `5 pF`
- Want `C_load` = `18 pF` (datasheet)
- `C1` = `C2` = `26 pF` class 1 (`C0G` or `NP0`)
- If `C_stray` = `4 pF`, `C1` = `C2` = `28 pF`
- `27 pF` is a standard size, available in 0603

