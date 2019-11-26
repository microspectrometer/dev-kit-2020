#include "SensorVis.h"
#include "BiColorLed.h" // for two bicolor LEDs on Sensor board
#include "Spi.h" // Sensor is the SPI slave
#include "UartSpi.h" // USART in MSPIM mode for ADC readout
#include "stdlib.h" // defines NULL
// Lis must be included as Lis.c, so it is better included by main() file.

// =====[ global for exposure time defined in main() application ]=====
extern uint16_t exposure_ticks; // default to 50 ticks (1ms)
/* =====[ SPI Flags and Data Register Buffer ]===== */
// SpiFifo points to the FIFO buffer where ISR buffers incoming SPI bytes.
extern volatile Queue_s * SpiFifo; // defined and allocated in vis-spi-out-.c

/* TODO: pull these constants from a common file along with Bridge.c */
sensor_cmd_key const NullCommand_key        = 0x00;
sensor_cmd_key const GetSensorLED_key       = 0x03;
sensor_cmd_key const SetSensorLED_key       = 0x04;
sensor_cmd_key const GetSensorConfig_key    = 0x07;
sensor_cmd_key const SetSensorConfig_key    = 0x08;
sensor_cmd_key const GetExposure_key        = 0x09;
sensor_cmd_key const SetExposure_key        = 0x0A;


void NullCommand(void){}

status_byte ok = 0; 
status_byte error = 1; 
status_byte invalid_cmd = 2;
status_byte led_off = 0; 
status_byte led_green = 1; 
status_byte led_red = 2; 
led_name led_0 = 0;
led_name led_1 = 1;
// Definition needed in SensorVis.h for inline function.
/* config_byte binning_off = 0x00; */
/* config_byte binning_on  = 0x01; */
config_byte gain1x  = 0x01;
config_byte gain25x = 0x25;
config_byte gain4x  = 0x04;
config_byte gain5x  = 0x05;
config_byte all_rows_active  = 0x1F; // 0b00011111 is all five rows

// ---Memory for `frame` allocated in main() application---
extern uint8_t frame[];

// =====[status_led pin number defined in BiColorLed-Hardware header]=====
extern uint8_t const led_TxRx;      // PINC0
extern uint8_t const led_Done;      // PINC1

/* Declare inline functions here to emit symbols in this translation unit. */
// See definition of these inline functions in SensorVis.h
void WordToTwoByteArray(uint16_t, uint8_t *);
static void GetFrame_Implementation(void)
{
    ExposePhotodiodeArray(exposure_ticks);
    LisWaitForReadoutToStart(); // wait for SYNC pulse
    LisReadout();
}
void (*GetFrame)(void) = GetFrame_Implementation;

/* TODO: unit test WriteSpiMaster */
static uint16_t WriteSpiMaster_Implementation(uint8_t const *write_buffer, uint16_t nbytes)
{
    uint16_t byte_index;
    for (byte_index = 0; byte_index < nbytes; byte_index++)
    {
        // ld   r24, Y+
        // SPDR is 0x2e
        // out	0x2e, r24	; 46
        *Spi_spdr = write_buffer[byte_index]; // load byte in SPI data register
        // cbi	0x05, 1	; 5
        ClearBit(Spi_port, Spi_DataReady); // LOW signals data is ready
        while (QueueIsEmpty(SpiFifo)); // queue is empty until SPI transfer is done
        QueuePop(SpiFifo); // ignore rx byte
        // Drive DataReady HIGH to synchronize with Master.
        SetBit(Spi_port, Spi_DataReady);
    }
    return byte_index; // byte_index == num_bytes_actually_sent
}
uint16_t (*WriteSpiMaster)(uint8_t const *, uint16_t) = WriteSpiMaster_Implementation;
/* inline uint16_t WriteFrameToSpiMaster(uint16_t nbytes_in_frame) */
/* { */
/*     uint8_t *pframe = frame; // point to start of frame */
/*     uint16_t byte_index; */
/*     for (byte_index = 0; byte_index < nbytes_in_frame; byte_index++) */
/*     { */
/*         /1* *Spi_spdr = pframe[byte_index]; // load byte in SPI data register *1/ */
/*         *Spi_spdr = *(pframe++); // load byte in SPI data register */
/*         ClearBit(Spi_port, Spi_DataReady); // LOW signals data is ready */
/*         while (QueueIsEmpty(SpiFifo)); // queue is empty until SPI transfer is done */
/*         QueuePop(SpiFifo); // ignore rx byte */
/*         // Drive DataReady HIGH to synchronize with Master. */
/*         SetBit(Spi_port, Spi_DataReady); */
/*     } */
/*     return byte_index; // byte_index == num_bytes_actually_sent */
/* } */
/* inline uint16_t WriteFrameToSpiMaster(uint8_t volatile *pframe, uint16_t nbytes_in_frame) */
/* { */
/*     uint16_t byte_index; */
/*     for (byte_index = 0; byte_index < nbytes_in_frame; byte_index++) */
/*     { */
/*         *Spi_spdr = pframe[byte_index]; // load byte in SPI data register */
/*         ClearBit(Spi_port, Spi_DataReady); // LOW signals data is ready */
/*         while (QueueIsEmpty(SpiFifo)); // queue is empty until SPI transfer is done */
/*         QueuePop(SpiFifo); // ignore rx byte */
/*         // Drive DataReady HIGH to synchronize with Master. */
/*         SetBit(Spi_port, Spi_DataReady); */
/*     } */
/*     return byte_index; // byte_index == num_bytes_actually_sent */
/* } */

