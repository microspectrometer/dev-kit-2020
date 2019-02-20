static uint16_t PeakCounts_Implementation_Use_All_Pixels(void)
{
    /* =====[ get a frame ]===== */
    MacroDebugLedsTurnRed(debug_led1);
    LisFrameReadout();  // store pixel readout in SRAM
    MacroDebugLedsTurnGreen(debug_led1);
    /* =====[ get the peak ]===== */
    // determine the number of pixels in a frame
    uint16_t npixels_in_frame;
    if (lis_sum_mode == lis_summing_on) npixels_in_frame = npixels_binned;
    else                                npixels_in_frame = npixels;
    // walk the frame to find the peak
    Lis_npixels_counter = 0;  // initialize the global pixel counter
    pframe = full_frame;  // point to the start of pixel readout memory
    uint16_t peak = 0;
    while (Lis_npixels_counter++ < npixels_in_frame)
    {
        uint16_t this = (*(pframe++))<<8;
        this |= (*(pframe++));
        if (this > peak) peak = this;
    }
    return peak;
}

