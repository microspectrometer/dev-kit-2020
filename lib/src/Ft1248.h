#ifndef _FT1248_H
#define _FT1248_H

#include <stdint.h> // uint8_t

//uint8_t const FtNbitCmd_8BitWide_Read  = 0xC6;
//extern void (*FtSendCommand)(uint8_t FtNbitCmd);
extern uint8_t const FtCmd_Read;
void FtSendCommand(uint8_t FtCmd);
extern void (*FtOutputCmdOnMiosio)(uint8_t FtCmd);

#endif // _FT1248_H
