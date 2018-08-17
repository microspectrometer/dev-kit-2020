#include "Lis.h"
#include "ReadWriteBits.h"

static void SetPixSelectAsOutput(void)
{
    SetBit(Lis_ddr2, Lis_PixSelect);
}
static void SetClkAsOutput(void)
{
    SetBit(Lis_ddr1, Lis_Clk);
}
static void SetRstAsOutput(void)
{
    SetBit(Lis_ddr1, Lis_Rst);
}
void LisInit(void)
{
    SetPixSelectAsOutput();
    SetClkAsOutput();
    SetRstAsOutput();
}
