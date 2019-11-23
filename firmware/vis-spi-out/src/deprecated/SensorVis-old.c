/* =====[ DEPRECATED ]===== */
/* Define a named key for each function (`FooBar_key` is the key for `FooBar`) */
/* sensor_cmd_key const SensorLed1Green_key = 0; */
/* sensor_cmd_key const SensorLed1Red_key = 1; */
/* sensor_cmd_key const SensorLed2Green_key = 2; */
/* sensor_cmd_key const SensorLed2Red_key = 3; */
/* TODO: left off here */

// .h
// Do not use this. Use ReplyCommandInvalid() instead.
void SpiSlaveWrite_StatusInvalid(sensor_cmd_key invalid_cmd);
// report status to SpiMaster
void SpiSlaveWrite_StatusOk(sensor_cmd_key valid_cmd);
// .c
// Do not use this.
void SpiSlaveWrite_StatusInvalid(sensor_cmd_key invalid_cmd)
{                                  // | nbytes  |   data          |
    uint8_t const StatusInvalid[] = { 0x00, 0x02, 0xFF, invalid_cmd };
    SpiSlaveSendBytes(StatusInvalid,4);
}
/* SpiSlaveSendBytes has been unit-tested. No need to unit test this. */
void SpiSlaveWrite_StatusOk(sensor_cmd_key valid_cmd)
{
                             // | nbytes  | data           |
    uint8_t const StatusOk[] = {0x00, 0x02, 0x00, valid_cmd };
    SpiSlaveSendBytes(StatusOk,4);
}

void oldGetSensorLED(void)
{
    /** GetSensorLED behavior:\n 
      * - replies with three bytes if led is non existent\n 
      * - replies msg status error if led is non existent\n 
      * - replies with four bytes if led number is recognized\n 
      * - replies msg status ok if led number is recognized\n 
      * - replies led off if led is off\n 
      * - replies led green if led is green\n 
      * - replies led red if led is red\n 
      * */
    // System test: is this function called? Yes.
    /* Tell SpiMaster you received the command and are ready to receive the led number. */
    uint8_t cmd_ok_reply[] = {ok}; //
    WriteSpiMaster(cmd_ok_reply, 1);
    // Read which LED to query (one byte of payload).
    uint8_t const num_bytes_payload = 1;
    uint8_t read_buffer[num_bytes_payload];
    ReadSpiMaster(read_buffer, num_bytes_payload);
    uint8_t led_number = read_buffer[0];
    // OK up to here! We know the correct led number is read in.
    // Test sending the multi-byte responses.
    //
    // Reply to SPI Master with size, message status byte, and optional data.
    // Return msg_status error if LED is nonexistent.
    if ((led_number != led_0) && (led_number != led_1))
    {
        uint8_t error_reply[] = {0x00, 0x01, error}; // size: 1, status: error
        /* DEBUG_LedsShowError(); */
        WriteSpiMaster(error_reply,3); // host is asking about nonexistent LED
        return;
    }
    uint8_t led;
    if (led_number == led_0) led = led_TxRx;
    else if (led_number == led_1) led = led_Done;
    uint8_t led_status;
    if (!BiColorLedIsOn(led)) led_status = led_off;
    else if (BiColorLedIsRed(led)) led_status = led_red;
    else led_status = led_green;
    uint8_t ok_reply[] = {0x00, 0x02, ok, led_status}; // size: 2, msg_status: ok, led_status:
    WriteSpiMaster(ok_reply,4);
    /* DEBUG START: */
    /* return; */
    /* DEBUG END */
}

