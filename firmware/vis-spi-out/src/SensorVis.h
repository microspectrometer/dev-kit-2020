/** \file */
#ifndef _SENSORVIS_H
#define _SENSORVIS_H

#include <stdint.h>     // uint8_t
#include <stdbool.h>    // bool, true, false
#include "Queue.h" // lib 'Queue' defines Queue_s for Sensor SPI Rx FIFO Buffer
// ---CaptureFrame dependencies---
#include "Pwm.h" // use PWM for the clock signal to the Photodiode Array
#include "Lis.h" // define I/O connections to Photodiode Array
#include "ReadWriteBits.h" // make inline functions visible to this translation unit

/* TODO: this is duplicated in Bridge.h. Consolidate into one file used by both. */
typedef uint8_t const status_byte;  // TODO: move this to a shared lib
status_byte ok;
status_byte error;
status_byte invalid_cmd;
status_byte led_off;
status_byte led_green;
status_byte led_red;
typedef uint8_t const led_name;  // TODO: move this to a shared lib
led_name led_0;
led_name led_1;

// ---CaptureFrame dependencies---
/* #define npixels 784 */
// =====[ globals for photodiode array config defined in main() application ]=====
extern uint8_t binning; // default to 392 pixels
extern uint8_t gain; // default to 1x gain
extern uint8_t active_rows; // default to using all 5 pixel rows
inline void WordToTwoByteArray(uint16_t word, uint8_t * parray)
{
    /** Return 16-bit word as a two-byte array MSB first.\n 
     * */
    *parray++ = word>>8;
    *parray++ = word & 0xFF;
    /* uint8_t npixels_msb_lsb[] = {(npixels_in_frame>>8), npixels_in_frame & 0xFF}; */
}


/* =====[ Sensor API ]===== */
/* Functions of type `SensorCmd` take nothing and return nothing. */
/* The *key* acts as the command since it is the command lookup. */
/* If the commands need additional parameters, they will read additional bytes. */
/* All functions in the lookup table must have the same signature, so commands that */
/* take return functions other than void-void need to go in a different jump table. */
typedef void (SensorCmd)(void);
// This is the datatype to use when calling LookupSensorCmd:
typedef uint8_t sensor_cmd_key;  // jump-table dictionary uses 8-bit keys
// LookupSensorCmd takes key from SpiMaster and returns the function pointer to call.
SensorCmd* LookupSensorCmd(sensor_cmd_key const key);

void DEBUG_LedsShowError(void);
void DEBUG_LedsShowNoError(void);
void ReplyCommandInvalid(void);
void GetSensorLED(void);
void SetSensorLED(void);
void GetSensorConfig(void);
void SetSensorConfig(void);
void GetExposure(void);
void SetExposure(void);
void CaptureFrame(void);

// SensorVis internal helpers (do not use in app)
extern uint16_t (*ReadSpiMaster)(uint8_t *read_buffer, uint16_t nbytes);
extern uint16_t (*WriteSpiMaster)(uint8_t const *write_buffer, uint16_t nbytes);
uint16_t WriteFrameToSpiMaster(uint16_t nbytes_in_frame);
/* uint16_t WriteFrameToSpiMaster(uint8_t volatile *pframe, uint16_t nbytes_in_frame); */
extern void (*ProgramPhotodiodeArray)(uint8_t const *config);
extern void (*GetFrame)(void);
//
void RepresentConfigAs4bytes(uint8_t *pconfig, uint8_t binning, uint8_t gain, uint8_t active_rows);
//
#endif // _SENSORVIS_H
