```final object file linking
gcc ... build/test_runner.o
...
../lib/build/BiColorLed.o
../lib/build/UartSpi.o
../lib/build/Flag.o
../lib/build/ReadWriteBits.o
../lib/build/StatusCode.o
../lib/build/Queue.o
build/test_Example.o
build/test_VisCmd.o
build/test_AutoExpose.o
build/Example.o
build/VisCmd.o
buil <--- quickfix list cannot handle this many characters
```

```linker multiple definition errors

build/test_VisCmd.o:../lib/src/Lis.h|394| multiple definition of `exposure_ticks';
build/test_runner.o:../lib/src/Lis.h:394: first defined here

build/test_AutoExpose.o:vis-spi-out/src/AutoExpose.h|51| multiple definition of `max_tries';
build/test_VisCmd.o:vis-spi-out/src/AutoExpose.h:51: first defined here

build/test_AutoExpose.o:vis-spi-out/src/AutoExpose.h|54| multiple definition of `start_pixel';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:54: first defined here

build/test_AutoExpose.o:vis-spi-out/src/AutoExpose.h|57| multiple definition of `stop_pixel';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:57: first defined here

build/test_AutoExpose.o:vis-spi-out/src/AutoExpose.h|60| multiple definition of `target';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:60: first defined here

build/test_AutoExpose.o:vis-spi-out/src/AutoExpose.h|63| multiple definition of `target_tolerance';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:63: first defined here

build/test_AutoExpose.o:vis-spi-out/src/AutoExpose.h|66| multiple definition of `max_dark';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:66: first defined here

build/test_AutoExpose.o:vis-spi-out/src/AutoExpose.h|71| multiple definition of `min_exposure';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:71: first defined here

build/test_AutoExpose.o:vis-spi-out/src/AutoExpose.h|84| multiple definition of `max_exposure';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:84: first defined here

build/test_AutoExpose.o:vis-spi-out/../lib/src/Lis.h|394| multiple definition of `exposure_ticks';
build/test_runner.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/../lib/src/Lis.h:394: first defined here

build/test_AutoExpose.o:vis-spi-out/src/VisCmd.h|40| multiple definition of `frame';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h:40: first defined here

build/VisCmd.o:vis-spi-out/../lib/src/Lis.h|394| multiple definition of `exposure_ticks';
build/test_runner.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/../lib/src/Lis.h:394: first defined here

build/VisCmd.o:vis-spi-out/src/AutoExpose.h|51| multiple definition of `max_tries';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:51: first defined here

build/VisCmd.o:vis-spi-out/src/AutoExpose.h|54| multiple definition of `start_pixel';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:54: first defined here

build/VisCmd.o:vis-spi-out/src/AutoExpose.h|57| multiple definition of `stop_pixel';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:57: first defined here

build/VisCmd.o:vis-spi-out/src/AutoExpose.h|60| multiple definition of `target';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:60: first defined here

build/VisCmd.o:vis-spi-out/src/AutoExpose.h|63| multiple definition of `target_tolerance';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:63: first defined here

build/VisCmd.o:vis-spi-out/src/AutoExpose.h|66| multiple definition of `max_dark';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:66: first defined here

build/VisCmd.o:vis-spi-out/src/AutoExpose.h|71| multiple definition of `min_exposure';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:71: first defined here

build/VisCmd.o:vis-spi-out/src/AutoExpose.h|84| multiple definition of `max_exposure';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:84: first defined here

build/VisCmd.o:vis-spi-out/src/VisCmd.h|40| multiple definition of `frame';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h:40: first defined here

build/AutoExpose.o:vis-spi-out/src/AutoExpose.h|51| multiple definition of `max_tries';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:51: first defined here

build/AutoExpose.o:vis-spi-out/src/AutoExpose.h|54| multiple definition of `start_pixel';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:54: first defined here

build/AutoExpose.o:vis-spi-out/src/AutoExpose.h|57| multiple definition of `stop_pixel';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:57: first defined here

build/AutoExpose.o:vis-spi-out/src/AutoExpose.h|60| multiple definition of `target';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:60: first defined here

