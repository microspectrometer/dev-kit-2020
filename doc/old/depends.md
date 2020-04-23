# mBrd/src/mBrd.c
## headers that resolve hardware abstractions in the libs
- mBrd/src/DebugLeds-Hardware.h
- mBrd/src/Spi-Hardware.h
- mBrd/src/UartSpi-Hardware.h
- mBrd/src/Lis-Hardware.h
- mBrd/src/Pwm-Hardware.h

## commands understood by SPI master and slave
- mBrd/src/Spi-Commands.h
    - [ ] move out the definitions that are not SPI commands

## libs
- lib/src/ReadWriteBits
- lib/src/DebugLeds
- lib/src/Spi
- lib/src/UartSpi
- lib/src/Lis
- lib/src/Pwm
- C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\avr\interrupt.h

## header to resolve lib dependencies on AVR asm macros
- lib/src/AvrAsmMacros.h
    - this breaks with the usual API pattern where a module lib is .h + .c
    - this follows the Hardware Abstraction pattern
    - the lib is header-only
    - the lib has `extern` function pointers
    - client code includes the lib to access the function pointer declarations,
      and it has its own definitions for implementations of the function
    - application code points to a function definition that wraps the AVR macro
    - test code points to an empty function definition
    - the header-only lib is effectively a list of AVR macros used in the
      project

# simBrd/src/simBrd.c
## headers that resolve hardware abstractions in the libs
- simBrd/src/DebugLed-Hardware.h
- simBrd/src/Ft1248-Hardware.h
- mBrd/src/Spi-Hardware.h

## libs
- lib/src/DebugLed.h
- lib/src/Ft1248.h
- lib/src/Usb.h
- lib/src/Spi.h
- lib/src/ReadWriteBits.h

## commands understood by SPI master and slave
- mBrd/src/Spi-Commands.h
    - [ ] move out the definitions that are not SPI commands

## header to resolve lib dependencies on AVR asm macros
- lib/src/AvrAsmMacros.h

