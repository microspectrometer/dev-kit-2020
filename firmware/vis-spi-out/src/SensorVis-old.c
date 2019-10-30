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

