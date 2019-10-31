/** \file */
#ifndef _BRIDGE_H
#define _BRIDGE_H

#include <stdbool.h>    // bool, true, false
#include <stdint.h>     // uint8_t

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
/* =====[ API support started 2019-10-04 ]===== */
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

extern void (*SerialWriteByte)(status_byte);
extern void (*SpiWriteByte)(uint8_t byte);
extern uint16_t (*WriteSensor)(uint8_t const *write_buffer, uint16_t nbytes);
extern uint16_t (*ReadSensor)(uint8_t *read_buffer, uint16_t nbytes);


/* =====[ API started 2019-10-02 ]===== */
// Do not use the `BridgeCmd` typedef
// Doxygen does not pick up documentation for functions defined with a typedef.
    // example of what *not* to do:
    // BridgeCmd GetBridgeLED;
void GetBridgeLED(void);
void SetBridgeLED(void);
void BridgeGetSensorLED(void);
void BridgeSetSensorLED(void);
void TestInvalidSensorCmd(void);
void TestInvalidSensorCmdPlusPayload(void);

/* This is the datatype to use when calling LookupBridgeCmd: */
typedef uint8_t bridge_cmd_key;  // jump-table dictionary uses 8-bit keys
/* Declare keys for callers of LookupBridgeCmd (values hidden in .c file) */
bridge_cmd_key const BridgeCfgLis_key;
/* =====[ API started 2019-10-02 ]===== */
bridge_cmd_key const GetBridgeLED_key;
bridge_cmd_key const SetBridgeLED_key;
bridge_cmd_key const BridgeGetSensorLED_key;

/* =====[ API: ]===== */
 /* LookUpCmd takes key from UsbHost and returns the function pointer to call. */
BridgeCmd* LookupBridgeCmd(bridge_cmd_key const key);

/* report status to UsbHost */
/* TODO: rename these from `Usb` to `Bridge` */
uint8_t UsbWriteStatusOk(uint8_t   cmd_done_by_bridge);
uint8_t UsbWriteStatusInvalid(void); // Bridge command not recognized
/* uint8_t old_UsbWriteStatusInvalid(uint8_t   invalid_cmd); // command not recognized */
uint8_t UsbWriteStatusBadArgs(uint8_t   bad_args_cmd); // args are wrong val
uint8_t UsbWriteStatusMissingArgs(uint8_t   missing_args_cmd); // not enough args
/* TODO: remove dependency on sensor_cmd_key by figuring out what the intent was here */
typedef uint8_t sensor_cmd_key;  // jump-table dictionary uses 8-bit keys
uint8_t UsbWriteStatusSpiBusError(sensor_cmd_key   spi_slave_cmd);
uint8_t FlushInvalidCommand(void);
bool SensorHasResponse(void);

#endif // _USB_H
