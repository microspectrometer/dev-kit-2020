#ifndef _BRIDGE_H
#define _BRIDGE_H

#include <stdbool.h>    // bool, true, false
#include <stdint.h>     // uint8_t

/* --------------------------------------------------------------------------------------- */
/* TODO: this SpiSlave stuff goes in a Sensor.h */
/* Every SpiSlave response starts with two bytes indicating how many more bytes are coming. */
typedef struct {
    uint8_t msb;
    uint8_t lsb;
} spi_BytesComing_s;
inline uint16_t BytesComing(spi_BytesComing_s response_size)
{
    return response_size.msb<<8 | response_size.lsb;
}
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
SensorCmd SensorLed1Red;
SensorCmd SensorLed1Green;
SensorCmd SensorLed2Red;
SensorCmd SensorLed2Green;
/* SensorCmd spi_CfgLis; */
/* This is the datatype to use when calling LookupSpiCmd: */
typedef uint8_t sensor_cmd_key;  // SpiSlave jump-table dictionary uses 8-bit keys
/* Declare keys for callers of LookupCmd (values hidden in .c file) */
/* extern sensor_cmd_key const spi_LedRed_key; */
extern sensor_cmd_key const SensorLed1Red_key;
extern sensor_cmd_key const SensorLed1Green_key;
extern sensor_cmd_key const SensorLed2Red_key;
extern sensor_cmd_key const SensorLed2Green_key;
/* extern sensor_cmd_key const spi_CfgLis_key; */
/* LookupSensorCmd takes key from SpiMaster and returns the function pointer to call. */
SensorCmd* LookupSensorCmd(sensor_cmd_key const key);
/* report status to SpiMaster */
void SpiSlaveWrite_StatusOk(sensor_cmd_key valid_cmd);
void SpiSlaveWrite_StatusInvalid(sensor_cmd_key invalid_cmd);
// SpiSlaveWrite_StatusError
/* =====[ API to communicate ]===== */
// SpiSlaveRead_OneByte
/* --------------------------------------------------------------------------------------- */

/* =====[ WIP: robust UsbRead functionality started 2019-03-01 ]===== */
/* =====[ Helpers for BridgeCmd functions ]===== */
bool CfgBytesAreValid(uint8_t const *cfg_bytes);
bool CfgTurnsOffAllPixels(uint8_t const *cfg_bytes);
bool CfgTurnsRowPartiallyOn(uint8_t const *cfg_bytes, uint8_t row_number);
bool XOR(bool a, bool b);

/* =====[ WIP: Clean Command Parsing with jump tables started 2019-03-01 ]===== */
/* Functions of type `BridgeCmd` take nothing and return nothing. */
/* The *key* acts as the command since it is the command lookup. */
/* If the commands need additional parameters, they will read additional bytes. */
/* All functions in the lookup table must have the same signature, so commands that */
/* take return functions other than void-void need to go in a different jump table. */
typedef void (BridgeCmd)(void);
/* Give tests of LookupBridgeCmd access to names of functions in */
/* jump table to compare pointer values. */
/* =====[ DEPRECATED ]===== */
BridgeCmd BridgeLedRed;
BridgeCmd BridgeLedGreen;
BridgeCmd BridgeCfgLis;
BridgeCmd SendSensorLed1Red;
BridgeCmd SendSensorLed1Green;
BridgeCmd SendSensorLed2Red;
BridgeCmd SendSensorLed2Green;
/* =====[ API support started 2019-10-04 ]===== */
typedef uint8_t const status_byte;  // TODO: move this to a shared lib
status_byte ok;
status_byte error;
status_byte led_off;
status_byte led_green;
status_byte led_red;
typedef uint8_t const led_name;  // TODO: move this to a shared lib
led_name led_0;
led_name led_1;

extern void (*SendStatus)(status_byte);
extern uint16_t (*WriteSensor)(uint8_t const *write_buffer, uint16_t nbytes);
extern uint16_t (*ReadSensor)(uint8_t *read_buffer, uint16_t nbytes);


/* =====[ API started 2019-10-02 ]===== */
BridgeCmd GetBridgeLED;
BridgeCmd SetBridgeLED;
BridgeCmd GetSensorLED;

/* This is the datatype to use when calling LookupBridgeCmd: */
typedef uint8_t bridge_cmd_key;  // jump-table dictionary uses 8-bit keys
/* Declare keys for callers of LookupBridgeCmd (values hidden in .c file) */
/* =====[ DEPRECATED ]===== */
bridge_cmd_key const BridgeLedRed_key;
bridge_cmd_key const BridgeLedGreen_key;
bridge_cmd_key const BridgeCfgLis_key;
bridge_cmd_key const SendSensorLed1Red_key;
bridge_cmd_key const SendSensorLed1Green_key;
bridge_cmd_key const SendSensorLed2Red_key;
bridge_cmd_key const SendSensorLed2Green_key;
/* =====[ API started 2019-10-02 ]===== */
bridge_cmd_key const GetBridgeLED_key;
bridge_cmd_key const SetBridgeLED_key;

/* =====[ API: ]===== */
 /* LookUpCmd takes key from UsbHost and returns the function pointer to call. */
BridgeCmd* LookupBridgeCmd(bridge_cmd_key const key);

/* report status to UsbHost */
/* TODO: rename these from `Usb` to `Bridge` */
uint8_t UsbWriteStatusOk(uint8_t   cmd_done_by_bridge);
uint8_t UsbWriteStatusInvalid(uint8_t   invalid_cmd); // command not recognized
uint8_t UsbWriteStatusBadArgs(uint8_t   bad_args_cmd); // args are wrong val
uint8_t UsbWriteStatusMissingArgs(uint8_t   missing_args_cmd); // not enough args
uint8_t UsbWriteStatusSpiBusError(sensor_cmd_key   spi_slave_cmd);

#endif // _USB_H
