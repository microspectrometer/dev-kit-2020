// Why doesn't this output any assembly?!?!?
// $ gcc src/avra/VisCmd-ReplyCommandInvalid.c -S -I../lib/src -I../lib/test/ -o -
//         .file   "VisCmd-ReplyCommandInvalid.c"
//         .text
//         .ident  "GCC: (GNU) 10.2.0"

#include "FakeAvr/interrupt.h"
#include "SpiSlave.h"
#include "StatusCode.h"
#ifdef USE_FAKES
#define SpiSlaveTxByte SpiSlaveTxByte_fake
#endif
inline void ReplyCommandInvalid(void)
{
    /** ReplyCommandInvalid behavior:\n 
      * - transmits one byte over SPI\n 
      * - sends byte INVALID CMD\n 
      * */
    SpiSlaveTxByte(INVALID_CMD);
}
#ifdef USE_FAKES
#undef SpiSlaveTxByte
#endif

