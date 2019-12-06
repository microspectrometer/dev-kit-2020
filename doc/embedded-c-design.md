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
- [ ] create a lib that has function definitions in its .c
    - use `Spi` because next up in vis-spi-out setup() is
      `SpiSlaveInit`
    - this lib has its own translation unit
    - the resulting .o file is needed by the linker
    - the compiler needs to know the values to use when it makes the translation
      unit
    - this is where I decide how to proceed for testing
    - so far I can test what I need without fakes
    - the need for fakes and the hiding of values forces me to work out exactly
      how to use macros for faking depending on the target
    - [x] first make the function inline in the header to get everything working
      as usual with the unit tests and with the assembly output
    - [ ] now move the function body into the .c

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
- if the function has many calls, `avr-gcc` may decide not to inline it
- in this case, making the function `static` makes `avr-gcc` inline it again

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
- target rule syntax is:

```make
target: dependency
	command
```

- `target` is usually the file to be created
- example: `build/test_runner.o` is the target and it is an
  object file created by the compiler

```make
build/test_runner.o: test/test_runner.c test/${HardwareFake} test/HardwareFake.h
	${compiler} $(CFLAGS) -c $< -o $@
```

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
    lib_src := BiColorLed
    HardwareFake := ${lib_src}
    HardwareFake := $(addsuffix -HardwareFake.h,${HardwareFake})
    test/test_runner.o: test/test_runner.c test/${HardwareFake} test/HardwareFake.h
        ${compiler} $(CFLAGS) -c $< -o $@
    ```

- `$?`
    - name of all prerequisites *newer than the target* with
      spaces between them

# Procedure To Make A New Lib
- open `firmware/lib/Makefile`
- add lib name to space-separated list `hw_lib_src`
- example adding lib `SpiSlave`

```make
hw_lib_src := BiColorLed SpiSlave
```

- try building the test either for real with `;mktgc` or fake with `;mkf`
- the build fails
- make reports which files are missing:

```bash
|| make: *** No rule to make target 'SpiSlave-HardwareFake.h', needed by 'build/test_runner.o'.  Stop.
```

- create the file
- a `-HardwareFake.h` goes in firmware/lib/test/
- try building the test again
- make reports which files are missing:

```bash
|| make: *** No rule to make target 'test/test_SpiSlave.c', needed by 'build/test_SpiSlave.o'.  Stop.
```

- create the file
- a `test_lib.c` goes in firmware/lib/test/

- try building the test again
- make reports which files are missing:

```bash
|| make: *** No rule to make target 'test/test_SpiSlave.h', needed by 'build/test_SpiSlave.o'.  Stop.
```

- create a header file by making a throwaway function in the `.c` and then
  `;fh` on the function to generate a header with the function declaration

```c
// test/test_SpiSlave.c
void SpiSlavePlaceholder(void){}
```

- this makes a `test_lib.h` in firmware/lib/test/
- it also automatically includes the header in the c file
- remember to manually add `#include "unity.h"` to the `.c` file, you will
  need it later when you write the first test

- try building the test again
- make reports which files are missing:

```bash
|| make: *** No rule to make target 'src/SpiSlave.c', needed by 'build/SpiSlave.o'.  Stop.
```

- again, make the `.c` file with a placeholder function and use `;fh` to
  auto-generate the header

- the test should build now
- erase the placeholder functions
- write the first test in the `test_lib.c` file with `ttt` to get
  `TEST_FAIL_MESSAGE("Implement test.");`
- `;fh` to list the test in the header for visibility to the `test_runner`
- `;yt` to add the `RUN_TEST` call to the `test_runner`

- the first test calls a function from the lib
- put the function in the lib
- the function uses hardware
- declare the registers and pins `extern` meaning the lib translation unit
  builds without defining these values
    - this is no problem if the function body is `inline` in the `.h`
    - but if the function body is in the `.c` we need clever macros to make
      the real hardware definitions visible when building for the avr-target

- when the test passes, clean the build and rebuild for avr-target
    - `vis-spi-out` also has a unit-test target, but it is not affected
      unless it's tests call the lib just added
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
- each translation unit compiles correctly again, and the build once again
  fails for undefined references
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


