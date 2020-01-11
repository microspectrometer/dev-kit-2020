#include "unity.h"
#include "Mock.h" // record call history in "mock"
#include "test_SpiSlave.h"
#include "SpiSlave.h"
#include "ReadWriteBits.h"

/* =====[ Check_SpiSlave_plumbing_for_fakes ]===== */
void SpiSlave_faked_calls_are_still_available_for_testing(void)
{
    printf("SpiSlave_faked_calls_are_still_available_for_testing:\n");
    /* =====[ Operate and Test]===== */
    EnableSpiInterrupt();
    printf(
        "- Able to test real version of `EnableSpiInterrupt`.\n"
        );
    /* =====[ Operate and Test ]===== */
    SpiSlaveInit();
    printf(
        "- And calling `SpiSlaveInit` calls "
        "`EnableSpiInterrupt_fake`:\n"
        );
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ClearSpiInterruptFlag"),
        "Expect SpiSlaveInit calls fake which records call name."
        );
}

/* =====[ _SignalDataReady ]===== */
void SignalDataReady_drives_DataReady_LOW(void)
{
    /* =====[ Setup ]===== */
    SetBit(Spi_port, Spi_DataReady);
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DataReady,
        *Spi_port,
        "Cannot run test: must start with DataReady HIGH!"
        );
    /* =====[ Operate ]===== */
    _SignalDataReady();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Spi_DataReady, *Spi_port);
}

/* =====[ _TransferIsDone ]===== */
void TransferIsDone_returns_true_when_ISR_sets_Flag_TransferIsDone(void)
{
    /* =====[ Setup ]===== */
    SetBit(Flag_SpiFlags, Flag_TransferDone);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(_TransferIsDone());
    /* TEST_FAIL_MESSAGE("Implement test."); */
}
void TransferIsDone_returns_false_until_ISR_sets_Flag_TransferIsDone(void)
{
    /* =====[ Setup ]===== */
    ClearBit(Flag_SpiFlags, Flag_TransferDone);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(_TransferIsDone());
}

/* =====[ SpiSlaveInit ]===== */
void SpiSlaveInit_makes_DataReady_an_output_pin(void)
{
    /* =====[ Setup ]===== */
    *Spi_ddr = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_DataReady,
        *Spi_ddr,
        "Cannot run test: must start with ddr bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DataReady,
        *Spi_ddr,
        "Expect DataReady to be an output."
        );
}
void SpiSlaveInit_idles_DataReady_high(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_DataReady,
        *Spi_port,
        "Cannot run test: must start with port bit clear!"
        );
    /* =====[ Operate ]===== */
    /* printf("Value of Spi_port before is 0x%02x\n", *Spi_port); */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    /* printf("Value of Spi_DataReady is pin %d\n", Spi_DataReady); */
    /* printf("Value of Spi_port after is 0x%02x\n", *Spi_port); */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DataReady,
        *Spi_port,
        "Expect DataReady to idle high."
        );
}
void SpiSlaveInit_makes_Miso_an_output_pin(void)
{
    /* =====[ Setup ]===== */
    *Spi_ddr = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_Miso,
        *Spi_ddr,
        "Cannot run test: must start with ddr bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Miso,
        *Spi_ddr,
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
void SpiSlaveTxByte_tells_SPI_ISR_to_ignore_rx_byte(void)
{
    /* =====[ Setup ]===== */
    SetBit(Flag_SpiFlags,Flag_SlaveRx);
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Flag_SlaveRx,
        *Flag_SpiFlags,
        "Cannot run test: must start with bit set!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveTxByte(0xFF);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Flag_SlaveRx, *Flag_SpiFlags);
}
void SpiSlaveTxByte_drives_DataReady_LOW_to_signal_data_is_ready(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0xFF;
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DataReady,
        *Spi_port,
        "Cannot run test: must start with DataReady HIGH!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveTxByte(0xFF);
    /* =====[ Test ]===== */
    /* TEST_ASSERT_BIT_LOW(Spi_DataReady, *Spi_port); */
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "_SignalDataReady"));
}
void SpiSlaveTxByte_waits_until_SPI_transfer_is_done(void)
{
    /* =====[ Setup ]===== */
    ClearBit(Flag_SpiFlags,Flag_TransferDone);
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Flag_TransferDone,
        *Flag_SpiFlags,
        "Cannot run test: must start with bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveTxByte(0xFF);
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    uint16_t call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "_TransferIsDone"));
    TEST_ASSERT_BIT_HIGH(Flag_TransferDone, *Flag_SpiFlags);
}
void SpiSlaveTxByte_drives_DataReady_HIGH_to_sync_with_Master(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_DataReady,
        *Spi_port,
        "Cannot run test: must start with DataReady LOW!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveTxByte(0xFF);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_DataReady, *Spi_port);
}

