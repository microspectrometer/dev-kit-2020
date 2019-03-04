// This PCB is the bridge from the USB host to the Chromation spectrometer.
//
#include <DebugLed.h>               // controls the debug LED
#include "DebugLed-Hardware.h"      // map debug LED to actual hardware
#include <Ft1248.h>                 // supports Usb -- TODO: does app need this?
#include "Ft1248-Hardware.h"        // map FT1248 (USB) I/O to actual hardware
#include <Usb.h>                    // USB host communication
#include <Spi.h>                    // Chromation spectrometer is a SPI slave
#include "../../mBrd/src/Spi-Hardware.h" // map SPI I/O to actual hardware
#include "../../mBrd/src/Spi-Commands.h" // commands understood by the SPI slave
#include <ReadWriteBits.h>
#include <stdio.h>  // snprintf()
#include <string.h> // strlen()
#include "AvrAsmMacros.h"       // resolve lib dependencies on AVR asm macros

/* =====[ Required Hardware Settings in FT_Prog ]===== */
/* - Hardware Specific -> Ft1248 Settings */
/* - Clock Polarity High: unchecked */
/* - Bit Order LSB: unchecked */
/* - Flow Ctrl not selected: checked */
/* - serial number: CHROMATION123456 */

/* =====[ Cfg on Windows for customer's optional access as a COM Port ]===== */
/* - check box `Load VCP` */

// Spi-master application-level API
uint16_t SpiMasterPassLisFrame(void);        // get the real Lis frame
uint16_t nbytes_in_frame;

/* =====[ Helper function for main setup ]===== */
void SetupDebugLed(void)
{
    DebugLedInit(
        DebugLed_ddr,
        DebugLed_port,
        DebugLed_pin,
        debug_led
        );
}

void AutoExpose(void)
{
    // Tell SpiSlave to AutoExpose.
    MacroSpiMasterWrite(cmd_auto_expose);
    // Pull new exposure time value from SpiSlave.
    uint8_t nticks[2];
    MacroSpiMasterWaitForResponse();
    MacroSpiMasterWrite(slave_ignore);      // transfer msb
    nticks[0] = *Spi_spdr;               // store msb
    MacroSpiMasterWaitForResponse();
    MacroSpiMasterWrite(slave_ignore);      // transfer lsb
    nticks[1] = *Spi_spdr;               // store lsb
    // Push new exposure time value to UsbHost.
    UsbWrite(nticks, 2);
}

/* =====[ WIP ]===== */
void Bridge_data_flow_between_USB_Host_and_SpiSlave(void)
{
    /* uint8_t cmd; */
    jump_index cmd;
    // Read USB buffer if there is any data to read.
    if (UsbReadOneByte(&cmd))
    {
        /* This first byte is *always* a command from the USB host. */
        UsbCmd* CmdFn = LookupCmd(cmd);
        /* Tell UsbHost if the command is invalid. */
        if (CmdFn == NULL) UsbWriteStatusInvalid(cmd);
        /* Do command if it is valid. */
        else CmdFn();
        /* It is up to CmdFn to let the UsbHost know */
        /* command was successfully carried out. */
        /* If the CmdFn requires sending additional data, */
        /* send UsbWriteStatusOk() before sending any other data. */
    }
}

/* =====[ GOOD ]===== */
void SetExposureTime(uint8_t *pnticks)
{
    // Push new exposure time to SpiSlave.
    // TODO: eliminate the need for the delay.
    // Delay after each write to let the slave catch-up before the next byte.
    MacroSpiMasterWriteAndDelay(cmd_set_exposure_time);
    MacroSpiMasterWriteAndDelay(*(pnticks++));
    MacroSpiMasterWrite(*(pnticks--));
    // Pull new exposure time value from SpiSlave.
    MacroSpiMasterWaitForResponse();
    MacroSpiMasterWrite(slave_ignore);      // transfer msb
    *(pnticks++) = *Spi_spdr;               // store msb
    MacroSpiMasterWaitForResponse();
    MacroSpiMasterWrite(slave_ignore);      // transfer lsb
    *(pnticks--) = *Spi_spdr;               // store lsb
    // Push new exposure time value to UsbHost.
    UsbWrite(pnticks, 2);
}

#define MacroDebugLedRed()          MacroSetBit(DebugLed_port, debug_led)
#define MacroDebugLedGreen()        MacroClearBit(DebugLed_port, debug_led)
#define MacroDebugLedToggleColor()  MacroToggleBit(DebugLed_port, debug_led)

