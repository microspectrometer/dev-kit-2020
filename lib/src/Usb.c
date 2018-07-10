#include "Usb.h"
#include "Ft1248.h"
void UsbRead(void)
{
    FtSendCommand(FtCmd_Read);
    bool has_data_to_read = FtBusTurnaround();
}
