/** \file */
#ifndef _SENSORVIS_H
#define _SENSORVIS_H

#include <stdint.h>     // uint8_t
#include <stdbool.h>    // bool, true, false
#include "Queue.h" // lib 'Queue' defines Queue_s for Sensor SPI Rx FIFO Buffer

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
typedef uint8_t const config_byte;  // TODO: move this to a shared lib
config_byte binning_off;
config_byte binning_on;
config_byte gain1x;
config_byte gain25x;
config_byte gain4x;
config_byte gain5x;
config_byte all_rows_active;

extern uint16_t (*ReadSpiMaster)(uint8_t *read_buffer, uint16_t nbytes);
extern uint16_t (*WriteSpiMaster)(uint8_t const *write_buffer, uint16_t nbytes);

/* --------------------------------------------------------------------------------------- */
/* TODO: Extract the useful SpiSlave stuff */
/* --------------------------------------------------------------------------------------- */
/* | 2019-03-03 WIP: New SpiSlave API functionality for robust multi-byte communication. | */
/* --------------------------------------------------------------------------------------- */
/* =====[ SPI Slave API ]===== */
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
// report status to SpiMaster
void SpiSlaveWrite_StatusOk(sensor_cmd_key valid_cmd);
// Do not use this. Use ReplyCommandInvalid() instead.
void SpiSlaveWrite_StatusInvalid(sensor_cmd_key invalid_cmd);
// SpiSlaveWrite_StatusError
/* =====[ API to communicate ]===== */
// SpiSlaveRead_OneByte
/* --------------------------------------------------------------------------------------- */
// sensor_cmd_key const GetSensorLED_key;

void DEBUG_LedsShowError(void);
void DEBUG_LedsShowNoError(void);
void ReplyCommandInvalid(void);
void GetSensorLED(void);
void SetSensorLED(void);
void GetSensorConfig(void);
void SetSensorConfig(void);
//
#endif // _SENSORVIS_H