#define UsbWriteDelayTicks 1
#define MacroUsbWriteByte(pbyte) do { \
    /* =====[ FtSendCommand(FtCmd_Write); \ ]===== */ \
    /* FtActivateInterface(); \ */ \
    MacroClearBit(Ft1248_port, Ft1248_Ss); \
    Delay3CpuCyclesPerTick(UsbWriteDelayTicks); \
    /* FtPushData(); \ */ \
    MacroSetBit(Ft1248_port, Ft1248_Sck); \
    /* FtLetMasterDriveBus(); \ */ \
    *FtMiosio_ddr = 0xFF; \
    /* FtOutputByte(FtCmd); \ */ \
    *FtMiosio_port = FtCmd_Write; \
    /* FtPullData(); \ */ \
    MacroClearBit(Ft1248_port, Ft1248_Sck); \
    /* if (!FtBusTurnaround()) DebugLedTurnRedToShowError(); \ */ \
    /* FtLetSlaveDriveBus(); */ \
    *FtMiosio_ddr = 0x00; \
    /* FtPushData(); \ */ \
    MacroSetBit(Ft1248_port, Ft1248_Sck); \
    /* FtPullData(); \ */ \
    MacroClearBit(Ft1248_port, Ft1248_Sck); \
    /* return FtIsBusOk(); */ \
    /* =====[ If bus is not OK, turn LED red and do nothing ]===== */ \
    if (!MacroBitIsClear(Ft1248_pin, Ft1248_Miso)) MacroDebugLedRed(); \
    else \
    { \
    /* =====[ FtWrite(pbyte); ]===== */ \
    /* FtLetMasterDriveBus(); \ */ \
    *FtMiosio_ddr = 0xFF; \
    /* FtPushData(); \ */ \
    MacroSetBit(Ft1248_port, Ft1248_Sck); \
    /* FtWriteData(*write_buffer); */ \
    *FtMiosio_port = *pbyte; \
    /* FtPullData(); \ */ \
    MacroClearBit(Ft1248_port, Ft1248_Sck); \
    /* if (!FtIsBusOk()) return false; */ \
    /* =====[ If write failed, turn LED red ]===== */ \
    if (!MacroBitIsClear(Ft1248_pin, Ft1248_Miso)) MacroDebugLedRed(); \
    } \
    /* =====[ No matter what happened, deactivate the interface. ]===== */ \
    /* FtDeactivateInterface(); \ */ \
    MacroSetBit(Ft1248_port, Ft1248_Ss); \
} while (0)

#define DebugPinInit() do { \
    /* =====[ Make SCL an output for debug on the oscilloscope ]===== */ \
    MacroSetBit(&DDRC,  PC5); \
    /* =====[ Init SCL high ]===== */ \
    MacroSetBit(&PORTC, PC5); \
} while (0)
#define DebugPinLow()  MacroClearBit(&PORTC, PC5)
#define DebugPinHigh() MacroSetBit(&PORTC, PC5)

int main()
{
    /* =====[ Setup ]===== */
    SetupDebugLed();
    SpiMasterInit();
    UsbInit();
    DebugPinInit();
    nbytes_in_frame = sizeof_full_frame;
    /* =====[ Loop ]===== */
    while(1){ Bridge_data_flow_between_USB_Host_and_SpiSlave(); }
}
uint16_t SpiMasterPassLisFrame(void)
{
    // Send the frame of Lis pixel data.
    // The slave just finished the readout.
    // Fetch each byte from the slave and send it to the USB host.
    // Return the number of bytes passed from spi-slave to USB host.
    DebugPinLow();
    MacroSpiMasterWrite(cmd_send_lis_frame);
    DebugPinHigh();
    uint16_t byte_counter = 0;
    uint8_t byte_buffer;
    MacroSpiMasterWaitForResponse(); // Slave signals when the response is ready.
    DebugPinLow();
    while (++byte_counter < nbytes_in_frame)
    {
        /* =====[ byte_buffer = SpiMasterRead(); // read this byte ]===== */
        MacroSpiMasterWrite(slave_ignore);  // fetch this byte
        byte_buffer = *Spi_spdr;            // store this byte for sending
        MacroSpiMasterWaitForResponse();    // slave signals the next byte is ready
        DebugPinHigh();
        /* =====[ MacroUsbWriteByte time is 4.5us. ]===== */
        MacroUsbWriteByte(&byte_buffer); // send this byte before fetching next
    }
    /* =====[ byte_buffer = SpiMasterRead(); // read last byte ]===== */
    MacroSpiMasterWrite(slave_ignore);      // fetch last byte
    byte_buffer = *Spi_spdr;                // store this byte for sending
    /* =====[ MacroUsbWriteByte time is 4.5us. ]===== */
    MacroUsbWriteByte(&byte_buffer);        // send last byte
    return byte_counter;
}

