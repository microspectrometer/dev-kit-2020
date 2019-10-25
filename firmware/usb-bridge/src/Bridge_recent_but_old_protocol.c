void oldBridgeGetSensorLED(void) // Sensor has `led_0` and `led_1`.
{
    /** Check the state of an LED on the Sensor board. */
    /** BridgeGetSensorLED behavior: \n
      * - reads one byte of payload\n 
      * - responds ok after reading host payload\n 
      * - passes cmd to Sensor and waits for response\n 
      * - passes Sensor command response back to host\n 
      * - exits if Sensor responds error to command\n 
      * - if Sensor responds OK to command:\n 
      * - sends Sensor led number (command parameter)\n 
      * - reads two bytes of reply size and nbytes of reply from Sensor\n 
      * - passes reply size and reply to host\n 
      * */

    // Read which LED to query (one byte of payload).
    uint8_t const num_bytes_payload = 1;
    uint8_t read_buffer[num_bytes_payload];
    UsbReadBytes(read_buffer, num_bytes_payload);
    /* 2019-10-14: UsbReadBytes finishes and correctly returns led_number. */
    // TODO: Add error checking for time out.
        // CASE: host does not send expected number of bytes.

    SerialWriteByte(ok); // Bridge finished reading its expected payload.

    // Set up message: pass command and led_number along to Sensor.
    uint8_t led_number = read_buffer[0];
    uint8_t msg_to_sensor[] = {GetSensorLED_key, led_number};
    // The Sensor needs time to look up the command.
    // Send the command only, not the command parameter.
    uint8_t *p_msg_byte = msg_to_sensor;
    SpiWriteByte(*(p_msg_byte++));
    /* MacroSpiMasterWrite(*(p_msg_byte++)); */
    // Read status from Sensor about the requested command.
    // TODO: move this constant out to somewhere accessible by all Bridge functions.
    uint16_t const max_reply_size = 1569; // 1 msg status byte + 2*num_pixels
    uint8_t sensor_reply[max_reply_size];
    ReadSensor(sensor_reply, 1);
    uint8_t sensor_status_msg = sensor_reply[0];

    // Pass Sensor OK back up to host.
    SerialWriteByte(sensor_status_msg); // Bridge passes Sensor status to host

    // Error handler: exit if sensor status is not OK. Host will handle this.
    if (sensor_status_msg != ok) return;

    // Send command parameter LED number.
    SpiWriteByte(*(p_msg_byte++));
    /* MacroSpiMasterWrite(*(p_msg_byte++)); */

    /* Read response from SPI slave. */
    // Get reply_size and reply from Sensor.
    uint8_t sensor_reply_size[2]; // sensor_reply_size is always two bytes
    ReadSensor(sensor_reply_size, 2);
    // Get reply from Sensor (expect msg status byte and led status byte).
    uint16_t nbytes_reply = sensor_reply_size[0]<<8 | sensor_reply_size[1];
    ReadSensor(sensor_reply, nbytes_reply);

    // Pass reply_size and reply to host.
    uint16_t byte_count = 0;
    while (byte_count < (2))
    {
        SerialWriteByte(sensor_reply_size[byte_count]);
        byte_count++;
    }
    byte_count = 0;
    while (byte_count < (nbytes_reply))
    {
        SerialWriteByte(sensor_reply[byte_count]);
        byte_count++;
    }
}
void oldGetBridgeLED(void) // Bridge `led_0` is the `status_led`
{
    /** Check the state of the LED on the Bridge board. */
    /** GetBridgeLED behavior:\n
     * - reads one byte of payload.\n 
     * - replies msg status error if host queries nonexistent led.\n 
     * - replies with one byte if led number is not recognized.\n 
     * - replies msg status ok if host queries status led.\n 
     * - replies with two bytes if led number is recognized.\n 
     * - replies with msg status byte and led status byte.\n 
     * - replies led off if status led is off.\n 
     * - replies led green if status led is green.\n 
     * - replies led red if status led is red.\n 
     * */
    // Read which LED to query (one byte of payload).
    uint8_t const num_bytes_payload = 1;
    uint8_t read_buffer[num_bytes_payload];
    UsbReadBytes(read_buffer, num_bytes_payload);
    // TODO: Add error checking for time out.
        // CASE: host does not send expected number of bytes.

    // Reply to USB Host with message status byte.
    uint8_t led_number = read_buffer[0];
    if (led_number != led_0)
    {
        SerialWriteByte(error); // host is asking about nonexistent LED
        return;
    }
    SerialWriteByte(ok); // led_number is recognized, send msg_status: ok
    // Reply to USB Host with led status byte.
    if (!BiColorLedIsOn(status_led)) SerialWriteByte(led_off);
    else if (BiColorLedIsRed(status_led)) SerialWriteByte(led_red);
    else SerialWriteByte(led_green);
}

