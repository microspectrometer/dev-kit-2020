#include "Usb.h"
#include "Ft1248.h"
#include "DebugLed.h"

uint16_t UsbRead(uint8_t *read_buffer_address)
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
    while (FtRead(read_buffer_address++))
    {
        num_bytes_read++;
    }

    FtDeactivateInterface();
    return num_bytes_read;
}
