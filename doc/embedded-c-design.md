- [x] want to see definition of variables in a lib optimized correctly
    - BiColorLed is a lib
    - it uses a register and pins
    - these are variables that the compiler needs to know the value of
      when compiling to use the optimal instructions
- well, that was useful but it did not prove what I need in general
    - BiColorLed is not its own translation unit
    - I do compile it separately, but:
    - all the function definitions are in the .h file
    - so the application includes those definitions and only a single
      translation unit is required
    - i.e., I could eliminate BiColorLed.o and it would make no
      difference to the linker when making `vis-spi-out.elf`
- [x] clean Makefile for lib project
    - clean out test runner
- [x] write a test
    - test needs its own fake hardware definitions
- [x] go back to avr-target
    - [x] check `sbi` is still used
    - [x] use SetBit function and check `sbi` is still used
    - [x] same for ClearBit function and check `cbi`
- [x] set read-only C files to use `goodwolf` scheme so I don't get mixed up
- [x] make background darker in `badwolf`
- [x] quick text color changes:
    - ;coPF ;coGF ;coWF
    - (switch buffer to change back)
- [x] create a lib that has function definitions in its `.c`
    - use `Spi` because next up in vis-spi-out setup() is
      `SpiSlaveInit`
    - this lib has its own translation unit
    - the resulting .o file is needed by the linker
    - the compiler needs to know the values to use when it makes
      the translation unit
    - this is where I decide how to proceed for testing
    - so far I can test what I need without fakes
    - the need for fakes and the hiding of values forces me to
      work out exactly how to use macros for faking depending on
      the target
    - [x] first make the function inline in the header to get
      everything working as usual with the unit tests and with
      the assembly output
    - [x] now move the function body into the .c
        - I think the trick will be to use macros that include
          the header in the lib file
        - the macros will get path info from the build target
        - and this means removing the include from the
          `Hardware.h` file
        - no, I did it with a simple `-include` flag
    - [ ] add `ifeq endif`
        - see if you can make the SpiSlave.o build to the same
          location instead of having two separate build targets
        - can I get back to a clean division of libraries into
          space-separated lists without a rule for each library
    - [ ] finish writing SpiSlaveInit
    - [ ] split common Spi stuff from SpiSlave into a Spi lib

# keyword const is required for avr-gcc optimal assembly 
- avr-gcc needs a `const` variable passed to a function to use the
  optimal assembly for that function
- it is not sufficient for the function definition to declare a
  `const` input argument
- the variable passed must be `const` as well

## const example
- example: `BiColorLedOn(led_0)` takes variable `led_0`
- if `led_0` is not `const`, then avr-gcc does not know at compile
  time if `led_0` will change value, so it does not know if it is OK
  to use the `sbi` instruction

## const example details
- here is the function definition showing the arg is `const`:
```c
// BiColorLed.h
#include <avr/io.h> // includes iom328p.h for hardware i/o values
uint8_t volatile * const BiColorLed_ddr = &DDRC; // controls if input or output
inline void BiColorLedOn(uint8_t const led_name)
{
    BiColorLed_ddr |= (uint8_t)(1<<led_name);
}
```
- `main` calls the function:
```c
// main.c
void setup(void);
int main()
{
    setup();
}
void setup(void)
{
    BiColorLedOn(led_0);
}

```
- `main` defines global `led_0`
- if `led_0` is `const`:
```c
// main.c
uint8_t const led_0 = 0;
```
- we get the desired assembly:
```asm
00000096 <main>:
    *BiColorLed_ddr |= (uint8_t)(1<<led_name);
  96:	38 9a       	sbi	0x07, 0	; 7
{
    BiColorLedOn(led_0);
}
```
- if `led_0` is not `const`:
```c
// main.c
uint8_t led_0 = 0;
```
- there is a lot of overhead in the assembly:
```asm
000000a6 <main>:
    *BiColorLed_ddr |= (uint8_t)(1<<led_name);
  a6:	27 b1       	in	r18, 0x07	; 7
  a8:	81 e0       	ldi	r24, 0x01	; 1
  aa:	90 e0       	ldi	r25, 0x00	; 0
  ac:	00 90 00 01 	lds	r0, 0x0100	; 0x800100 <__data_end>
  b0:	02 c0       	rjmp	.+4      	; 0xb6 <main+0x10>
  b2:	88 0f       	add	r24, r24
  b4:	99 1f       	adc	r25, r25
  b6:	0a 94       	dec	r0
  b8:	e2 f7       	brpl	.-8      	; 0xb2 <main+0xc>
  ba:	82 2b       	or	r24, r18
  bc:	87 b9       	out	0x07, r24	; 7
{
    BiColorLedOn(led_0);
}
```

