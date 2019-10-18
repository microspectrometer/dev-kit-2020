#include "SensorVis.h"
#include "BiColorLed.h"
#include "Spi.h"
#include "Lis.h" // because SensorCfgLis() calls LisWriteCfg()
#include "stdlib.h" // defines NULL

/* TODO: pull these constants from a common file along with Bridge.c */
/* sensor_cmd_key const dummy0_key = 0; */
/* sensor_cmd_key const dummy1_key = 1; */
sensor_cmd_key const GetSensorLED_key = 3;
sensor_cmd_key const SensorCfgLis_key = 4;


void NullCommand(void){}

status_byte ok = 0; 
status_byte error = 1; 
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
    ClearBit(Spi_spcr, Spi_InterruptEnable); // Disable SPI interrupt
    SpiSlaveSendBytes(write_buffer, nbytes); // Placeholder until I can clean this up.
    SetBit(Spi_spcr, Spi_InterruptEnable); // Enable SPI interrupt
    return nbytes; // TODO: use actual num_bytes_sent
    /* return num_bytes_sent; */
}
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
    while (!HasSpiData); // wait for the led number
    uint8_t led_number = SpiData;
    HasSpiData = false; // consumed the data, so clear the flag
    if ((led_number != led_0) && (led_number != led_1))
    {
        uint8_t error_reply[] = {error, 0x00}; // send error and placeholder byte
        LedsShowError();
        WriteSpiMaster(error_reply,2); // host is asking about nonexistent LED
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
    /* BiColorLedRed(led_TxRx); // first LED red: all is good */
    WriteSpiMaster(ok_reply,2);
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
        LedsShowError();
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
    /* BiColorLedRed(led_TxRx); // first LED red: all is good */
    WriteSpiMaster(ok_reply,4);
    /* DEBUG START: */
    /* return; */
    /* DEBUG END */
}


/* --------------------------------------------------------------------------------------- */
/* TODO: extract the useful SpiSlave stuff */
/* --------------------------------------------------------------------------------------- */
/* | 2019-03-03 WIP: New SpiSlave API functionality for robust multi-byte communication. | */
/* --------------------------------------------------------------------------------------- */
/* Define command functions in jump table */
/* =====[ DEPRECATED FUNCTIONS ]===== */
/* void SensorLed1Red(void) */
/* { */
/*     BiColorLedRed(status_led1); */
/*     SpiSlaveWrite_StatusOk(SensorLed1Red_key); */
/* } */
/* void SensorLed2Red(void) */
/* { */
/*     BiColorLedRed(status_led2); */
/*     SpiSlaveWrite_StatusOk(SensorLed2Red_key); */
/* } */
/* void SensorLed1Green(void) */
/* { */
/*     BiColorLedGreen(status_led1); */
/*     SpiSlaveWrite_StatusOk(SensorLed1Green_key); */
/* } */
/* void SensorLed2Green(void) */
/* { */
/*     BiColorLedGreen(status_led2); */
/*     SpiSlaveWrite_StatusOk(SensorLed2Green_key); */
/* } */
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
    /* pf is an array of pointers to SensorCmd functions */
    /* pf lives in static memory, not on the `LookupSensorCmd` stack frame */
    static SensorCmd* const pf[] = {
        NullCommand,
        NULL, // placeholder to bump key value of later function names
        NULL, // placeholder to bump key value of later function names
        GetSensorLED,
        SensorCfgLis,
        /* SensorLed1Green, */
        /* SensorLed1Red, */
        /* SensorLed2Green, */
        /* SensorLed2Red, */
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
void LedsShowError(void)
{
    // first and second LEDs turn red to indicate error
    BiColorLedRed(led_TxRx);
    BiColorLedRed(led_Done);
}
void ReplyCommandInvalid(void)
{
    uint8_t cmd_invalid[] = {error};
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

