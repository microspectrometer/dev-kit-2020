/** \file */
#ifndef _BRIDGE_H
#define _BRIDGE_H

#include <stdbool.h>    // bool, true, false
#include <stdint.h>     // uint8_t

/* =====[ Move this to a shared lib ]===== */
typedef uint8_t const status_byte;
status_byte ok;
status_byte error;
status_byte invalid_cmd;
status_byte led_off;
status_byte led_green;
status_byte led_red;
typedef uint8_t const led_name;
led_name led_0;
led_name led_1;
typedef uint8_t const config_byte;
/* config_byte binning_off; */
/* config_byte binning_on; */
config_byte gain1x;
config_byte gain25x;
config_byte gain4x;
config_byte gain5x;
config_byte all_rows_active;

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
/* =====[ API started 2019-10-02 ]===== */
/* bridge_cmd_key const NullCommand_key; */
/* bridge_cmd_key const GetBridgeLED_key; */
/* bridge_cmd_key const SetBridgeLED_key; */
/* bridge_cmd_key const BridgeGetSensorLED_key; */
/* bridge_cmd_key const BridgeSetSensorLED_key; */
/* bridge_cmd_key const TestInvalidSensorCmd_key; */
/* bridge_cmd_key const TestInvalidSensorCmdPlusPayload_key; */
/* bridge_cmd_key const BridgeGetSensorConfig_key; */
/* bridge_cmd_key const BridgeSetSensorConfig_key; */
/* bridge_cmd_key const BridgeGetExposure_key; */
/* bridge_cmd_key const BridgeSetExposure_key; */
/* bridge_cmd_key const BridgeCaptureFrame_key; */

 /* LookUpCmd takes key from UsbHost and returns the function pointer to call. */
typedef void (BridgeCmd)(void);
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
void BridgeSetSensorConfig(void);
void BridgeGetSensorConfig(void);
void BridgeGetExposure(void);
void BridgeSetExposure(void);
void BridgeCaptureFrame(void);

#endif // _USB_H
