# Assembly Example
- `Spi_ddr` is `DDRB` in `vis-spi-out/src/Spi-Hardware.h`
- `Spi_Miso` is `PB4` in `vis-spi-out/src/Spi-Hardware.h`
- `DDRB` is address 0x04
    - this is between 0x00 and 0x1F, so it is in range for single-bit instructions
    - single bit instructions are CBI SBI SBIC SBIS
    - these instructions only affect a single bit in the register
- `DDRB` is listed as address 0x04 (0x24)
    - this notation means the I/O address is 0x04 but the data space address is
      0x24
    - use 0x24 if the instruction is LD or ST
    - context:
        - the *complete* I/O register range is 0x00 to 0x3F
        - any register in this range has I/O instructions IN and OUT for
          writing/reading the entire register instead of a single bit
        - when using IN and OUT, use the I/O register address
        - when using LD and ST, use the data space address
        - adding 0x20 maps the address to data space
- expect `SetBit(Spi_ddr, Spi_Miso);` to be a single `sbi` instruction:
    - `sbi 0x04, 4`
- but that is not what happens because the call is indirect
- call chain:
    - main() calls SpiSlaveInit()
    - SpiSlaveInit() calls SetMisoAsOutput()
    - SetMisoAsOutput() is a `static` function that calls
      `SetBit(Spi_ddr, Spi_Miso);`
    - why `static`? that makes it private
    - why a private one-liner definition?
    - the private one-liner definition in lib `Spi` means `SetMisoAsOutput`
      makes the `SetBit` function readable without a comment
- so somewhere in `SpiSlaveInit()` we're expecting to find the single `sbi`
- search the `.lst` for `<SpiSlaveInit>:`

# What if I care?
I absolutely don't care that the assembly for `SpiSlaveInit` is horribly
complicated. It runs once, at startup.

I do care if I've failed to inline something that gets called *a lot* in a
time-sensitive loop.

Look at GetFrame by searching `.lst` for `<GetFrame_Implementation>:`. Wait for
readout to start by checking if an I/O pin is low:

```c
// Wait for readout to start
while(BitIsClear(Lis_pin1, Lis_Sync)); // wait for SYNC rising redge
```
Lis_pin1 is &PIND. Lis_Sync is PD7. PIND is 0x09. So looking for sbis or sbic
0x09, 7. Never happens. Search for `Lis_pin1` to find where this code actually
starts in the function. It's a fucking mess.

`Pwm_tifr0  = &TIFR0;`. TIFR0 is 0x15. That's within range too!

Calling `SetBit(Pwm_tifr0, Pwm_Ocf0b);` directly from `main()` inlines
perfectly.

Calling `SetBit(Pwm_tifr0, Pwm_Ocf0b);` from a function in `SensorVis.c` does
not inline because the PWM hardware dependencies are unresolved when this
translation unit is compiled.

Try making the `avr` build target build `SensorVis.c` only when it builds
`vis-spi-out.c`, i.e., do not make `vis-spi-out.elf` dependent on `SensorVis.o`
in the same way that `vis-spi-out.elf` is not dependent on `vis-spi-out.o`.
