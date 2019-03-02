#include "Usb.h"
#include "Ft1248.h"
#include "DebugLed.h"
#include "stdlib.h" // defines NULL

void UsbInit(void)
{
    FtInit();
}

/* =====[ WIP: Clean Command Parsing with jump tables started 2019-03-01 ]===== */
/* Define command functions in jump table */
void CmdLedRed(void){ DebugLedTurnRed(); UsbWriteStatusOk(); }
void CmdLedGreen(void){ DebugLedTurnGreen(); UsbWriteStatusOk(); }
/* TODO: unit test CfgBytesAreValid */
bool CfgBytesAreValid(uint8_t const *cfg_bytes)
{
    /* in the future I'll find a way to define many valid 4-byte sequences */
    uint8_t const all_rows_1x_gain_binned[] = {0x07, 0xff, 0xff, 0xf9};
    uint8_t const *valid = all_rows_1x_gain_binned;
    uint8_t const num_cfgbytes = 4; // 4 bytes follow cfg_spectrometer command
    uint8_t nbytes_compared = 0; bool bytes_match = true;
    while ((nbytes_compared++ < num_cfgbytes) && bytes_match)
    {
        if (*(valid++) != *(cfg_bytes++)) bytes_match=false;
    }
    return bytes_match;
}
/* TODO: unit test CmdCfgLis after UsbReadN */
void CmdCfgLis(void)
{
    uint8_t const num_cfgbytes = 4; // 4 bytes follow cfg_spectrometer command
    uint8_t read_buffer[num_cfgbytes];
    uint16_t nbytes_read = UsbReadN(read_buffer, num_cfgbytes);
    // TODO: Error checking that we timed out.
    // This will be next error (253). Timeout on expected num_cfgbytes.
    // Error checking that nbytes_read != 4
    if (nbytes_read != num_cfgbytes) { UsbWriteStatusTimedOut(CmdCfgLis_key); }
    
    // Have the right number of bytes.
    // Error check that bytes_read are valid.
    // TODO: do another jump table for this
    // each valid set of bytes is an entry in the jump table
    // This will be next error (254). Mimsatch between command and args.
    // TODO: how do you make this a jump table?
    // For now just compare equality with one valid value.
    if (!CfgBytesAreValid(read_buffer)) { UsbWriteStatusMismatch(CmdCfgLis_key); }
    else
    {
        // TODO: [ ] pass to mBrd (do this after testin UsbSpi with Python)
        // mBrd converts to uint32_t and does cfg
        // ...
        // at end of happy path
        UsbWriteStatusOk();
        // echo back cfg bytes
        UsbWrite(read_buffer,4);
    }
}
/* Define a named key for each function (`FooBar_key` is the key for `FooBar`) */
jump_index const CmdLedRed_key = 0;
jump_index const CmdLedGreen_key = 1;
jump_index const CmdCfgLis_key = 2;
UsbCmd* LookupCmd(jump_index const key) {
    /* pf is an array of pointers to UsbCmd functions */
    /* pf lives in static memory, not on the `LookupCmd` stack frame */
    static UsbCmd* const pf[] = {
        CmdLedRed,
        CmdLedGreen,
        CmdCfgLis
        };
    /* Return func ptr. Prevent attempts at out-of-bounds access. */
    if (key < sizeof(pf)/sizeof(*pf))   return pf[key];
    /* Out of bounds keys return a NULL pointer. */
    else return NULL;
    /* Up to caller to check for NULL and take appropriate action. */
    /* Recommended action: tell UsbHost the command was not recognized. */
}
uint8_t UsbReadOneByte(uint8_t *read_buffer)
{
    uint8_t num_bytes_read = 0;
    FtSendCommand(FtCmd_Read);
    bool has_data_to_read = FtBusTurnaround();
    if (!has_data_to_read)
    {
        // sad path
        // No, not an error. Not a sad path.
        // Use this in place of checking if Ft_Miso is low in app.
        /* DebugLedTurnRedToShowError(); */
        FtDeactivateInterface();
        return num_bytes_read;
    }
    FtRead(read_buffer); num_bytes_read++;
    FtDeactivateInterface();
    return num_bytes_read;
}
// TODO: unit test UsbReadN
uint16_t UsbReadN(uint8_t *read_buffer, uint16_t nbytes)
{
    uint16_t num_bytes_read = 0;
    FtSendCommand(FtCmd_Read);
    bool has_data_to_read = FtBusTurnaround();
    if (!has_data_to_read)
    {
        // sad path
        DebugLedTurnRedToShowError();
        FtDeactivateInterface();
        return num_bytes_read;
    }
    bool read_ok = true; bool finished = false;
    /* TODO: add a timer to prevent this while loop from hanging */
    /* // This will be next error (254). Timeout on expected nbytes. */
    while (read_ok && !finished)
    {
        read_ok = FtRead(read_buffer++);
        if (read_ok) num_bytes_read++;
        finished = (num_bytes_read >= nbytes);
    }
    FtDeactivateInterface();
    return num_bytes_read;
}
uint16_t UsbRead(uint8_t *read_buffer)
{
    uint16_t num_bytes_read = 0;
    FtSendCommand(FtCmd_Read);
    bool has_data_to_read = FtBusTurnaround();
    if (!has_data_to_read)
    {
        // sad path
        DebugLedTurnRedToShowError();
        FtDeactivateInterface();
        return num_bytes_read;
    }
    while (FtRead(read_buffer++))
    {
        num_bytes_read++;
    }

    FtDeactivateInterface();
    return num_bytes_read;
}
bool UsbWriteStatusOk(void)
{
    uint8_t const StatusOk = 0;
    return UsbWrite(&StatusOk,1);
}
uint8_t UsbWriteStatusInvalid(jump_index invalid_cmd)
{
    uint8_t const StatusInvalid[] = { 255, invalid_cmd };
    return UsbWrite(StatusInvalid,2);
}
uint8_t UsbWriteStatusMismatch(jump_index bad_args_cmd)
{
    uint8_t const StatusMismatch[] = { 254, bad_args_cmd };
    return UsbWrite(StatusMismatch,2);
}
uint8_t UsbWriteStatusTimedOut(jump_index missing_args_cmd)
{
    uint8_t const StatusTimedOut[] = { 253, missing_args_cmd };
    return UsbWrite(StatusTimedOut,2);
}

uint16_t UsbWrite(uint8_t const *write_buffer, uint16_t nbytes)
{
    uint16_t num_bytes_sent = 0;
    FtSendCommand(FtCmd_Write);
    if (!FtBusTurnaround())
    {
        DebugLedTurnRedToShowError();
        FtDeactivateInterface();
        return num_bytes_sent;
    }
    // TODO: rename `byte_sent` as `byte_sent_OK` or `byte_was_sent`
    bool byte_sent = true; bool finished = false;
    while (byte_sent && !finished)
    {
        byte_sent = FtWrite(write_buffer++);
        if (byte_sent) num_bytes_sent++;
        finished = (num_bytes_sent >= nbytes);
    }
    FtDeactivateInterface();
    return num_bytes_sent;
}
bool UsbHasDataToRead(void) { return FtHasDataToRead(); }
bool UsbHasRoomToWrite(void) { return FtHasRoomToWrite(); }
