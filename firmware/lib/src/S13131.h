#ifndef _S13131_H
#define _S13131_H

typedef uint8_t volatile * const s13131_ptr; // reg address type
typedef uint8_t const s13131_pin; // pin-number type
typedef uint8_t const s13131_bit; // bit-index type

// Register address, pin number, and bit definitions depend on compiler:
    // "gcc" uses test/HardwareFake.h -> S13131-HardwareFake.h
    // "avr-gcc" uses src/S13131-Hardware.h
// ---Registers---
extern s13131_ptr S13131_ddr;
extern s13131_ptr S13131_port;
extern s13131_ptr S13131_pin; // PortINput reg, NOT pin-number
extern s13131_ptr S13131_TCCR0A; // PWM timer 0 ctrl reg A
extern s13131_ptr S13131_TCCR0B; // PWM timer 0 ctrl reg B
extern s13131_ptr S13131_TIFR0;  // PWM timer 0 interrupt flags
extern s13131_ptr S13131_OCR0A;  // PWM period: fcpu/OCR0A
extern s13131_ptr S13131_OCR0B;  // PWM duty cycle: OCR0B/OCR0A

// ---Pins---
extern s13131_pin S13131_Clk; // port and ddr (PWM)
extern s13131_pin S13131_St;  // port and ddr
extern s13131_pin S13131_Eos; // pin and ddr

// ---Bits---
extern s13131_bit S13131_WGM00;  // PWM
extern s13131_bit S13131_WGM01;  // PWM
extern s13131_bit S13131_WGM02;  // PWM
extern s13131_bit S13131_CS00;   // PWM
extern s13131_bit S13131_CS01;   // PWM
extern s13131_bit S13131_CS02;   // PWM
extern s13131_bit S13131_COM0B0; // PWM
extern s13131_bit S13131_COM0B1; // PWM
extern s13131_bit S13131_OCF0A;  // PWM
extern s13131_bit S13131_OCF0B;  // PWM
#endif // _S13131_H
