#include "Usb.h"
#include "Ft1248.h"
#include "BiColorLed.h"
#include "stdlib.h" // defines NULL

// =====[status_led defined in BiColorLed-Hardware header]=====
extern uint8_t const status_led;

void UsbInit(void)
{
    FtInit();
}

uint8_t UsbReadOneByte(uint8_t *read_buffer)
{
    uint8_t num_bytes_read = 0;
    FtSendCommand(FtCmd_Read);
    bool has_data_to_read = FtBusTurnaround();
    if (!has_data_to_read)
    {
        // This path lets `UsbReadOneByte` work as a looping check to
        // see when a first byte is ready.
        // This is preferable to using a low-level Ft1248 call to
        // check Ft_Miso LOW in the application.
        FtDeactivateInterface();
        return num_bytes_read;
    }
    FtRead(read_buffer); num_bytes_read++;
    FtDeactivateInterface();
    return num_bytes_read;
}
static uint16_t UsbReadBytes_Implementation(uint8_t *read_buffer, uint16_t nbytes)
{
    uint16_t num_bytes_read = 0;
    while (num_bytes_read < nbytes)
    {
        while (!UsbReadOneByte(read_buffer)); // Loops until FtBus has data.
        num_bytes_read++;
        read_buffer++;
    }
    return num_bytes_read;
}
uint16_t (*UsbReadBytes)(uint8_t *, uint16_t) = UsbReadBytes_Implementation;

// TODO: unit test UsbReadN
static uint16_t UsbReadN_Implementation(uint8_t *read_buffer, uint16_t nbytes)
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
uint16_t (*UsbReadN)(uint8_t *, uint16_t) = UsbReadN_Implementation;

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

static uint16_t UsbWrite_Implementation(uint8_t const *write_buffer, uint16_t nbytes)
{
    uint16_t num_bytes_sent = 0;
    FtSendCommand(FtCmd_Write);
    if (!FtBusTurnaround())
    {
        /* BiColorLedRed(status_led); */
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
uint16_t (*UsbWrite)(uint8_t const *, uint16_t) = UsbWrite_Implementation;

bool UsbHasDataToRead(void) { return FtHasDataToRead(); }
bool UsbHasRoomToWrite(void) { return FtHasRoomToWrite(); }
