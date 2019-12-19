/** \file */
#ifndef _CMD_H
#define _CMD_H

//! Functions in the command look up table are type `Cmd`.
typedef void (Cmd)(void);
//! Keys in the command look up table are type `cmd`.
typedef uint8_t cmd;

#endif // _CMD_H
