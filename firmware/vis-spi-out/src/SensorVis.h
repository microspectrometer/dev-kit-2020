/** \file */
#ifndef _SENSORVIS_H
#define _SENSORVIS_H

#include <stdint.h>     // uint8_t

/* TODO: this is duplicated in Bridge.h. Consolidate into one file used by both. */
typedef uint8_t const status_byte;  // TODO: move this to a shared lib
status_byte ok;
status_byte error;
status_byte led_off;
status_byte led_green;
status_byte led_red;
typedef uint8_t const led_name;  // TODO: move this to a shared lib
led_name led_0;
led_name led_1;

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
/* Give tests of LookupSensorCmd access to names of functions in */
/* jump table to compare pointer values. */
/* SensorCmd SensorLed1Red; */
/* SensorCmd SensorLed1Green; */
/* SensorCmd SensorLed2Red; */
/* SensorCmd SensorLed2Green; */
/* This is the datatype to use when calling LookupSensorCmd: */
typedef uint8_t sensor_cmd_key;  // jump-table dictionary uses 8-bit keys

/* Declare keys for callers of LookupCmd (values hidden in .c file) */
/* extern sensor_cmd_key const SensorLed1Red_key; */
/* extern sensor_cmd_key const SensorLed1Green_key; */
/* extern sensor_cmd_key const SensorLed2Red_key; */
/* extern sensor_cmd_key const SensorLed2Green_key; */
extern sensor_cmd_key const SensorCfgLis_key;
/* LookupSensorCmd takes key from SpiMaster and returns the function pointer to call. */
SensorCmd* LookupSensorCmd(sensor_cmd_key const key);
/* report status to SpiMaster */
void SpiSlaveWrite_StatusOk(sensor_cmd_key valid_cmd);
// Do not use this. Use ReplyCommandInvalid() instead.
void SpiSlaveWrite_StatusInvalid(sensor_cmd_key invalid_cmd);
// SpiSlaveWrite_StatusError
/* =====[ API to communicate ]===== */
// SpiSlaveRead_OneByte
/* --------------------------------------------------------------------------------------- */
sensor_cmd_key const GetSensorLED_key;

void ReplyCommandInvalid(void);
void GetSensorLED(void);
void LedsShowError(void);

#endif // _SENSORVIS_H
