#ifndef _BRIDGE_H
#define _BRIDGE_H

#include <stdint.h>     // uint8_t
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
BridgeCmd SendSensorLed2Red;
BridgeCmd SendSensorLed2Green;
BridgeCmd SendSensorLed3Red;
BridgeCmd SendSensorLed3Green;
BridgeCmd SendSensorLed4Red;
BridgeCmd SendSensorLed4Green;
/* This is the datatype to use when calling LookupBridgeCmd: */
typedef uint8_t bridge_cmd_key;  // jump-table dictionary uses 8-bit keys
/* Declare keys for callers of LookupBridgeCmd (values hidden in .c file) */
bridge_cmd_key const BridgeLedRed_key;
bridge_cmd_key const BridgeLedGreen_key;
bridge_cmd_key const BridgeCfgLis_key;
bridge_cmd_key const SendSensorLed1Red_key;
bridge_cmd_key const SendSensorLed1Green_key;
bridge_cmd_key const SendSensorLed2Red_key;
bridge_cmd_key const SendSensorLed2Green_key;
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
