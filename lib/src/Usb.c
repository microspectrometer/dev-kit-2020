#include "Usb.h"
#include "Ft1248.h"
#include "DebugLed.h"

void UsbInit(void)
{
    FtInit();
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
uint16_t UsbWrite(uint8_t *write_buffer, uint16_t nbytes)
{
    uint16_t num_bytes_sent = 0;
    FtSendCommand(FtCmd_Write);
    if (!FtBusTurnaround())
    {
        DebugLedTurnRedToShowError();
        FtDeactivateInterface();
        return num_bytes_sent;
    }
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
