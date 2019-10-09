#include "Bridge.h"
#include "BiColorLed.h"
#include "Spi.h"
#include "Usb.h"
#include "Lis.h" // because SensorCfgLis() calls LisWriteCfg()
#include "stdlib.h" // defines NULL

/* --------------------------------------------------------------------------------------- */
/* TODO: this SpiSlave stuff goes in a Sensor.c */
uint16_t BytesComing(spi_BytesComing_s response_size); // inline defined in .h
/* --------------------------------------------------------------------------------------- */
/* | 2019-03-03 WIP: New SpiSlave API functionality for robust multi-byte communication. | */
/* --------------------------------------------------------------------------------------- */
/* Define command functions in jump table */
extern uint8_t const status_led1;
extern uint8_t const status_led2;
extern uint8_t const status_led3;
extern uint8_t const status_led4;
void SensorLed1Red(void)
{
    BiColorLedRed(status_led1);
    SpiSlaveWrite_StatusOk(SensorLed1Red_key);
}
void SensorLed2Red(void)
{
    BiColorLedRed(status_led2);
    SpiSlaveWrite_StatusOk(SensorLed2Red_key);
}

void SensorLed1Green(void)
{
    BiColorLedGreen(status_led1);
    SpiSlaveWrite_StatusOk(SensorLed1Green_key);
}
void SensorLed2Green(void)
{
    BiColorLedGreen(status_led2);
    SpiSlaveWrite_StatusOk(SensorLed2Green_key);
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

/* Define a named key for each function (`FooBar_key` is the key for `FooBar`) */
sensor_cmd_key const SensorLed1Green_key = 0;
sensor_cmd_key const SensorLed1Red_key = 1;
sensor_cmd_key const SensorLed2Green_key = 2;
sensor_cmd_key const SensorLed2Red_key = 3;
/* TODO: left off here */
sensor_cmd_key const SensorCfgLis_key = 8;
SensorCmd* LookupSensorCmd(sensor_cmd_key const key) {
    /* pf is an array of pointers to SensorCmd functions */
    /* pf lives in static memory, not on the `LookupSensorCmd` stack frame */
    static SensorCmd* const pf[] = {
        SensorLed1Green,
        SensorLed1Red,
        SensorLed2Green,
        SensorLed2Red,
        SensorCfgLis
        };
    /* Return func ptr. Prevent attempts at out-of-bounds access. */
    if (key < sizeof(pf)/sizeof(*pf))   return pf[key];
    /* Out of bounds keys return a NULL pointer. */
    else return NULL;
    /* Up to caller to check for NULL and take appropriate action. */
    /* Recommended action: tell SpiMaster the command was not recognized. */
}
/* SpiSlaveSendBytes has been unit-tested. No need to unit test this. */
void SpiSlaveWrite_StatusOk(sensor_cmd_key valid_cmd)
{
                             // | nbytes  | data           |
    uint8_t const StatusOk[] = {0x00, 0x02, 0x00, valid_cmd };
    SpiSlaveSendBytes(StatusOk,4);
}
void SpiSlaveWrite_StatusInvalid(sensor_cmd_key invalid_cmd)
{                                  // | nbytes  |   data          |
    uint8_t const StatusInvalid[] = { 0x00, 0x02, 0xFF, invalid_cmd };
    SpiSlaveSendBytes(StatusInvalid,4);
}
/* --------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------- */

/* =====[ WIP: Clean Command Parsing with jump tables started 2019-03-01 ]===== */
/* TODO: implement a top-level error handler */
/* to do something useful with a conditional check on UsbWriteStatusBlah(). */
    /* In the application, if you are good-paranoid, */
    /* you actually do check that the status was sent. */
    /* // */
    /* But I have yet to implement a top-level error handler */
    /* to do something useful with this information. */
    /* // */
    /* I just assume status bytes were sent */
    /* and let the UsbHost deal with the possibility */
    /* of not getting any response to its command. */
/* // */
// =====[status_led pin number defined in BiColorLed-Hardware header]=====
extern uint8_t const status_led;
void BridgeLedRed(void)
{
    BiColorLedRed(status_led);
    UsbWriteStatusOk(BridgeLedRed_key);
}
/* SpiSlave command lookup keys are defined in `lib/src/Spi.c` */
extern sensor_cmd_key const SensorLed1Red_key;
extern sensor_cmd_key const SensorLed1Green_key;
/* Define command functions in jump table for looking up USB commands. */
void SendSensorCommand(
    sensor_cmd_key cmd_to_sensor,       // send this command key to the sensor
    bridge_cmd_key cmd_done_by_bridge)  // and report status on this command key
{
    /* Send the command. */
    SpiMasterWriteByte(cmd_to_sensor);

    /* TODO: add timer to timeout in case SpiSlave is never ready. */
    /* The first two bytes from the SpiSlave are the remaining number of bytes
     * in the response. */
    spi_BytesComing_s   response_size;
    SpiMasterWaitForSlaveReady(); response_size.msb = SpiMasterReadByte();
    SpiMasterWaitForSlaveReady(); response_size.lsb = SpiMasterReadByte();
    /* The response should be two bytes. */
    uint16_t const nbytes_expected = 2; // 16-bit is the general case

    /* There is an unknown SPI communication error if there are not exactly 2
     * bytes coming. */
    /* It could be the SpiSlave or the SpiMaster. */
    /* Treat this like other BridgeCmd errors. Send back two bytes. */
    /* byte 1: error code
     * byte 2: command sent to SpiSlave */
    if ( BytesComing(response_size) != nbytes_expected )
    { UsbWriteStatusSpiBusError(cmd_to_sensor); return; }

    /* Read all of the bytes from the SpiSlave. */
    uint8_t rx[nbytes_expected];
    for (uint8_t index=0; index < nbytes_expected; index++)
    { SpiMasterWaitForSlaveReady(); rx[index]= SpiMasterReadByte(); }
    /* There is an error if the SpiSlave is still signaling *Data Ready*. */
    if ( SpiSlaveShowsDataReady() )
    { UsbWriteStatusSpiBusError(cmd_to_sensor); return; }

    /* The SpiMaster has succeeded at this point, */
    /* even if the SpiSlave sent an error code. */
    UsbWriteStatusOk(cmd_done_by_bridge);
    /* Whatever the SpiSlave sent, pass it up to the UsbHost. */
    uint8_t nbytes_of_data[] = {response_size.msb, response_size.lsb};
    UsbWrite(nbytes_of_data, 2);
    UsbWrite(rx,nbytes_expected);
}

void SendSensorLed1Green(void)
{
    SendSensorCommand(
            SensorLed1Green_key,   // send this command key to the sensor
        SendSensorLed1Green_key);  // and report status on this command key
}
void SendSensorLed1Red(void)
{
    SendSensorCommand(
            SensorLed1Red_key,   // send this command key to the sensor
        SendSensorLed1Red_key);  // and report status on this command key 
}
void SendSensorLed2Green(void)
{
    SendSensorCommand(
            SensorLed2Green_key,   // send this command key to the sensor
        SendSensorLed2Green_key);  // and report status on this command key
}
void SendSensorLed2Red(void)
{
    SendSensorCommand(
            SensorLed2Red_key,   // send this command key to the sensor
        SendSensorLed2Red_key);  // and report status on this command key 
}
void BridgeLedGreen(void)
{
    BiColorLedGreen(status_led);
    UsbWriteStatusOk(BridgeLedGreen_key);
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
/* =====[ API started 2019-10-02 ]===== */
/* void SendStatus(status_byte status) {UsbWrite(&status,1);} */
static void SendStatus_Implementation(status_byte status) {UsbWrite(&status,1);}
void (*SendStatus)(status_byte) = SendStatus_Implementation;

void GetBridgeLED(void) // Bridge `led_0` is the `status_led`
{
    // TODO: wait for a byte to read before attempting to read.

    // Read which LED to query (one byte of payload).
    uint8_t const num_bytes_payload = 1;
    uint8_t read_buffer[num_bytes_payload];
    /* UsbReadN(read_buffer, num_bytes_payload); */
    UsbReadBytes(read_buffer, num_bytes_payload);
    // TODO: Add error checking for time out.
        // CASE: host does not send expected number of bytes.

    // Reply to USB Host with message status byte.
    uint8_t led_number = read_buffer[0];
    if (led_number == led_0)
    {
        SendStatus(error); // host is asking about nonexistent LED
        return;
    }
    SendStatus(ok); // led_number is recognized, send msg_status: ok
    // Reply to USB Host with led status byte.
    if (!BiColorLedIsOn(status_led)) SendStatus(led_off);
    else if (BiColorLedIsRed(status_led)) SendStatus(led_red);
    else SendStatus(led_green);
}
void SetBridgeLED(void) // Bridge `led_0` is the `status_led`
{
    // Read which LED to set (one byte of payload).
    uint8_t const num_bytes_payload = 2;
    uint8_t read_buffer[num_bytes_payload];
    UsbReadN(read_buffer, num_bytes_payload);

    // Reply to USB Host with message status byte.
    uint8_t led_number = read_buffer[0];
    if (led_number != led_0)
    {
        SendStatus(error); // host is asking about nonexistent LED
        return;
    }
    SendStatus(ok); // led_number is recognized, send msg_status: ok
    uint8_t desired_led_state = read_buffer[1];
    if (desired_led_state == led_off)
    {
        BiColorLedOff(status_led);
        return;
    }
    else if (desired_led_state == led_green)
    {
        BiColorLedGreen(status_led);
        BiColorLedOn(status_led);
        return;
    }
    else if (desired_led_state == led_red)
    {
        BiColorLedRed(status_led);
        BiColorLedOn(status_led);
        return;
    }
}

/* Define a named key for each function (`FooBar_key` is the key for `FooBar`) */
bridge_cmd_key const GetBridgeLED_key = 0;
bridge_cmd_key const SetBridgeLED_key = 1;
bridge_cmd_key const GetSensorLED_key = 2;
bridge_cmd_key const SetSensorLED_key = 3;
BridgeCmd* LookupBridgeCmd(bridge_cmd_key const key)
{
    /* pf is an array of pointers to BridgeCmd functions */
    /* pf lives in static memory, not on the `LookupBridgeCmd` stack frame */
    static BridgeCmd* const pf[] =
    {
        GetBridgeLED,   // 0
        SetBridgeLED,   // 1
        /* GetSensorLED,   // 2 */
        /* SetSensorLED,   // 3 */
    };

    /* Return func ptr. Prevent attempts at out-of-bounds access. */
    if (key < sizeof(pf)/sizeof(*pf))   return pf[key];
    /* Out of bounds keys return a NULL pointer. */
    else return NULL;
    /* Up to caller to check for NULL and take appropriate action. */
    /* Recommended action: tell UsbHost the command was not recognized. */
}
/* ---DEPRECATED--- */
bridge_cmd_key const BridgeLedRed_key = 0;
bridge_cmd_key const BridgeLedGreen_key = 1;
bridge_cmd_key const BridgeCfgLis_key = 2;
bridge_cmd_key const SendSensorLed1Red_key = 3;
bridge_cmd_key const SendSensorLed1Green_key = 4;
bridge_cmd_key const SendSensorLed2Red_key = 5;
bridge_cmd_key const SendSensorLed2Green_key = 6;
/* =====[ API started 2019-10-02 ]===== */
status_byte ok = 0; 
status_byte error = 1; 
status_byte led_off = 0; 
status_byte led_green = 1; 
status_byte led_red = 2; 

BridgeCmd* oldLookupBridgeCmd(bridge_cmd_key const key) {
    /* pf is an array of pointers to BridgeCmd functions */
    /* pf lives in static memory, not on the `LookupBridgeCmd` stack frame */
    static BridgeCmd* const pf[] = {
        BridgeLedRed,
        BridgeLedGreen,
        BridgeCfgLis,
        SendSensorLed1Red,
        SendSensorLed1Green,
        SendSensorLed2Red,
        SendSensorLed2Green,
        };
    /* Return func ptr. Prevent attempts at out-of-bounds access. */
    if (key < sizeof(pf)/sizeof(*pf))   return pf[key];
    /* Out of bounds keys return a NULL pointer. */
    else return NULL;
    /* Up to caller to check for NULL and take appropriate action. */
    /* Recommended action: tell UsbHost the command was not recognized. */
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
uint8_t UsbWriteStatusInvalid(bridge_cmd_key invalid_cmd)
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
