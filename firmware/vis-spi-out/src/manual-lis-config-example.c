#define manual_config 0 // troubleshoot LIS programming sequence

// ...

    if (manual_config)
    {
        // ---Manual config---
        // binning on
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        // gain 1x
        ClearBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        ClearBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        // all the rows on
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
        SetBit(Lis_port1, Lis_Rst); LisWaitForClockRisingEdge(); LisWaitForClockFallingEdge();
    }