## const typedef recommendation
- use `typedef` instead of remembering `const`
- put `typedef` in library header
- match type name with library name

## const typedef recommendation example

- define the type in the library header:

```c
// lib/src/BiColorLed.h
typedef uint8_t const bicolorled_num; // i/o bit/pin number
typedef uint8_t volatile * const bicolorled_ptr; // i/o reg address
// I/O register definitions are in Hardware.h
// This makes hardware values depend on make target
extern bicolorled_ptr BiColorLed_ddr;
```

- define the variable using the type:

```c
// vis-spi-out/src/BiColorLed-Hardware.h
#include "BiColorLed.h" // CFLAG -I../lib/src/ finds this header

bicolorled_num led_0 = PINC0;
bicolorled_ptr BiColorLed_ddr = &DDRC; // controls if input or output
```

- `Hardware.h` pulls all hardware definitions into one stream:

```c
// vis-spi-out/src/Hardware.h
#include "BiColorLed-Hardware.h"
```

- main file includes `Hardware.h` which includes the lib hardware
  definitions and each of those includes its respective lib header
- therefore translation unit main.o has all of the hardware
  definitions when it builds

```c
// vis-spi-out/src/vis-spi-out.c (the main.c)
#include "Hardware.h"
// now when main.c uses `led_0` and `BiColorLed_ddr`
// `avr-gcc` translates from C to assembly using `sbi`
// because the definitions:
//      are `const`
//      and are in a single translation unit
```

# avr-gcc might automatically inlines functions if they only call inline functions
- avr-gcc tries to inline a function that calls inline functions,
  even if the function is not inline or static
- if the function has many calls, `avr-gcc` may decide not to
  inline it
- in this case, making the function `static` makes `avr-gcc`
  inline it again

## example
- example: `setup` only calls `inline` functions, so the call to
  `setup` is inlined even without the `inline` keyword

## example details
- `main` calls `setup` and `loop`
- `setup` is inlined but `loop` is not
- it is because:
    - `setup` only calls inline functions
    - and `loop` calls one function that is *not* inline
- it is *not* because:
    - `setup` is `static`
    - `loop` is called in a `while` loop
- however, after adding more inline calls in `setup`, `avr-gcc` turned `setup`
  into a function call
- now making setup `static` caused `avr-gcc` to inline it again

```c
// main.c
static void setup(void);
static void loop(void);
int main()
{
    setup();
    while(1) loop();
}
void setup(void)
{
    example_inline_function();
}
void loop(void)
{
    example_function();
    example_inline_function();
}
```

- symbol `setup` does not appear in the assembly
- symbol `loop` appears in the assembly

```asm
00000096 <main>:
    example_inline_function();
}
void loop(void)
{
    example_function();
  96:	0e 94 4e 00 	call	0x9c	; 0x9c <example_function>
  9a:	fd cf       	rjmp	.-6      	; 0x96 <main>

0000009c <example_function>:
#include "Example.h"
void example_function(void) {}
  9c:	08 95       	ret
```

## avr-gcc outputs short assembly if all definitions are in headers
- the main file calls an inline function
- the inline function is in a lib header
- the inline function sets a bit in an io register
- the main file includes headers that define the bit and the io
  register with const values
- result: the main.c translation unit translates the inline call
  as an `sbi` instruction

### details
- this is the *easy* case where all definitions are in headers:
    - the inline function body is in a header
        - `lib/src/BiColorLed.h`
    - the hardware register and pin values are in a header
        - `vis-spi-out/src/BiColorLed-Hardware.h`
    - both headers are included by the main file
        - `BiColorLed.h` is included directly
        - `BiColorLed-Hardware.h` is included indirectly by
          including `Hardware.h` which includes all the hardware
          headers
    - so the translation unit `main.c` contains all the
      information needed to pick the `sbi` instruction for
      `main.o`

### example
- here is the main file:

