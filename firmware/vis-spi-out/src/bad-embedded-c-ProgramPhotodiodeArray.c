static void ProgramPhotodiodeArray_Implementation(uint8_t *config)
{
    EnterLisProgrammingMode(); // Wait for Clock falling edge, PixSelect HIGH
    // This while loop reading a 32-bit value is problematic.
    uint8_t bit=0;
    if (0)
    {
        // I might not need these, but they are in the old code, no idea why.
        /* ClearBit(Lis_port1, Lis_Rst); // old code does this */
        /* LisWaitForClockRisingEdge(); // old code does this */
        /* LisWaitForClockFallingEdge(); // old code does this */
    }
    while (bit < 28)
    {
        // TODO: this is too time intensive to set up each time.
        // The old code just jumps straight into pulling Rst high/low.
        // config & (1<<bit) requires four `and` operations
        if (config & (1<<bit)) SetBit(Lis_port1, Lis_Rst); // sbi 0x0b, 6
        else ClearBit(Lis_port1, Lis_Rst); // cbi 0x0b, 6
        bit++;
        // Wait for Lis_Rst value to clock in before loading the next bit.
        LisWaitForClockRisingEdge(); // bit is read on rising edge
        LisWaitForClockFallingEdge(); // hold bit until falling edge
    }
    ExitLisProgrammingMode();
}

