# ;mktgc in lib folder
for build/Lis.o, build/test_Lis.o, and build/Lis_faked.o:

```
/home/mike/chromation/dev-kit-2020/firmware/lib/src/Lis.h|394|
multiple definition of `exposure_ticks';
build/test_runner.o:/home/mike/chromation/dev-kit-2020/firmware/lib/src/Lis.h:394: first defined here
```

# ;mktgc in vis-spi-out folder
for ../lib/build/Lis.o, build/test_VisCmd.o, build/test_AutoExpose.o, build/VisCmd.o, ../lib/build/Lis_faked.o

```
../lib/src/Lis.h|394|
multiple definition of `exposure_ticks';
build/test_runner.o:/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out/../lib/src/Lis.h:394: first defined here
```

# Problem happens when linking

The problem is not in generating `test_runner.o`. It is in
linking `test_runner.o` to make `TestSuite.exe`.

*Here is the recipe:*

```
build/TestSuite.exe: build/test_runner.o ${unittest_o} \
${lib_o} ${test_app_o} ${app_o} ${lib_faked_o}
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)
```

*See what it expands to using `-n -B`:*

```
$ make -n -B build/TestSuite.exe compiler=gcc
```

First object files are made.

*For example, `Lis.o` and `Lis_faked.o`:*

```
gcc ... -c ../lib/src/Lis.c -o ../lib/build/Lis.o
gcc ... -c ../lib/test/Lis_faked.c -o ../lib/build/Lis_faked.o
```

*I'm abbreviating the output with `...` and only showing the
relevant bits.*

And then all the object files are linked with `test_runner.o` to
make `TestSuite.exe`.

*For example, `Lis.o` and `Lis_faked.o` get linked with
`test_runner.o`:*

```
gcc ... build/test_runner.o ... ../lib/build/Lis.o ... ../lib/build/Lis_faked.o
-o build/TestSuite.exe ...
```

Compare this with how the `.elf` is built.

*Expand the recipe with `-n -B`:*

```
$ make -n -B build/vis-spi-out.elf

avr-gcc ... build/vis-spi-out.o ... ../lib/build/Lis.o ...
-o build/vis-spi-out.elf ...
```

I suspect linking `Lis.o` and `Lis_faked.o` is never supposed to
happen. `Lis.o` probably contain hardware values since there is
not other `Lis` file being linked to make the `.elf`. And
`Lis_faked.o` probably contains fake hardware values. Hardware
values are defined twice, hence the linker error.

Now take a closer look at `Lis` and `Lis_faked`.

- `lib/src/Lis.h` defines `exposure_ticks`
- `DUSE_FAKES` defines macro `USE_FAKES`
    - it doesn't define it to be anything in particular
    - the "definition" is just to make `#ifdef USE_FAKES` true
- `Lis.h` includes `Lis_faked.h`, that's OK
- Compare `Lis.c` and `Lis_faked.c`
    - `Lis.c` includes `Lis.h`, of course
    - `Lis_faked.c` includes `Lis.h`
    - there's the problem
    - it's OK for them to both include `Lis.h` if they are both
      in the same compilation unit -- in that scenario, `Lis.h`
      is only included once
    - but if they are compiled separately and then linked, they
      each have their own definition of variables defined in
      `Lis.h`, variables like `exposure_ticks`
- So now the question is why am I linking `Lis.o` and
  `Lis_faked.o` to build the unit tests?

Here's the `TestSuite.exe` recipe again:

```
build/TestSuite.exe: build/test_runner.o ${unittest_o} \
${lib_o} ${test_app_o} ${app_o} ${lib_faked_o}
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)
```

Probably `lib_o` and `lib_faked_o` are in conflict. `lib_o`
probably contains `Lis.o` and `lib_faked_o` probably contains
`Lis_faked.o`. But I can't just get rid of `lib_o` because it
probably contains libs that are not faked.

*Run `:make print-vars` to see the libs in variables `lib_o` and
`lib_faked_o`. Mark any libs that appear in both as `CONFLICT`*:

```
...
lib_o:
- ../lib/build/BiColorLed.o
- ../lib/build/Spi.o <---------------- CONFLICT
- ../lib/build/UartSpi.o
- ../lib/build/Lis.o <---------------- CONFLICT
- ../lib/build/Flag.o
- ../lib/build/ReadWriteBits.o
- ../lib/build/StatusCode.o
- ../lib/build/Queue.o
- ../lib/build/SpiSlave.o <----------- CONFLICT

lib_faked_o:
- ../lib/build/Spi_faked.o
- ../lib/build/SpiSlave_faked.o
- ../lib/build/Lis_faked.o

...
```

It is not as simple as removing these conflicts.

I hard-coded `lib_o` like this:

```
build/TestSuite.exe: build/test_runner.o ${unittest_o} \
../lib/build/BiColorLed.o ../lib/build/UartSpi.o ../lib/build/Flag.o ../lib/build/ReadWriteBits.o ../lib/build/StatusCode.o ../lib/build/Queue.o \
${test_app_o} \
${app_o} \
${lib_faked_o}
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)
```

And that removes the conflicts:

```
gcc ... \

build/test_runner.o ... \

../lib/build/BiColorLed.o ../lib/build/UartSpi.o ../lib/build/Flag.o ../lib/build/ReadWriteBits.o ../lib/build/StatusCode.o ../lib/build/Queue.o \

build/test_Example.o build/test_VisCmd.o build/test_AutoExpose.o \

build/Example.o build/VisCmd.o build/AutoExpose.o \

../lib/build/Spi_faked.o ../lib/build/SpiSlave_faked.o ../lib/build/Lis_faked.o \

-o build/TestSuite.exe ...
```

But I still get linker errors:

```final object file linking (no errors yet)
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
buil <--- (quickfix list cannot handle this many characters)
```

The errors start here. There are "multiple definition" errors and
"undefined reference" errors.

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

So there are other conflicts.

The multiple definition errors involve variables defined in `Lis`
being claimed by these four object files:

- `build/VisCmd.o`
- `build/AutoExpose.o`
- `build/test_VisCmd.o`
- `build/test_AutoExpose.o`

Look at the variables again:

```print-vars
lib_o:
- ../lib/build/BiColorLed.o
- ../lib/build/Spi.o
- ../lib/build/UartSpi.o
- ../lib/build/Lis.o
- ../lib/build/Flag.o
- ../lib/build/ReadWriteBits.o
- ../lib/build/StatusCode.o
- ../lib/build/Queue.o
- ../lib/build/SpiSlave.o

lib_faked_o:
- ../lib/build/Spi_faked.o
- ../lib/build/SpiSlave_faked.o
- ../lib/build/Lis_faked.o

test_app_o:
- build/test_Example.o
- build/test_VisCmd.o
- build/test_AutoExpose.o

app_o:
- build/Example.o
- build/VisCmd.o
- build/AutoExpose.o
```

Ignore `Example`. It's basically empty.

Every lib in `test_app_o` and `app_o` is problematic.