```c
// vis-spi-out/src/vis-spi-out.c (main.c)
//
#include "BiColorLed.h" // hardware const variables are extern
#include "Hardware.h" // includes headers that const define hardware

void setup(void);
int main()
{
    setup();
}
void setup(void)
{
    BiColorLedOn(led_0);
}
```

- the `Hardware` header:

```c
// vis-spi-out/src/Hardware.h
//
#include "BiColorLed-Hardware.h"
```

- the hardware definitions for BiColorLed on the `vis-spi-out`
  board:

```c
// vis-spi-out/src/BiColorLed-Hardware.h
//
#include "BiColorLed.h"

bicolorled_ptr BiColorLed_ddr = &DDRC; // controls if input or output
bicolorled_num led_0 = PINC0;
```

- the definition of `BiColorLedOn` called by main:

```c
// lib/src/BiColorLed.h
//
#include "ReadWriteBits.h"

typedef uint8_t const bicolorled_num; // i/o bit/pin number
typedef uint8_t volatile * const bicolorled_ptr; // i/o reg address

// I/O register definitions in Hardware.h for dependency on make target
extern bicolorled_ptr BiColorLed_ddr;

inline void BiColorLedOn(bicolorled_num led)
{
    /** BiColorLedOn behavior:\n 
      * - sets bit in ddr\n 
      * */
    SetBit(BiColorLed_ddr, led);
}
```

- and the definition of `SetBit` called by `BiColorLedOn`:

```c
// lib/src/ReadWriteBits.h
//
typedef uint8_t volatile * const register_address;
typedef uint8_t const bit_index;
inline void SetBit(register_address reg_addr, bit_index bit)
{
    *reg_addr |= 1<<bit;
}
```

- and the final translated assembly output is still the
  one-line `sbi` instruction:

```assembly
00000096 <main>:

typedef uint8_t volatile * const register_address;
typedef uint8_t const bit_index;
inline void SetBit(register_address reg_addr, bit_index bit)
{
    *reg_addr |= 1<<bit;
  96:	38 9a       	sbi	0x07, 0	; 7
{
    BiColorLedOn(led_0);
}
```

# Makefile

## target rules

### syntax

- target rule syntax is:

```make
target: dependency
	command
```

### target

- `target` is usually the file to be created
- example: `build/test_runner.o` is the target and it is an
  object file created by the compiler

```make
build/test_runner.o: test/test_runner.c test/${HardwareFake} test/HardwareFake.h
	${compiler} $(CFLAGS) -c $< -o $@
```

### target without a command

- without a `command`, the `target` just lists prererequisites,
  so it becomes a way of identifying one or more targets under
  one name
- example:
    - run `make` at the command line with`$ make unit-test`
    - this looks for the `unit-test` target in the Makefile
    - the Makefile says `unit-test` depends on
      `build/TestSuite-results.md`
    - so `make` then looks for a rule to build
      `build/TestSuite-results.md`
    - the specific file name `build/TestSuite-results.md` does
      not show up in the command line invocation of `make`

```make
unit-test: build/TestSuite-results.md
```

### clean target

- similarly, a `target` without a `command` is a way of
  identifying special targets that do not result in creating
  files
- this uses the `make` Built-In Target Name `.PHONY`
- example:
    - `clean` is not a file but it is still a target
    - run `make` at the command line with `$ make clean`
    - the Makefile rule for `clean` decides which particular
      files are deleted 
    - `clean` is a pre-requisite of `.PHONY`
    - this means `$ make clean` runs regardless of whether a file
      named `clean` exists

```make
.PHONY: clean
clean:
	rm -f build/TestSuite-results.md
	rm -f build/TestSuite.exe
	rm -f build/test_runner.o
	rm -f ${lib_build_src}
	rm -f ${lib_build_test}
```

### dependency
- `dependency` is an optional space-separated list of
  dependencies
    - make rebuilds the target if the timestamp on the target is
      older than the timestamp on any of the dependencies
    - make refuses to build the target if any depenendency does
      not exist
        - this is a good way to drive creation of source files
          and all other required files:
        - add the name to the appropriate list in the Makefile
        - rebuild
        - the build fails because of a missing file:
            - source file
            - source file header
            - tests for source file
            - test header that lists the tests
            - hardware file the application depends on
            - hardware-fake file the test-runner depends on

### command
- `command` is an optional list of commands, each one prefaced by
  a `tab` character
    - if there is no command, then the `target: dependency`
      syntax is a way to list the prerequisites of the target
    - an example of such a list is `.PHONY: clean`

