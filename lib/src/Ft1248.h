#ifndef _FT1248_H
#define _FT1248_H

#include <stdint.h>   // uint8_t
#include <stdbool.h>  // bool, true, false

//=====[ Ft1248-Hardware ]=====
// Clients of lib Ft1248 define the following symbols.
// control lines
extern uint8_t volatile * const Ft1248_ddr;
extern uint8_t volatile * const Ft1248_port;
extern uint8_t volatile * const Ft1248_pin;
// bi-directional data lines (driven by both master and slave)
extern uint8_t volatile * const FtMiosio_ddr;
extern uint8_t volatile * const FtMiosio_port;
extern uint8_t volatile * const FtMiosio_pin;
// control lines
extern uint8_t const Ft1248_Ss;
extern uint8_t const Ft1248_Sck;
extern uint8_t const Ft1248_Miso;
// bi-directional data lines (driven by both master and slave)
extern uint8_t const FtMiosio0;
extern uint8_t const FtMiosio1;
extern uint8_t const FtMiosio2;
extern uint8_t const FtMiosio3;
extern uint8_t const FtMiosio4;
extern uint8_t const FtMiosio5;
extern uint8_t const FtMiosio6;
extern uint8_t const FtMiosio7;

//=====[ Ft1248 API ]=====
//uint8_t const FtNbitCmd_8BitWide_Read  = 0xC6;
//extern void (*FtSendCommand)(uint8_t FtNbitCmd);
//=====[ Low-level API ]=====
extern void (*FtOutputCmdOnMiosio)(uint8_t FtCmd);
extern void (*FtActivateInterface)(void);
extern void (*FtPushData)(void);
extern void (*FtOutputByte)(uint8_t);
extern void (*FtPullData)(void);
extern void (*FtLetMasterDriveBus)(void);
extern void (*FtLetSlaveDriveBus)(void);
extern bool (*FtIsBusOk)(void);
extern bool (*FtHasDataToRead)(void);
extern uint8_t (*FtReadData)(void);
extern void (*FtWriteData)(uint8_t);
extern void (*FtDeactivateInterface)(void);
extern bool (*FtHasRoomToWrite)(void);
//=====[ High-level API ]=====
extern uint8_t const FtCmd_Read;
extern uint8_t const FtCmd_Write;
extern void (*FtSendCommand)(uint8_t FtCmd);
extern bool (*FtBusTurnaround)(void);
extern bool (*FtRead)(uint8_t *read_buffer);
extern bool (*FtWrite)(uint8_t *write_buffer);

#endif // _FT1248_H
