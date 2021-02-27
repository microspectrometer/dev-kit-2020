Look at expanded recipes. Break lines for readability.

# When avr-gcc builds target `test_runner.o`:

```bash
$ make -n -B build/test_runner.o

avr-gcc
-Isrc
-I../lib/src
-includeavr/interrupt.h
-includeavr/io.h
-g -Wall -Wextra -pedantic -Winline -Wno-pointer-sign -O3
-ffunction-sections -fdata-sections -fshort-enums -mmcu=atmega328p
-c test/test_runner.c -o build/test_runner.o
```

# When avr-gcc builds target `vis-spi-out.o`:

```bash
$ make -n -B build/vis-spi-out.o

avr-gcc
-Isrc
-I../lib/src
-includeavr/interrupt.h
-includeavr/io.h
-g -Wall -Wextra -pedantic -Winline -Wno-pointer-sign -O3
-ffunction-sections -fdata-sections -fshort-enums -mmcu=atmega328p
-c src/vis-spi-out.c -o build/vis-spi-out.o
```

# When gcc builds target `test_runner.o`:

```bash
$ make -n -B build/test_runner.o compiler=gcc

gcc
-I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
-DUSE_FAKES
-I/cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/include
-I/cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/test/unity

-Isrc
-I../lib/src
-Itest
-I../lib/test
-include../lib/test/FakeAvr/interrupt.h
-include../lib/test/FakeAvr/io.h
-g -Wall -Wextra -pedantic -Winline
-c test/test_runner.c -o build/test_runner.o
```

## -I flags are not the problem

- what does the `-I` flag do exactly?
    - `--include-dir`
    - include directories to search

*From the docs:*

> '-I DIR'
>
> '--include-dir=DIR'
>
>      Specifies a directory DIR to search for included
>      makefiles.  *Note Including Other Makefiles: Include.
>
>      If several '-I' options are used to specify several
>      directories, the directories are searched in the order
>      specified.

Yep, documentation sucks. `-I` is used for including directories
to find source files, not makefiles.

All the `-I` flags come from my `CFLAGS` variable. I have a
conditional that defines `CFLAGS` based on the compiler:

- `CFLAGS_for_avr`
- `CFLAGS_for_tests`

*That is why/how the compiler flags are different for unit-tests
and avr builds. The different `-I` flags are intentional.*

## Move this make trivia to my C tutorial
- `=` is recursively expanded variable
    - e.g., this is an error:
    - `CFLAGS = $(CFLAGS) -O` 
- `:=` is simply expanded variable
    - e.g., change `=` to `:=` and now this is OK:
    - `CFLAGS := $(CFLAGS) -O` 
