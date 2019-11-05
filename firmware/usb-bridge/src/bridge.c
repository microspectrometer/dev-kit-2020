#include "Bridge.h"
#include "BiColorLed.h"
#include "Spi.h"
#include "Usb.h"
#include "stdlib.h" // defines NULL

#define npixels 784
extern uint8_t frame[npixels*2];

// =====[status_led pin number defined in BiColorLed-Hardware header]=====
extern uint8_t const status_led;

/* =====[ API started 2019-10-02 ]===== */
/* Define a named key for each function (`FooBar_key` is the key for `FooBar`) */
bridge_cmd_key const NullCommand_key                        = 0x00;
bridge_cmd_key const GetBridgeLED_key                       = 0x01;
bridge_cmd_key const SetBridgeLED_key                       = 0x02;
bridge_cmd_key const BridgeGetSensorLED_key                 = 0x03;
bridge_cmd_key const BridgeSetSensorLED_key                 = 0x04;
bridge_cmd_key const TestInvalidSensorCmd_key               = 0x05;
bridge_cmd_key const TestInvalidSensorCmdPlusPayload_key    = 0x06;
bridge_cmd_key const BridgeGetSensorConfig_key              = 0x07;
bridge_cmd_key const BridgeSetSensorConfig_key              = 0x08;
bridge_cmd_key const BridgeGetExposure_key                  = 0x09;
bridge_cmd_key const BridgeSetExposure_key                  = 0x0A;

void NullCommand(void){}
BridgeCmd* LookupBridgeCmd(bridge_cmd_key const key)
{
    /* pf is an array of pointers to BridgeCmd functions */
    /* pf lives in static memory, not on the `LookupBridgeCmd` stack frame */
    static BridgeCmd* const pf[] =
    {
        NullCommand, // 0
        GetBridgeLED, // 1
        SetBridgeLED, // 2
        BridgeGetSensorLED, // 3
        BridgeSetSensorLED, // 4
        TestInvalidSensorCmd,            // 5
        TestInvalidSensorCmdPlusPayload, // 6
        BridgeGetSensorConfig, // 7
        BridgeSetSensorConfig, // 8
        BridgeGetExposure, // 9
        BridgeSetExposure, // 10
    };

    /* Return func ptr. Prevent attempts at out-of-bounds access. */
    if (key < sizeof(pf)/sizeof(*pf))   return pf[key];
    /* Out of bounds keys return a NULL pointer. */
    else return NULL;
    /* Up to caller to check for NULL and take appropriate action. */
    /* Recommended action: tell UsbHost the command was not recognized. */
}
/* =====[ API started 2019-10-02 ]===== */
status_byte ok = 0; 
status_byte error = 1; 
status_byte invalid_cmd = 2;
status_byte led_off = 0; 
status_byte led_green = 1; 
status_byte led_red = 2; 
led_name led_0 = 0;
led_name led_1 = 1;
config_byte binning_off = 0x00;
config_byte binning_on  = 0x01;
config_byte gain1x  = 0x01;
config_byte gain25x = 0x25;
config_byte gain4x  = 0x04;
config_byte gain5x  = 0x05;
config_byte all_rows_active  = 0x1F; // 0b00011111 is all five rows

/* void SerialWriteByte(status_byte status) {UsbWrite(&status,1);} */
static void SerialWriteByte_Implementation(status_byte status)
{
    UsbWrite(&status,1);
}
void (*SerialWriteByte)(status_byte) = SerialWriteByte_Implementation;
static void SpiWriteByte_Implementation(uint8_t byte)
{
    MacroSpiMasterWrite(byte);
}
void (*SpiWriteByte)(uint8_t) = SpiWriteByte_Implementation;
/* TODO: unit test WriteSensor */
/* WriteSensor is *not used* yet!! I've only needed to send one byte at a time. */
static uint16_t WriteSensor_Implementation(uint8_t const *write_buffer, uint16_t nbytes)
{
    uint16_t num_bytes_sent = 0;
    while (num_bytes_sent < nbytes)
    {
        MacroSpiMasterWrite(*(write_buffer++));
        /* MacroSpiMasterWaitForResponse(); // Why? Wait for what? */
        num_bytes_sent++;
    }
    return num_bytes_sent;
}
uint16_t (*WriteSensor)(uint8_t const *, uint16_t) = WriteSensor_Implementation;