## pattern rules work on a list of file names
- *target* uses `%` to extract the file name `stem`
- `stem` is more general, but I just use `stem` as the file name
  without its path or extension
- extracting a `stem` is useful for working on a list of files
- `%` in the *target* identifies the `stem` from the file list
    - this gives access to a file name without path and extension
- each *dependency* uses `%` to access the stem
    - this gives a way to wrap the `stem` in the dependency path
      and with a different extension
- each *command* uses `$*` to access the `stem`
    - again, this gives a way to specify a different extension

## example

```make
 # make a list of object file names
lib_src := Led BiColorLed TriColorLed
lib_build_src := $(addsuffix .o,${lib_src})
lib_build_src := $(addprefix ../lib/build/,${lib_build_src})
 # define a pattern rule to build these object files
${lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
	${compiler} $(CFLAGS) -c ../lib/src/$*.c -o $@
 # *target* is specific to object files in ../lib/build/ and that
 # match the names listed in lib_src
 # *dependencies* are stem.c and stem.h
 # *command* only compiles stem.c
```

## Automatic Variables

<https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html>

- `$@`
    - file name of target
    - if multiple targets, whichever target caused the recipe to run
- `$^`
    - name of all prerequisites with spaces between them
- `$<`
    - name of first prerequisite
    - useful for listing header dependencies
    - example 1:
    - stem.o depends on stem.c and stem.h, but only compile
      stem.c to make stem.o

    ```make
    ${lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
        ${compiler} $(CFLAGS) -c $< -o $@
    ```

    - example 2:
    - `test_runner.o` depends on `test_runner.c,`
      `HardwareFake.h,` and the `-HardwareFake.h` for every lib,
      but only compile test_runner.c to make test_runner.o

    ```make
    hw_lib_src := BiColorLed SpiSlave
    HardwareFakes := ${hw_lib_src}
    HardwareFakes := $(addsuffix -HardwareFake.h,${HardwareFakes})
    HardwareFakes := $(addprefix test/,${HardwareFakes})
    build/test_runner.o: test/test_runner.c  test/HardwareFake.h ${HardwareFakes}
        ${compiler} $(CFLAGS) -c $< -o $@
    ```

- `$?`
    - name of all prerequisites *newer than the target* with
      spaces between them

# Procedure To Make A New Lib

## Add lib name to lib/Makefile
- open `firmware/lib/Makefile`
- add lib name to space-separated list `hw_lib_src`
- example adding lib `SpiSlave`

```make
hw_lib_src := BiColorLed SpiSlave
```

- try building the test:
    - build fake with `;mkf`
    - this is a little faster than a build for real at spotting
      missing dependencies
    - or build for real with `;mktgc`
- the build fails
- make reports which files are missing:

```bash
|| make: *** No rule to make target 'test/SpiSlave-HardwareFake.h', needed by 'build/test_runner.o'.  Stop.
```

## create a -HardwareFake.h in lib/test/
- create the file
- a `-HardwareFake.h` goes in firmware/lib/test/
- the missing dependency is fixed

- try building the test again
- make reports which files are missing:

```bash
|| make: *** No rule to make target 'test/test_SpiSlave.c', needed by 'build/test_SpiSlave.o'.  Stop.
```

## create a `test_BlahLibName.c` in lib/test/
- create the file
- the `test_BlahLibName.c` goes in firmware/lib/test/
- the missing dependency is fixed

- try building the test again
- make reports which files are missing:

```bash
|| make: *** No rule to make target 'test/test_SpiSlave.h', needed by 'build/test_SpiSlave.o'.  Stop.
```

## ;fh to create a `test_BlahLibName.h` in lib/test/
- create a header file by making a throwaway function in the `.c`
  and then `;fh` on the function to generate a header with the
  function declaration

```c
// test/test_SpiSlave.c
void SpiSlavePlaceholder(void){}
```

- this makes a `test_lib.h` in firmware/lib/test/
- it also automatically includes the header in the c file
- remember to manually add `#include "unity.h"` to the `.c` file,
  you will need it later when you write the first test
- the missing dependency is fixed

- try building the test again
- make reports which files are missing:

```bash
|| make: *** No rule to make target 'src/SpiSlave.c', needed by 'build/SpiSlave.o'.  Stop.
```

