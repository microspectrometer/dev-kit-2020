commit 7295c78
Date:   2 hours ago

    Implement *correct* use of inline in lib `ReadWriteBits`. Compiles to
    intended `sbi` and `cbi` even with `-Os`. Exception is in `DebugLed` lib
    where AVR macros are passed at runtime. `ReadWriteBits` functions compile
    with `jump` and `return` if inline call is made via a function pointer.
    Delete macro versions of this lib across the whole project.
    WIP: pass AVR macros to `DebugLed` lib at compile time. WIP: remove function pointer seams
    from low-level calls to reduce code size and speed execution. This prevents
    mocking. Clean the associated mocks out from the unit tests. WIP: WIPs from
    previous commit.


commit d3073aa
Date:   2 days ago

    Create Doxyfile. LisWriteCfg programs the LIS. WIP: multi-byte SPI
    communication on mBrd, then resume CmdCfgLis pushes cfg to mBrd and pulls
    updated cfg.

commit 78eb283
Date:   3 days ago

    CfgBytesAreValid returns true for all 255 valid configs. WIP: CmdCfgLis
    pushes cfg to mBrd and pulls updated cfg.

commit 34f9821
Date:   3 days ago

    `LisWriteCfg` works. `CfgBytesAreValid` checks against one valid cfg. WIP:
    `CfgBytesAreValid` checks against all 255 valid cfgs.

commit ff344b5
Date:   4 days ago

    WIP: LisWriteCfg.
