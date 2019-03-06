#include "Usb.h"
#include "Ft1248.h"
#include "BiColorLed.h"
#include "stdlib.h" // defines NULL

void UsbInit(void)
{
    FtInit();
}

/* =====[ WIP: Clean Command Parsing with jump tables started 2019-03-01 ]===== */
/* TODO: implement a top-level error handler */
/* to do something useful with a conditional check on UsbWriteStatusBlah(). */
    /* In the application, if you are good-paranoid, */
    /* you actually do check that the status was sent. */
    /* // */
    /* But I have yet to implement a top-level error handler */
    /* to do something useful with this information. */
    /* // */
    /* I just assume status bytes were sent */
    /* and let the UsbHost deal with the possibility */
    /* of not getting any response to its command. */
/* // */
// =====[status_led Pin Connection On SpiMaster hardware ]=====
/* The led_name is defined in the hardware header. */
extern uint8_t const status_led;
/* Define command functions in jump table */
void CmdLedRed(void){
    BiColorLedRed(status_led);
    ; // placeholder: tell mBrd to turn led1 red: BiColorLedRed(status_led_1);
    UsbWriteStatusOk();
}
void CmdLedGreen(void){ BiColorLedGreen(status_led); UsbWriteStatusOk(); }
void CmdCfgLis(void)
{
    /* Spectrometer configuration is four bytes. */
    uint8_t const num_cfgbytes = 4;
    uint8_t read_buffer[num_cfgbytes];
    uint16_t nbytes_read = UsbReadN(read_buffer, num_cfgbytes);
    // TODO: Error checking that we timed out.
    // This will be next error (253). Timeout on expected num_cfgbytes.
    // Error checking that nbytes_read != 4
    if (nbytes_read != num_cfgbytes) { UsbWriteStatusMissingArgs(CmdCfgLis_key); }
    // Have the right number of bytes.
    // Error check that bytes_read are valid.
    // This is error (254). Mimsatch between command and args.
    if (!CfgBytesAreValid(read_buffer)) { UsbWriteStatusBadArgs(CmdCfgLis_key); }
    else
    {
        // [x] Do system tests with
        // /cygdrive/c/chromation-dropbox/Dropbox/sales/spect-py3-examples/main.py
        //
        // TODO: [ ] pass to mBrd
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

/* =====[ Helper for CmdFn: CmdCfgLis ]===== */
bool CfgBytesAreValid(uint8_t const *cfg_bytes)
{
    /* 4 bytes follow the cfg_spectrometer command */
    /* - picture as a 32-bit word: 11111111 11111111 11111111 11111111 */
    /* - 4 MSB of the 32 bits are ignored: XXXX1111 11111111 11111111 11111111 */
    /* - remaining 28 LSB contain LisCfg data */
    /* Of those 28 bits: */
    /* - 3 LSB are for gain and binning: these can be any value */
    /* - remaining 25 signify 25 pixel groups: 1111 11111111 11111111 11111XXX */
    /* - picture the pixel groups as a 5 x 5 grid */
    /* - each group is a 154-pixel span from each of the five rows: */
    /*     row number: 5432 15432154 32154321 54321 */
    /*     bits:       1111 11111111 11111111 11111XXX */
    /* - 1 turns pixel on, 0 turns pixels off */
    /* - a configuration is valid *if*: */
    /*     - pixels from the same row have the same on/off value */
    /*     - the configuration is not *all* rows off */
    /* - examples: */
    /*     - invalid (all rows are off): */
    /*         row number: 5432 15432154 32154321 54321 */
    /*         bits:       0000 00000000 00000000 00000XXX */
    /*     - valid (row 1 are all on, other rows are all off): */
    /*         row number: 5432 15432154 32154321 54321 */
    /*         bits:       0000 10000100 00100001 00001XXX */
    /*     - valid (row 1 is some on, some off): */
    /*         row number: 5432 15432154 32154321 54321 */
    /*         bits:       0000 00000000 00100001 00001XXX */

    /* Invalid if all rows are off. */
    if (CfgTurnsOffAllPixels(cfg_bytes)) return false;
    /* Invalid if any rows are part-on/part-off. */
    if (CfgTurnsRowPartiallyOn(cfg_bytes, 1) ) return false;
    if (CfgTurnsRowPartiallyOn(cfg_bytes, 2) ) return false;
    if (CfgTurnsRowPartiallyOn(cfg_bytes, 3) ) return false;
    if (CfgTurnsRowPartiallyOn(cfg_bytes, 4) ) return false;
    if (CfgTurnsRowPartiallyOn(cfg_bytes, 5) ) return false;
    /* All rows are fully on/off and at least one row is on. */
    return true;
}
/* =====[ Helper for CfgBytesAreValid ]===== */
bool CfgTurnsOffAllPixels(uint8_t const *cfg_bytes)
{
    /* 4 bytes follow cfg_spectrometer command. */
    uint8_t const num_cfgbytes = 4; // 
    /* Bytes are received from UsbHost MSB first */
    /* so bit 0 of cfg_bytes[0] is bit number 24, bit 7 is bit number 31. */
    /* // */
    /* Make a mask of pixel bits: set all pixel bits, clear all don't-care bits. */
    uint8_t const pixel_mask_array[] = {0x0F, 0xFF, 0xFF, 0xF8};
    uint8_t const * pixel_mask = pixel_mask_array;
    uint8_t nbytes_checked = 0; bool all_pixels_are_off = true;
    /* check for any pixels that are on, break the loop as soon as you find one */
    while ((nbytes_checked++ < num_cfgbytes) && all_pixels_are_off)
    {
        // assign boolean to NOT( cfg_bytes bitmasked by pixel_mask)
        // if any bits visible through mask are one, then boolean is false
        // if all bits visible through mask are zero, boolean stays true
        all_pixels_are_off = !( *(cfg_bytes++) & *(pixel_mask++) );
    }
    return all_pixels_are_off;
}
/* =====[ Helper function for CfgTurnsRowPartiallyOn ]===== */
bool XOR(bool a, bool b)
{
    /* No logical XOR operator in C. Use (not(X and Y) and (X or Y)) */
    /* a b (aXORb) !(a&&b) (a||b) (  !(a&&b))  &&  (a||b)  ) */
    /* 0 0    0        1      0                 0            */
    /* 0 1    1        1      1                 1            */
    /* 1 0    1        1      1                 1            */
    /* 1 1    0        0      1                 0            */
    return (  (!(a&&b))  &&  (a||b)  );
}
/* I didn't unit test AllPixelsAreOff. */
/* But it's callers have passing unit tests. */
bool AllPixelsAreOff(uint8_t const *cfg_bytes, uint8_t const *pixel_mask)
{
    // Check if all pixels are off for the row specified by `pixel_mask`.
    uint8_t const num_cfgbytes = 4;
    uint8_t nbytes_checked = 0; bool all_pixels_are_off = true;
    /* check for any pixels that are on, break the loop as soon as you find one */
    while ((nbytes_checked++ < num_cfgbytes) && all_pixels_are_off)
    {
        // assign boolean to NOT( cfg_bytes bitmasked by pixel_mask)
        // if any bits visible through mask are one, then boolean is false
        // if all bits visible through mask are zero, boolean stays true
        all_pixels_are_off = !( *(cfg_bytes++) & *(pixel_mask++) );
    }
    return all_pixels_are_off;
    /* Row 1 does not show up in the first byte (bits 24 to 27). That's OK.*/
    /* The mask for row 1 is 0x00 in the first byte (bit 27 is row 5, bit 24 is row 2). */
    /* Anything & 0 = 0. */
    /* 0 is false. */
    /* Not false is true. */
    /* So all pixels in row 1 are *off* as far as this last byte is concerned. */
}
/* I didn't unit test AllPixelsAreOn. */
/* But it's callers have passing unit tests. */
/* And it's a copy of AllPixelsAreOff. */
bool AllPixelsAreOn(uint8_t const *cfg_bytes, uint8_t const *pixel_mask)
{
    // Check if all pixels are on for the row specified by `pixel_mask`.
    /* This is identical to `AllPixelsAreOff` except */
    /* you *invert* each byte of cfg_bytes before masking. */
    uint8_t const num_cfgbytes = 4;
    uint8_t nbytes_checked = 0; bool all_pixels_are_on = true;
    /* check for any pixels that are off, break the loop as soon as you find one */
    while ((nbytes_checked++ < num_cfgbytes) && all_pixels_are_on)
    {
        /* Flip bits in cfg_bytes so that 0 means on. Simplifies the logic. */
        // assign boolean to NOT( cfg_bytes bitmasked by pixel_mask)
        // if all bits visible through mask are zero (on), boolean stays true
        // if any bits visible through mask are one (off), then boolean is false
        all_pixels_are_on = !(  ( ~(*(cfg_bytes++)) )  &  *(pixel_mask++)  );
    }
    return all_pixels_are_on;
    /* Row 1 does not show up in the first byte (bits 24 to 27). That's OK.*/
    /* The mask for row 1 is 0x00 in the first byte (bit 27 is row 5, bit 24 is row 2). */
    /* Anything & 0 = 0. */
    /* 0 is false. */
    /* Not false is true. */
    /* So all pixels in row 1 are *on* as far as this last byte is concerned. */
}
bool CfgTurnsRowPartiallyOn(uint8_t const *cfg_bytes, uint8_t row_number)
{
    /* There are only five rows. Make sure row_number is between 1 and 5. */
    if ( (row_number < 1) || (row_number > 5) ) { return true; } // fail and bail
    /* Create masks to look only at the pixels in a given row. */
    static uint8_t const row1_mask[] = {0x00, 0x84, 0x21, 0x08};
    static uint8_t const row2_mask[] = {0x01, 0x08, 0x42, 0x10};
    static uint8_t const row3_mask[] = {0x02, 0x10, 0x84, 0x20};
    static uint8_t const row4_mask[] = {0x04, 0x21, 0x08, 0x40};
    static uint8_t const row5_mask[] = {0x08, 0x42, 0x10, 0x80};
    static uint8_t const * pm[] = {
        row1_mask,
        row2_mask,
        row3_mask,
        row4_mask,
        row5_mask
    };
    /* /1* Create a pointer to walk the bytes in the mask during the loop. *1/ */
    /* uint8_t const * row_mask; */
    /* /1* Use the mask for the row chosen by the caller. *1/ */
    /* /1* TODO: Replace with a jump-table. *1/ */
    /* if (1 == row_number) { row_mask = row1_mask; } */
    /* if (2 == row_number) { row_mask = row2_mask; } */
    /* if (3 == row_number) { row_mask = row3_mask; } */
    /* if (4 == row_number) { row_mask = row4_mask; } */
    /* if (5 == row_number) { row_mask = row5_mask; } */

    /* To be valid, a given row must be *all on* or *all off*. */
    /* Return value *true* means the cfg byte is *not valid*. */
    /* all-off |   all-on  | !XOR  |   result */
    /* ------- |   ------  |  ---  |   ------ */
    /*     T   |   F       |   F   |   CfgTurnsRowPartiallyOn is false: all are off */
    /*     F   |   T       |   F   |   CfgTurnsRowPartiallyOn is false: all are on */
    /*     F   |   F       |   T   |   CfgTurnsRowPartiallyOn is true: some must be on */
    /*     T   |   T       |   T   |   CfgTurnsRowPartiallyOn is true: impossible case */
    return !XOR(
        AllPixelsAreOff(cfg_bytes, pm[row_number-1]), // false if cfg is row 1 all on
        AllPixelsAreOn(cfg_bytes, pm[row_number-1]) // true if cfg is row 1 all on
        );
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* all_pixels  = 0b  0000 1111  1111 1111  1111 1111  1111 1000 */
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* row_1       = 0b  0000 0000  1000 0100  0010 0001  0000 1000 */
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* row_2       = 0b  0000 0001  0000 1000  0100 0010  0001 0000 */
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* row_3       = 0b  0000 0010  0001 0000  1000 0100  0010 0000 */
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* row_4       = 0b  0000 0100  0010 0001  0000 1000  0100 0000 */
                    /*   xxxx 5432  1543 2154  3215 4321  5432 1ggb */
    /* row_5       = 0b  0000 1000  0100 0010  0001 0000  1000 0000 */
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
        /* BiColorLedRed(status_led); */
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
        BiColorLedRed(status_led);
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
        BiColorLedRed(status_led);
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
/* =====[ Status ]===== */
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
uint8_t UsbWriteStatusBadArgs(jump_index bad_args_cmd)
{
    uint8_t const StatusBadArgs[] = { 254, bad_args_cmd };
    return UsbWrite(StatusBadArgs,2);
}
uint8_t UsbWriteStatusMissingArgs(jump_index missing_args_cmd)
{
    uint8_t const StatusMissingArgs[] = { 253, missing_args_cmd };
    return UsbWrite(StatusMissingArgs,2);
}

uint16_t UsbWrite(uint8_t const *write_buffer, uint16_t nbytes)
{
    uint16_t num_bytes_sent = 0;
    FtSendCommand(FtCmd_Write);
    if (!FtBusTurnaround())
    {
        BiColorLedRed(status_led);
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
