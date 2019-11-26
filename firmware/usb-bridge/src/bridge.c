#include "Bridge.h"
#include "BiColorLed.h"
#include "Spi.h"
#include "Usb.h"
#include "stdlib.h" // defines NULL

#define npixels 784
extern uint8_t frame[npixels*2];

// =====[status_led pin number defined in BiColorLed-Hardware header]=====
extern uint8_t const status_led;

// Define a named key for each function (`FooBar_key` is the key for `FooBar`)
/* bridge_cmd_key const NullCommand_key                        = 0x00; */
/* bridge_cmd_key const GetBridgeLED_key                       = 0x01; */
/* bridge_cmd_key const SetBridgeLED_key                       = 0x02; */
/* bridge_cmd_key const BridgeGetSensorLED_key                 = 0x03; */
/* bridge_cmd_key const BridgeSetSensorLED_key                 = 0x04; */
/* bridge_cmd_key const TestInvalidSensorCmd_key               = 0x05; */
/* bridge_cmd_key const TestInvalidSensorCmdPlusPayload_key    = 0x06; */
/* bridge_cmd_key const BridgeGetSensorConfig_key              = 0x07; */
/* bridge_cmd_key const BridgeSetSensorConfig_key              = 0x08; */
/* bridge_cmd_key const BridgeGetExposure_key                  = 0x09; */
/* bridge_cmd_key const BridgeSetExposure_key                  = 0x0A; */
/* bridge_cmd_key const BridgeCaptureFrame_key                 = 0x0B; */

// ---Constants---
status_byte ok = 0; 
status_byte error = 1; 
status_byte invalid_cmd = 2;
status_byte led_off = 0; 
status_byte led_green = 1; 
status_byte led_red = 2; 
led_name led_0 = 0;
led_name led_1 = 1;

// ---Helper functions for USB read/write---
static void SerialWriteByte_Implementation(status_byte status)
{
    UsbWrite(&status,1);
}
void (*SerialWriteByte)(status_byte) = SerialWriteByte_Implementation;

// ---Helper functions for SPI read/write---
bool SensorHasResponse(void)
{
    return BitIsClear(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
}
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

// ---API defined in USBProtocol table ---
void NullCommand(void){}
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
    //
    // Flush Sensor reply to this command (invalid command on Sensor)
    uint8_t sensor_reply[1]; ReadSensor(sensor_reply, 1);
    // Do not pass Sensor response back up to the host.
    /* SerialWriteByte(sensor_reply[0]); // Bridge passes Sensor status to host */
    //
    uint8_t led_number = read_buffer[0];
    if (led_number != led_0)
    {
        SerialWriteByte(error); // host is asking about nonexistent LED
        //
        // TODO: unit-test this, if there is a led_number error Bridge still
        // sends a placeholder (0xAB) reply
        // TODO: update doc string with new unit test output
        SerialWriteByte(0xAB);
        //
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
    //
    // Flush Sensor reply to this command (invalid command on Sensor)
    uint8_t sensor_reply[1]; ReadSensor(sensor_reply, 1);
    // Do not pass Sensor response back up to the host.
    /* SerialWriteByte(sensor_reply[0]); // Bridge passes Sensor status to host */
    //
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
    //
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
    //
    // Send led_number to Sensor.
    SpiWriteByte(led_number);
    // Get reply from Sensor.
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    // Pass reply to host.
    SerialWriteByte(sensor_reply);
    //
    // TODO: unit-test this, if there is a led_number error Sensor still sends a
    // placeholder reply
    // TODO: update doc string with new unit test output
    // If there was no error, get next byte and pass to host.
    /* if (ok==sensor_reply) */
    /* { */
    /*     ReadSensor(&sensor_reply, 1); */
    /*     SerialWriteByte(sensor_reply); */
    /* } */
    //
    // Sensor replies with an LED status, even if there is an error.
    ReadSensor(&sensor_reply, 1);
    SerialWriteByte(sensor_reply);
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
    //
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
    //
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
    //
    uint8_t payload = read_buffer[0];
    //
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
    //
    SerialWriteByte(ok); // Bridge finished reading the expected payload.
    uint8_t binning = read_buffer[0];
    uint8_t gain = read_buffer[1];
    uint8_t active_rows = read_buffer[2];
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
    //
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
    //
    // TODO: unit-test this, don't need a second status byte
    // TODO: update doc string with new unit test output
    // Get and send status byte from sensor.
    // Status is error if npixels in this frame exceeds constant `npixels`
    /* ReadSensor(&sensor_reply, 1); SerialWriteByte(sensor_reply); */
    // Do not attempt to read the frame if Sensor returns error.
    /* if (ok!=sensor_reply) return; */
    //
    // Read the bytes in the frame
    ReadSensor(frame, nbytes);
    // Send the frame to the USB host
    uint16_t byte_count = 0; uint8_t *pframe = frame;
    while (byte_count++ < nbytes) SerialWriteByte(*pframe++);
}

// ---Used in application to handle invalid Bridge commands---
uint8_t FlushInvalidCommand(void)
{
    // Get reply from Sensor.
    uint8_t sensor_reply; ReadSensor(&sensor_reply, 1);
    // Send invalid-command-error for Bridge and send reply from Sensor.
    uint8_t const StatusInvalid[] = { invalid_cmd, sensor_reply };
    return UsbWrite(StatusInvalid,2);
}

// ---Jump Table: position of function name determines its key value---
BridgeCmd* LookupBridgeCmd(bridge_cmd_key const key)
{
    // pf is an array of pointers to BridgeCmd functions
    // pf lives in static memory, not on the `LookupBridgeCmd` stack frame
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
        BridgeCaptureFrame, // 11
    };
    //
    // Return func ptr. Prevent attempts at out-of-bounds access.
    if (key < sizeof(pf)/sizeof(*pf))   return pf[key];
    // Out of bounds keys return a NULL pointer.
    else return NULL;
    // Up to caller to check for NULL and take appropriate action.
    // Recommended action: call FlushInvalidCommand().
}

