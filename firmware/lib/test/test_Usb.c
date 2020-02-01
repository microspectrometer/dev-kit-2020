#include "unity.h" // unit test framework
#include "Mock.h" // function faking framework
#include "test_Usb.h"
#include "Usb.h"
#include "ReadWriteBits.h"

/* =====[ UsbRxbufferIsEmpty ]===== */
void UsbRxbufferIsEmpty_returns_true_if_pin_FT1248_MISO_is_HIGH(void)
{
    /* =====[ Setup ]===== */
    SetBit(FtCtrl_pin, FtMiso);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(UsbRxbufferIsEmpty());
}
void UsbRxbufferIsEmpty_returns_false_if_pin_FT1248_MISO_is_LOW(void)
{
    /* =====[ Setup ]===== */
    ClearBit(FtCtrl_pin, FtMiso);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(UsbRxbufferIsEmpty());
}

/* =====[ UsbRxbufferIsFull ]===== */
void UsbRxbufferIsFull_returns_false_if_pin_MIOSIO0_is_HIGH(void)
{
    /* =====[ Setup ]===== */
    SetBit(FtData_pin, FtMiosio0);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(UsbRxbufferIsFull());
}
void UsbRxbufferIsFull_returns_true_if_pin_MIOSIO0_is_LOW(void)
{
    /* =====[ Setup ]===== */
    ClearBit(FtData_pin, FtMiosio0);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(UsbRxbufferIsFull());
}