static uint16_t NoIsrWriteSpiMaster_Implementation(uint8_t const *write_buffer, uint16_t nbytes)
{
    ClearBit(Spi_spcr, Spi_InterruptEnable); // Disable SPI interrupt
    SpiSlaveSendBytes(write_buffer, nbytes); // Placeholder until I can clean this up.
    SetBit(Spi_spcr, Spi_InterruptEnable); // Enable SPI interrupt
    return nbytes; // TODO: use actual num_bytes_sent
    /* return num_bytes_sent; */
}
// ---Start From Lis.h---
void LisWriteCfg(uint8_t const * cfg);
void OutputCfgByte(uint8_t const cfg_byte, uint8_t const nbits);
// ---End From Lis.h---
// ---Start From Lis.c---
void LisWriteCfg(uint8_t const * cfg)
{
    /** Program the Lis with the four `cfg` bytes sent from the SpiMaster.
     *  The programming sequence clocks 28 bits into the LIS-770i.
     *  `cfg` is the 28-bit sequence stored in four bytes.
     *  Bit[0] is bit index 0 of `cfg[3]`.
     *  Bit[27] is bit index 3 of `cfg[0]`.
     *
     *  The 28 bits are output sequentially on Lis_Rst, starting with bit 0.
     *  Bit 0 is binning on/off. Bits 1 and 2 set the gain. The remaining 25
     *  bits select which pixels are active.
     * */

    /* bit 0: bin_on_off */
    /*     bin_on: bin_on_off = 1 */
    /* bit 1: gain bit G2 */
    /* bit 2: gain bit G1 */
    /*     gain_1x: G2 G1 = 0 0 */
    /* next are 25 bits to select pixels by row in groups of 154 */
    /* use these 25 bits to select entire rows (groups of 784) */
    // Plan of attack:
        // Four bytes received from SpiMaster.
        // Most significant byte received first.
        // Start with the least significant byte (cfg[3]).
        // Load all of its bits onto Lis_Rst
        // do the same for the next two bytes
        // then only load the four least LSB of the most significant byte
    EnterLisProgrammingMode();
    // load bits
    // TODO: unit test this.
        // I only wrote this code to silence the -Wunused parameter warning:
        // Spectrometer configuration is four bytes.
    uint8_t const num_cfgbytes = 4;
    // Start at the least significant byte (the last byte in cfg).
    // Only do cfg[3] to cfg[1]. Byte cfg[0] is a special case.
    uint8_t byte_index = num_cfgbytes;
    while (--byte_index != 0)
    {
        uint8_t const nbits = 8;
        OutputCfgByte(cfg[byte_index], nbits);
        /* printf("\nByte cfg[%d] uses nbits=%d", byte_index, nbits); */
    }
    // Most significant byte (the first byte in cfg) only has four bits to output.
    uint8_t const nbits = 4;
    OutputCfgByte(cfg[0], nbits);
    /* printf("\nByte cfg[%d] uses nbits=%d\n", byte_index, nbits); */
    ExitLisProgrammingMode();
}
/* =====[ Helper function for LisWriteCfg ]===== */
inline void OutputCfgByte(uint8_t const cfg_byte, uint8_t const nbits)
{
    /** Output `nbits` of cfg_byte, from bit[0] to bit[n-1].
     *  Output values sequentially on pin `Lis_Rst`.
     *  Do nothing if `nbits` is 0 or `nbits` is > 8.
     * */

    // This function does nothing if nbits is out of bounds.
    if ( (nbits < 1) || (nbits > 8) ) return;
    // For each bit in cfg_byte[0:nbits], output masked bit to Lis_Rst.
    for (uint8_t bit_index = 0; bit_index < nbits; bit_index++)
    {
        if ( cfg_byte & (1<<bit_index)) { SetBit(Lis_port1, Lis_Rst); }
        else { ClearBit(Lis_port1, Lis_Rst); }
        // Wait for Lis_Rst value to clock in before loading the next bit.
        LisWaitForClkRiseEdge(); // bit is read on rising edge
        LisWaitForClkFallEdge(); // hold bit until falling edge
    }
}
/* TODO: figure out how the SpiSlave gets cfg from SpiMaster, then come back to this */