static uint16_t ReadSpiMaster_Implementation(uint8_t *read_buffer, uint16_t nbytes)
{
    /* TODO: add error-checking */
    uint16_t num_bytes_read = 0;
    while (num_bytes_read < nbytes)
    {
        while( !SpiTransferIsDone() ); // wait for a byte
        *(read_buffer++) = *Spi_spdr;  // store byte
        num_bytes_read++;
    }
    return num_bytes_read;
}
uint16_t (*ReadSpiMaster)(uint8_t *, uint16_t) = ReadSpiMaster_Implementation;

void GetSensorLED(void)
{
    /** Check the state of an LED on the Sensor board. */
    /** GetSensorLED behavior:\n 
      * - receives led number\n 
      * - replies msg status error if led does not exist\n 
      * - sends no additional bytes if msg status is error\n 
      * - replies msg status ok if led number is recognized\n 
      * - sends led status byte after sending msg status ok\n 
      * - replies led off if led is off\n 
      * - replies led green if led is green\n 
      * - replies led red if led is red\n 
      * */
    while (QueueIsEmpty(SpiFifo)); // wait for the led number
    // Pop a byte off the Queue. This is the led number.
    uint8_t led_number = QueuePop(SpiFifo);
    if ((led_number != led_0) && (led_number != led_1))
    { // host is asking about nonexistent LED
        uint8_t status = error;
        WriteSpiMaster(&status,1);
        // TODO: unit-test this, if there is a led_number error Sensor still
        // sends a placeholder (0xAB) reply
        // TODO: update doc string with new unit test output
        uint8_t placeholder = 0xAB; WriteSpiMaster(&placeholder,1);
        return;
    }
    uint8_t led;
    if (led_number == led_0) led = led_TxRx;
    else if (led_number == led_1) led = led_Done;
    uint8_t led_status;
    if (!BiColorLedIsOn(led)) led_status = led_off;
    else if (BiColorLedIsRed(led)) led_status = led_red;
    else led_status = led_green;
    uint8_t ok_reply[] = {ok, led_status};
    WriteSpiMaster(ok_reply,2);
}
void SetSensorLED(void)
{
    /** Turn an LED off/green/red on the Sensor board. */
    /** SetSensorLED behavior:\n 
      * - receives led number and led state from Bridge\n 
      * - replies with one byte\n 
      * - replies msg status error if led number is not valid\n 
      * - replies msg status error if led state is not valid\n 
      * - replies msg status ok if led number and led state are valid\n 
      * - turns off led if payload is led off\n 
      * - turns led on and green if payload is led green\n 
      * - turns led on and red if payload is led red\n 
      * */
    while (QueueIsEmpty(SpiFifo)); // wait for the led number
    // Pop a byte off the Queue. This is the led number.
    uint8_t led_number = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo)); // wait for the led state
    // Pop a byte off the Queue. This is the led state.
    uint8_t led_state = QueuePop(SpiFifo);
    if ((led_number != led_0) && (led_number != led_1))
    { // led number is not valid
        uint8_t status = error;
        WriteSpiMaster(&status,1);
        return;
    }
    if (led_state == led_off)
    {
        BiColorLedOff(led_number);
        uint8_t status = ok;
        WriteSpiMaster(&status,1);
        return;
    }
    else if (led_state == led_green)
    {
        BiColorLedGreen(led_number);
        BiColorLedOn(led_number);
        uint8_t status = ok;
        WriteSpiMaster(&status,1);
        return;
    }
    else if (led_state == led_red)
    {
        BiColorLedRed(led_number);
        BiColorLedOn(led_number);
        uint8_t status = ok;
        WriteSpiMaster(&status,1);
        return;
    }
    else
    { // led state is not valid
        uint8_t status = error;
        WriteSpiMaster(&status,1);
    }
}
void GetSensorConfig(void)
{
    /** Send three bytes of photodiode array config data to the Bridge.\n 
     * - first byte sent: binning (off: 0x00, on: 0x01)\n 
     * - next byte sent: gain (1x, 2.5x, 4x, 5x)\n 
     * - last byte sent: which of the five rows are active (all active: b00011111)\n 
     * */
    /** GetSensorConfig behavior:\n 
      * - sends msg status ok to Bridge\n 
      * - sends three bytes of data to Bridge after sending ok\n 
      * */
    uint8_t status = ok;
    WriteSpiMaster(&status, 1);
    uint8_t data[] = {binning, gain, active_rows};
    uint8_t const nbytes_data = 3;
    WriteSpiMaster(data, nbytes_data);
}
static void ProgramPhotodiodeArray_Implementation(uint8_t const *config)
{ // TODO: clean this up using optimized inline functions
    EnterLisProgrammingMode(); // Wait for Clock falling edge, PixSelect HIGH
    LisWriteConfig(config);
    ExitLisProgrammingMode();
}
void (*ProgramPhotodiodeArray)(uint8_t const *) = ProgramPhotodiodeArray_Implementation;
void RepresentConfigAs4bytes(uint8_t *config, uint8_t binning, uint8_t gain, uint8_t active_rows)
{
    /** Return the 28-bit LIS config given three config bytes.\n 
     *  Input format for the three config bytes:
     *  - binning: 0x00 (off), 0x01 (on)\n 
     *  - gain: 0x01 (1x), 0x25 (2.5x), 0x04 (4x), 0x05 (5x)\n 
     *  - active_rows: 5 rows, set the bit to turn the row on\n 
     *    bits [8..0]: xxx54321 (1 to 5 are the row numbers, x is don't care)\n 
     *  Output format:\n 
     *  - input `config` points to an array of four bytes to hold the output\n 
     *  - bit0 of the 28-bit sequence is byte0, bit0 in config\n 
     *  - bit27 of the 28-bit sequence is byte3, bit3 in config\n 
     *  - program the LIS starting with bit0 */
    /** RepresentConfigAs4bytes behavior:\n 
      * - writes config byte0 bit0 set if binning is on\n 
      * - writes config byte0 bit0 clear if binning is off\n 
      * - writes config byte0 bits1to2 clear if gain is 1x\n 
      * - writes config byte0 bit1 clear bit2 set if gain is 2p5x\n 
      * - writes config byte0 bit1 set bit2 clear if gain is 4x\n 
      * - writes config byte0 bits1to2 set if gain is 5x\n 
      * - writes config as little endian so prog bit27 is byte 3 bit3 not byte0 bit 3\n 
      * - writes config bits3to27 set if all rows are active\n 
      * - writes config b3b8b13b18b23 set if row1 is active\n 
      * - writes config b4b9b14b19b24 set if row2 is active\n 
      * - writes config b5b10b15b20b25 set if row3 is active\n 
      * - writes config b6b11b16b21b26 set if row4 is active\n 
      * - writes config b7b12b17b22b27 set if row5 is active\n 
      * */
    // Clear all bits in array at input address `config`.
    config[0]=0x00; config[1]=0x00; config[2]=0x00; config[3]=0x00;
    // binning is bit 0 of byte 0
    uint8_t bit = 0;
    if (binning_on == binning) config[0] |= 1<<(bit++); // bit 0: bin on/off
    else bit++;
    // bit 1: gain bit G2
    // bit 2: gain bit G1
    // {G2,G1}: {0,0} 1x; {0,1} 2.5x; {1,0} 4x; {1,1} 5x
    if      (gain25x == gain) { bit++; config[0] |= 1<<(bit++); }
    else if (gain4x == gain)  { config[0] |= 1<<(bit++); bit++; }
    else if (gain5x == gain)  { config[0] |= 1<<(bit++); config[0] |= 1<<(bit++); }
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
    uint8_t const row1 = 0; uint8_t const row1_mask[] = {0x00,0x84,0x21,0x08};
    uint8_t const row2 = 1; uint8_t const row2_mask[] = {0x01,0x08,0x42,0x10};
    uint8_t const row3 = 2; uint8_t const row3_mask[] = {0x02,0x10,0x84,0x20};
    uint8_t const row4 = 3; uint8_t const row4_mask[] = {0x04,0x21,0x08,0x40};
    uint8_t const row5 = 4; uint8_t const row5_mask[] = {0x08,0x42,0x10,0x80};
    // byte orders are mirrored below because
    // rowN_mask[] is big endian, but
    // config[] is little endian
    if (active_rows&(1<<row1))
    {
        config[0] |= row1_mask[3];
        config[1] |= row1_mask[2];
        config[2] |= row1_mask[1];
        config[3] |= row1_mask[0];
    }
    if (active_rows&(1<<row2))
    {
        config[0] |= row2_mask[3];
        config[1] |= row2_mask[2];
        config[2] |= row2_mask[1];
        config[3] |= row2_mask[0];
    }
    if (active_rows&(1<<row3))
    {
        config[0] |= row3_mask[3];
        config[1] |= row3_mask[2];
        config[2] |= row3_mask[1];
        config[3] |= row3_mask[0];
    }
    if (active_rows&(1<<row4))
    {
        config[0] |= row4_mask[3];
        config[1] |= row4_mask[2];
        config[2] |= row4_mask[1];
        config[3] |= row4_mask[0];
    }
    if (active_rows&(1<<row5))
    {
        config[0] |= row5_mask[3];
        config[1] |= row5_mask[2];
        config[2] |= row5_mask[1];
        config[3] |= row5_mask[0];
    }
}

