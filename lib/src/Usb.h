#ifndef _USB_H
#define _USB_H

#include <stdbool.h>    // bool, true, false
#include <stdint.h>     // uint8_t

void UsbInit(void);
uint16_t UsbRead(uint8_t *read_buffer);
uint16_t UsbWrite(uint8_t const *write_buffer, uint16_t nbytes);
bool UsbHasDataToRead(void);
bool UsbHasRoomToWrite(void);

/* =====[ WIP: robust UsbRead functionality started 2019-03-01 ]===== */
uint8_t UsbReadOneByte(uint8_t *read_buffer);
/* =====[ WIP: Clean Command Parsing with jump tables started 2019-03-01 ]===== */
/* Functions of type `UsbCmd` take nothing and return nothing. */
/* The *key* acts as the command since it is the command lookup. */
/* If the commands need additional parameters, they will read additional bytes. */
/* All functions in the lookup table must have the same signature, so commands that */
/* take return functions other than void-void need to go in a different jump table. */
typedef void (UsbCmd)(void);
/* Give tests of LookupCmd access to names of functions in */
/* jump table to compare pointer values. */
UsbCmd CmdLedRed;
UsbCmd CmdLedGreen;
UsbCmd CmdCfgLis;
/* This is the datatype to use when calling LookupCmd: */
typedef uint8_t jump_index;  // jump-table dictionary uses 8-bit keys
/* Declare keys for callers of LookupCmd (values hidden in .c file) */
jump_index const CmdLedRed_key;
jump_index const CmdLedGreen_key;
jump_index const CmdCfgLis_key;
/* =====[ API: ]===== */
 /* LookUpCmd takes key from UsbHost and returns the function pointer to call. */
UsbCmd* LookupCmd(jump_index const key);
/* report status to UsbHost */
bool UsbWriteStatusOk(void); // command was successfully carried out
uint8_t UsbWriteStatusInvalid(jump_index invalid_cmd); // command not recognized
uint8_t UsbWriteStatusMismatch(jump_index bad_args_cmd); // args are wrong val
uint8_t UsbWriteStatusTimedOut(jump_index missing_args_cmd); // not enough args
// TODO: rename UsbRead to UsbReadAll then UsbReadN to UsbRead
uint16_t UsbReadN(uint8_t *read_buffer, uint16_t nbytes);
bool CfgBytesAreValid(uint8_t const *cfg_bytes);
bool CfgTurnsOffAllPixels(uint8_t const *cfg_bytes);
bool CfgTurnsRowPartiallyOn(uint8_t const *cfg_bytes, uint8_t row_number);
bool XOR(bool a, bool b);

#endif // _USB_H