## create a `BlahLibName.c` and `BlahLibName.h` in lib/src/
- again, make the `.c` file with a placeholder function and use `;fh` to
  auto-generate the header

- the test should build now
- erase the placeholder functions

## write the first test in `test_BlahLibName.c`
- write the first test in the `test_BlahLibName.c` file
    - `ttt` in insert mode in the function body to get
      `TEST_FAIL_MESSAGE("Implement test.");`
- `;fh` to list the test in the header for visibility to the
  `test_runner`
- `;yt` to add the `RUN_TEST` call to the `test_runner`

## add to the test to drive writing code in the lib
- test calls the function under test
    - build fails, so put the function in the lib
    - silence warnings about unused arguments with the
      `(void)arg_name;` trick
- the function under test uses hardware
    - hardware means registers and pins
    - attempt to access the hardware in the function body
    - and fail because the variables are undeclared
- declare the registers and pins `extern` meaning the lib
  translation unit builds without defining these values

## test passes but that's not enough
- now the build passes because the variables are defined
- but working lib code is *not sufficient!*
- next:
    - define real hardware for the avr-target
    - make sure it translates correctly

## fix undefined hardware references for avr-target
- when the test passes, clean the build and rebuild for
  avr-target
    - `vis-spi-out` also has a unit-test target, but it is not
      affected unless it's tests call the lib just added
- the build fails because the hardware is undefined
- for example:

```bash
|| build/vis-spi-out.o: In function `SpiSlaveInit':
/home/Mike/chromation/dev-kit-mike/firmware/lib/src/SpiSlave.h|23| undefined reference to `Spi_port'
```

- the Makefile catches this if we remember to add it!
- this goes in the `vis-spi-out/Makefile`

```make
hw_lib_src := BiColorLed SpiSlave
```

- now the unit-test target for `vis-spi-out` expects to find
  `../lib/test/SpiSlave-HardwareFake.h`
- and the avr-target for `vis-spi-out` expects to find
  `src/SpiSlave-Hardware.h`

```bash
- || make: *** No rule to make target 'src/SpiSlave-Hardware.h', needed by 'build/vis-spi-out.o'.  Stop.
```

- create `firmware/vis-spi-out/src/SpiSlave-Hardware.h`
- each translation unit compiles correctly again, and the build
  once again fails for undefined references
- put the real definitions in `src/SpiSlave-Hardware.h`

```c
#ifndef _SPISLAVE_HARDWARE_H
#define _SPISLAVE_HARDWARE_H
#include <stdint.h>
#include "SpiSlave.h"

spi_ptr Spi_ddr = &DDRB; // controls if pin is input or output
spi_ptr Spi_port = &PORTB; // controls if pin outputs HIGH or LOW

#endif // _SPISLAVE_HARDWARE_H
```

- add the `#include` to the `Hardware.h` header:

```c
// src/Hardware.h
//
#include "SpiSlave-Hardware.h"
```

## check translation when lib is used in avr-target
- check:
    1. `inline` happens
    2. translated assembly uses the correct instructions

## check `inline` happens
- search for the function name
- if the assembly *jumps* to the function, the function name
  shows up as a comment in the code
- if the function is `inline`, the function name does not show up
  in the code

### example checking for `inline`

- SpiSlaveInit body is `inline` in the header:

```c
// lib/src/SpiSlave.h
// 
inline void SpiSlaveInit(void)
{
    SetBit(Spi_port, Spi_DataReady); // sbi	0x05, 1
    SetBit(Spi_ddr, Spi_DataReady); // sbi	0x04, 1
}
```

- `SpiSlaveInit` is called in the main.c:

```c
// vis-spi-out/src/vis-spi-out.c
//
void setup(void)
{
    BiColorLedOn(led_0); // sbi	0x07, 0
    BiColorLedOn(led_1); // sbi	0x07, 1
    BiColorLedGreen(led_0); // cbi	0x08, 0
    BiColorLedGreen(led_1); // cbi	0x08, 1
    SpiSlaveInit();
}
```

- main.c does not include any .c files
- main.c and its headers comprise a single stream compiled into
  main.o

```make
build/${board-name}.o: src/${board-name}.c ${Hardware}
	${compiler} $(CFLAGS) -c $< -o $@
```

- main.o *is linked* with other local object files and the
  Chromation `lib` object files

