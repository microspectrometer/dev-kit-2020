commit 7295c7815a3bd030f7988896db07919e1918cfad
Author: sustainablelab <sustainablelab@gmail.com>
Date:   Tue Mar 5 12:21:44 2019 -0500

    Implement *correct* use of inline in lib `ReadWriteBits`. Compiles to intended `sbi` and `cbi` even with `-Os`. Exception is in `DebugLed` lib where AVR macros are passed at runtime. `ReadWriteBits` functions compile with `jump` and `return` if inline call is made via a function pointer. Delete macro versions of this lib across the whole project. WIP: pass AVR macros to `DebugLed` lib at compile time. WIP: remove function pointer seams from low-level calls to reduce code size and speed execution. This prevents mocking. Clean the associated mocks out from the unit tests. WIP: WIPs from previous commit.