void SetSensorConfig(void)
{
    /** Read three bytes of photodiode array config data from the Bridge.\n 
     * - first byte received: binning (off: 0x00, on: 0x01)\n 
     * - next byte received: gain (1x, 2.5x, 4x, 5x)\n 
     * - last byte received: which of the five rows are active (all active: b00011111)\n 
     * Configure photodiode array.\n 
     * */
    /** SetSensorConfig behavior:\n 
      * - receives three bytes of config from Bridge\n 
      * - replies msg status error if binning is invalid\n 
      * - replies msg status error if gain is invalid\n 
      * - replies msg status error if active rows is invalid\n 
      * - replies msg status ok if all config bytes are valid\n 
      * - programs the photodiode array with the config\n 
      * */
    while (QueueIsEmpty(SpiFifo)); // wait for binning
    binning = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo)); // wait for gain
    gain = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo)); // wait for active_rows
    active_rows = QueuePop(SpiFifo);
    // Check that inputs are valid.
    if (
            ((binning != binning_off) && (binning != binning_on))
        ||  ((gain != gain1x) && (gain != gain25x) && (gain != gain4x) && (gain != gain5x))
        ||  ((active_rows & 0xE0) != 0x00)
       )
    { // one or more of the inputs is not valid
        uint8_t status = error;
        WriteSpiMaster(&status,1);
        return;
    }
    uint8_t status = ok;
    WriteSpiMaster(&status,1);

    // Convert three data bytes to 28-bit LIS programming sequence.
    uint8_t config[4]; RepresentConfigAs4bytes(config, binning, gain, active_rows);
    ProgramPhotodiodeArray(config);
    // typical 28-bit sequence: 0000 1111   1111 1111   1111 1111   1111 1001
    // msb 0000 is ignored
}
void GetExposure(void)
{
    /** Send two bytes of exposure time in units of clock ticks to the Bridge.\n 
     * - exposure is a 16-bit word\n 
     * - exposure is in units of clock ticks\n 
     * - multiply exposure by 20e-6 seconds to get exposure time in seconds\n
     * - clock ticks are 20us because the photodiode array is clocked at 50kHz\n 
     * */
    /** GetExposure behavior:\n 
      * - sends status byte ok\n 
      * - sends two bytes of exposure time most significant byte first\n 
      * */
    uint8_t status = ok;
    WriteSpiMaster(&status, 1);
    uint8_t const nbytes_data = 2;
    uint8_t exposure_msb = exposure_ticks >> 8;
    uint8_t exposure_lsb = exposure_ticks & 0xFF;
    uint8_t data[] = {exposure_msb, exposure_lsb};
    WriteSpiMaster(data, nbytes_data);
}
void SetExposure(void)
{
    /** Set exposure time. */
    /** SetExposure behavior:\n 
      * - receives two bytes of exposure msb first from Bridge\n 
      * - replies msg status ok\n 
      * - converts two data bytes to new 16 bit exposure ticks value\n 
      * */
    while (QueueIsEmpty(SpiFifo)); // wait for the msb of exposure time
    uint8_t exposure_msb = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo)); // wait for the lsb of exposure time
    uint8_t exposure_lsb = QueuePop(SpiFifo);
    (void) exposure_msb; (void) exposure_lsb;
    uint8_t status = ok; WriteSpiMaster(&status, 1);
    exposure_ticks = (exposure_msb << 8) | exposure_lsb;
}
void CaptureFrame(void)
{
    /** CaptureFrame sends one frame of pixel data to the Bridge.*/
    /** CaptureFrame behavior:\n 
      * - sends status byte ok\n 
      * - collects a frame of pixel data\n 
      * - sends two bytes msb first with number of pixels in frame\n 
      * - sends another status byte ok\n 
      * - sends the frame as two bytes per pixel msb first\n 
      * */
    // Send status OK
    uint8_t status = ok;
    WriteSpiMaster(&status, 1);
    // Collect the frame from the photodiode array
    GetFrame(); // implemented (copied from old code) but not tested
    // Send the number of pixels in the frame
    uint8_t msb_lsb[2]; WordToTwoByteArray(NumPixelsInFrame(), msb_lsb);
    WriteSpiMaster(msb_lsb, 2);
    //
    // TODO: unit-test this, don't need a second status byte
    // TODO: update doc string with new unit test output
    // Send status OK
    /* WriteSpiMaster(&status, 1); */
    //
    // Send frame
    uint16_t nbytes_in_frame = 2*NumPixelsInFrame();
    /* WriteFrameToSpiMaster(frame, nbytes_in_frame); */
    /* WriteFrameToSpiMaster(nbytes_in_frame); */
    WriteSpiMaster(frame, nbytes_in_frame);
}