```make
build/${board-name}.elf: build/${board-name}.o ${app_build_src} ${lib_build_src}
	avr-gcc $(CFLAGS) $^ -o $@ $(LFLAGS)
```

- and one of these object files *is* `../lib/build/SpiSlave.o`

```make
hw_lib_src := BiColorLed SpiSlave
lib_src := ${hw_lib_src} ReadWriteBits
lib_build_src := $(addsuffix .o,${lib_src})
lib_build_src := $(addprefix ../lib/build/,${lib_build_src})
```

- *but that object file is just a formality*
    - it is in the build recipe to link against in case the
      `inline` is somehow blocked from happening
- the build *does depend* on the object file existing
- but the object file *does not contain anything the linker
  needs*
- this is because the main.c translation unit has a `#include
  SpiSlave.h` in its source
- so the translation unit stream containing main.c has the
  function body via the `inline` body in `../lib/src/SpiSlave.h`
- the compiler does not create a symbol in `main.o` named
  `SpiSlaveInit` because the definition is already known
- so in the final link stage, the linker does not look for a
  definition of `SpiSlaveInit`
- the existence of this symbol in `../lib/build/SpiSlave.o` is
  not included in the program memory
    - no one calls it
    - it does not appear in the final translated assembly

```assembly
00000096 <main>:
    BiColorLedOn(led_0); // sbi	0x07, 0
    BiColorLedOn(led_1); // sbi	0x07, 1
  96:	38 9a       	sbi	0x07, 0	; 7
  98:	39 9a       	sbi	0x07, 1	; 7
    BiColorLedGreen(led_0); // cbi	0x08, 0
    BiColorLedGreen(led_1); // cbi	0x08, 1
  9a:	40 98       	cbi	0x08, 0	; 8
  9c:	41 98       	cbi	0x08, 1	; 8
    SpiSlaveInit();
  9e:	29 9a       	sbi	0x05, 1	; 5
  a0:	21 9a       	sbi	0x04, 1	; 4
}
```

## check translation

### translation for `inline` body in the `.h`
- translation to single instructions should be no problem if the
  function body is `inline` in the `.h`
- if `avr-gcc` fails to use single instructions:
    - make sure the variables are `const`

### translation for body in the `.c`
- function uses hardware but does not need to be `inline`
    - example `SpiSlaveInit` does not need to be `inline`
    - but the functions it calls should still be `inline`
    - and the instructions should translate to single
      instructions
- `SpiSlaveInit` function body is in the `.c`
- two options:
    1. clever macros
        - this is the technique for stubbing functions without
          using function pointers
    2. single compilation unit
        - this avoids macros
            - no -Dmacro in build recipe
            - no `#ifdef` in lib source code
        - just add `-include ` flag when building the hardware
          libs for the avr-target
            - use flag to include the -Hardware.h files
            - do *not* include those files in the Hardware.h that
              is included in the main.c translation unit

### clever macros
- make all hardware definitions macros
- use `#ifdef` to select between real and fake hardware
  definitions
- can also work for multiple real definitions, but I
  shouldn't need this because each avr-target has its own
  Makefile
- for simple case of single real/fake, build target defines a
  test macro with -Dmacro

### single compilation unit
- ~build for avr-target does *not* compile the
  `../lib/src/BlahLibName.c`~
- ~instead, the libs build for the avr-target by compiling
src/BlahLibName.c~
    - this file is a short .c with no code except #include
      statements to include the ../lib/src/BlahLibName.c, avr/io,
      and the src/BlahLibName-Hardware.h file

- ...try that again...
- build for avr-target is identical to build for unit-test
  except:
    - compile with flag `-include src/SpiSlave-Hardware.h`
    - target is `build/SpiSlave.o` instead of
      `../lib/build/SpiSlave.o`
    - need to build for a different target or `make` complains
      about overriding target
- now the lib compilation unit has the hardware info

### example translation for a single compilation unit

- first `SpiSlaveInit()` is moved from `../lib/src/SpiSlave.h` to
  `../lib/src/SpiSlave.c`
- look at the problems this raises before fixing them