// ---End From Lis.c---
// ---Start from test_Lis.c---
/* =====[ LisWriteCfg ]===== */
/* This is an API call for the client to load a configuration into the LIS. */
void LisWriteCfg_example_usage(void)
{
    /** `LisWriteCfg` handles entering and exiting the LIS programming mode.
     *  For each byte in the `cfg` byte stream,
     *  `LisWriteCfg` calls `OutputCfgByte`.
     * */
    /** LisWriteCfg cannot be tested because I do not mock its helpers:
     *  EnterLisProgrammingMode()
     *  OutputCfgByte()
     *  ExitLisProgrammingMode()
     *  I do not mock its helpers because I do not want function pointer seams.
     *  Function pointer seams prevent inlining. Calls compile with `jump` and
     *  `return`.
     * . */
    /* Therefore, this *test* is just a documentation example. */

    /* Fake receiving a valid `cfg`. */
    uint8_t const valid_cfg[] = {0x0F, 0xFF, 0xFF, 0xF9};
    /* =====[ Operate ]===== */
    LisWriteCfg(valid_cfg);
}
void OutputCfgByte_does_nothing_if_arg_nbits_is_0(void)
{
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00; // fake Lis_Rst pin is low
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* Fake a cfg byte that makes Rst high. */
    uint8_t const cfg_byte = 0xFF;
    /* =====[ Operate ]===== */
    uint8_t const bad_nbits = 0;
    OutputCfgByte(cfg_byte, bad_nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[nbits-1] *if nbits is in bound*. */
    /* But nbits is out of bounds, so Rst is still low from set up. */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
}
void OutputCfgByte_does_nothing_if_arg_nbits_is_more_than_8(void)
{
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00; // fake Lis_Rst pin is low
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* Fake a cfg byte that makes Rst high. */
    uint8_t const cfg_byte = 0xFF;
    /* =====[ Operate ]===== */
    uint8_t const bad_nbits = 9;
    OutputCfgByte(cfg_byte, bad_nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[nbits-1] *if nbits is in bound*. */
    /* But nbits is out of bounds, so Rst is still low from set up. */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
}
void OutputCfgByte_outputs_cfg_byte_on_Lis_Rst(void)
{
    uint8_t nbits, cfg_byte;
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00;
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* =====[ Operate ]===== */
    /* Fake a cfg byte that makes Rst high on bit[nbits-1]. */
    nbits = 1; cfg_byte = 0x01; // 0b 0000 0001
    OutputCfgByte(cfg_byte, nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[nbits-1]. */
    TEST_ASSERT_BIT_HIGH(Lis_Rst, *Lis_port1);
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00;
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* =====[ Operate ]===== */
    /* Fake a cfg byte that makes Rst high on bit[nbits-1]. */
    nbits = 2; cfg_byte = 0x02; // 0b 0000 0010
    OutputCfgByte(cfg_byte, nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[nbits-1]. */
    TEST_ASSERT_BIT_HIGH(Lis_Rst, *Lis_port1);
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00;
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* =====[ Operate ]===== */
    /* Fake a cfg byte that makes Rst high on bit[nbits-1]. */
    nbits = 8; cfg_byte = 0x80; // 0b 1000 0000
    OutputCfgByte(cfg_byte, nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[nbits-1]. */
    TEST_ASSERT_BIT_HIGH(Lis_Rst, *Lis_port1);
    //
    /* Prove the test really is looking at bit index nbits-1. */
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00;
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* =====[ Operate ]===== */
    /* Fake a cfg byte that makes Rst high on bit[nbits-1]. */
    nbits = 8; cfg_byte = 0x08; // 0b 0000 1000
    OutputCfgByte(cfg_byte, nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[7]. */
    /* But this test uses a cfg_byte with bit[3] set and bit[7] clear. */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
}

// ---End from test_Lis.c---
void SensorCfgLis(void)
{
    /* TODO: left off here */
    ;// get 4-byte arg from bridge
    /* Fake receiving a valid `cfg`. */
    uint8_t const valid_cfg[] = {0x0F, 0xFF, 0xFF, 0xF9};
    LisWriteCfg(valid_cfg);
    // LisWriteCfg must handle the StatusOk since it follow that with
    // the updated cfg.
}

// ---Start SensorVish---
// RepresentConfigAs28bits is deprecated
uint32_t RepresentConfigAs28bits(uint8_t binning, uint8_t gain, uint8_t active_rows);
// ---End SensorVis.h---
// ---Start SensorVis.c---
// RepresentConfigAs28bits is deprecated -- need four bytes instead of uint32_t
uint32_t RepresentConfigAs28bits(uint8_t binning, uint8_t gain, uint8_t active_rows)
{
    /** Return the 28-bit LIS config given three config bytes.\n 
     *  Input format for the three config bytes:
     *  - binning: 0x00 (off), 0x01 (on)\n 
     *  - gain: 0x01 (1x), 0x25 (2.5x), 0x04 (4x), 0x05 (5x)\n 
     *  - active_rows: 5 rows, set the bit to turn the row on\n 
     *    bits [8..0]: xxx54321 (1 to 5 are the row numbers, x is don't care)\n 
     *  Output format:\n 
     *  - bit0 of the 28-bit sequence is bit0 of the uint32 return value\n 
     *  - program the LIS starting with bit0 */
    /** RepresentConfigAs28bits behavior:\n 
      * - returns uint32 with bit0 set if binning is on\n 
      * - returns uint32 with bit0 clear if binning is off\n 
      * - returns uint32 with bits1to2 clear if gain is 1x\n 
      * - returns uint32 with bit1 clear bit2 set if gain is 2p5x\n 
      * - returns uint32 with bit1 set bit2 clear if gain is 4x\n 
      * - returns uint32 with bits1to2 set if gain is 5x\n 
      * - returns uint32 with bits3to27 set if all rows are active\n 
      * - returns uint32 with b3b8b13b18b23 set if row1 is active\n 
      * - returns uint32 with b4b9b14b19b24 set if row2 is active\n 
      * - returns uint32 with b5b10b15b20b25 set if row3 is active\n 
      * - returns uint32 with b6b11b16b21b26 set if row4 is active\n 
      * - returns uint32 with b7b12b17b22b27 set if row5 is active\n 
      * */
    // Convert three data bytes to 28-bit LIS programming sequence.
    uint32_t config = 0x00000000;
    uint8_t bit = 0;
    if (binning_on == binning) config |= 1<<(bit++); // bit 0: bin on/off
    else bit++;
    // bit 1: gain bit G2
    // bit 2: gain bit G1
    // {G2,G1}: {0,0} 1x; {0,1} 2.5x; {1,0} 4x; {1,1} 5x
    if      (gain25x == gain) { bit++; config |= 1<<(bit++); }
    else if (gain4x == gain)  { config |= 1<<(bit++); bit++; }
    else if (gain5x == gain)  { config |= 1<<(bit++); config |= 1<<(bit++); }
    else { bit++; bit++; }
    // bit 3 to 27 are pixel groups P25 to P1 to select active rows
    // Example with binning_on and gain1x
    // ----3----  ----2----  ----1----  ----0-(---) // byte
    // 7654 3210  7654 3210  7654 3210  7654 3(210) // bit
    // xxxx 1111  1111 1111  1111 1111  1111 1(001) // all rows on
    // xxxx 0000  1000 0100  0010 0001  0000 1(001) // row 1 (or 5?)
    // xxxx 0001  0000 1000  0100 0010  0001 0(001) // row 2 (or 4?)
    // xxxx 0010  0001 0000  1000 0100  0010 0(001) // row 3
    // xxxx 0100  0010 0001  0000 1000  0100 0(001) // row 4 (or 2?)
    // xxxx 1000  0100 0010  0001 0000  1000 0(001) // row 5 (or 1?)
    uint8_t const row1 = 0; uint32_t const row1_mask = 0x00842108;
    uint8_t const row2 = 1; uint32_t const row2_mask = 0x01084210;
    uint8_t const row3 = 2; uint32_t const row3_mask = 0x02108420;
    uint8_t const row4 = 3; uint32_t const row4_mask = 0x04210840;
    uint8_t const row5 = 4; uint32_t const row5_mask = 0x08421080;
    if (active_rows&(1<<row1)) config |= row1_mask;
    if (active_rows&(1<<row2)) config |= row2_mask;
    if (active_rows&(1<<row3)) config |= row3_mask;
    if (active_rows&(1<<row4)) config |= row4_mask;
    if (active_rows&(1<<row5)) config |= row5_mask;
    return config;
}
// ---End SensorVis.c---
// ---Start test_SensorVis.c---
// tdd LIS programming sequence
void bit_number_is_correct_on_each_iteration_of_while_loop(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_on;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    uint32_t config = RepresentConfigAs28bits(binning, gain, active_rows);
    /* =====[ Operate and Test ]===== */
    uint8_t bit=0;
    while (bit < 28)
    {
        if (config & (1<<bit))
        {
            /* SetBit(Lis_port1, Lis_Rst); */
            printf("set bit %d\n", bit);
        }
        else
        {
            /* ClearBit(Lis_port1, Lis_Rst); */
            printf("clear bit %d\n", bit);
        }
        bit++;
        // Wait for Lis_Rst value to clock in before loading the next bit.
        /* LisWaitForClkRiseEdge(); // bit is read on rising edge */
        /* LisWaitForClkFallEdge(); // hold bit until falling edge */
    }
    TEST_FAIL_MESSAGE("Implement test.");
}
// ---End test_SensorVis.c---