static uint16_t ReadSensor_Implementation(uint8_t *read_buffer, uint16_t nbytes)
{
    /* TODO: test that this correctly waits for Sensor to be ready to send data. */
    /* TODO: get error-checking from deprecated SendSensorCommand */
    uint16_t num_bytes_read = 0;
    while (num_bytes_read < nbytes)
    {
        SpiMasterWaitForSlaveReady();
        MacroSpiMasterWrite(slave_ignore);      // transfer byte
        *(read_buffer++) = *Spi_spdr;  // store byte
        num_bytes_read++;
        SpiMasterWaitForSlaveReadyReset(); // added 2019-10-23
    }
    return num_bytes_read;
}
uint16_t (*ReadSensor)(uint8_t *, uint16_t) = ReadSensor_Implementation;

void GetBridgeLED(void) // Bridge `led_0` is the `status_led`
{
    /** Check the state of the LED on the Bridge board. */
    /** GetBridgeLED behavior:\n 
      * - reads and ignores Sensor reply to GetBridgeLED\n 
      * - receives led number\n 
      * - replies msg status error if led does not exist\n 
      * - sends no additional bytes if msg status is error\n 
      * - replies msg status ok if led number is recognized\n 
      * - sends led status byte after sending msg status ok\n 
      * - replies led off if led is off\n 
      * - replies led green if led is green\n 
      * - replies led red if led is red\n 
      * */
    // Read which LED to query (one byte of payload).
    uint8_t const num_bytes_payload = 1;
    uint8_t read_buffer[num_bytes_payload];
    UsbReadBytes(read_buffer, num_bytes_payload);
    // TODO: Add error checking for time out.
        // CASE: host does not send expected number of bytes.

    // Flush Sensor reply to this command (invalid command on Sensor)
    uint8_t sensor_reply[1]; ReadSensor(sensor_reply, 1);
    // Do not pass Sensor response back up to the host.
    /* SerialWriteByte(sensor_reply[0]); // Bridge passes Sensor status to host */

    uint8_t led_number = read_buffer[0];
    if (led_number != led_0)
    {
        SerialWriteByte(error); // host is asking about nonexistent LED
        /* SerialWriteByte(0x00); // response is always two bytes, pad with 0 */
        return;
    }
    SerialWriteByte(ok); // led_number is recognized, send msg_status: ok
    // Reply to USB Host with led status byte.
    if (!BiColorLedIsOn(status_led)) SerialWriteByte(led_off);
    else if (BiColorLedIsRed(status_led)) SerialWriteByte(led_red);
    else SerialWriteByte(led_green);
}
void SetBridgeLED(void) // Bridge `led_0` is the `status_led`
{
    /** Turn LED off, green, or red on the Bridge board. */
    /** SetBridgeLED behavior:\n 
      * - reads and ignores Sensor reply to SetBridgeLED\n 
      * - reads two bytes of payload from usb host\n 
      * - replies with one byte\n 
      * - replies msg status error if led number is not recognized\n 
      * - replies msg status ok if led number is status led\n 
      * - turns off led if payload is led off\n 
      * - turns led on and green if payload is led green\n 
      * - turns led on and red if payload is led red\n 
      * */
    // Read which LED to set (one byte of payload).
    uint8_t const num_bytes_payload = 2;
    uint8_t read_buffer[num_bytes_payload];
    /* UsbReadN(read_buffer, num_bytes_payload); */
    UsbReadBytes(read_buffer, num_bytes_payload);

    // Flush Sensor reply to this command (invalid command on Sensor)
    uint8_t sensor_reply[1]; ReadSensor(sensor_reply, 1);
    // Do not pass Sensor response back up to the host.
    /* SerialWriteByte(sensor_reply[0]); // Bridge passes Sensor status to host */

    // Reply to USB Host with message status byte.
    uint8_t led_number = read_buffer[0];
    if (led_number != led_0)
    {
        SerialWriteByte(error); // host is asking about nonexistent LED
        return;
    }
    uint8_t desired_led_state = read_buffer[1];
    if (desired_led_state == led_off)
    {
        BiColorLedOff(status_led);
        SerialWriteByte(ok);
        return;
    }
    else if (desired_led_state == led_green)
    {
        BiColorLedGreen(status_led);
        BiColorLedOn(status_led);
        SerialWriteByte(ok);
        return;
    }
    else if (desired_led_state == led_red)
    {
        BiColorLedRed(status_led);
        BiColorLedOn(status_led);
        SerialWriteByte(ok);
        return;
    }
    else
    { // led_state is not recognized
        SerialWriteByte(error);
    }
}
void BridgeGetSensorLED(void) // Sensor has `led_0` and `led_1`.
{
    /** Send GetSensorLED command to Sensor and pass reply back up to USB host.
     * */
    /** BridgeGetSensorLED behavior:\n 
      * - reads one byte of host payload\n 
      * - checks for invalid command error from Sensor\n 
      * - does not send payload if Sensor says invalid cmd\n 
      * - passes invalid cmd reply back to host\n 
      * - responds ok if Sensor does not say invalid cmd\n 
      * - writes led number to Sensor\n 
      * - reads msg status byte from Sensor and sends to USB host\n 
      * - reads and sends led status byte if Sensor status is ok\n 
      * - reads no more bytes if Sensor status is error\n 
      * */
    // Read which LED to query (one byte of payload).
    uint8_t const num_bytes_payload = 1;
    uint8_t read_buffer[num_bytes_payload];
    UsbReadBytes(read_buffer, num_bytes_payload);
    /* 2019-10-14: UsbReadBytes finishes and correctly returns led_number. */
    // TODO: Add error checking for time out.
        // CASE: host does not send expected number of bytes.

    uint8_t led_number = read_buffer[0];
    /* SerialWriteByte(ok); // Bridge finished reading its expected payload. */
    if (SensorHasResponse())
    { // something is wrong, let the USB host figure it out
        uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
        SerialWriteByte(sensor_reply);
        return;
    }
    else
    { // Sensor is waiting for the payload.
        SerialWriteByte(ok); // Bridge finished reading the expected payload.
    }

    // Send led_number to Sensor.
    SpiWriteByte(led_number);
    // Get reply from Sensor.
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    // Pass reply to host.
    SerialWriteByte(sensor_reply);
    // If there was no error, get next byte and pass to host.
    if (ok==sensor_reply)
    {
        ReadSensor(&sensor_reply, 1);
        SerialWriteByte(sensor_reply);
    }
}
void BridgeSetSensorLED(void)
{
    /** Send SetSensorLED command to Sensor and pass reply back up to USB host.
     * */
    /** BridgeSetSensorLED behavior:\n 
      * - reads two bytes of host payload\n 
      * - checks for invalid command error from Sensor\n 
      * - does not send payload if Sensor says invalid cmd\n 
      * - passes invalid cmd reply back to host\n 
      * - responds ok if Sensor does not say invalid cmd\n 
      * - passes two bytes of payload to Sensor\n 
      * - reads and sends one byte Sensor reply to host\n 
      * */
    // Read LED number and state (two bytes of payload).
    uint8_t const num_bytes_payload = 2;
    uint8_t read_buffer[num_bytes_payload];
    UsbReadBytes(read_buffer, num_bytes_payload);
    // TODO: Add error checking for time out.
        // CASE: host does not send expected number of bytes.

    uint8_t led_number = read_buffer[0];
    uint8_t led_state = read_buffer[1];
    if (SensorHasResponse())
    { // something is wrong, let the USB host figure it out
        uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
        SerialWriteByte(sensor_reply);
        return;
    }
    else
    { // Sensor is waiting for the payload.
        SerialWriteByte(ok); // Bridge finished reading the expected payload.
    }

    // Send led_number and led_state to Sensor.
    SpiWriteByte(led_number); SpiWriteByte(led_state);
    // Get reply from Sensor.
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    // Pass reply to host.
    SerialWriteByte(sensor_reply);
}
void TestInvalidSensorCmd(void) // Test how Sensor responds to invalid cmd.
{
    SerialWriteByte(ok); // Bridge recognized this command.
    // Get Sensor reply
    uint8_t sensor_reply[1]; ReadSensor(sensor_reply, 1);
    // Pass Sensor response back up to the host.
    SerialWriteByte(sensor_reply[0]); // Bridge passes Sensor status to host
}
void TestInvalidSensorCmdPlusPayload(void) // Test how Sensor responds to invalid cmd with payload
{
    // Read one byte of payload.
    uint8_t const num_bytes_payload = 1;
    uint8_t read_buffer[num_bytes_payload];
    UsbReadBytes(read_buffer, num_bytes_payload);

    uint8_t payload = read_buffer[0];

    // Expect Sensor is expecting a payload of one byte.
    // But if command is invalid on Sensor, it is *not* expecting a payload.
    // Instead, Sensor wants to respond with and invalid-command-error.
    // If Sensor has a response already, then read it and send it back to the
    // host before continuing.
    if (SensorHasResponse())
    { // something is wrong, let the USB host figure it out
        uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
        SerialWriteByte(sensor_reply);
        return;
    }
    else
    { // Sensor is waiting for the payload.
        SerialWriteByte(ok); // Bridge finished reading the expected payload.
    }
    // Send payload to Sensor.
    SpiWriteByte(payload);
    // Get reply from Sensor.
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    // Pass reply to host.
    SerialWriteByte(sensor_reply);
    // If there was no error, get next byte and pass to host.
    if (ok==sensor_reply)
    {
        ReadSensor(&sensor_reply, 1);
        SerialWriteByte(sensor_reply);
    }
}
void BridgeGetSensorConfig(void)
{
    /** Send GetSensorConfig command to Sensor and pass reply back up to USB host.
     * */
    /** BridgeGetSensorConfig behavior:\n 
      * - replies ok to USB host\n 
      * - reads msg status byte from Sensor and sends to USB host\n 
      * - reads no more bytes if Sensor status is error\n 
      * - if status is ok then read config data from Sensor and send to USB host\n 
      * */
    SerialWriteByte(ok);
    // Get reply from Sensor.
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    // Pass reply to host.
    SerialWriteByte(sensor_reply);
    // If there was no error, get data and pass to host.
    if (ok==sensor_reply)
    {
        uint8_t const nbytes_data = 3;
        uint8_t sensor_data[nbytes_data]; ReadSensor(sensor_data, nbytes_data);
        uint8_t *pdata = sensor_data;
        uint8_t byte_count=0;
        while (byte_count++ < nbytes_data) SerialWriteByte(*(pdata++));
    }
}
void BridgeSetSensorConfig(void)
{
    /** Send SetSensorConfig command to Sensor and pass reply back up to USB host.
     * */
    /** BridgeSetSensorConfig behavior:\n 
      * - reads three bytes of host payload\n 
      * - sends msg status ok after reading payload\n 
      * - checks for invalid command error from Sensor\n 
      * - does not send payload if Sensor says invalid cmd\n 
      * - passes invalid cmd reply back to host\n 
      * - passes three bytes of payload to Sensor if it does not say invalid cmd\n 
      * - reads and sends one byte Sensor reply to host\n 
      * */
    // Read config (three bytes of payload).
    uint8_t const num_bytes_payload = 3;
    uint8_t read_buffer[num_bytes_payload];
    UsbReadBytes(read_buffer, num_bytes_payload);
    // TODO: Add error checking for time out.
        // CASE: host does not send expected number of bytes.

    SerialWriteByte(ok); // Bridge finished reading the expected payload.
    uint8_t binning = read_buffer[0];
    uint8_t gain = read_buffer[1];
    uint8_t active_rows = read_buffer[2];
    // TODO: this is wrong: Bridge should respond OK no matter what, long as it
    // gets here. How did this pass the system test?
    if (SensorHasResponse())
    { // something is wrong, let the USB host figure it out
        uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
        SerialWriteByte(sensor_reply);
        return;
    }
    // Send payload to Sensor.
    SpiWriteByte(binning); SpiWriteByte(gain); SpiWriteByte(active_rows);
    // Read reply (status byte) from Sensor.
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    // Pass reply (status byte) back to host.
    SerialWriteByte(sensor_reply);
}
void BridgeGetExposure(void)
{
    /** Send GetExposure command to Sensor and pass reply back up to USB host.
     * */
    /** BridgeGetExposure behavior:\n 
      * - replies ok to USB host\n 
      * - reads msg status byte from Sensor and sends to USB host\n 
      * - reads no more bytes if Sensor status is error\n 
      * - reads two bytes of exposure time from Sensor MSB first and sends to USB host\n 
      * */
    SerialWriteByte(ok);
    // Get reply from Sensor.
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    // Pass reply to host.
    SerialWriteByte(sensor_reply);
    if (ok==sensor_reply)
    {
        uint8_t const nbytes_data = 2;
        uint8_t sensor_data[nbytes_data]; ReadSensor(sensor_data, nbytes_data);
        uint8_t *pdata = sensor_data;
        uint8_t byte_count=0;
        while (byte_count++ < nbytes_data) SerialWriteByte(*(pdata++));
    }
}
void BridgeSetExposure(void)
{
    /** Send SetExposure command to Sensor and pass reply back up to USB host.
     * */
    /** BridgeSetExposure behavior:\n 
      * - reads two bytes of payload from usb host\n 
      * - sends msg status ok to usb host\n 
      * - checks for invalid command error from Sensor\n 
      * - does not send payload if Sensor says invalid cmd\n 
      * - passes invalid cmd reply back to host\n 
      * - passes two bytes of payload to Sensor if it does not say invalid cmd\n 
      * - reads and sends one byte Sensor reply to host\n 
      * */
    // Read exposure time (two bytes of payload).
    uint8_t const num_bytes_payload = 2;
    uint8_t read_buffer[num_bytes_payload];
    UsbReadBytes(read_buffer, num_bytes_payload);
    // TODO: Add error checking for time out.
        // CASE: host does not send expected number of bytes.

    SerialWriteByte(ok); // Bridge finished reading the expected payload.
    uint8_t exposure_msb = read_buffer[0];
    uint8_t exposure_lsb = read_buffer[1];
    (void)exposure_msb; (void)exposure_lsb;
    if (SensorHasResponse())
    { // something is wrong, let the USB host figure it out
        uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
        SerialWriteByte(sensor_reply);
        return;
    }
    // Send payload to Sensor.
    SpiWriteByte(exposure_msb); SpiWriteByte(exposure_lsb);
    // Read reply (status byte) from Sensor.
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    // Pass reply (status byte) back to host.
    SerialWriteByte(sensor_reply);
}
void BridgeCaptureFrame(void)
{
    /** Send CaptureFrame command to Sensor and pass reply back up to USB host.
     * */
    /** BridgeCaptureFrame behavior:\n 
      * - sends msg status ok to usb host\n 
      * - reads msg status byte from Sensor and sends to USB host\n 
      * - reads no more bytes if Sensor status is error\n 
      * - reads npixels in frame and sends to USB host\n 
      * - reads another status byte from Sensor and sends to USB host\n 
      * - does not read frame if Sensor status is error\n 
      * - reads and sends frame if Sensor status is ok\n 
      * */
    // Send Bridge status byte
    SerialWriteByte(ok);
    // Get and send Sensor status byte
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    SerialWriteByte(sensor_reply);
    if (ok!=sensor_reply) return;
    // Get and send Sensor frame size (16-bit word)
    uint8_t data[2]; ReadSensor(data, 2);
    uint8_t npixels_msb = data[0]; uint8_t npixels_lsb = data[1];
    SerialWriteByte(npixels_msb); SerialWriteByte(npixels_lsb);
    // Calculate npixels in this frame
    uint16_t npixels_in_frame = (npixels_msb << 8) | npixels_lsb;
    // Calculate nbytes in this frame (2 bytes for each pixel)
    uint16_t nbytes = 2*npixels_in_frame;
    // Get and send status byte from sensor.
    // Status is error if npixels in this frame exceeds constant `npixels`
    ReadSensor(&sensor_reply, 1); SerialWriteByte(sensor_reply);
    // Do not attempt to read the frame if Sensor returns error.
    if (ok!=sensor_reply) return;
    // Read the bytes in the frame
    ReadSensor(frame, nbytes);
    // Send the frame to the USB host
    uint16_t byte_count = 0; uint8_t *pframe = frame;
    while (byte_count++ < nbytes) SerialWriteByte(*pframe++);
}