build/AutoExpose.o:vis-spi-out/src/AutoExpose.h|63| multiple definition of `target_tolerance';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:63: first defined here

build/AutoExpose.o:vis-spi-out/src/AutoExpose.h|66| multiple definition of `max_dark';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:66: first defined here

build/AutoExpose.o:vis-spi-out/src/AutoExpose.h|71| multiple definition of `min_exposure';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:71: first defined here

build/AutoExpose.o:vis-spi-out/src/AutoExpose.h|84| multiple definition of `max_exposure';
build/test_VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/AutoExpose.h:84: first defined here

../lib/build/Lis_faked.o:vis-spi-out/../lib/src/Lis.h|394| multiple definition of `exposure_ticks';
build/test_runner.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/../lib/src/Lis.h:394: first defined here
```

```linker undefined reference errors
build/VisCmd.o: in function `GetSensorLED':
src/VisCmd.h|192| undefined reference to `SpiSlaveTxByte'
src/VisCmd.h|192| (.text+0x1b9): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|193| undefined reference to `SpiSlaveTxByte'
src/VisCmd.h|193| (.text+0x1ce): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|198| undefined reference to `SpiSlaveTxByte'
src/VisCmd.h|198| (.text+0x1e4): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|199| undefined reference to `SpiSlaveTxByte'
src/VisCmd.h|199| (.text+0x1f8): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `SpiSlaveTxByte'
/usr/lib/gcc/x86_64-pc-cygwin/10/../../../../x86_64-pc-cygwin/bin/ld: build/VisCmd.o: in function `SetSensorLED':
src/VisCmd.h|231| undefined reference to `SpiSlaveTxByte'
src/VisCmd.h|231| (.text+0x285): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `SpiSlaveTxByte'
build/VisCmd.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|236| more undefined references to `SpiSlaveTxByte' follow

build/VisCmd.o: in function `SetSensorLED':
src/VisCmd.h|236| (.text+0x2b0): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `SpiSlaveTxByte'
src/VisCmd.h|257| (.text+0x34a): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `SpiSlaveTxByte'

build/VisCmd.o: in function `GetSensorConfig':
src/VisCmd.h|270| (.text+0x36d): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `SpiSlaveTxByte'
src/VisCmd.h|271| (.text+0x381): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `SpiSlaveTxByte'
src/VisCmd.h|272| (.text+0x395): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `SpiSlaveTxByte'
src/VisCmd.h|273| (.text+0x3a9): additional relocation overflows omitted from the output

build/VisCmd.o: in function `GetExposure':
src/VisCmd.h|285| undefined reference to `MSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|285| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|286| undefined reference to `LSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|286| undefined reference to `SpiSlaveTxByte'

build/VisCmd.o: in function `SetExposure':
src/VisCmd.h|316| undefined reference to `SpiSlaveTxByte'

build/VisCmd.o: in function `AutoExposure':
src/VisCmd.h|390| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|393| undefined reference to `MSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|393| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|394| undefined reference to `LSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|394| undefined reference to `SpiSlaveTxByte'

build/VisCmd.o: in function `GetAutoExposeConfig':
src/VisCmd.h|399| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|400| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|401| undefined reference to `MSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|401| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|402| undefined reference to `LSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|402| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|403| undefined reference to `MSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|403| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|404| undefined reference to `LSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|404| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|405| undefined reference to `MSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|405| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|406| undefined reference to `LSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|406| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|407| undefined reference to `MSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|407| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|408| undefined reference to `LSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|408| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|409| undefined reference to `MSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|409| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|410| undefined reference to `LSB'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|410| undefined reference to `SpiSlaveTxByte'

build/VisCmd.o: in function `SetAutoExposeConfig':
src/VisCmd.h|474| undefined reference to `SpiSlaveTxByte'
/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/src/VisCmd.h|487| undefined reference to `SpiSlaveTxByte'
build/VisCmd.o: in function `SetSensorConfig':
src/VisCmd.h|523| undefined reference to `LisConfigIsValid'
```