```c
// lib/src/SpiSlave.c
//
#include "SpiSlave.h"
/* void SpiSlaveInit(void); */
void SpiSlaveInit(void)
{
    /** SpiSlaveInit behavior:\n 
      * - makes DataReady an output pin\n 
      * - idles DataReady high\n 
      * */
    // DataReady pin idle high
    SetBit(Spi_port, Spi_DataReady); // sbi	0x05, 1
    // Set DataReady as an output pin
    SetBit(Spi_ddr, Spi_DataReady); // sbi	0x04, 1
}
```

```c
// lib/src/SpiSlave.h
//
void SpiSlaveInit(void);
/*inline void SpiSlaveInit(void) */
/*{ */
// ...
```

- clean the build and rebuild the unit-test target
- the `SpiSlaveInit` tests still pass
- clean the build and rebuild the `avr-target`
- check the size with `avr-size build/vis-spi-out.elf`
    - the program was 174 bytes with the `inline`
    - the program is now 224 bytes!
    - so the added overhead is at least 50 bytes
    - compare this with the code in `SpiSlaveInit`
    - each of the six calls translates to a 2-byte instruction
    - so the actual code should only take 12 bytes
    - this extra 38 bytes is a combination of:
        - the `SpiSlaveInit` call overhead
        - the overhead for loading and storing the hardware
          values from memory since they are no longer hard-coded
        - extra instructions replace the single instructions
          because the compiler does not know it can use the
          single instructions
    - there's nothing to be done about the call overhead unless
      we put the function definition back in the header
    - the goal now is just:
        - avoid loading and storing hardware values
        - use the single instructions
    - this reduces the code size to 180 bytes
    - so only six bytes of overhead from the function call!

### check the translation to confirm the problems exist

- now main.o calls symbol `SpiSlaveInit`

```assembly
00000096 <main>:
    BiColorLedOn(led_0); // sbi	0x07, 0
    BiColorLedOn(led_1); // sbi	0x07, 1
  96:	38 9a       	sbi	0x07, 0	; 7
  98:	39 9a       	sbi	0x07, 1	; 7
    BiColorLedGreen(led_0); // cbi	0x08, 0
    BiColorLedGreen(led_1); // cbi	0x08, 1
  9a:	40 98       	cbi	0x08, 0	; 8
  9c:	41 98       	cbi	0x08, 1	; 8
    SpiSlaveInit();
  9e:	0e 94 55 00 	call	0xaa	; 0xaa <SpiSlaveInit>
}
```

- the call is not that bad
- the real overhead comes in how `SpiSlaveInit` is translated
- neither `SetBit` call is implemented as `sbi`

```assembly
000000aa <SpiSlaveInit>:
    // DataReady pin idle high
    SetBit(Spi_port, Spi_DataReady); // sbi	0x05, 1
  aa:	e0 91 01 01 	lds	r30, 0x0101	; 0x800101 <Spi_port>
  ae:	f0 91 02 01 	lds	r31, 0x0102	; 0x800102 <Spi_port+0x1>
inline void SetBit(register_address reg_addr, bit_index bit)
{
    /** SetBit behavior:\n 
      * - sets bit in register\n 
      * */
    *reg_addr |= 1<<bit;
  b2:	20 81       	ld	r18, Z
  b4:	81 e0       	ldi	r24, 0x01	; 1
  b6:	90 e0       	ldi	r25, 0x00	; 0
  b8:	00 90 00 01 	lds	r0, 0x0100	; 0x800100 <Spi_DataReady>
  bc:	02 c0       	rjmp	.+4      	; 0xc2 <SpiSlaveInit+0x18>
  be:	88 0f       	add	r24, r24
  c0:	99 1f       	adc	r25, r25
  c2:	0a 94       	dec	r0
  c4:	e2 f7       	brpl	.-8      	; 0xbe <SpiSlaveInit+0x14>
  c6:	92 2f       	mov	r25, r18
  c8:	98 2b       	or	r25, r24
  ca:	90 83       	st	Z, r25
    // Set DataReady as an output pin
    SetBit(Spi_ddr, Spi_DataReady); // sbi	0x04, 1
  cc:	e0 91 03 01 	lds	r30, 0x0103	; 0x800103 <Spi_ddr>
  d0:	f0 91 04 01 	lds	r31, 0x0104	; 0x800104 <Spi_ddr+0x1>
  d4:	90 81       	ld	r25, Z
  d6:	89 2b       	or	r24, r25
  d8:	80 83       	st	Z, r24
  da:	08 95       	ret
```

- the main.c translation unit no longer includes the definition
  of `SpiSlaveInit()`