/* =====[ Helper for CmdFn: BridgeCfgLis ]===== */
bool CfgBytesAreValid(uint8_t const *cfg_bytes)
{
    /* 4 bytes follow the cfg_spectrometer command */
    /* - picture as a 32-bit word: 11111111 11111111 11111111 11111111 */
    /* - 4 MSB of the 32 bits are ignored: XXXX1111 11111111 11111111 11111111 */
    /* - remaining 28 LSB contain LisCfg data */
    /* Of those 28 bits: */
    /* - 3 LSB are for gain and binning: these can be any value */
    /* - remaining 25 signify 25 pixel groups: 1111 11111111 11111111 11111XXX */
    /* - picture the pixel groups as a 5 x 5 grid */
    /* - each group is a 154-pixel span from each of the five rows: */
    /*     row number: 5432 15432154 32154321 54321 */
    /*     bits:       1111 11111111 11111111 11111XXX */
    /* - 1 turns pixel on, 0 turns pixels off */
    /* - a configuration is valid *if*: */
    /*     - pixels from the same row have the same on/off value */
    /*     - the configuration is not *all* rows off */
    /* - examples: */
    /*     - invalid (all rows are off): */
    /*         row number: 5432 15432154 32154321 54321 */
    /*         bits:       0000 00000000 00000000 00000XXX */
    /*     - valid (row 1 are all on, other rows are all off): */
    /*         row number: 5432 15432154 32154321 54321 */
    /*         bits:       0000 10000100 00100001 00001XXX */
    /*     - valid (row 1 is some on, some off): */
    /*         row number: 5432 15432154 32154321 54321 */
    /*         bits:       0000 00000000 00100001 00001XXX */

    /* Invalid if all rows are off. */
    if (CfgTurnsOffAllPixels(cfg_bytes)) return false;
    /* Invalid if any rows are part-on/part-off. */
    if (CfgTurnsRowPartiallyOn(cfg_bytes, 1) ) return false;
    if (CfgTurnsRowPartiallyOn(cfg_bytes, 2) ) return false;
    if (CfgTurnsRowPartiallyOn(cfg_bytes, 3) ) return false;
    if (CfgTurnsRowPartiallyOn(cfg_bytes, 4) ) return false;
    if (CfgTurnsRowPartiallyOn(cfg_bytes, 5) ) return false;
    /* All rows are fully on/off and at least one row is on. */
    return true;
}
/* =====[ Helper for CfgBytesAreValid ]===== */
bool CfgTurnsOffAllPixels(uint8_t const *cfg_bytes)
{
    /* 4 bytes follow cfg_spectrometer command. */
    uint8_t const num_cfgbytes = 4; // 
    /* Bytes are received from UsbHost MSB first */
    /* so bit 0 of cfg_bytes[0] is bit number 24, bit 7 is bit number 31. */
    /* // */
    /* Make a mask of pixel bits: set all pixel bits, clear all don't-care bits. */
    uint8_t const pixel_mask_array[] = {0x0F, 0xFF, 0xFF, 0xF8};
    uint8_t const * pixel_mask = pixel_mask_array;
    uint8_t nbytes_checked = 0; bool all_pixels_are_off = true;
    /* check for any pixels that are on, break the loop as soon as you find one */
    while ((nbytes_checked++ < num_cfgbytes) && all_pixels_are_off)
    {
        // assign boolean to NOT( cfg_bytes bitmasked by pixel_mask)
        // if any bits visible through mask are one, then boolean is false
        // if all bits visible through mask are zero, boolean stays true
        all_pixels_are_off = !( *(cfg_bytes++) & *(pixel_mask++) );
    }
    return all_pixels_are_off;
}
/* =====[ Helper function for CfgTurnsRowPartiallyOn ]===== */
bool XOR(bool a, bool b)
{
    /* No logical XOR operator in C. Use (not(X and Y) and (X or Y)) */
    /* a b (aXORb) !(a&&b) (a||b) (  !(a&&b))  &&  (a||b)  ) */
    /* 0 0    0        1      0                 0            */
    /* 0 1    1        1      1                 1            */
    /* 1 0    1        1      1                 1            */
    /* 1 1    0        0      1                 0            */
    return (  (!(a&&b))  &&  (a||b)  );
}
/* I didn't unit test AllPixelsAreOff. */
/* But it's callers have passing unit tests. */
bool AllPixelsAreOff(uint8_t const *cfg_bytes, uint8_t const *pixel_mask)
{
    // Check if all pixels are off for the row specified by `pixel_mask`.
    uint8_t const num_cfgbytes = 4;
    uint8_t nbytes_checked = 0; bool all_pixels_are_off = true;
    /* check for any pixels that are on, break the loop as soon as you find one */
    while ((nbytes_checked++ < num_cfgbytes) && all_pixels_are_off)
    {
        // assign boolean to NOT( cfg_bytes bitmasked by pixel_mask)
        // if any bits visible through mask are one, then boolean is false
        // if all bits visible through mask are zero, boolean stays true
        all_pixels_are_off = !( *(cfg_bytes++) & *(pixel_mask++) );
    }
    return all_pixels_are_off;
    /* Row 1 does not show up in the first byte (bits 24 to 27). That's OK.*/
    /* The mask for row 1 is 0x00 in the first byte (bit 27 is row 5, bit 24 is row 2). */
    /* Anything & 0 = 0. */
    /* 0 is false. */
    /* Not false is true. */
    /* So all pixels in row 1 are *off* as far as this last byte is concerned. */
}
/* I didn't unit test AllPixelsAreOn. */
/* But it's callers have passing unit tests. */
/* And it's a copy of AllPixelsAreOff. */
bool AllPixelsAreOn(uint8_t const *cfg_bytes, uint8_t const *pixel_mask)
{
    // Check if all pixels are on for the row specified by `pixel_mask`.
    /* This is identical to `AllPixelsAreOff` except */
    /* you *invert* each byte of cfg_bytes before masking. */
    uint8_t const num_cfgbytes = 4;
    uint8_t nbytes_checked = 0; bool all_pixels_are_on = true;
    /* check for any pixels that are off, break the loop as soon as you find one */
    while ((nbytes_checked++ < num_cfgbytes) && all_pixels_are_on)
    {
        /* Flip bits in cfg_bytes so that 0 means on. Simplifies the logic. */
        // assign boolean to NOT( cfg_bytes bitmasked by pixel_mask)
        // if all bits visible through mask are zero (on), boolean stays true
        // if any bits visible through mask are one (off), then boolean is false
        all_pixels_are_on = !(  ( ~(*(cfg_bytes++)) )  &  *(pixel_mask++)  );
    }
    return all_pixels_are_on;
    /* Row 1 does not show up in the first byte (bits 24 to 27). That's OK.*/
    /* The mask for row 1 is 0x00 in the first byte (bit 27 is row 5, bit 24 is row 2). */
    /* Anything & 0 = 0. */
    /* 0 is false. */
    /* Not false is true. */
    /* So all pixels in row 1 are *on* as far as this last byte is concerned. */
}
bool CfgTurnsRowPartiallyOn(uint8_t const *cfg_bytes, uint8_t row_number)
{
    /* There are only five rows. Make sure row_number is between 1 and 5. */
    if ( (row_number < 1) || (row_number > 5) ) { return true; } // fail and bail
    /* Create masks to look only at the pixels in a given row. */
    static uint8_t const row1_mask[] = {0x00, 0x84, 0x21, 0x08};
    static uint8_t const row2_mask[] = {0x01, 0x08, 0x42, 0x10};
    static uint8_t const row3_mask[] = {0x02, 0x10, 0x84, 0x20};
    static uint8_t const row4_mask[] = {0x04, 0x21, 0x08, 0x40};
    static uint8_t const row5_mask[] = {0x08, 0x42, 0x10, 0x80};
    static uint8_t const * pm[] = {
        row1_mask,
        row2_mask,
        row3_mask,
        row4_mask,
        row5_mask
    };
    /* /1* Create a pointer to walk the bytes in the mask during the loop. *1/ */
    /* uint8_t const * row_mask; */
    /* /1* Use the mask for the row chosen by the caller. *1/ */
    /* /1* TODO: Replace with a jump-table. *1/ */
    /* if (1 == row_number) { row_mask = row1_mask; } */
    /* if (2 == row_number) { row_mask = row2_mask; } */
    /* if (3 == row_number) { row_mask = row3_mask; } */
    /* if (4 == row_number) { row_mask = row4_mask; } */
    /* if (5 == row_number) { row_mask = row5_mask; } */

    /* To be valid, a given row must be *all on* or *all off*. */
    /* Return value *true* means the cfg byte is *not valid*. */
    /* all-off |   all-on  | !XOR  |   result */
    /* ------- |   ------  |  ---  |   ------ */
    /*     T   |   F       |   F   |   CfgTurnsRowPartiallyOn is false: all are off */
    /*     F   |   T       |   F   |   CfgTurnsRowPartiallyOn is false: all are on */
    /*     F   |   F       |   T   |   CfgTurnsRowPartiallyOn is true: some must be on */
    /*     T   |   T       |   T   |   CfgTurnsRowPartiallyOn is true: impossible case */
    return !XOR(
        AllPixelsAreOff(cfg_bytes, pm[row_number-1]), // false if cfg is row 1 all on
        AllPixelsAreOn(cfg_bytes, pm[row_number-1]) // true if cfg is row 1 all on
        );
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* all_pixels  = 0b  0000 1111  1111 1111  1111 1111  1111 1000 */
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* row_1       = 0b  0000 0000  1000 0100  0010 0001  0000 1000 */
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* row_2       = 0b  0000 0001  0000 1000  0100 0010  0001 0000 */
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* row_3       = 0b  0000 0010  0001 0000  1000 0100  0010 0000 */
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* row_4       = 0b  0000 0100  0010 0001  0000 1000  0100 0000 */
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* row_5       = 0b  0000 1000  0100 0010  0001 0000  1000 0000 */
}

