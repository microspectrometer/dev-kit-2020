/** \file
  * *Empty tests are for generating function doc strings*\n 
  * Empty tests contain one line: `TEST_PASS();`
  * */
#include "unity.h"
#include "Mock.h" // record call history in "mock"
#include "test_SpiSlave.h"
#include "Spi.h"
#include "SpiSlave.h"
#include "ReadWriteBits.h"

/* =====[ Check_SpiSlave_plumbing_for_fakes ]===== */
void SpiSlave_faked_calls_are_still_available_for_testing(void)
{
    printf("SpiSlave_faked_calls_are_still_available_for_testing:\n");
    /* =====[ Operate and Test]===== */
    EnableSpiInterrupt();
    printf(
        "1. Tests are able to call "
        "real version of `EnableSpiInterrupt` "
        "(compiler successfully outputs `test_SpiSlave.o`).\n"
        );
    /* =====[ Operate and Test ]===== */
    SpiSlaveInit();
    printf(
        "2. When tests call `SpiSlaveInit`, it calls "
        "`EnableSpiInterrupt_fake` (the fake records the call):\n"
        );
    uint16_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "EnableSpiInterrupt"),
        "Expect SpiSlaveInit calls fake which records call name."
        );
}

/* =====[ _SignalDataReady ]===== */
void SignalDataReady_drives_DataReady_LOW(void)
{
    /* =====[ Setup ]===== */
    SetBit(Spi_PortOutput, Spi_DataReady);
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DataReady,
        *Spi_PortOutput,
        "Cannot run test: must start with DataReady HIGH!"
        );
    /* =====[ Operate ]===== */
    _SignalDataReady();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Spi_DataReady, *Spi_PortOutput);
}

/* =====[ DisableSpiInterrupt ]===== */
void DisableSpiInterrupt_clears_the_SPI_Interrupt_Enable_bit(void)
{
    /* =====[ Setup ]===== */
    SetBit(Spi_SPCR, Spi_InterruptEnable);
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_InterruptEnable,
        *Spi_SPCR,
        "Cannot run test: must start with bit HIGH!"
        );
    /* =====[ Operate ]===== */
    DisableSpiInterrupt();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Spi_InterruptEnable, *Spi_SPCR);
}

/* =====[ EnableSpiInterrupt ]===== */
void EnableSpiInterrupt_clears_SPI_interrupt_flag(void)
{
    /* =====[ Operate ]===== */
    EnableSpiInterrupt();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE(
        AssertCall(mock, call_n, "ClearSpiInterruptFlag")
        );
}
void EnableSpiInterrupt_enables_SPI_transfer_complete_interrupt(void)
{
    /* =====[ Setup ]===== */
    ClearBit(Spi_SPCR, Spi_InterruptEnable);
    /* =====[ Operate ]===== */
    EnableSpiInterrupt();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_InterruptEnable, *Spi_SPCR);
}
void EnableSpiInterrupt_consumes_6_cycles(void)
{
    TEST_PASS();
    // ---Expected Assembly---
    // cli
    // in	r24, 0x2d	; 45
    // in	r24, 0x2e	; 46
    // in	r24, 0x2c	; 44
    // ori	r24, 0x80	; 128
    // out	0x2c, r24	; 44
    // Total number of cycles: 6
    // Total number of instructions: 6
}

/* =====[ SpiSlaveInit ]===== */
void SpiSlaveInit_makes_DataReady_an_output_pin(void)
{
    /* =====[ Setup ]===== */
    *Spi_PortDirection = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_DataReady,
        *Spi_PortDirection,
        "Cannot run test: must start with ddr bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DataReady,
        *Spi_PortDirection,
        "Expect DataReady to be an output."
        );
}
void SpiSlaveInit_idles_DataReady_high(void)
{
    /* =====[ Setup ]===== */
    *Spi_PortOutput = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_DataReady,
        *Spi_PortOutput,
        "Cannot run test: must start with port bit clear!"
        );
    /* =====[ Operate ]===== */
    /* printf("Value of Spi_PortOutput before is 0x%02x\n", *Spi_PortOutput); */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    /* printf("Value of Spi_DataReady is pin %d\n", Spi_DataReady); */
    /* printf("Value of Spi_PortOutput after is 0x%02x\n", *Spi_PortOutput); */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DataReady,
        *Spi_PortOutput,
        "Expect DataReady to idle high."
        );
}
void SpiSlaveInit_makes_Miso_an_output_pin(void)
{
    /* =====[ Setup ]===== */
    *Spi_PortDirection = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_Miso,
        *Spi_PortDirection,
        "Cannot run test: must start with ddr bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Miso,
        *Spi_PortDirection,
        "Expect Miso to be an output."
        );
}
void SpiSlaveInit_enables_SPI(void)
{
    /* =====[ Setup ]===== */
    *Spi_SPCR = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_Enable,
        *Spi_SPCR,
        "Cannot run test: must start with bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Enable,
        *Spi_SPCR,
        "Expect bit 6 HIGH to enable SPI module."
        );
}
void SpiSlaveInit_enables_SPI_interrupt(void)
{
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    // check SpiSlaveInit calls `EnableSpiInterrupt`
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "EnableSpiInterrupt"),
        "Expect SpiSlaveInit enables the SPI interrupt."
        );
}