- this introduces two sources of overhead:

1. the main.o output needs a symbol to call `SpiSlaveInit`
    - this is OK
    - the function is only called once
    - the `call` is one instruction plus the preamble and
      postamble in the call itself
2. the `../lib/build/SpiSlave.o` needs symbols for the hardware
   definitions
    - this is not OK
    - the compiler does not know to use the single instructions
    - and the hardware definitions take up space in memory
      instead of being hardcoded wherever they are needed

### fix the problems with the single compilation unit
- update the `vis-spi-out/Makefile`:
    - separate libs with hardware dependencies, like SpiSlave,
      from libs without any hardware dependencies, like
      ReadWriteBits
- unit-test target:
    - builds all libs without hardware definitions
    - all builds go into `../lib/builds/`
    - the `test_runner` translation unit includes the fake
      hardware definitions
- avr-target:
    - builds hardware libs with hardware definitions and puts
      these builds into `builds/`
    - builds non-hardware libs without hardware definitions and
      puts these builds into `../lib/builds/`
    - the only additional file is the object file placed in the
      `vis-spi-out/build` folder
    - there are no additional source files to create
    - the translation unit is the `../lib/src/` file
    - the hardware dependency is included in the stream using the
      `-include` flag in the build recipe

```make
 # unit-tests build libs without any Hardware definitions
../lib/build/SpiSlave.o: ../lib/src/SpiSlave.c ../lib/src/SpiSlave.h
	${compiler} $(CFLAGS) -c $< -o $@

 # avr-target builds lib with lib-Hardware.h definitions
build/SpiSlave.o: ../lib/src/SpiSlave.c ../lib/src/SpiSlave.h src/SpiSlave-Hardware.h
	${compiler} -include src/SpiSlave-Hardware.h $(CFLAGS) -c $< -o $@
```

### check the problems are fixed by the single compilation unit

- `:!avr-size build/vis-spi-out.elf` says the code size is only
  180 bytes
- check the translation
- there is still the call to the SpiSlaveInit symbol as before

```assembly
00000096 <main>:
    BiColorLedOn(led_0); // sbi	0x07, 0
    BiColorLedOn(led_1); // sbi	0x07, 1
  96:	38 9a       	sbi	0x07, 0	; 7
  98:	39 9a       	sbi	0x07, 1	; 7
    BiColorLedGreen(led_0); // cbi	0x08, 0
    BiColorLedGreen(led_1); // cbi	0x08, 1
  9a:	40 98       	cbi	0x08, 0	; 8
  9c:	41 98       	cbi	0x08, 1	; 8
  9e:	0e 94 55 00 	call	0xaa	; 0xaa <SpiSlaveInit>
}
```

- but SpiSlaveInit is much shorter
- it is just the four bytes from the two `sbi` calls, followed by
  two bytes for a `ret` instruction

```assembly
000000aa <SpiSlaveInit>:
  aa:	29 9a       	sbi	0x05, 1	; 5
  ac:	21 9a       	sbi	0x04, 1	; 4
  ae:	08 95       	ret
```

### new problem when trying with BiColorLed
- vis-spi-out.c calls `BiColorLedOn(led_0)`
- this is an inline function
- so the compiler copies the source code to vis-spi-out.c
- vis-spi-out.c used to include BiColorLed-Hardware.h
- so it could translate the copied source code correctly
- but after the *single compilation unit* fix, BiColorLed is
  built with BiColorLed-Hardware.h
- it's a hardware library
- so it gets built with the hardware header
- but now tow translation units are built with the same hardware
  header
- the hardware header defines values
- the values are defined twice
- just change all hardware values to macros and these problems go
  away
- no, it's not obvious how to make registers in libraries macros
- I tried this on BiColorLed and it was a mess
- so back to the hardware header multiple define problem
- I can only include the hardware header once
- otherwise the linker ends up with two objects files that each
  claim to have the definitions for the registers and pins
- I'm stuck with either:
1. make function inline by putting it in lib .h,
    - but then that hardware lib does not get built with the
      hardware-header, and vis-spi-out does (because vis-spi-out
      needs the hardware)
2. put function in lib .c
    - then lib is built with included hardware-header and
      vis-spi-out is not built with this header
- So a lib does 1 or 2. Not both. If one lib does both, I need to
  split it into two libs.
