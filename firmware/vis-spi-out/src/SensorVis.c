#include "SensorVis.h"
#include "BiColorLed.h" // for two bicolor LEDs on Sensor board
#include "Spi.h" // Sensor is the SPI slave
#include "Lis.h" // because SensorCfgLis() calls LisWriteCfg()
#include "stdlib.h" // defines NULL

/* =====[ SPI Flags and Data Register Buffer ]===== */
// SpiFifo points to the FIFO buffer where ISR buffers incoming SPI bytes.
extern volatile Queue_s * SpiFifo; // defined and allocated in vis-spi-out-.c

/* TODO: pull these constants from a common file along with Bridge.c */
sensor_cmd_key const NullCommand_key = 0;
sensor_cmd_key const GetSensorLED_key = 3;
sensor_cmd_key const SetSensorLED_key = 4;
/* sensor_cmd_key const SensorCfgLis_key = 4; */
sensor_cmd_key const TestInvalidSensorCmd_key = 5;


void NullCommand(void){}

status_byte ok = 0; 
status_byte error = 1; 
status_byte invalid_cmd = 2;
status_byte led_off = 0; 
status_byte led_green = 1; 
status_byte led_red = 2; 
led_name led_0 = 0;
led_name led_1 = 1;

// =====[status_led pin number defined in BiColorLed-Hardware header]=====
extern uint8_t const led_TxRx;      // PINC0
extern uint8_t const led_Done;      // PINC1

/* TODO: unit test WriteSpiMaster */
static uint16_t WriteSpiMaster_Implementation(uint8_t const *write_buffer, uint16_t nbytes)
{
    uint16_t byte_index;
    for (byte_index = 0; byte_index < nbytes; byte_index++)
    {
        *Spi_spdr = write_buffer[byte_index]; // load byte in SPI data register
        ClearBit(Spi_port, Spi_DataReady); // LOW signals data is ready
        while (QueueIsEmpty(SpiFifo)); // queue is empty until SPI transfer is done
        QueuePop(SpiFifo); // ignore rx byte
        // Drive DataReady HIGH to synchronize with Master.
        SetBit(Spi_port, Spi_DataReady);
    }
    return byte_index; // byte_index == num_bytes_actually_sent
}
/* static uint16_t NoIsrWriteSpiMaster_Implementation(uint8_t const *write_buffer, uint16_t nbytes) */
/* { */
/*     ClearBit(Spi_spcr, Spi_InterruptEnable); // Disable SPI interrupt */
/*     SpiSlaveSendBytes(write_buffer, nbytes); // Placeholder until I can clean this up. */
/*     SetBit(Spi_spcr, Spi_InterruptEnable); // Enable SPI interrupt */
/*     return nbytes; // TODO: use actual num_bytes_sent */
/*     /1* return num_bytes_sent; *1/ */
/* } */
uint16_t (*WriteSpiMaster)(uint8_t const *, uint16_t) = WriteSpiMaster_Implementation;

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
      * - ~always replies with two bytes~\n 
      * - replies msg status error if led is non existent\n 
      * - replies msg status ok if led number is recognized\n 
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

/* --------------------------------------------------------------------------------------- */
/* TODO: extract the useful SpiSlave stuff */
/* --------------------------------------------------------------------------------------- */
/* | 2019-03-03 WIP: New SpiSlave API functionality for robust multi-byte communication. | */
/* --------------------------------------------------------------------------------------- */
/* Define command functions in jump table */
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
/* =====[ DEPRECATED ]===== */
/* Define a named key for each function (`FooBar_key` is the key for `FooBar`) */
/* sensor_cmd_key const SensorLed1Green_key = 0; */
/* sensor_cmd_key const SensorLed1Red_key = 1; */
/* sensor_cmd_key const SensorLed2Green_key = 2; */
/* sensor_cmd_key const SensorLed2Red_key = 3; */
/* TODO: left off here */

/* =====[ TODO: Salvage any of this? ]===== */
SensorCmd* LookupSensorCmd(sensor_cmd_key const key) {
    // pf is an array of pointers to SensorCmd functions
    // pf lives in static memory, not on the `LookupSensorCmd` stack frame
    static SensorCmd* const pf[] = {
        NullCommand,
        NULL, // placeholder to bump key value of later function names
        NULL, // placeholder to bump key value of later function names
        GetSensorLED,
        SetSensorLED,
        NULL, // an invalid key for the system test: TestInvalidSensorCmd
        };
    // Return func ptr. Prevent attempts at out-of-bounds access.
    if (key < sizeof(pf)/sizeof(*pf)) return pf[key];
    // Out of bounds keys return a NULL pointer.
    else return NULL; // error
    // Up to caller to check for NULL and take appropriate action.
    // Recommended action: tell SpiMaster the command was not recognized.
}
/* SpiSlaveSendBytes has been unit-tested. No need to unit test this. */
void SpiSlaveWrite_StatusOk(sensor_cmd_key valid_cmd)
{
                             // | nbytes  | data           |
    uint8_t const StatusOk[] = {0x00, 0x02, 0x00, valid_cmd };
    SpiSlaveSendBytes(StatusOk,4);
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
void ReplyCommandInvalid(void)
{
    uint8_t cmd_invalid[] = {invalid_cmd};
    WriteSpiMaster(cmd_invalid, 1);
}
// Do not use this.
void SpiSlaveWrite_StatusInvalid(sensor_cmd_key invalid_cmd)
{                                  // | nbytes  |   data          |
    uint8_t const StatusInvalid[] = { 0x00, 0x02, 0xFF, invalid_cmd };
    SpiSlaveSendBytes(StatusInvalid,4);
}


/* --------------------------------------------------------------------------------------- */
/* --------------------------------------------------------------------------------------- */


