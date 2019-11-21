static inline void LisReadOnePixel(void)
{
    // ---Obtain 16-bit value for next pixel and save to frame---
    LisWaitForClockRisingEdge();
    // sbi	0x15, 1	; 21
    // sbis	0x15, 1	; 21
    // rjmp	.-4      	;
    // sbi	0x15, 1	; 21
    /* Delay3ClocksPerTick(6); // 6*0.3us = 1.8us delay here, no affect */
    StartAdcConversion(); // sbi	0x0b, 2	; 11
    // ---Wait for conversion to end---
    // Need to delay for at least 4.66us
    /* WaitForConversion(); // hard-coded delay of 16*0.3us=4.8us */
    WaitForConversion(16); // hard-coded delay of 16*0.3us=4.8us
    // ldi	r22, 0x10	; 16
    // (wait for rising edge happens here, after loading 16 ticks)
    // (then after ADC conversion starts, the delay starts)
    // mov	r24, r22
    // dec	r24
    // brne	.-4      	;
    /* LisWaitForClockFallingEdge(); // 10us delay (half-clock period) */
    // sbi	0x15, 2	; 21
    // sbis	0x15, 2	; 21
    // rjmp	.-4      	;
    // sbi	0x15, 2	; 21
    StartAdcReadout(); // cbi	0x0b, 2	; 11
    WaitForEmptyTxBuffer(); // always empty on this first check
    // UCSR0A (0xC0), UDRE0 (bit 5) is set when the data transfer is done
    // lds	r24, 0x00C0	; 0x8000c0 <__TEXT_REGION_LENGTH__+0x7e00c0>
    // sbrs	r24, 5
    // rjmp	.-8      	; 0x714 <GetFrame_Implementation+0x70>
    // TEST: What if I don't wait for an empty Tx buffer?
    // RESULT: Makes no difference.
    /* StartAdcReadout(); // cbi	0x0b, 2	; 11 */
    // TEST: Pull CONV low now instead of earlier?
    // RESULT: Makes no difference.
    // Transfer 16 bits by writing two dummy bytes to UartSpi_Data.
    // UartSpi_Data is reg UDR0 (0xC6)
    // See datasheet section 20.5 "Frame Formats"
    // > 16-bit data transfer can be achieved by writing two data
    // > bytes to UDRn. A UART transmit complete interrupt will
    // > then signal that the 16-bit value has been shifted out. 
    // TEST: Send the bytes one at a time instead of two at a time?
    // RESULT: Makes no difference.
    // sts is store direct to SRAM
    // 0xC6 is UDR0 (UartSpi_data), and r1 is 0x00
        // sts	0x00C6, r1	;
    *UartSpi_data = 0x00;
    // TEST: wait before sending second byte.
    /* WaitForEmptyTxBuffer(); // always empty on this first check */
    // RESULT: Makes no difference.
    // sts	0x00C6, r1	;
    *UartSpi_data = 0x00;
    // Uart Spi data reg is UDR0 = 0xC6
    // sts	0x00C6, r1	;
    // sts	0x00C6, r1	;
    WaitForByteFromAdc(); // first byte of ADC readout
    // RXC0 (RxComplete) is bit 7
    // lds	r24, 0x00C0	;
    // sbrs	r24, 7
    // rjmp	.-8      	;
    // Save the most significant byte of the pixel reading in the frame
    /* (void) *UartSpi_data; *(pframe++) =  pixel_count >> 8; // ramp for system testing */ 
    *(pframe++) =  *UartSpi_data;
    // Load direct from SRAM: UDR0 (0xC6) load into r24
        // lds	r24, 0x00C6	;
    // Write this byte of pixel data to Z 
        // st	Z, r24
    // Z has address 0x0158:
        // ldi	r30, 0x58	; 88
        // ldi	r31, 0x01	; 1
    // so this is the address pframe is pointing to
    // does this address increment like it's supposed to? no
    // immediately, the code waits for another byte:
        // lds	r24, 0x00C0	;
        // sbrs	r24, 7
        // rjmp	.-8      	;
    // But now it increments the pointer:
    // r30 is the lo byte of the address pointed to by pframe
        // movw	r24, r30
    // increment address by 2 (for 2 bytes)
        // adiw	r24, 0x02	; 2
    // save least significant byte of pixel reading in r18
        // lds	r18, 0x00C6	;
    // and store at address pointed to by pframe
        // std	Z+1, r18	; 0x01
    // TEST: Replace data with ramp to check communication.
    /* *(pframe++) =  pixel_count >> 8; // ramp for system testing */ 
    /* *(pframe++) =  pixel_count & 0xFF; // ramp for system testing */
    // RESULT: Pass. Output is a ramp (counts = pixel number).
    // CONCLUSION: problem is the ADC conversion or the LIS itself.
    //
    // Save the least significant byte of the pixel reading in the frame
    // TEST: Do not wait for a second byte. Read immediately.
    /* *(pframe++) =  *UartSpi_data; */
    // RESULT: Makes no difference. Probably because the MSByte is bad.
    WaitForByteFromAdc(); // second byte of ADC readout
    /* (void) *UartSpi_data; *(pframe++) =  pixel_count & 0xFF; // ramp for system testing */
    *(pframe++) =  *UartSpi_data;
}