/* --------------------------------------------------------------------------------------- */
/* Define command functions in jump table */
void ReplyCommandInvalid(void)
{
    uint8_t cmd_invalid[] = {invalid_cmd};
    WriteSpiMaster(cmd_invalid, 1);
}
// Do not use these DEBUG functions in production code.
void DEBUG_LedsShowError(void)
{
    // first and second LEDs turn red to indicate error
    BiColorLedRed(led_TxRx);
    BiColorLedRed(led_Done);
}
void DEBUG_LedsShowNoError(void)
{
    // first and second LEDs turn green to indicate error cleared
    BiColorLedGreen(led_TxRx);
    BiColorLedGreen(led_Done);
}

// ---Jump table---
SensorCmd* LookupSensorCmd(sensor_cmd_key const key) {
    // pf is an array of pointers to SensorCmd functions
    // pf lives in static memory, not on the `LookupSensorCmd` stack frame
    static SensorCmd* const pf[] = {
        NullCommand,
        NULL, // placeholder to bump key value of later function names
        NULL, // placeholder to bump key value of later function names
        GetSensorLED,
        SetSensorLED,
        NULL, // invalid key for system test: TestInvalidSensorCmd
        NULL, // invalid key for system test: TestInvalidSensorCmdPlusPayload
        GetSensorConfig, // 7
        SetSensorConfig, // 8
        GetExposure, // 9
        SetExposure, // 10
        CaptureFrame, // 11
        };
    // Return func ptr. Prevent attempts at out-of-bounds access.
    if (key < sizeof(pf)/sizeof(*pf)) return pf[key];
    // Out of bounds keys return a NULL pointer.
    else return NULL; // error
    // Up to caller to check for NULL and take appropriate action.
    // Recommended action: call ReplyCommandInvalid().
}
