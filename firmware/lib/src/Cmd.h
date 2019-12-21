/** \file
 * Commands are described in doc "USBProtocol": a table showing the
 * expected payload and responses (if any) for each command.
 *
 *  Each command is one byte.
 *  Firmware source code refers to this command byte as the `key`.
 *
 *  The user issues a command by sending the command byte (`key`) to
 *  the firmware. The firmware looks up the command `key` in a *jump
 *  table* and returns the function that performs that command. The
 *  *jump table* is just an **array of function pointers**, one
 *  function for each command in "USBProtocol".
 *
 *  Firmware source code names both the **key** and the **function**
 *  with the command's name:\n 
 *    - the **key** name is the command name with a `_key` suffix
 *    - the **function** name is simply the command name
 * */
#ifndef _CMD_H
#define _CMD_H

//! Keys in the command lookup table are type `cmd`.
typedef uint8_t cmd;
//! Functions in the command lookup table are type `Cmd`.
typedef void (Cmd)(void);

#endif // _CMD_H
