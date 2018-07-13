#include "Usb.h"
#include "Ft1248.h"
#include "DebugLed.h"

bool UsbRead(uint8_t *read_buffer_address)
{
    FtSendCommand(FtCmd_Read);
    bool has_data_to_read = FtBusTurnaround();
    if (!has_data_to_read)
    {
        // sad path
        DebugLedTurnRedToShowError();
        FtDeactivateInterface();
        return false;
    }
    FtRead(read_buffer_address);
    FtDeactivateInterface();
    return true;
}
