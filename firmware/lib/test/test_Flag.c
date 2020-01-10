#include "unity.h"
#include "test_Flag.h"
#include "Flag.h"
#include "ReadWriteBits.h"
void test_Flag(void)
{
    /* =====[ Operate ]===== */
    SetBit(Flag_SpiFlags, Flag_SlaveRx);
}
