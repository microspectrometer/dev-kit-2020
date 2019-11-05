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
