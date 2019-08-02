# WIPs from git log
WIPs are listed starting with the most recent and going back to March 1st, 2019.
If the WIP was done, I still list it but I cross it out.

- WIP: implement `SensorCfgLis` in lib `Spi` and add status reporting to
  `LisWriteCfg` in lib `Lis`.
- WIP: Add `BridgeCfgLis` call to `SendSensorCommand`. Extend this to send a
  `uint8_t *` arg after the command.
- WIP: ~pass AVR macros to `DebugLed` lib at compile time.~
- WIP: remove function pointer seams from low-level calls to reduce code size
  and speed execution. This prevents mocking. Clean the associated mocks out
  from the unit tests.
- WIP: multi-byte SPI communication on mBrd, then resume CmdCfgLis pushes cfg to
  mBrd and pulls updated cfg.
- WIP: CmdCfgLis pushes cfg to mBrd and pulls updated cfg.
- ~WIP: `CfgBytesAreValid` checks against all 255 valid cfgs.~
- ~WIP: LisWriteCfg.~
- WIP: command parser

# 2019-03-08
- WIP: implement `SensorCfgLis` in lib `Spi` and add status reporting to
  `LisWriteCfg` in lib `Lis`.
- WIP: Add `BridgeCfgLis` call to `SendSensorCommand`. Extend this to send a
  `uint8_t *` arg after the command.

# 2019-03-08
- Bridge pushes all data from Sensor to USB Host, including the 2-byte header
  from the sensor stating how many bytes it is sending.

# 2019-03-07
- Standardize device names in communication protocol to match USB Host
  perspective: `bridge` and `sensor`. Replace `mBrd` lib `DebugLeds` with lib
  `BiColorLed`.

# 2019-03-05
- Propagate replacement of lib `DebugLed` with lib `BiColorLed` to test suite.

# 2019-03-05
- Rename lib `DebugLed` to `BiColorLed` and simplify API. Pass AVR macros to
  `BiColorLed` lib at compile time. Propagate LED API change in `simBrd` and in
  libs `Usb` and `Ft1248`. WIP: propagate LED API change to test suite.

# 2019-03-05
- Implement *correct* use of inline in lib `ReadWriteBits`. Compiles to intended
  `sbi` and `cbi` even with `-Os`. Exception is in `DebugLed` lib where AVR
  macros are passed at runtime. `ReadWriteBits` functions compile with `jump`
  and `return` if inline call is made via a function pointer. Delete macro
  versions of this lib across the whole project.
- WIP: pass AVR macros to `DebugLed` lib at compile time.
- WIP: remove function pointer seams from low-level calls to reduce code size
  and speed execution. This prevents mocking. Clean the associated mocks out
  from the unit tests.
- WIP: WIPs from previous commit.

# 2019-03-03
- Create Doxyfile. LisWriteCfg programs the LIS.
- WIP: multi-byte SPI communication on mBrd, then resume CmdCfgLis pushes cfg to
  mBrd and pulls updated cfg.

# 2019-03-03
- CfgBytesAreValid returns true for all 255 valid configs. WIP: CmdCfgLis pushes
  cfg to mBrd and pulls updated cfg.

# 2019-03-02
- `LisWriteCfg` works. `CfgBytesAreValid` checks against one valid cfg. WIP:
  `CfgBytesAreValid` checks against all 255 valid cfgs.

# 2019-03-01
- WIP: LisWriteCfg.

# 2019-03-01
- Fix bug: usb hangs on python back-to-back commands.
- WIP: command parser.