/* =====[ Status ]===== */

uint8_t UsbWriteStatusOk(bridge_cmd_key   cmd_done_by_bridge)
{
    uint8_t const StatusOk[] = { 0x00, cmd_done_by_bridge};
    return UsbWrite(StatusOk,2);
}
// replaced byte FlushInvalidCommand:
uint8_t UsbWriteStatusInvalid(void)
{
    uint8_t const StatusInvalid[] = { invalid_cmd };
    return UsbWrite(StatusInvalid,1);
}
uint8_t FlushInvalidCommand(void)
{
    // Get reply from Sensor.
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    // Send invalid-command-error for Bridge and send reply from Sensor.
    uint8_t const StatusInvalid[] = { invalid_cmd, sensor_reply };
    return UsbWrite(StatusInvalid,2);
}
bool SensorHasResponse(void)
{
    return BitIsClear(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
}
uint8_t old_UsbWriteStatusInvalid(bridge_cmd_key invalid_cmd)
{
    uint8_t const StatusInvalid[] = { 0xFF, invalid_cmd };
    return UsbWrite(StatusInvalid,2);
}
uint8_t UsbWriteStatusBadArgs(bridge_cmd_key bad_args_cmd)
{
    uint8_t const StatusBadArgs[] = { 0xFE, bad_args_cmd };
    return UsbWrite(StatusBadArgs,2);
}
uint8_t UsbWriteStatusMissingArgs(bridge_cmd_key missing_args_cmd)
{
    uint8_t const StatusMissingArgs[] = { 0xFD, missing_args_cmd };
    return UsbWrite(StatusMissingArgs,2);
}
uint8_t UsbWriteStatusSpiBusError(sensor_cmd_key spi_slave_cmd)
{
    /** An unknown error occurred on the SPI bus. */
    /* The SpiMaster thinks the SpiSlave is not sending the expected number of
     * bytes. The UsbHost should reset the system and resend the command. */
    uint8_t const StatusSpiBusError[] = { 0xFC, spi_slave_cmd };
    return UsbWrite(StatusSpiBusError, 2);
}
void BridgeCfgLis(void)
{
    /* Spectrometer configuration is four bytes. */
    uint8_t const num_cfgbytes = 4;
    uint8_t read_buffer[num_cfgbytes];
    uint16_t nbytes_read = UsbReadN(read_buffer, num_cfgbytes);
    // TODO: Error checking that we timed out.
    // This will be next error (253). Timeout on expected num_cfgbytes.
    // Error checking that nbytes_read != 4
    if (nbytes_read != num_cfgbytes) { UsbWriteStatusMissingArgs(BridgeCfgLis_key); }
    // Have the right number of bytes.
    // Error check that bytes_read are valid.
    // This is error (254). Valid command with bad args.
    if (!CfgBytesAreValid(read_buffer)) { UsbWriteStatusBadArgs(BridgeCfgLis_key); }
    else
    {
        // [x] Do system tests with
        // /cygdrive/c/chromation-dropbox/Dropbox/sales/spect-py3-examples/main.py
        //
        // TODO: Left off here
        // Need something like SendSensorCommand but with the ability to follow
        // the command with an arg for the four bytes of cfg.
        // TODO: [ ] pass to mBrd
        // mBrd converts to uint32_t and does cfg
        // ...
        // at end of happy path
        UsbWriteStatusOk(BridgeCfgLis_key);
        // echo back cfg bytes
        UsbWrite(read_buffer,4);
    }
}