/* =====[ OLD ]===== */
void DoCmdSendFourDummyBytes(void)
{
    SpiMasterWrite(cmd_send_four_dummy_bytes);
    uint8_t fake_data[4]; uint8_t * pfake_data = fake_data;
    uint16_t nbytes = sizeof(fake_data);
    uint16_t byte_counter = 0;
    while (byte_counter++ < nbytes)
    {
        SpiMasterWaitForResponse(); // Slave signals when the response is ready.
        *(pfake_data++) = SpiMasterRead();
    }
    UsbWrite(fake_data, nbytes);
    DebugLedTurnRed();
}
void SpiMaster_pass_commands_from_USB_Host_pass_data_from_slave_old(void)
{
    while(1) // loop forever responding to the USB host
    {
        if (UsbHasDataToRead())
        {
            /* MacroDebugLedToggleColor(); */
            /* MacroDebugLedRed(); */
            uint8_t read_buffer[4];
            uint8_t nbytes_in_cmd = UsbRead(read_buffer);
            uint8_t cmd = read_buffer[0];
            if (1 == nbytes_in_cmd)
            {
                if      (cmd == cmd_send_lis_frame) { MacroDebugLedRed(); SpiMasterPassLisFrame(); MacroDebugLedGreen(); }
                // for using Arduino as the SPI master via the ISP header:
                else if (cmd == cmd_disable_spi_master) { SpiMasterDisable(); MacroDebugLedRed(); }
                else if (cmd == cmd_enable_spi_master)  { SpiMasterInit(); MacroDebugLedGreen(); }
                // test commands
                else if (cmd == cmd_send_four_dummy_bytes) DoCmdSendFourDummyBytes();
                else if (cmd == cmd_auto_expose) AutoExpose();
            }
            else if (2 == nbytes_in_cmd)
            {
                uint8_t gain_setting = read_buffer[1];
                if      (gain_setting == cmd_cfg_lis_gain_5x)  MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_5x);
                else if (gain_setting == cmd_cfg_lis_gain_4x)  MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_4x);
                else if (gain_setting == cmd_cfg_lis_gain_2pt5x)  MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_2pt5x);
                else if (gain_setting == cmd_cfg_lis_gain_1x)  MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_1x);
                if (cmd == cmd_write_cfg_to_lis) MacroSpiMasterWriteAndDelay(cmd_write_cfg_to_lis);
            }
            else if (3 == nbytes_in_cmd)
            {
                uint8_t *pnticks = &read_buffer[1]; // two bytes of nticks_exposure
                if      (cmd == cmd_set_exposure_time) SetExposureTime(pnticks);
            }
            else if (4 == nbytes_in_cmd)
            {
                uint8_t summing_mode    = read_buffer[1];
                uint8_t gain_setting    = read_buffer[2];
                uint8_t rowselect       = read_buffer[3];
                if      (summing_mode == cmd_cfg_summing_mode_on)   MacroSpiMasterWriteAndDelay(cmd_cfg_summing_mode_on);
                else                                                MacroSpiMasterWriteAndDelay(cmd_cfg_summing_mode_off);
                if      (summing_mode == cmd_cfg_summing_mode_on)   nbytes_in_frame = sizeof_half_frame;
                else                                                nbytes_in_frame = sizeof_full_frame;
                if      (gain_setting == cmd_cfg_lis_gain_5x)       MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_5x);
                else if (gain_setting == cmd_cfg_lis_gain_4x)       MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_4x);
                else if (gain_setting == cmd_cfg_lis_gain_2pt5x)    MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_2pt5x);
                else                                                MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_1x);
                if      (rowselect == cmd_cfg_lis_rowselect_1)      MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_1);
                else if (rowselect == cmd_cfg_lis_rowselect_2)      MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_2);
                else if (rowselect == cmd_cfg_lis_rowselect_3)      MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_3);
                else if (rowselect == cmd_cfg_lis_rowselect_4)      MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_4);
                else if (rowselect == cmd_cfg_lis_rowselect_5)      MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_5);
                else if (rowselect == cmd_cfg_lis_rowselect_12)     MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_12);
                else if (rowselect == cmd_cfg_lis_rowselect_123)    MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_123);
                else if (rowselect == cmd_cfg_lis_rowselect_1234)   MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_1234);
                else if (rowselect == cmd_cfg_lis_rowselect_124)    MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_124);
                else                                                MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_12345);
                if (cmd == cmd_write_cfg_to_lis) MacroSpiMasterWriteAndDelay(cmd_write_cfg_to_lis);
            }
            /* MacroDebugLedGreen(); */
        }
    }
}