/* =====[ SpiSlaveTx ]===== */
void SpiSlaveTx_sends_nbytes_of_input_buffer_to_SpiMaster(void)
{
    /* =====[ Setup ]===== */
    uint8_t input_buffer[] = {0xA0, 0xB1, 0xC2};
    uint16_t nbytes = (uint16_t)sizeof(input_buffer);
    /* printf("sizeof(input_buffer)=%zu\n",sizeof(input_buffer)); */
    /* printf("nbytes=%d\n",nbytes); */
    /* =====[ Operate ]===== */
    SpiSlaveTx(input_buffer, nbytes);
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    uint16_t arg_n = 1; uint8_t *p_argval = input_buffer;
    uint16_t call_n = 1;
    while (call_n <= nbytes)
    {
        TEST_ASSERT_TRUE(
            AssertCall(mock, call_n, "SpiSlaveTxByte")
            );
        TEST_ASSERT_TRUE(
            AssertArg(mock, call_n++, arg_n, p_argval++)
            );
    }
}

/* =====[ SpiSlaveTxByte ]===== */
void SpiSlaveTxByte_loads_SPI_data_register_with_input_byte(void)
{
    /* =====[ Setup ]===== */
    uint8_t input_byte = 0xAB;
    /* =====[ Operate ]===== */
    SpiSlaveTxByte(input_byte);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(input_byte, *Spi_SPDR);
}
void SpiSlaveTxByte_disables_SPI_ISR_before_signaling_data_ready(void)
{
    /* =====[ Operate ]===== */
    SpiSlaveTxByte(0xFF);
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n++, "DisableSpiInterrupt"));
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "_SignalDataReady"));
}
void SpiSlaveTxByte_drives_DataReady_LOW_to_signal_data_is_ready(void)
{
    /* =====[ Setup ]===== */
    *Spi_PortOutput = 0xFF;
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DataReady,
        *Spi_PortOutput,
        "Cannot run test: must start with DataReady HIGH!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveTxByte(0xFF);
    /* =====[ Test ]===== */
    /* TEST_ASSERT_BIT_LOW(Spi_DataReady, *Spi_PortOutput); */
    uint16_t call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "_SignalDataReady"));
}
void SpiSlaveTxByte_waits_until_SPI_transfer_is_done(void)
{
    /* =====[ Setup ]===== */
    ClearBit(Spi_SPSR, Spi_InterruptFlag);
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_InterruptFlag,
        *Spi_SPSR,
        "Cannot run test: must start with bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveTxByte(0xFF);
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    uint16_t call_n = 3;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "_SpiTransferIsDone"));
    TEST_ASSERT_BIT_HIGH(Spi_InterruptFlag, *Spi_SPSR);
}
void SpiSlaveTxByte_drives_DataReady_HIGH_immediately_after_SPI_transfer_finishes(void)
{
    /* =====[ Setup ]===== */
    *Spi_PortOutput = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_DataReady,
        *Spi_PortOutput,
        "Cannot run test: must start with DataReady LOW!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveTxByte(0xFF);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_DataReady, *Spi_PortOutput);
}
void SpiSlaveTxByte_enables_SPI_ISR_after_transfer(void)
{
    /* =====[ Setup ]===== */
    ClearBit(Spi_SPCR, Spi_InterruptEnable);
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_InterruptEnable,
        *Spi_SPCR,
        "Cannot run test: must start with bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveTxByte(0xFF);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_InterruptEnable, *Spi_SPCR);
}
