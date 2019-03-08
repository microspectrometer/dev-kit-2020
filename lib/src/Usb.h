#ifndef _USB_H
#define _USB_H

#include <stdbool.h>    // bool, true, false
#include <stdint.h>     // uint8_t
/* TODO: move the jump-table functions to a new file so that lib Usb does not depend on Spi.h */
#include "Spi.h" // for `SpiMasterWriteByte` and definitions of `extern` /spi_.*_key

void UsbInit(void);
uint16_t UsbRead(uint8_t *read_buffer);
uint16_t UsbWrite(uint8_t const *write_buffer, uint16_t nbytes);
bool UsbHasDataToRead(void);
bool UsbHasRoomToWrite(void);

/* =====[ WIP: Clean Command Parsing with jump tables started 2019-03-01 ]===== */
/* Functions of type `BridgeCmd` take nothing and return nothing. */
/* The *key* acts as the command since it is the command lookup. */
/* If the commands need additional parameters, they will read additional bytes. */
/* All functions in the lookup table must have the same signature, so commands that */
/* take return functions other than void-void need to go in a different jump table. */
typedef void (BridgeCmd)(void);
/* Give tests of LookupBridgeCmd access to names of functions in */
/* jump table to compare pointer values. */
BridgeCmd BridgeLedRed;
BridgeCmd BridgeLedGreen;
BridgeCmd BridgeCfgLis;
BridgeCmd SendSensorLed1Red;
BridgeCmd SendSensorLed1Green;
/* This is the datatype to use when calling LookupBridgeCmd: */
typedef uint8_t bridge_cmd_key;  // jump-table dictionary uses 8-bit keys
/* Declare keys for callers of LookupBridgeCmd (values hidden in .c file) */
bridge_cmd_key const BridgeLedRed_key;
bridge_cmd_key const BridgeLedGreen_key;
bridge_cmd_key const BridgeCfgLis_key;
bridge_cmd_key const SendSensorLed1Red_key;
bridge_cmd_key const SendSensorLed1Green_key;
/* =====[ API: ]===== */
 /* LookUpCmd takes key from UsbHost and returns the function pointer to call. */
BridgeCmd* LookupBridgeCmd(bridge_cmd_key const key);
/* report status to UsbHost */
uint8_t UsbWriteStatusOk(bridge_cmd_key   cmd_done_by_bridge);
uint8_t UsbWriteStatusInvalid(bridge_cmd_key   invalid_cmd); // command not recognized
uint8_t UsbWriteStatusBadArgs(bridge_cmd_key   bad_args_cmd); // args are wrong val
uint8_t UsbWriteStatusMissingArgs(bridge_cmd_key   missing_args_cmd); // not enough args
uint8_t UsbWriteStatusSpiBusError(sensor_cmd_key   spi_slave_cmd);

/* =====[ WIP: robust UsbRead functionality started 2019-03-01 ]===== */
uint8_t UsbReadOneByte(uint8_t *read_buffer);
// TODO: rename UsbRead to UsbReadAll then UsbReadN to UsbRead
uint16_t UsbReadN(uint8_t *read_buffer, uint16_t nbytes);
/* =====[ Helpers for BridgeCmd functions ]===== */
bool CfgBytesAreValid(uint8_t const *cfg_bytes);
bool CfgTurnsOffAllPixels(uint8_t const *cfg_bytes);
bool CfgTurnsRowPartiallyOn(uint8_t const *cfg_bytes, uint8_t row_number);
bool XOR(bool a, bool b);

#endif // _USB_H
