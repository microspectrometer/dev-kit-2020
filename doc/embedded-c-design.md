# Safely butcher my project
- I am about to butcher my project
- first commit all changes as usual
- I can branch, or I can do a reset

## 1. checkout throwaway branch

```bash
git checkout -b experiment
```

- abandon experiment and return to good code:

```bash
git checkout clean-master
```

## 2. stay on master branch and do a reset

### Work I do now is throwaway
Whether staged or unstaged, any changes will be lost after the
reset.

### Check hash for sanity check
Record the hash of the last commit:

Get the last hash into the clipboard using Vim shortcut `;Gh`. Or
display any number of previous hashes:

4e14d1bdb352edaacefdd4412000802a629136c3

```bash
$ git log | grep commit -m 2
commit 58961ae2c5e17dd5d2520a76df0f829f51a448ed
commit a79386030aaa50d8928a18fc6929248cdf9d92d9
```

- Use this hash to verify Git did expected reset:
- Paste the expected HEAD hash
- Do the reset
- Paste the actual HEAD hash on the next line
- they should be identical

### Make any changes
- edit Makefile
- edit code
- rebuild
- but don't make any changes to the repo that I want to keep

### Reset
Here is the reset. This discards any changes made since last
commit:

```bash
$ git reset --hard HEAD
Press ENTER or type command to continue
HEAD is now at 9ef51af Merged in rainbots/dev-kit-mike (pull request #3)
```

The `HEAD` is the last commit. The last commit is the default for
`git reset --hard`. I explicitly include `HEAD` as a reminder
that I'm reverting to the working state pointed to by `HEAD`.

# Train of thought
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
    - [x] add `ifeq endif`
        - see if you can make the SpiSlave.o build to the same
          location instead of having two separate build targets
        - can I get back to a clean division of libraries into
          space-separated lists without a rule for each library
        - yes
        - there are three groups of libs now:
        - hw_lib_src
            - functions in this lib need hw defs
            - functions are not inline
            - so lib translation unit needs the hw def
            - avr-gcc builds with -inline lib-Hardware.h
        - inlhw_lib_src
            - functions in this lib also need hw defs
            - but functions are inline
            - so lib translation unit does not need the hw def
            - the main() file needs the hw def
            - avr-gcc builds lib without -inline lib-Hardware.h
            - main() file includes Hardware.h which includes the
              lib-Hardware.h
        - nonhw_lib_src
            - functions in this lib do not need hw defs
            - functions might be inline or not, whatever
            - avr-gcc builds lib without -inline lib-Hardware.h
            - main() file includes Hardware.h
            - Hardware.h makes no mention of this lib
    - [x] fix avr header problem
        - SpiSlave is a hardware lib
        - it needs avr/interrupt.h
        - avr-gcc knows where to find this file
        - gcc does not know (and even if it did, we don't want to
          use the real def)
        - two ways to fix this:
            1. -include avr/interrupt.h for avr-target only
            2. macro:
                - use ifdef conditional preprocessor in
                  SpiSlave.c to select whether to include real
                  avr/interrupt.h or fake version
                - build rule adds a -Dmacro to define the macro
                  to indicate test build or avr-gcc build
        - option 2 is better to make clear to someone reading the
          source code what's happening and which libs need which
          avr headers, and it keeps the Makefile simple
        - option 1 is simpler and keeps the source code cleaner
          (is putting conditional includes in the source helpful
          or just noise?)
        - I went with option 1
        - the Makefile for vis-spi-out shows how to do this
        - but now I need the test/FakeAvrInclude/interrupt.h to
          macro define the functions to stub out
        - this should go in the ../lib/test folder
        - ok, done
        - I think this works in general
        - I think I can use this with reg and pin values as well,
          using the #define trick
        - check out
          /home/Mike/chromation/dev-kit-mike/firmware/lib/test/FakeAvr/interrupt.h
        - need to sleep, return here tomorrow
    - [x] finish writing SpiSlaveInit
    - [x] add unit tests for SpiSlaveInit
        - cli and sei build well with avr-gcc
        - do these cause problems when unit testing?
        - yes
        - so I stub them to ignore them in testing
- [x] how do I test that a lib .c function calls a static
  function?
    - do not
    - treat `static` the way the compiler does: `inline` the function
    - so instead of checking for a call, just directly test for what the
      function does
    - this makes the test name better
    - which makes the function docstring better with ;xdc
    - but it *is* possible to check if a static function is called by faking the
      function
    - the function is static in the arv-target build
    - but the function is not static in the test build
    - switching between the two definitions is done with macros:
        - `#ifndef LIBNAME_FAKED` surrounds the real definition
        - `#ifdef LIBNAME_FAKED` surrounds the include for the `_faked.h`
        - always include the real `.h`
        - include flag `-DLIBNAME_FAKED` in the `Makefile` `CFLAGS_for_cygwin`
        - recipe for `_faked.o`
        - target build links against `_faked.o`
    - I can't figure out why `AssertCall` is returning
      `false` when it should *clearly* be returning true
    - I fixed that problem by replacing the `==` comparison with a
      `g_strcmp0` comparison
- [x] why is there a `-DSPISLAVE_FAKED` in the Makefile when the other hardware
  libs do not need a `-DLIBNAME_FAKED`?
    - because I "unit test libs using -DMacro to define fakes"
    - it is to test with a faked version of `EnableSpiInterrupt`
    - the fake records itself in `mock`

# [ ] split common Spi stuff from SpiSlave into a Spi lib

# [ ] move more code from old vis-spi-out to new vis-spi-out

# [ ] what can I do to use the GPIO registers in lib Queue

# embedded C on old micros: make all lib functions `inline`
- making everything `inline` is the only practical option
- in other words, after splitting the application up into libs,
  *every* function in those libs is `inline`:
    - the function body is placed in the `header`
    - the function prototype is placed in the `.c`

## why `inline`?
- two goals lead to the need for `inline`:
1. splitting code up into multile `.c` files creates multiple
   translation units
2. unit-testing hardware-dependent-code without a million
   `#ifdef` statements polluting the code means replacing vendor
   hardware macros with `const` variables
    - [ ] come back to this
    - once I've inlined, this problem actually goes away
    - just treat the `io` vendor headers like the other vendor
      headers: create a fake header for tests
    - I think this would eliminate a ton of header files

## two drawbacks to inlining everything

### 1. no more `static` functions
The compiler inlines `static` functions within a translation
unit. But that only generates optimized assembly if the
translation unit sees the hardware definitions.

The lib functions need to be `inline` in a different translation
unit. In this case, the `static` lib function is still inlined,
but the assembly cannot be optimized.

So the functions cannot remain `static` in the lib. They become
`inline`.

Here are three problems with losing `static` for lib functions,
and the solution I use to each problem.

1. namespace conflicts:
- solution: longer function names
- a function name that might occur in different libs
    - Example: `PinClk_SetOutput`
- now needs the *lib* name somewhere in its name, like:
    - Example: `PinLisClk_SetOutput`
2. functions are exposed:
- solution: documentation!
- rely on user to respect comment `---Private---`
- user should not consider `Private` functions as part of the API
3. API is not "self-documenting":
- solution: manually list API calls under `\file` tag in `.c`
- the `.h` file is no longer adequate for viewing a short list of
  API calls
- Add `API` doc to `.c` file
- Example:

```c
// (First line of file)
/** \file SpiSlave.c
 * # ---API---
 * void SpiSlaveInit(void);\n 
 * void SpiSlaveTx(uint8_t const *input_buffer, uint16_t nbytes);\n 
 * uint8_t ReadSpiStatusRegister(void);\n 
 * uint8_t ReadSpiDataRegister(void);\n 
 * void ClearSpiInterruptFlag(void);\n 
 * void DisableSpiInterrupt(void);\n 
 * void EnableSpiInterrupt(void);\n 
 * */
```

- [ ] write a script that scrapes the project for the `API`
  functions and makes a pretty output

### 2. order function definitions to avoid "implicit declarations"
- usually declaration is in `.h`
- then calls always come `after` the declaration
- but now declaration is in `.c`
- so other translation units do not get a declaration, just a
  definition
- so if one `inline` calls another `inline`, the caller
  definition has to come *after*
- cannot fix this with `extern` declarations either
- otherwise compiler throws "implicit declaration" error


- I tried 
- this third type of lib does not work in practice
- the hardware definitions are used by the `Init` function, but
  also used by functions that need to be inline
- i.e., both `lib.o` and `main.o` need the hardware definitions

# Use the `-include` flag for AVR headers like `<avr/io.h>`
This is a way for me to tell the compiler to include a file:

- without having to know its exact path
- without having to pollute the source code with `#include`

This makes it convenient to use the real AVR headers for the
board target and fake AVR headers for test targets, because all
of the work is done in the Makefile.

This is how I include the AVR headers. Since I do the AVR headers
that way, I do the Fake AVR headers the same way for symmetry.

I know exactly where the fakes are because I made them, so I
could have done the fakes with the usual `-I` flag. What's nice
about `-include` is I don't need to pollute the source files with
these includes.

From the `gcc` documentation about the `-include` flag:

> processes the file as if "#include "file"" appeared as the
> first line of the primary source file.

The directory search is slightly different. It makes no
difference for my purposes. I specify the fake header locations
relative to the Makefile.

> Process file as if "#include "file"" appeared as the first line
> of the primary source file.  However, the first directory
> searched for file is the preprocessor's working directory
> instead of the directory containing the main source file.  If
> not found there, it is searched for in the remainder of the
> "#include "..."" search chain as normal.
>
> If multiple -include options are given, the files are included
> in the order they appear on the command line.

The one minor downside is I don't think I can trigger a rebuild
if I update the `avr-gcc` tool and pick up updated AVR headers.
If I ever do such an update, remember to clean-all (`;mca`)
before the build.

# Flag -include uses "#include "..."" search chain
Fold `man-gcc.man` by `indent` and see section `Options
Controlling the Kind of Output`

Use `-v` to print header search paths.

> -v  Print (on standard error output) the commands executed to
>     run the stages of compilation.  Also print the version
>     number of the compiler driver program and of the
>     preprocessor and the compiler proper.

Example:

```make
build/${board-name}.o: src/${board-name}.c ${Hardware} ${lib_headers}
	${compiler} $(CFLAGS) -v -c $< -o $@
```

This performs the usual compile, but the output is verbose.
Buried in the Quickfix window is this info:

```bash
|| #include "..." search starts here:
|| #include <...> search starts here:
||  src
||  ../lib/src
||  c:\program files (x86)\atmel\studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\../lib/gcc/avr/5.4.0/include
||  c:\program files (x86)\atmel\studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\../lib/gcc/avr/5.4.0/include-fixed
||  c:\program files (x86)\atmel\studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\../lib/gcc/avr/5.4.0/../../../../avr/include
|| End of search list.
```

None of these exist as paths I can get to. They are fake paths
coming from the avr-gcc.exe via the `-B` flag.

# AVR headers like `<avr/io.h>` are found via -B in LFLAGS
Fold `man-gcc.man` by `indent` and see section `Options for
Directory Search`.

See section `-Bprefix`.

Note I do not have to add a path for the `AvrHeaders`. `LFAGS`
uses `-B` to add:

```make
atmega328_lib = '/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'
```

This tells `avr-gcc`:

> where to find executables, libraries, include files, and data
> files of the compiler itself.

# Unit-test headers are included with -I in CFLAGS
Fold `man-gcc.man` by `indent` and see section `Options for
Directory Search`.

`CFLAGS` use `-I` to add include paths.

# `avr-gcc` can always `inline` but not always optimize
- define all lib functions `inline`
- build `main.o` with `lib-Hardware.h`
- do not build `lib.o` with `lib-Hardware.h`

## `inline` is not the problem
- if a function is defined in a header:
    - any `.c` file that includes the header *sees* the
      definition
    - the `.o` built from that `.c` file will `inline` instead of
      `call`
- so `inline` is not the problem
- both the application and lib `.c` files will `inline` the call
- but if register addresses are in header file `-Hardware.h`,
  then only one `.c` file can include the header
    - if more than one `.c` file includes it, there is a multiple
      definition error when the `.o` files are linked
- if a lib contains calls that:
    - are frequently made from the application
    - rely on knowledge of the register address for picking the
      best instruction
- then the lib cannot be built with the `-Hardware.h`
- the application must be built with `-Hardware.h`
- if the lib then puts *any* function definitions in its `.c`
  file and those definitions need register values, the lib `.o`
  will not be optimized for that function call
- on the other hand, if the application calls the lib functions
  only once (like in a `setup()`), then it's OK to leave the
  function definitions in `.c` and build the lib with
  `-Hardware.h`
- but that scenario never happens
- I originally setup `SpiSlave` and `UartSpi` this way
    - look at `.avra` and try to find `Spi_spdr` -- is it loaded
      from mem?
    - Yes, check it out. SPDR is 0x2E and SPSR is 0x2D.
    - The values are hard-coded into the assembly, not loaded
      from memory.

    ```asm
    inline uint8_t ReadSpiStatusRegister(void) { return *Spi_spsr; }
     1b8:	8d b5       	in	r24, 0x2d	; 45
    inline uint8_t ReadSpiDataRegister(void) { return *Spi_spdr; }
     1ba:	8e b5       	in	r24, 0x2e	; 46
    ```

    - so main.c can call an `Init` function in lib.c
    - and that's compiles as a `call`
    - but the `Init` calls `inline` functions
    - ''avr-gcc'' inlines the functions
    - and ''avr-gcc'' uses the best instructions

## problem is translation unit missing hardware definitions
- but I immediately ran into the problem that I needed to call
  those functions again in `main.c`, but `main.c` does not have
  the hardware definitions, so ''avr-gcc'' does not choose the
  best instructions
- `vis-spi-out.o` does not have the hardware file in its
  translation unit
    - it cannot fill in the hardware values
- Here is what is happening
    - These functions are *not* called from within
      `vis-spi-out.o`.
    - These functions are called from within `SpiSlaveInit` in
      `SpiSlave.o`.
    - `SpiSlave.o` is built with `-include
      src/SpiSlave-Hardware.h`
    - proof:

I put `ClearSpiInterruptFlag()` as the first call in `main()` in
`vis-spi-out.c`:

```c
void setup(void)
{
    ClearSpiInterruptFlag();
```

This translates to:

```avra
000000a6 <main>:
inline uint8_t ReadSpiStatusRegister(void) { return *Spi_SPSR; }
  a6:	e0 91 00 01 	lds	r30, 0x0100	; 0x800100 <Spi_SPSR>
  aa:	f0 91 01 01 	lds	r31, 0x0101	; 0x800101 <Spi_SPSR+0x1>
  ae:	80 81       	ld	r24, Z
inline uint8_t ReadSpiDataRegister(void) { return *Spi_SPDR; }
  b0:	c0 91 02 01 	lds	r28, 0x0102	; 0x800102 <Spi_SPDR>
  b4:	d0 91 03 01 	lds	r29, 0x0103	; 0x800103 <Spi_SPDR+0x1>
  b8:	88 81       	ld	r24, Y
```

Later on, when that same function is called from within
`SpiSlaveInit()`, defined in `SpiSlave.c`, the correct
instructions are used:

```c
// vis-spi-out.c
    SpiSlaveInit(); // call	0xaa	; 0xaa <SpiSlaveInit>
```

```avra
inline uint8_t ReadSpiStatusRegister(void) { return *Spi_SPSR; }
 1ea:	8d b5       	in	r24, 0x2d	; 45
inline uint8_t ReadSpiDataRegister(void) { return *Spi_SPDR; }
 1ec:	8e b5       	in	r24, 0x2e	; 46
```

This means if I want optimized instructions, the hardware defs
need to go with one object file. The bonehead way to do this is
to *always* include hardware defs with the main application,
never with the lib object files, but then all of the functions
need to be inline.

The only other choice is to always make a call to the lib. This
works great for long calls. The instructions dwarf the call
overhead.

Where I pay a price here is for short calls, like reading a
register. I want to do this *fast*, so why make a call. It should
be inline. But I have to make the call to pick up the hardware
defs.

## linker error if lib.c and main.c both include -Hardware.h

```Makefile
 # vis-spi-out/Makefile
${hw_lib_o}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h src/%-Hardware.h
	${compiler} -include src/$*-Hardware.h $(CFLAGS) -c $< -o $@
```

- every hardware def has a *multiple definition* error:

```bash
multiple definition of `Spi_SPDR'
```

# ---Completed Tasks---

# [x] make `;ds` shortcut to paste time-stamped flash size
- record flash size before altering project in any way

# [x] inline all lib functions
- Every function definition goes in the header
- `-Hardware.h` headers are only included with the application
    - do this via `vis-spi-out/src/Hardware.h`
- [x] create branch `clean/inline-all-lib-funcs`
- plan is to merge this back with `clean-master` later

## [x] make a checklist
- checklist:
- [-] edit `vis-spi-out/Makefile`:
    - move lib from `hw_lib_src` to `inlhw_lib_src`
- [-] add `#include "lib-Hardware.h"` to
  `vis-spi-out/src/Hardware.h`
- move all functions from `lib.c` to `lib.h`:
    - remove `static` keyword from private functions
    - create prototypes in `lib.c` for all functions
- [-] avr-target builds
- [-] code size did not change or decreased slightly
- [-] unit-test target builds
    - turn on tests for the affected lib

## [x] edit `vis-spi-out/Makefile`
- list libs to edit
- simplify structure: everything becomes like `BiColorLed`

Before moving `Lis`

```date-and-size
Mon, Jan  6, 2020 11:04:36 AM
   text	   data	    bss	    dec	    hex	filename
    648	      2	     21	    671	    29f	build/vis-spi-out.elf
```

- move lib `Lis` from `hw_lib_src` to `inlhw_lib_src`
- add `#include "Lis-Hardware.h"` to `vis-spi-out/src/Hardware.h`
- move all functions from `Lis.c` to `Lis.h`:
- remove `static` keyword from private functions
    - there is no *call* symbol
    - the function is `inline`, so `static` has no meaning:
        - `static` means the function name is unique to this lib,
          so even if another translation unit has the same
          function name, the compiler treats them as different
          symbols
        - `inline` says eliminate the symbol altogether
- the compiler warns if `static` is not removed:
- Example: `gcc` to output `-o build/test_runner.o`

```bash
src/Lis.h|157 col 5| warning: ‘Pin_LisPixSelect_SetOutput’ is static but used in inline function ‘LisInit’ which is not static
```

- similarly, `static` cannot be used in the prototype:
- Example: `gcc` to output `-o build/Lis.o`

```bash
src/Lis.c|4 col 13| warning: ‘Pin_LisPixSelect_SetOutput’ declared ‘static’ but never defined [-Wunused-function]
```

- create prototypes for all functions in the `.c` file
- this is to prevent linker errors
- it does not increase code size (because these symbols are never
  used, i.e., the functions are never called)
- Example: for `-o build/TestSuite.exe` if there is no prototype,
  then you get a linker error when building the `TestSuite` with
  `gcc`:

```bash
|| build/Lis.o: In function `LisInit':
src/Lis.h|157| undefined reference to `Pin_LisPixSelect_SetOutput'
src/Lis.h|157| (.text+0x9): relocation truncated to fit: R_X86_64_PC32 against undefined symbol `Pin_LisPixSelect_SetOutput'
src/Lis.h|157| undefined reference to `Pin_LisPixSelect_SetOutput'
```

- Interestingly, `avr-gcc` does not throw any linker error when
  building the `.elf`: `-o build/vis-spi-out.elf`
- so `static` keyword is dropped in `.h`, `inline` is added in
  `.h`, nothing has a keyword in `.c`
- if any keyword were to be added, add `extern inline` to the
  prototype as described here:
  <https://gustedt.wordpress.com/2010/11/29/myth-and-reality-about-inline-in-c99/>
    - this is not required
    - I do not do it

After moving `Lis`

```date-and-size
Mon, Jan  6, 2020 11:05:50 AM
   text	   data	    bss	    dec	    hex	filename
    642	      2	     21	    665	    299	build/vis-spi-out.elf
```

As expected, code size reduced slightly because the `call` to
`LisInit` is eliminated.

## [x] no change required in `lib/Makefile`
- there is nothing to change!
- just make sure the tests still run for the chagned 

## [x] inline `Lis`
- checklist:
- [x] edit `vis-spi-out/Makefile`:
    - move lib from `hw_lib_src` to `inlhw_lib_src`
- [x] add `#include "lib-Hardware.h"` to
  `vis-spi-out/src/Hardware.h`
- move all functions from `lib.c` to `lib.h`:
    - remove `static` keyword from private functions
    - create prototypes in `lib.c` for all functions
- [x] avr-target builds
- [x] code size did not change or decreased slightly
- [x] unit-test target builds
    - turn on tests for the affected lib

## [x] inline `SpiSlave`
Before

```date-and-size
Mon, Jan  6, 2020  2:38:27 PM
   text	   data	    bss	    dec	    hex	filename
    642	      2	     21	    665	    299	build/vis-spi-out.elf
```

- checklist:
- [x] edit `vis-spi-out/Makefile`:
    - move lib from `hw_lib_src` to `inlhw_lib_src`
- [x] add `#include "lib-Hardware.h"` to
  `vis-spi-out/src/Hardware.h`

After

```date-and-size
Mon, Jan  6, 2020  2:41:54 PM
   text	   data	    bss	    dec	    hex	filename
    770	     14	     21	    805	    325	build/vis-spi-out.elf
```

- move all functions from `lib.c` to `lib.h`:
    - remove `static` keyword from private functions
    - create prototypes in `lib.c` for all functions

- [x] build avr-target
- [x] code size did not change or decreased slightly


```date-and-size
Mon, Jan  6, 2020  3:10:07 PM
   text	   data	    bss	    dec	    hex	filename
    610	      0	     21	    631	    277	build/vis-spi-out.elf
```

- [x] build unit-test target
    - turn on tests for the affected lib

## [x] inline `UartSpi`
Before

```date-and-size
Mon, Jan  6, 2020  3:10:07 PM
   text	   data	    bss	    dec	    hex	filename
    610	      0	     21	    631	    277	build/vis-spi-out.elf
```
- checklist:
- [x] edit `vis-spi-out/Makefile`:
    - move lib from `hw_lib_src` to `inlhw_lib_src`
- [x] add `#include "lib-Hardware.h"` to
  `vis-spi-out/src/Hardware.h`
- [x] move all functions from `lib.c` to `lib.h`:
    - remove `static` keyword from private functions
    - create prototypes in `lib.c` for all functions

After Hardware.h change, but before move

```date-and-size
Mon, Jan  6, 2020  4:42:31 PM
   text	   data	    bss	    dec	    hex	filename
    812	     20	     21	    853	    355	build/vis-spi-out.elf
```

After move (where are the extra 14 bytes from?)

```date-and-size
Mon, Jan  6, 2020  4:48:38 PM
   text	   data	    bss	    dec	    hex	filename
    624	      0	     21	    645	    285	build/vis-spi-out.elf
```

- [x] avr-target builds
- [x] code size did not change or decreased slightly
- [x] unit-test target builds
    - turn on tests for the affected lib

# [x] clean Makefile variables after all the inlining

## [x] clean `vis-spi-out/Makefile`

### Old variables

```make
hw_lib_src :=
inlhw_lib_src := BiColorLed SpiSlave UartSpi Lis
Hardware := ${inlhw_lib_src}
HardwareFakes := ${Hardware}
hw_lib_o := $(addsuffix .o,${hw_lib_src})
nonhw_lib_o := $(addsuffix .o,${nonhw_lib_src})
lib_headers := ${hw_lib_src} ${inlhw_lib_src} ${nonhw_lib_src}
```

### Intermediate output of `;mpv` while rewriting...

```Makefile-variables
hwlib:
- BiColorLed SpiSlave UartSpi Lis
nohwlib:
- ReadWriteBits Queue
Hardware:
- src/BiColorLed-Hardware.h
- src/SpiSlave-Hardware.h
- src/UartSpi-Hardware.h
- src/Lis-Hardware.h
HardwareFakes:
- ../lib/test/BiColorLed-HardwareFake.h
- ../lib/test/SpiSlave-HardwareFake.h
- ../lib/test/UartSpi-HardwareFake.h
- ../lib/test/Lis-HardwareFake.h
hw_lib_o:
- ../lib/build/BiColorLed.o
- ../lib/build/SpiSlave.o
- ../lib/build/UartSpi.o
- ../lib/build/Lis.o
nonhw_lib_o:
- ../lib/build/ReadWriteBits.o
- ../lib/build/Queue.o
lib_headers:
- ../lib/src/BiColorLed.h
- ../lib/src/SpiSlave.h
- ../lib/src/UartSpi.h
- ../lib/src/Lis.h
- ../lib/src/ReadWriteBits.h
- ../lib/src/Queue.h
IncludeAvrHeaders:
- -includeavr/interrupt.h
- -includeavr/io.h
IncludeFakeAvrHeaders:
- -include../lib/test/FakeAvr/interrupt.h
- -include../lib/test/FakeAvr/io.h
```

### Final new variables
- app libs (separated because of path):

```make
applib := Example
test_app := $(addprefix test_,${applib})
test_app_o := $(addsuffix .o,${test_app})
test_app_o := $(addprefix build/,${test_app_o})
app_o := $(addsuffix .o,${applib})
app_o := $(addprefix build/,${app_o})
```

- hardware libs:

```make
hwlib := BiColorLed SpiSlave UartSpi Lis
```

- non-hardware libs:

```make
nohwlib := ReadWriteBits Queue
```

- group hw and nohw libs to make pre-reqs:

```make
Hardware := ${hwlib}
HardwareFakes := ${Hardware}
Hardware := $(addsuffix -Hardware.h,${Hardware})
Hardware := $(addprefix src/,${Hardware})
HardwareFakes := $(addsuffix -HardwareFake.h,${HardwareFakes})
HardwareFakes := $(addprefix ../lib/test/,${HardwareFakes})
```

- group hw and nohw libs to make build targets and header
  names

```make
all_libs := ${hwlib} ${nohwlib}
lib_o := $(addsuffix .o,${all_libs})
lib_o := $(addprefix ../lib/build/,${lib_o})
lib_headers := $(addsuffix .h,${all_libs})
lib_headers := $(addprefix ../lib/src/,${lib_headers})
```

### Final output

```Makefile-variables
hwlib:
- BiColorLed SpiSlave UartSpi Lis
nohwlib:
- ReadWriteBits Queue
Hardware:
- src/BiColorLed-Hardware.h
- src/SpiSlave-Hardware.h
- src/UartSpi-Hardware.h
- src/Lis-Hardware.h
HardwareFakes:
- ../lib/test/BiColorLed-HardwareFake.h
- ../lib/test/SpiSlave-HardwareFake.h
- ../lib/test/UartSpi-HardwareFake.h
- ../lib/test/Lis-HardwareFake.h
lib_o:
- ../lib/build/BiColorLed.o
- ../lib/build/SpiSlave.o
- ../lib/build/UartSpi.o
- ../lib/build/Lis.o
- ../lib/build/ReadWriteBits.o
- ../lib/build/Queue.o
lib_headers:
- ../lib/src/BiColorLed.h
- ../lib/src/SpiSlave.h
- ../lib/src/UartSpi.h
- ../lib/src/Lis.h
- ../lib/src/ReadWriteBits.h
- ../lib/src/Queue.h
IncludeAvrHeaders:
- -includeavr/interrupt.h
- -includeavr/io.h
IncludeFakeAvrHeaders:
- -include../lib/test/FakeAvr/interrupt.h
- -include../lib/test/FakeAvr/io.h
```



## [x] clean `lib/Makefile`

```Makefile-variables
hwlib:
- BiColorLed
- SpiSlave
- UartSpi
- Lis
nohwlib:
- ReadWriteBits
- Queue
HardwareFakes:
- test/BiColorLed-HardwareFake.h
- test/SpiSlave-HardwareFake.h
- test/UartSpi-HardwareFake.h
- test/Lis-HardwareFake.h
lib_o:
- build/BiColorLed.o
- build/SpiSlave.o
- build/UartSpi.o
- build/Lis.o
- build/ReadWriteBits.o
- build/Queue.o
lib_headers:
- src/BiColorLed.h
- src/SpiSlave.h
- src/UartSpi.h
- src/Lis.h
- src/ReadWriteBits.h
- src/Queue.h
IncludeFakeAvrHeaders:
- -includetest/FakeAvr/interrupt.h
- -includetest/FakeAvr/io.h
```

## 2019-12-13
- right now, avr-gcc build is 202 bytes

```bash
$ date | clip
Tue, Dec 10, 2019 11:58:42 PM
$ avr-size.exe vis-spi-out/build/vis-spi-out.elf | clip
text | data|bss|dec|hex|filename
202  | 0   |0  |202|ca |vis-spi-out/build/vis-spi-out.elf
```
- copy in code
- set up lib calls
- set up tests
- check build size does not suddenly jump
- exercise everything figured out so far
- add a lib

## 2019-12-14 through 2019-12-18
- right now, avr-gcc build is 284 bytes

```bash
$ date | clip
Tue, Dec 17, 2019 12:14:11 AM
$ avr-size.exe build/vis-spi-out.elf | clip
   text	   data	    bss	    dec	    hex	filename
    284	      0	     21	    305	    131	build/vis-spi-out.elf
```

- next up for code to move is:

### [x] `ISR(SPI_STC_vect)`
- add code, check size increase, check assembly

```bash
Tue, Dec 17, 2019 12:25:54 AM
text	   data	    bss	    dec	    hex	filename
 434	      2	     21	    457	    1c9	build/vis-spi-out.elf
```

- wow, size jumped up 150 bytes from 284 to 434 bytes
- why?

#### try just adding the interrupt

- calculate time for empty ISR routine:
    - Total number of cycles: 19
    - Total number of instructions: 10

```asm
ISR(SPI_STC_vect)
{
  fc:	1f 92       	push	r1
  fe:	0f 92       	push	r0
 100:	0f b6       	in	r0, 0x3f	; 63
 102:	0f 92       	push	r0
 104:	11 24       	eor	r1, r1
 106:	0f 90       	pop	r0
 108:	0f be       	out	0x3f, r0	; 63
 10a:	0f 90       	pop	r0
 10c:	1f 90       	pop	r1
 10e:	18 95       	reti
```

- size increases 20 bytes from 284 to 304 bytes

#### try adding the call to QueueIsFull

- calculate time for ISR routine with call to QueueIsFull :
    - Total number of cycles: 61
    - Total number of instructions: 32

```asm
ISR(SPI_STC_vect)
{
  fc:	1f 92       	push	r1
  fe:	0f 92       	push	r0
 100:	0f b6       	in	r0, 0x3f	; 63
 102:	0f 92       	push	r0
 104:	11 24       	eor	r1, r1
 106:	2f 93       	push	r18
 108:	3f 93       	push	r19
 10a:	8f 93       	push	r24
 10c:	9f 93       	push	r25
 10e:	ef 93       	push	r30
 110:	ff 93       	push	r31
    // ISR triggers when a SPI transfer completes.
    if (QueueIsFull(SpiFifo))
 112:	e0 91 00 01 	lds	r30, 0x0100	; 0x800100 <__data_end>
 116:	f0 91 01 01 	lds	r31, 0x0101	; 0x800101 <__data_end+0x1>
{ //! Return true if Queue is full
    /** QueueIsFull behavior:\n 
      * - returns true if Queue is full\n 
      * - returns false if Queue is not full\n 
      * */
    if (pq->length >= pq->max_length) return true;
 11a:	24 81       	ldd	r18, Z+4	; 0x04
 11c:	35 81       	ldd	r19, Z+5	; 0x05
 11e:	86 81       	ldd	r24, Z+6	; 0x06
 120:	97 81       	ldd	r25, Z+7	; 0x07
 122:	28 17       	cp	r18, r24
 124:	39 07       	cpc	r19, r25
 126:	08 f0       	brcs	.+2      	; 0x12a <__vector_17+0x2e>
 128:	40 9a       	sbi	0x08, 0	; 8
 12a:	ff 91       	pop	r31
 12c:	ef 91       	pop	r30
 12e:	9f 91       	pop	r25
 130:	8f 91       	pop	r24
 132:	3f 91       	pop	r19
 134:	2f 91       	pop	r18
 136:	0f 90       	pop	r0
 138:	0f be       	out	0x3f, r0	; 63
 13a:	0f 90       	pop	r0
 13c:	1f 90       	pop	r1
 13e:	18 95       	reti
```

- size increases 48 bytes from 304 to 352 bytes

#### try adding the call to QueuePush

- calculate time for ISR routine with two Queue calls:
    - Total number of cycles: 130
    - Total number of instructions: 71

```asm
ISR(SPI_STC_vect)
{
  fc:	1f 92       	push	r1
  fe:	0f 92       	push	r0
 100:	0f b6       	in	r0, 0x3f	; 63
 102:	0f 92       	push	r0
 104:	11 24       	eor	r1, r1
 106:	2f 93       	push	r18
 108:	3f 93       	push	r19
 10a:	4f 93       	push	r20
 10c:	8f 93       	push	r24
 10e:	9f 93       	push	r25
 110:	af 93       	push	r26
 112:	bf 93       	push	r27
 114:	ef 93       	push	r30
 116:	ff 93       	push	r31
    // ISR triggers when a SPI transfer completes.
    if (QueueIsFull(SpiFifo))
 118:	e0 91 02 01 	lds	r30, 0x0102	; 0x800102 <__data_end>
 11c:	f0 91 03 01 	lds	r31, 0x0103	; 0x800103 <__data_end+0x1>
{ //! Return true if Queue is full
    /** QueueIsFull behavior:\n 
      * - returns true if Queue is full\n 
      * - returns false if Queue is not full\n 
      * */
    if (pq->length >= pq->max_length) return true;
 120:	24 81       	ldd	r18, Z+4	; 0x04
 122:	35 81       	ldd	r19, Z+5	; 0x05
 124:	86 81       	ldd	r24, Z+6	; 0x06
 126:	97 81       	ldd	r25, Z+7	; 0x07
 128:	28 17       	cp	r18, r24
 12a:	39 07       	cpc	r19, r25
 12c:	78 f0       	brcs	.+30     	; 0x14c <__vector_17+0x50>
 12e:	40 9a       	sbi	0x08, 0	; 8
 130:	ff 91       	pop	r31
 132:	ef 91       	pop	r30
 134:	bf 91       	pop	r27
 136:	af 91       	pop	r26
 138:	9f 91       	pop	r25
 13a:	8f 91       	pop	r24
 13c:	4f 91       	pop	r20
 13e:	3f 91       	pop	r19
 140:	2f 91       	pop	r18
 142:	0f 90       	pop	r0
 144:	0f be       	out	0x3f, r0	; 63
 146:	0f 90       	pop	r0
 148:	1f 90       	pop	r1
 14a:	18 95       	reti
    else
    {
        // Buffer the received byte in the queue
        QueuePush(SpiFifo, *Spi_spdr);
 14c:	a0 91 00 01 	lds	r26, 0x0100	; 0x800100 <Spi_spdr>
 150:	b0 91 01 01 	lds	r27, 0x0101	; 0x800101 <Spi_spdr+0x1>
 154:	4c 91       	ld	r20, X
 156:	24 81       	ldd	r18, Z+4	; 0x04
 158:	35 81       	ldd	r19, Z+5	; 0x05
 15a:	86 81       	ldd	r24, Z+6	; 0x06
 15c:	97 81       	ldd	r25, Z+7	; 0x07
 15e:	28 17       	cp	r18, r24
 160:	39 07       	cpc	r19, r25
 162:	30 f7       	brcc	.-52     	; 0x130 <__vector_17+0x34>
      * - does not write byte if Queue is full\n 
      * - hits end of buffer and wraps around if Queue is not full\n 
      * */
    if (QueueIsFull(pq)) return;
    // wrap head to beginning of buffer when it reaches the end of the buffer
    if (pq->head >= pq->max_length) pq->head = 0;
 164:	82 81       	ldd	r24, Z+2	; 0x02
 166:	26 81       	ldd	r18, Z+6	; 0x06
 168:	37 81       	ldd	r19, Z+7	; 0x07
 16a:	90 e0       	ldi	r25, 0x00	; 0
 16c:	82 17       	cp	r24, r18
 16e:	93 07       	cpc	r25, r19
 170:	08 f0       	brcs	.+2      	; 0x174 <__vector_17+0x78>
 172:	12 82       	std	Z+2, r1	; 0x02
    pq->buffer[pq->head++] = data;
 174:	a0 81       	ld	r26, Z
 176:	b1 81       	ldd	r27, Z+1	; 0x01
 178:	82 81       	ldd	r24, Z+2	; 0x02
 17a:	91 e0       	ldi	r25, 0x01	; 1
 17c:	98 0f       	add	r25, r24
 17e:	92 83       	std	Z+2, r25	; 0x02
 180:	a8 0f       	add	r26, r24
 182:	b1 1d       	adc	r27, r1
 184:	4c 93       	st	X, r20
    pq->length++;
 186:	84 81       	ldd	r24, Z+4	; 0x04
 188:	95 81       	ldd	r25, Z+5	; 0x05
 18a:	01 96       	adiw	r24, 0x01	; 1
 18c:	95 83       	std	Z+5, r25	; 0x05
 18e:	84 83       	std	Z+4, r24	; 0x04
        // Non-ISR code must pop this byte from SpiFifo queue,
        // even if it does not need it.
    }
}
 190:	cf cf       	rjmp	.-98     	; 0x130 <__vector_17+0x34>
```

- size increases 82 bytes from 352 to 343 bytes
- it looks like the more members of the global Queue struct
  that a call accesses, the more working space the ISR needs,
  so it has *many* more registers to stash and unstash
- and a Push is an expensive operation because it hits several
  members of the struct
- ideally I would not do a Push in the ISR, but I cannot
  think of any alternative
- so the ISR is a beast
- even with Queue inlined, it takes time to run the ISR
  because of the setup and teardown to handle all the values:
  the data, the head, the tail, the length, etc.

### [x] how costly is having QueuePush in the ISR?
- how many cycles is the ISR with QueuePush?
    - copy .avra code into a buffer
    - `;avrt` to calculate time in cycles
- 19 cycles:
    - ISR with no calls (empty)
- 61 cycles:
    - ISR with a call to `QueueIsFull`
- 130 cycles:
    - ISR with a calls to:
        - `QueueIsFull`
        - `QueuePush`
- so:
    - call to `QueueIsFull` is an additional 42 cycles
    - call to `QueuePush` is an additional 69 cycles

### list different scenarios when the ISR is triggered
- what is the processor doing while this runs?
- it *knows* it is waiting on a transfer:
    - just sit in a blocking loop
- it is still busy with the last received command when:
    - more data is sent for the last command
    - a new command is sent
- it is writing and not waiting for any data
    - the interrupt triggers on both writes and reads

### explore flags for speed up
- is it faster to have a *SPI read* flag?
- at a minimum I should eliminate the call to `QueueIsFull`
- `QueuePush` will handle the case that the `QueueIsFull`

### idea 1: ISR checks the flag to decide whether to queue the data
- decision:
    - if *SPI read* is clear, do not push onto queue
    - if *SPI read* is set, push onto queue
- expect speed up for frame writes:
    - it's more data to work with
    - but in the case of sending a frame of data, the write
      should go faster if the ISR is not pushing onto the
      stack
    - this is probably the big hit causing sluggishness
    - the wasted ISR calls during the writes

### idea 2: ISR just sets flags
- main loop checks flags in a round-robin and acts on the flags
- actually, thinking of it this way, if there is nothing
  I can do about the Queue being full, there is no help
  in checking it in the ISR
- right now I only check it because I can and I want to
  set an LED red
- that is a real waste of time
- I should only call QueueIsFull when there is something
  actionable to be done about a full Queue

### Assembly for flags-only in ISR

```asm
ISR(SPI_STC_vect)
{
 160:	1f 92       	push	r1
 162:	0f 92       	push	r0
 164:	0f b6       	in	r0, 0x3f	; 63
 166:	0f 92       	push	r0
 168:	11 24       	eor	r1, r1
 16a:	8f 93       	push	r24
    if (byte_received)
 16c:	80 91 03 01 	lds	r24, 0x0103	; 0x800103 <byte_received>
 170:	81 11       	cpse	r24, r1
 172:	09 c0       	rjmp	.+18     	; 0x186 <__vector_17+0x26>
        ERROR_byte_lost = true;
    }
    else
    {
        // Set flag: received byte over SPI
        byte_received = true;
 174:	81 e0       	ldi	r24, 0x01	; 1
 176:	80 93 03 01 	sts	0x0103, r24	; 0x800103 <byte_received>
    }
}
 17a:	8f 91       	pop	r24
 17c:	0f 90       	pop	r0
 17e:	0f be       	out	0x3f, r0	; 63
 180:	0f 90       	pop	r0
 182:	1f 90       	pop	r1
 184:	18 95       	reti
ISR(SPI_STC_vect)
{
    if (byte_received)
    {
        // Another transfer finished before reading previous byte
        ERROR_byte_lost = true;
 186:	81 e0       	ldi	r24, 0x01	; 1
 188:	80 93 02 01 	sts	0x0102, r24	; 0x800102 <__data_end>
    else
    {
        // Set flag: received byte over SPI
        byte_received = true;
    }
}
 18c:	8f 91       	pop	r24
 18e:	0f 90       	pop	r0
 190:	0f be       	out	0x3f, r0	; 63
 192:	0f 90       	pop	r0
 194:	1f 90       	pop	r1
 196:	18 95       	reti
```

### Using flags makes the ISR shorter and faster
- calculate time for ISR routine with flags instead of Queue
  calls:
    - Total number of cycles: 49
    - Total number of instructions: 25
- the ISR is shorter and faster
- but total code size does not change
    - total code size actually grew a few bytes because I changed
      from:
        - a red LED as an error handler placeholder
    - to:
        - setting an `ERROR_flag`

### takeaways so far: move out QueueIsFull
- the overhead of ISR setup/teardown to make space for making
  Queue calls is no different than the overhead in calling
  these functions outside the ISR
- the real issue, therefore, is speed, not overall code size
- get *out* of the ISR as quick as possible by avoiding calls
- at a minimum eliminate the call to QueueIsFull in the ISR
- flags are a way to eliminate the call to QueuePush
- but flags make the code more complicated
- try using flags for speed ups to branch around calls instead of
  eliminating calls in the ISR

### checking `byte_received` flag is slower than checking `QueueIsEmpty`
- main loop with call to QueueIsEmpty:
```c
    // Idle until a command is received
    while (QueueIsEmpty(SpiFifo));
```
- Total number of cycles: 9
- Total number of instructions: 5
```asm
ldd	r24, Z+4	; 0x04
ldd	r25, Z+5	; 0x05
or	r24, r25
breq	.-8      	; 0x100 <main+0x5a>
rjmp	.-34     	; 0xe8 <main+0x42>
```

- try checking a `byte_received` flag instead of checking if the
  Queue is empty
- main loop with flag:
```c
    while (!byte_received);
    // Clear flag.
    byte_received = false;
```
- Total number of cycles: 7
- Total number of instructions: 4
```asm
 lds	r24, 0x0102	; 0x800102 <__data_end>
 and	r24, r24
 breq	.-8      	; 0xf8 <main+0x52>
 sts	0x0102, r1	; 0x800102 <__data_end>
```

### takeaway: do not use a `byte_received` flag
- the flag cuts two cycles off the while loop, but it adds some
  cycles to the ISR to set this flag!
- that is the **opposite** of what I'm trying to do
- I want faster ISR code at the expense of slower non-ISR code

### idea 3: QueuePush is only call in ISR and depends on flag
- only call is QueuePush
- and it is only called if the flag is set
- the flag indicates the processor is in a state where it is
  expecting data from the master

#### what is the speed difference
- difference between executing the ISR when:
    - the flag *is set*: QueuePush is called
    - vs:
    - the flag *is not set*: QueuePush is skipped

#### C code without any flags to check in ISR

```c
ISR(SPI_STC_vect)
{
    QueuePush(SpiFifo, *Spi_spdr);
}
```

#### Assembly code without any flags to check in ISR

```asm
ISR(SPI_STC_vect)
{
 10a:	1f 92       	push	r1
 10c:	0f 92       	push	r0
 10e:	0f b6       	in	r0, 0x3f	; 63
 110:	0f 92       	push	r0
 112:	11 24       	eor	r1, r1
 114:	2f 93       	push	r18
 116:	3f 93       	push	r19
 118:	4f 93       	push	r20
 11a:	8f 93       	push	r24
 11c:	9f 93       	push	r25
 11e:	af 93       	push	r26
 120:	bf 93       	push	r27
 122:	ef 93       	push	r30
 124:	ff 93       	push	r31
    QueuePush(SpiFifo, *Spi_spdr); // "client" must pop data from SpiFifo queue
 126:	e0 91 00 01 	lds	r30, 0x0100	; 0x800100 <Spi_spdr>
 12a:	f0 91 01 01 	lds	r31, 0x0101	; 0x800101 <Spi_spdr+0x1>
 12e:	40 81       	ld	r20, Z
 130:	e0 91 02 01 	lds	r30, 0x0102	; 0x800102 <__data_end>
 134:	f0 91 03 01 	lds	r31, 0x0103	; 0x800103 <__data_end+0x1>
{ //! Return true if Queue is full
    /** QueueIsFull behavior:\n 
      * - returns true if Queue is full\n 
      * - returns false if Queue is not full\n 
      * */
    if (pq->length >= pq->max_length) return true;
 138:	24 81       	ldd	r18, Z+4	; 0x04
 13a:	35 81       	ldd	r19, Z+5	; 0x05
 13c:	86 81       	ldd	r24, Z+6	; 0x06
 13e:	97 81       	ldd	r25, Z+7	; 0x07
 140:	28 17       	cp	r18, r24
 142:	39 07       	cpc	r19, r25
 144:	a8 f4       	brcc	.+42     	; 0x170 <__vector_17+0x66>
      * - does not write byte if Queue is full\n 
      * - hits end of buffer and wraps around if Queue is not full\n 
      * */
    if (QueueIsFull(pq)) return;
    // wrap head to beginning of buffer when it reaches the end of the buffer
    if (pq->head >= pq->max_length) pq->head = 0;
 146:	82 81       	ldd	r24, Z+2	; 0x02
 148:	26 81       	ldd	r18, Z+6	; 0x06
 14a:	37 81       	ldd	r19, Z+7	; 0x07
 14c:	90 e0       	ldi	r25, 0x00	; 0
 14e:	82 17       	cp	r24, r18
 150:	93 07       	cpc	r25, r19
 152:	e0 f4       	brcc	.+56     	; 0x18c <__vector_17+0x82>
    pq->buffer[pq->head++] = data;
 154:	a0 81       	ld	r26, Z
 156:	b1 81       	ldd	r27, Z+1	; 0x01
 158:	82 81       	ldd	r24, Z+2	; 0x02
 15a:	91 e0       	ldi	r25, 0x01	; 1
 15c:	98 0f       	add	r25, r24
 15e:	92 83       	std	Z+2, r25	; 0x02
 160:	a8 0f       	add	r26, r24
 162:	b1 1d       	adc	r27, r1
 164:	4c 93       	st	X, r20
    pq->length++;
 166:	84 81       	ldd	r24, Z+4	; 0x04
 168:	95 81       	ldd	r25, Z+5	; 0x05
 16a:	01 96       	adiw	r24, 0x01	; 1
 16c:	95 83       	std	Z+5, r25	; 0x05
 16e:	84 83       	std	Z+4, r24	; 0x04
}
 170:	ff 91       	pop	r31
 172:	ef 91       	pop	r30
 174:	bf 91       	pop	r27
 176:	af 91       	pop	r26
 178:	9f 91       	pop	r25
 17a:	8f 91       	pop	r24
 17c:	4f 91       	pop	r20
 17e:	3f 91       	pop	r19
 180:	2f 91       	pop	r18
 182:	0f 90       	pop	r0
 184:	0f be       	out	0x3f, r0	; 63
 186:	0f 90       	pop	r0
 188:	1f 90       	pop	r1
 18a:	18 95       	reti
      * - does not write byte if Queue is full\n 
      * - hits end of buffer and wraps around if Queue is not full\n 
      * */
    if (QueueIsFull(pq)) return;
    // wrap head to beginning of buffer when it reaches the end of the buffer
    if (pq->head >= pq->max_length) pq->head = 0;
 18c:	12 82       	std	Z+2, r1	; 0x02
 18e:	e2 cf       	rjmp	.-60     	; 0x154 <__vector_17+0x4a>
```

#### calculate time for this ISR routine
- Total number of cycles: 116
- Total number of instructions: 63

#### C code with flag to check before QueuePush

```c
volatile bool listening = true;
// ...
ISR(SPI_STC_vect)
{
    // Push data onto Queue when listening for SPI Master.
    if (listening) QueuePush(SpiFifo, *Spi_spdr);
}
```

#### Assembly code with flag to check before QueuePush
```asm
ISR(SPI_STC_vect)
{
 10a:	1f 92       	push	r1
 10c:	0f 92       	push	r0
 10e:	0f b6       	in	r0, 0x3f	; 63
 110:	0f 92       	push	r0
 112:	11 24       	eor	r1, r1
 114:	2f 93       	push	r18
 116:	3f 93       	push	r19
 118:	4f 93       	push	r20
 11a:	8f 93       	push	r24
 11c:	9f 93       	push	r25
 11e:	af 93       	push	r26
 120:	bf 93       	push	r27
 122:	ef 93       	push	r30
 124:	ff 93       	push	r31
    // Push data onto Queue when listening for SPI Master.
    if (listening) QueuePush(SpiFifo, *Spi_spdr);
 126:	80 91 00 01 	lds	r24, 0x0100	; 0x800100 <__data_start>
 12a:	88 23       	and	r24, r24
 12c:	81 f0       	breq	.+32     	; 0x14e <__vector_17+0x44>
 12e:	e0 91 01 01 	lds	r30, 0x0101	; 0x800101 <Spi_spdr>
 132:	f0 91 02 01 	lds	r31, 0x0102	; 0x800102 <Spi_spdr+0x1>
 136:	40 81       	ld	r20, Z
 138:	e0 91 04 01 	lds	r30, 0x0104	; 0x800104 <__data_end>
 13c:	f0 91 05 01 	lds	r31, 0x0105	; 0x800105 <__data_end+0x1>
{ //! Return true if Queue is full
    /** QueueIsFull behavior:\n 
      * - returns true if Queue is full\n 
      * - returns false if Queue is not full\n 
      * */
    if (pq->length >= pq->max_length) return true;
 140:	24 81       	ldd	r18, Z+4	; 0x04
 142:	35 81       	ldd	r19, Z+5	; 0x05
 144:	86 81       	ldd	r24, Z+6	; 0x06
 146:	97 81       	ldd	r25, Z+7	; 0x07
 148:	28 17       	cp	r18, r24
 14a:	39 07       	cpc	r19, r25
 14c:	70 f0       	brcs	.+28     	; 0x16a <__vector_17+0x60>
}
 14e:	ff 91       	pop	r31
 150:	ef 91       	pop	r30
 152:	bf 91       	pop	r27
 154:	af 91       	pop	r26
 156:	9f 91       	pop	r25
 158:	8f 91       	pop	r24
 15a:	4f 91       	pop	r20
 15c:	3f 91       	pop	r19
 15e:	2f 91       	pop	r18
 160:	0f 90       	pop	r0
 162:	0f be       	out	0x3f, r0	; 63
 164:	0f 90       	pop	r0
 166:	1f 90       	pop	r1
 168:	18 95       	reti
      * - does not write byte if Queue is full\n 
      * - hits end of buffer and wraps around if Queue is not full\n 
      * */
    if (QueueIsFull(pq)) return;
    // wrap head to beginning of buffer when it reaches the end of the buffer
    if (pq->head >= pq->max_length) pq->head = 0;
 16a:	82 81       	ldd	r24, Z+2	; 0x02
 16c:	26 81       	ldd	r18, Z+6	; 0x06
 16e:	37 81       	ldd	r19, Z+7	; 0x07
 170:	90 e0       	ldi	r25, 0x00	; 0
 172:	82 17       	cp	r24, r18
 174:	93 07       	cpc	r25, r19
 176:	08 f0       	brcs	.+2      	; 0x17a <__vector_17+0x70>
 178:	12 82       	std	Z+2, r1	; 0x02
    pq->buffer[pq->head++] = data;
 17a:	a0 81       	ld	r26, Z
 17c:	b1 81       	ldd	r27, Z+1	; 0x01
 17e:	82 81       	ldd	r24, Z+2	; 0x02
 180:	91 e0       	ldi	r25, 0x01	; 1
 182:	98 0f       	add	r25, r24
 184:	92 83       	std	Z+2, r25	; 0x02
 186:	a8 0f       	add	r26, r24
 188:	b1 1d       	adc	r27, r1
 18a:	4c 93       	st	X, r20
    pq->length++;
 18c:	84 81       	ldd	r24, Z+4	; 0x04
 18e:	95 81       	ldd	r25, Z+5	; 0x05
 190:	01 96       	adiw	r24, 0x01	; 1
 192:	95 83       	std	Z+5, r25	; 0x05
 194:	84 83       	std	Z+4, r24	; 0x04
 196:	db cf       	rjmp	.-74     	; 0x14e <__vector_17+0x44>

```

#### calculate time for this ISR routine
- Total number of cycles: 121
- Total number of instructions: 66

#### slow down when listening is negligible
- 116 cycles: *always* push onto Queue, no flag
- 121 cycles: check for `listening` flag before pushing

#### code skipped if `listening` flag is not set

```asm
  lds	r30, 0x0101	; 0x800101 <Spi_spdr>
  lds	r31, 0x0102	; 0x800102 <Spi_spdr+0x1>
  ld	r20, Z
  lds	r30, 0x0104	; 0x800104 <__data_end>
  lds	r31, 0x0105	; 0x800105 <__data_end+0x1>
  ldd	r18, Z+4	; 0x04
  ldd	r19, Z+5	; 0x05
  ldd	r24, Z+6	; 0x06
  ldd	r25, Z+7	; 0x07
  cp	r18, r24
  cpc	r19, r25
  brcs	.+28     	; 0x16a <__vector_17+0x60>
```

#### skipping QueuePush saves 61 cycles!
Total number of cycles: 61
Total number of instructions: 35

- How much time does this save?
- Reduce 121 cycles to 61 cycles
- at 10MHz, this saves 6.1µs
    - `61*1/10.0e6= 6.1e-6`
- for comparison, ref-dev-kit-mike code is 151 cycles:
    - 91 cycles for the ISR
    - plus 60 cycles for a call to QueuePush

#### time savings justifies the added complexity
- flag is *clear* when writing a frame
- a frame is 784 bytes
- that is 784 times that 6.1µs is saved
- then the time wasted in the ISR is 4.782ms
    - `6.1e-6*784 = 0.004782`
- but the client has to pop the garbage byte off the Queue
- assume that also takes 6.1µs
- then time per byte sent is 12.2µs
- and the total time saved in sending a frame is 9.565ms
    - `12.2e-6*784 = 0.009565` 
- that is almost 10ms
- compared with a 1ms exposure time, plus the time to actually
  send the data, shaving off 10ms is *just* long enough to make a
  noticeable decrease in the snappiness of a live response
- this speed-up is worth dealing with the state introduced by the
  flag
- keeping the code simpler is not worth adding 10ms of overhead
  to send a frame

### [x] unit test BiColorLedRed
- I added it to the code without testing it

# [ ] Add function `ReplyCommandInvalid`
- [ ] fix `loop` (use this function instead of placeholder)
- [ ] add this function
- [ ] hmm.... calls `WriteSpiMaster`
    - logically part of lib `SpiSlave`
    - but `WriteSpiMaster` uses lib `Queue`
    - what happens if I make this an `inline` function in `SpiSlave.h`
        - name it `SpiSlaveTx` (because the SpiSlave is transmitting to the
          SpiMaster)
    - what do I *think* will happen?
    - register accesses I care about are either:
        - directly bit-accessible
        - accessible as registers without adding 0x20 to the address
    - all of these register accesses are for lib SpiSlave
    - lib Queue has no such register accesses
    - lib Queue is just a data object


# [-] unit test SpiSlaveTx

## Code size before adding new code is 612 bytes

```date-and-size
Thu, Jan  2, 2020  6:30:58 PM
   text	   data	    bss	    dec	    hex	filename
    612	      2	     21	    635	    27b	build/vis-spi-out.elf
```

## After adding code that writes to `SPDR` size is 626 bytes

```date-and-size
Fri, Jan  3, 2020  5:30:44 PM
   text	   data	    bss	    dec	    hex	filename
    626	      2	     21	    649	    289	build/vis-spi-out.elf
```

## SpiSlaveTx lib code (WIP)

```c
// SpiSlave.h
//
for (byte_index = 0; byte_index < nbytes; byte_index++)
{
    *Spi_SPDR = input_buffer[byte_index];
}
```

## vis-spi-out code (placeholder)

```c
// vis-spi-out.c
//
uint8_t const input_buffer[] = {0x0A, 0x1B, 0x2C};
uint16_t nbytes = 3;
SpiSlaveTx(input_buffer, nbytes);
```

## assembly is split in two parts
- first the values are stored in working registers

```avra
  f6:	8a e0       	ldi	r24, 0x0A	; 10
  f8:	f8 2e       	mov	r15, r24
  fa:	0b e1       	ldi	r16, 0x1B	; 27
  fc:	1c e2       	ldi	r17, 0x2C	; 44
```

- later the values are loaded into `SPDR`

```avra
 12c:	f8 82       	st	Y, r15
 12e:	08 83       	st	Y, r16
 130:	18 83       	st	Y, r17
```

- Total number of cycles: 10
- Total number of instructions: 7

## assembly is `inline` (no call), but not optimized

- [x] Why aren't the instructions optimized?

- `SPDR` is `0x2E`
- so it is possible to do:

```avra
; say r24 has the value to send
out	0x2e, r24
```

- so why isn't `avr-gcc` doing this?
- is `st` faster?
- no, something is wrong
- here is the code and some analysis:

## fail: expect direct write to `SPDR`, but writes to mem
- `lds` (Load direct from SRAM) is 2 cycles
- load SRAM addresses `0x0100` and `0x0101` into `Y`

```avra
  ee:	lds	r28, 0x0100	; 0x800100 <Spi_SPDR>
  f2:	lds	r29, 0x0101	; 0x800101 <Spi_SPDR+0x1>
```

- `ldi` (Load immediate) is 1 cycle
- loads literal into register

```avra
  f6:	ldi	r15, 0x0A	; 10
  f8:	ldi	r16, 0x1B	; 27
  fa:	ldi	r17, 0x2C	; 44
```

- `st` (Store indirect) is 2 cycles
- stores the register value at the address stored in `Y`

```avra
 12c:	st	Y, r15
 12e:	st	Y, r16
 130:	st	Y, r17
```

- I am writing these values to some random spot in SRAM?
- that means I am failing to write to `SPDR` register `0x2e`

- [x] why isn't `*SPDR = 0x0A` translating to something like:
    - `ldi	r15,0x0a`
    - `out	0x2e,r15`

## solution: inline all lib functions
- I think the issue is that `SpiSlave.o` gets the hardware def,
  but the function is inlined in `vis-spi-out.o` and that code
  misses the def
    - [x] test this theory
    - the defs for `SpiSlaveInit` work because `SpiSlave.o` gets
      the hardware def
    - `SpiSlaveInit` is optimized
    - `vis-spi-out.o` just calls `SpiSlaveInit`
- see completed task `inline all lib functions`
- build `vis-spi-out.o` with hardware defs 

# ---Pending Tasks---

# [ ] unit test SpiSlaveTx
- fixed issue about optimized lib code

## here is the placeholder for testing the fix
- this shows correct use of `SPDR` with the `in` instruction

```c
uint8_t garbage = ReadSpiDataRegister();
```

```avra
in	r24, 0x2e	; 46
```

```c
    if (garbage==0xFC) BiColorLedRed(led_0);
```

```avra
cpi	r24, 0xFC	; 252
breq	.+6      	; 0x1ea <main+0x144>
```

## erase placeholder
- before erasing placeholder

```date-and-size
Tue, Jan  7, 2020  4:15:42 PM
   text	   data	    bss	    dec	    hex	filename
    624	      0	     21	    645	    285	build/vis-spi-out.elf
```

- after erasing placeholder

```date-and-size
Tue, Jan  7, 2020  4:19:54 PM
   text	   data	    bss	    dec	    hex	filename
    604	      0	     21	    625	    271	build/vis-spi-out.elf
```

## QueuePop in loop() or not?
- change arg `*SPDR`

```c
Cmd* DoSensorCmd = LookupSensorCmd(*Spi_SPDR);
```

```avra
in	r24, 0x2e	; 46
```

- to `QueuePop(SpiFifo))`

```c
Cmd* DoSensorCmd = LookupSensorCmd(QueuePop(SpiFifo));
```

```avra
ldd	r24, Z+3	; 0x03
ldd	r18, Z+6	; 0x06
ldd	r19, Z+7	; 0x07
ldi	r25, 0x00	; 0
cp	r24, r18
cpc	r25, r19
brcs	.+2      	; 0x1e2 <main+0x13c>
std	Z+3, r1	; 0x03
ldd	r24, Z+4	; 0x04
ldd	r25, Z+5	; 0x05
sbiw	r24, 0x01	; 1
std	Z+5, r25	; 0x05
std	Z+4, r24	; 0x04
ld	r24, Z
ldd	r25, Z+1	; 0x01
ldd	r18, Z+3	; 0x03
ldi	r19, 0x01	; 1
add	r19, r18
std	Z+3, r19	; 0x03
movw	r30, r24
add	r30, r18
adc	r31, r1
ld	r24, Z
```

- after changing arg size increases 52 bytes

```date-and-size
Tue, Jan  7, 2020  4:21:53 PM
   text	   data	    bss	    dec	    hex	filename
    656	      0	     21	    677	    2a5	build/vis-spi-out.elf
```

- so calling `QueuePop` instead of the direct read from `SPDR`
  adds:
    - 52 bytes of mem
    - Total number of cycles: 38
    - Total number of instructions: 23

- takeaway: every call to `QueuePop` adds 52 bytes to mem size

## return to task of unit-testing `SpiSlaveTx`

```date-and-size
Tue, Jan  7, 2020  4:47:21 PM
   text	   data	    bss	    dec	    hex	filename
    604	      0	     21	    625	    271	build/vis-spi-out.elf
```

- [ ] write `SpiSlaveTxByte`
    - sends just one byte
    - `inline` call in `SpiSlaveTx`
    - gives me a call to fake to test all bytes are sent by
      `SpiSlaveTx`
    - will also help in application as shorthand for sending one
      byte

# [ ] Replace `listening_for_SPIM` with Enable/DisableInterrupt
- [x] delete `listening_for_SPIM`
- instead of checking the flag in the ISR
- [ ] disable the interrupt when writing a frame
- [ ] clear the interrupt flag manually
    - i.e., instead of running the ISR to clear the flag
    - manual-clear is *much* faster than executing the ISR to
      clear the flag!
- ~`EnableSpiInterrupt` is private~
- [x] make `EnableSpiInterrupt` public
- [x] create `DisableSpiInterrupt`
    - do not write a unit test
    - this is low-level assembly
- [x] make all of these functions inline:
    - `EnableSpiInterrupt`
    - `DisableSpiInterrupt`
    - `ClearSpiInterruptFlag`
        - `ReadSpiStatusRegister`
        - `ReadSpiDataRegister`

### [x] LookupSensorCmd
- jump table
- no reason to define different types of jump table function
  pointers
- make a header-only lib called `Cmd`

```c
// firmware/lib/src/Cmd.h
//
#ifndef _CMD_H
#define _CMD_H
//! Functions in the command look up table are type `Cmd`.
typedef void (Cmd)(void);
//! Keys in the command look up table are type `cmd`.
typedef uint8_t cmd;
#endif // _CMD_H
```

- `Cmd` does not show up in list of libs in Makefile, so how does
  it get picked up in the project?
    - `Cmd.h` is in `lib/src` directory
    - `-I../lib/src` enables `vis-spi-out.c` to pick up the `Cmd`
      typedef with `#include "Cmd.h"`
- Example:

```c
// firmware/src/vis-spi-out.c
//
#include "Cmd.h"
//...
void loop(void)
{
    //...
    Cmd* DoSensorCmd = LookupSensorCmd(*Spi_spdr);
    if (DoSensorCmd == NULL) ReplyCommandInvalid();
    else DoSensorCmd();
    //...
}
```

### [x] lib `UartSpi`
- this lib is low-level hardware, needs to be fast!
- inline ~everything~ almost everything
- it seems I can treat this the same way as SpiSlave
    - [ ] how is this happening?
    - I can inline what I want
    - I can also leave other functions private
    - *miraculously, both vis-spi-out.o and UartSpi.o are picking
      up the hardware definitions in their translation units*

#### [x] `UartSpiInit` is tested and assembly optimization is good
- see TiddlyWiki
- the added code is optimized
- adding `UartSpiInit`
    - consumes an additional 76 bytes of program memory:
    - 37 instructions
    - 66 cycles

- before adding `UartSpiInit`:
- [x] check size: 462 bytes
    - Total number of cycles: 347
    - Total number of instructions: 180

```bash
$ date | clip
Thu, Dec 26, 2019  9:46:15 AM
$ avr-size.exe build/vis-spi-out.elf | clip
   text	   data	    bss	    dec	    hex	filename
    462	      2	     21	    485	    1e5	build/vis-spi-out.elf
```

- after adding `UartSpiInit`:
- [x] check size: 538 bytes
    - Total number of cycles: 413
    - Total number of instructions: 217

```bash
$ date | clip
Mon, Dec 23, 2019 10:33:12 AM
$ avr-size.exe build/vis-spi-out.elf | clip
   text	   data	    bss	    dec	    hex	filename
    538	      2	     21	    561	    231	build/vis-spi-out.elf
```

# Hardware files
## simple case: hardware only in `inline` functions in header
- Example: lib `UartSpi`
- three files:
- `lib/src/UartSpi.h`
    - `typedef` for ports, pins, bits
    - `extern` for hardware symbols: ports, pins, bits
    - function declarations
    - inline function bodies
    - many functions include this file
- `vis-spi-out/src/UartSpi-Hardware.h`
    - define hardware ports, pins, bits
    - use *real* AVR values
    - this file is included via `#include` in `vis-spi-out/src/Hardware.h`
    - `vis-spi-out.c` includes `Hardware.h`
- `lib/test/UartSpi-HardwareFake.h`
    - define hardware ports, pins, bits
    - use *fake* values
    - define *fake* values in this file with keyword `static`
    - this file is included via `#include` in `lib/test/HardwareFake.h`
    - `test_runner.c` includes `HardwareFake.h`

### [ ] lib `Lis`
- this lib is low-level hardware, needs to be fast!
- inline everything
- LisInit is done.
    - everything is static so it inlines
    - only call is LisInit, which happens in setup
    - no functions made inline in Lis header yet
    - expect this will change soon

#### [x] LisInit

##### Setup
- add call to in src/vis-spi-out.c
- build for avr-target is broken:

```bash
src/vis-spi-out.c|42 col 5| warning: implicit declaration of function 'LisInit' [-Wimplicit-function-declaration]
```

- add lib name to `lib/Makefile`:

```make
 # lib/Makefile
hw_lib_src := BiColorLed SpiSlave UartSpi Lis
```

- follow instructions in TiddlyWiki for lib `UartSpi`
- then add headers
    - Lis.h
    - Lis-Hardware.h
    - Lis-HardwareFake.h
    - [ ] where is this step documented?
- get a failing unit test

Size before adding `LisInit`:

```bash
$ date | clip
Thu, Jan  2, 2020  8:52:37 AM
$ avr-size.exe build/vis-spi-out.elf | clip
   text	   data	    bss	    dec	    hex	filename
    538	      2	     21	    561	    231	build/vis-spi-out.elf
```

##### First static function call
Add `LisInit` with one static function call:

```c
// Lis.c
//
static void SetPixSelectAsOutput(void)
{
    SetBit(Lis_ddr2, Lis_PixSelect);
}
void LisInit(void)
{
    SetPixSelectAsOutput();
}
//

Get passing test.

Add call to application.

```c
// vis-spi-out.c
//
void setup(void)
{
    // ...
    LisInit();
    // ...
}
```

Check code size.

```bash
$ date | clip
Thu, Jan  2, 2020  8:55:52 AM
$ avr-size.exe build/vis-spi-out.elf | clip
   text	   data	    bss	    dec	    hex	filename
    546	      2	     21	    569	    239	build/vis-spi-out.elf
```

Check that static function call is:

- `inline`
- uses a direct bit-access instruction

```avra
vis-spi-out.avra:

000000a6 <main>:
    ...
    LisInit();
  call	0x21a	; 0x21a <LisInit>

0000021a <LisInit>:
  sbi	0x04, 0	; 4
  ret
```

Code size increase of 8 bytes makes sense:

- Total number of cycles: 10
- Total number of instructions: 3

##### Add more calls

```date-and-size
Thu, Jan  2, 2020  9:55:26 AM
   text	   data	    bss	    dec	    hex	filename
    546	      2	     21	    569	    239	build/vis-spi-out.elf
```

As expected, adding a call that does `cbi` only added two byes to mem:

```date-and-size
Thu, Jan  2, 2020 11:02:51 AM
   text	   data	    bss	    dec	    hex	filename
    548	      2	     21	    571	    23b	build/vis-spi-out.elf
```

```avra
cbi	0x05, 0	; 5
```

- Total number of cycles: 2
- Total number of instructions: 1

##### Add more calls

Before

```date-and-size
Thu, Jan  2, 2020 11:15:09 AM
   text	   data	    bss	    dec	    hex	filename
    548	      2	     21	    571	    23b	build/vis-spi-out.elf
```

```avra
sbi	0x0a, 5	; 10
```

- Total number of cycles: 2
- Total number of instructions: 1

After

```date-and-size
Thu, Jan  2, 2020 11:33:06 AM
   text	   data	    bss	    dec	    hex	filename
    550	      2	     21	    573	    23d	build/vis-spi-out.elf
```

##### Add more calls

Before
```date-and-size
Thu, Jan  2, 2020 11:37:27 AM
   text	   data	    bss	    dec	    hex	filename
    550	      2	     21	    573	    23d	build/vis-spi-out.elf
```

Add

```avra
sbi	0x0a, 6	; 10
```

- Total number of cycles: 2
- Total number of instructions: 1

After

```date-and-size
Thu, Jan  2, 2020 11:47:46 AM
   text	   data	    bss	    dec	    hex	filename
    552	      2	     21	    575	    23f	build/vis-spi-out.elf
```

##### Add more calls

Before

```date-and-size
Thu, Jan  2, 2020 11:48:33 AM
   text	   data	    bss	    dec	    hex	filename
    552	      2	     21	    575	    23f	build/vis-spi-out.elf
```

Add

```avra
cbi	0x0b, 6	; 11
```

- Total number of cycles: 2
- Total number of instructions: 1

After

```date-and-size
Thu, Jan  2, 2020 11:56:34 AM
   text	   data	    bss	    dec	    hex	filename
    554	      2	     21	    577	    241	build/vis-spi-out.elf
```

##### Finished LisInit

Before

```date-and-size
Thu, Jan  2, 2020 11:57:03 AM
   text	   data	    bss	    dec	    hex	filename
    554	      2	     21	    577	    241	build/vis-spi-out.elf
```

After

```date-and-size
Thu, Jan  2, 2020  3:36:58 PM
   text	   data	    bss	    dec	    hex	filename
    612	      2	     21	    635	    27b	build/vis-spi-out.elf
```

All calls checked for `inline` and optimal instruction choice. The 58 byte
increase is correct.

# Add a lib
- add a line of code in `vis-spi-out` that requires lib `Queue`

## add lib Queue
- this is a non-hardware lib

### Edit lib list in `lib/Makefile` and add the files make needs
```make
 # lib/Makefile
 #
 # ---add non-hardware libs here---
lib_src := ${hw_lib_src} ReadWriteBits Queue
```

#### add file `test_Queue.c`
- rebuild with ;mktgc

```bash
make: *** No rule to make target 'test/test_Queue.c', needed by 'build/test_Queue.o'.  Stop.
```

- create `lib/test/test_Queue.c`

```bash
$ touch test/test_Queue.c
```

#### add file `test_Queue.h`
- rebuild with ;mktgc

```bash
make: *** No rule to make target 'test/test_Queue.h', needed by 'build/test_Queue.o'.  Stop.
```

- create `lib/test/test_Queue.h` by creating a placeholder
  function in `test_Queue.c` and using `fh` to generate a header
  file with the prototype

#### add file `Queue.c`
- rebuild with ;mktgc

```bash
make: *** No rule to make target 'src/Queue.c', needed by 'build/Queue.o'.  Stop.
```

- create `lib/src/Queue.c` and rebuild

#### add file `Queue.h`

```bash
make: *** No rule to make target 'src/Queue.h', needed by 'build/Queue.o'.  Stop.
```

- create `lib/src/Queue.h` by creating a placeholder
  function in `Queue.c` and using `fh` to generate a header
  file with the prototype

- `lib/Makefile` unit-test build succeeds

### Edit lib list in `vis-spi-out/Makefile`

```make
 # vis-spi-out/Makefile
 #
 # ---add nonhardware libs here---
nonhw_lib_src := ${inlhw_lib_src} ReadWriteBits Queue
```

- rebuild with ;mktgc
- build is OK, no files to add

## add function body for `QueueInit` to `Queue.c`

### Check impact on program size and try to optimize
- build with ;mka
- check program size

#### adds 60 bytes of program and 10+buffer-size bytes of data
- now build `vis-spi-out.elf` with ;mka
- look at increase in program size: `text`
- look at increase in data: `bss`

- before adding `SpiFifo = QueueInit(...)`

```bash
Tue, Dec 10, 2019 11:58:42 PM
text|   data|    bss|    dec|    hex|filename
202 |   0   |    0  |    202|    ca |build/vis-spi-out.elf
```

- after adding `SpiFifo = QueueInit(...)`

```bash
Wed, Dec 11, 2019 12:48:34 AM
text|   data|    bss|    dec|    hex|filename
262 |   0   |     15|    277|    115|build/vis-spi-out.elf
```

- FIFO buffer size is 5 bytes (only stores 4 bytes)
- this is four of the 15 bytes in `bss`
- FIFO buffer size does not affect `text`
- so I increased the buffer size to 11 bytes (stores 10 bytes)

```bash
Wed, Dec 11, 2019 12:56:43 AM
text|   data|    bss|    dec|    hex|filename
 262|      0|     21|    283|    11b|build/vis-spi-out.elf
```

#### nothing to optimize

- what are the 60 bytes of new instructions for this one
  `QueueInit` call?

- well, making it `inline` by moving it to the header with
  keyword `inline` succeeds in inlining the function call
- but the code size actually grows by eight bytes:

```bash
Wed, Dec 11, 2019  1:05:27 AM
text|   data|    bss|    dec|    hex|filename
 270|      0|     21|    291|    123|build/vis-spi-out.elf
```

- ok, this just is what it is
- nothing to be gained by inline
- no hardware register information will help pick faster
  instructions

### Check vis-spi-out unit-test build again
- ;mktgc
- vis-spi-out unit-test build is still OK

## unit-test `QueueInit`
- [x] leave off here
    - do not try to optimize QueueInit
    - just add test coverage to it

### create test suite

- create a function that runs all tests of the `Queue` lib

```c
// test/test_runner.c
// 
void Queue(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(test_QueueInit_sets_buffer_length);
    }
}
```

- include `test_Queue.h` to declare the test names

```c
// test/test_runner.c
// 
// ---Lists of tests---
#include "test_BiColorLed.h"
#include "test_ReadWriteBits.h"
#include "test_SpiSlave.h"
#include "test_Queue.h"
```

## troubleshoot seg fault when running TestSuite.exe

- run gdb

```bash
$ cd chromation/dev-kit-mike/firmware/lib/build
$ gdb -q TestSuite.exe
```

- find the code causing the segfault:
    - run the code
    - list the code where it crashes

```gdb
(gdb) r
(gdb) list
```

- as expected, `QueueInit` is causing problems
- set a breakpoint at the function that is causing trouble

```gdb
(gdb) b QueueInit
```

- view source and assembly in split view while entering commands
- run to the breakpoint

```gdb
(gdb) layout split
(gdb) r
```

- the code is

```c
volatile Queue_s * pq = &Queue;
```

- the assembly is

```asm
<QueueInit+18>      lea    -0x44e7(%rip),%rax
```

- instruction `lea` is Load Effective Address
- `lea` loads the effective address of %rax into %rip
- what value is in %rax?

```gdb
(gdb) info registers rax
rax            0x5      5
```

- compare this with the working version of `TestSuite.exe` from
  the ref-dev-kit-mike folder
- the surrounding code is identical
- the only difference is the first line of code in the function (where the breakpoint is set)

```asm
<QueueInit+18>      lea    0x1c3a7(%rip),%rax
```

- what value is in %rax?

```gdb
(gdb) info registers rax
rax            0x5      5
```

- so `rax` has the same value as before
- and `rip` seems like it is just the instruction address

- and the only difference between the two is the hex value
- in the bad version the value is -0x44e7
- in the good version the value is 0x1c3a7

- step each program one instruction with `s` and look at the
  values of `rax` again:

```gdb
(gdb) s
```
- good program:
```gdb
rax            0x1004213c0      4299297728
```
- bad program:
```gdb
rax            0x1004012a2      4299166370
```

- both programs are still running!
- so setting `pq = &Queue` was *not* the problem
- `Queue` becomes a problem the moment I try to write to its
  memory
- next line of code is

```
pq->buffer = pqmem;
```

- step again: `s`
- this is the line that causes the seg fault

- `pqmem` is the first arg passed to `QueueInit`
- here is the declaration:
```c
volatile Queue_s * QueueInit(
    volatile uint8_t * buffer, // SPI FIFO Rx Buffer
    uint16_t const size_of_buffer_in_bytes // Max length of Spi Rx Queue
    );
```
- `pqmem` is the address of the buffer created by the caller
- `pq` is a pointer to the `Queue_s` named `Queue`
    - the `Queue` is allocated in `Queue.o`
- `pq->buffer` is the value of element `buffer` in the struct
  `pq` points to
- the `buffer` element is a pointer to a byte:

```c
struct Queue_s {
    volatile uint8_t * buffer; // address of SPI FIFO Rx Buffer
    volatile uint8_t head; // index buffer at head for Push
    volatile uint8_t tail; // index buffer at tail for Pop
    volatile uint16_t length; // number of bytes waiting to be read
    volatile uint16_t max_length; // maximum number of bytes the queue can hold
};
```
- good program:
```gdb
(gdb) p pq
$2 = (volatile Queue_s *) 0x1004213c0 <Queue>
(gdb) p pq->buffer
$3 = (volatile uint8_t *) 0xffffcb19 "\221\022\200\001"
(gdb) p *pqmem
$4 = 145 '\221'
(gdb) p *(pq->buffer)
$5 = 145 '\221'
```
- bad program:
```gdb
(gdb) p pq
$1 = (volatile Queue_s *) 0x1004012a2 <Queue>
(gdb) p pq->buffer
$2 = (volatile uint8_t *) 0x20ec8348e5894855 <error: Cannot access memory at address 0x20ec8348e5894855>
(gdb) p *pqmem
$3 = 145 '\221'
(gdb) p *(pq->buffer)
Cannot access memory at address 0x20ec8348e5894855
(gdb)
```

- the arg, `pqmem`, is the same in both programs
- so `pq->buffer` becomes an invalid memory address
- why?
- well, `pq` is a pointer to `Queue`
- `Queue` is defined in `Queue.o`
- look at value of `Queue`:
- good program:
```gdb
(gdb) p Queue
$6 = {buffer = 0xffffcb19 "\221\022\200\001", head = 0 '\000',
  tail = 0 '\000', length = 0, max_length = 0}
```
- bad program:
```gdb
(gdb) p Queue
$4 = {
  buffer = 0x20ec8348e5894855 <error: Cannot access memory at address 0x20ec8348e5894855>, head = 137 '\211', tail = 200 '\310',
  length = 17800, max_length = 32784}
```

- this is *really* weird
- according to the good program, `Queue` should have 0 `length`
  and 0 `max_length` because those values are not assigned yet
  (assigning `max_length` is the next line of source code)
- but somehow the bad program has huge values for these two
  variables

- the problem is how `Queue` is allocated
- look at `Queue` before execution even starts:
- good program
```gdb
$ gdb -q TestSuite.exe
Reading symbols from TestSuite.exe...done.
(gdb) p Queue
$1 = {buffer = 0x0, head = 0 '\000', tail = 0 '\000', length = 0,
  max_length = 0}
```
- bad program
```gdb
$ gdb -q TestSuite.exe
Reading symbols from TestSuite.exe...done.
(gdb) p Queue
$1 = {void (_Bool)} 0x1004012a2 <Queue>
```
- how is the bad program getting the wrong definition of `Queue`?
- ah fuck -- I named a test suite `Queue`
```c
void Queue(bool run_test)
{
    // ...
}
```

- the names are conflicting
- how did the compiler not catch that with an error or warning of
  some sort!
- fix: rename all test suites with suffix `_tests` to prevent
  this kind of namespace problem from happening again
- example:

```c
void Queue_tests(bool run_test)
```

## Queue abstract data type makes testing boot-strappish

### need QueueLength and QueuePush to test QueueInit
- Queue is an abstract data type
- that means I cannot access members of Queue struct without
  a Queue method
- so I cannot manually put data in the Queue, I have to
  QueuePush
- and I cannot check Queue contents or Queue length, I need
  QueuePop and QueueLength

### need QueueInit to test QueuePush, QueuePop, QueueLength
- these methods operate on a Queue
- I need to initialize a Queue first

### start with empty tests
- use test names to list functionality
- create empty tests like this:

```c
void QueueInit_assigns_input_buffer_as_Queue_buffer(void)
{ TEST_PASS(); }
void QueueInit_size_input_is_the_maximum_Queue_length(void)
{ TEST_PASS(); }
void QueueInit_initializes_Queue_with_length_0(void)
{ TEST_PASS(); }
```

- the first two will probably remain empty
- but when I have `QueueLength` I can check for `length_0`
- and here it is:
```c
void QueueInit_initializes_Queue_with_length_0(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    /* =====[ Operate ]===== */
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(0, QueueLength(SpiFifo));
}
```

## Queue methods `inline` or not
- `QueueInit` is not `inline`
- what about the rest?
- I cannot make them `inline` in the header without exposing the
  `Queue_s` data type
- start with these *not* `inline`, then see if a switch to
  `inline` decreases instruction count in `vis-spi-out.elf`
- write tests for QueueMethods
    - [x] QueueLength
    - [x] QueuePush
    - [x] QueuePop
    - [x] QueueIsFull
    - [x] QueueIsEmpty
- really what I'm doing is writing better documentation by
  renaming the tests
    - say what the test is really testing
    - do not mistake internal details for functionality
- use documentation to check that tests are all there
    - ;DU updates Doxygen
    - ;DV view Doxygen in browser
    - functions without links have no docstrings
    - functions without docstrings have no tests

## Check size of .elf has not changed
- I do not expect a change because I have only changed lib Queue,
  so as long as I am not calling any more functions, the .elf
  should not be affected
- last size check:

```bash
Wed, Dec 11, 2019 12:56:43 AM
text|   data|    bss|    dec|    hex|filename
 262|      0|     21|    283|    11b|build/vis-spi-out.elf
```

- build `vis-spi-out.elf` with ;mka
- look at increase in program size: `text`
- look at increase in data: `bss`
- check size again:
```bash
Mon, Dec 16, 2019  1:43:17 PM
$ avr-size.exe build/vis-spi-out.elf | clip
   text	   data	    bss	    dec	    hex	filename
    262	      0	     21	    283	    11b	build/vis-spi-out.elf
```

- actually, calculate size again after commenting out the dummy
  functions in `loop()`:

```c
void loop(void)
{
    /* example_function(); */
    /* example_inline_function(); */
}
```
```bash
$ date | clip
Mon, Dec 16, 2019  1:48:17 PM
$ avr-size.exe build/vis-spi-out.elf | clip
   text	   data	    bss	    dec	    hex	filename
    256	      0	     21	    277	    115	build/vis-spi-out.elf
```

## Use Queue lib in vis-spi-out.c
- Use Queue lib:
- check queue for commands to execute

```c
// src/vis-spi-out.c
//
void loop(void)
{
    /* example_function(); */
    /* example_inline_function(); */
    // Idle until a command is received from the SPI Master.
    while (QueueIsEmpty(SpiFifo));
```
```bash
Mon, Dec 16, 2019  1:51:48 PM
   text	   data	    bss	    dec	    hex	filename
    284	      0	     21	    305	    131	build/vis-spi-out.elf
```

- program size increased by (284-256=) 28 bytes
- increase is from:
    - making the function call once
    - including function definition now that it is used

- call to `QueueIsEmpty`:

```asm
call	0xec	; 0xec <QueueIsEmpty>
lds	r24, 0x0100	; 0x800100 <__data_end>
lds	r25, 0x0101	; 0x800101 <__data_end+0x1>
rjmp	.-14     	; 0xc6 <main+0x20>
```

- definition of `QueueIsEmpty`:

```asm
movw	r30, r24
ldd	r18, Z+4	; 0x04
ldd	r19, Z+5	; 0x05
ldi	r24, 0x01	; 1
or	r18, r19
breq	.+2      	; 0xfa <QueueIsEmpty+0xe>
ldi	r24, 0x00	; 0
ldi	r24, 0x00	; 0
ret
```

- how slow is this check?
- this alone might make performance sluggish

## Run time cost to check Queue empty without inline
- how long does it take to check if the Queue is empty?
- it takes 24 cycles without inline
- and the clock is 10MHz
- then each check for Queue empty takes 2.4µs
    - `24*(1/10.0e6) = 2.4e-6`
- with inline, it takes 6 cycles
- then each check for Queue empty takes 0.6µs
    - `6*(1/10.0e6) = 6.0e-7`
- I don't think this is a show-stopper:
    - response time to command is about 400x faster than a
      typical exposure
        - take 1ms as a typical exposure time
        - `(1.0e-3)/(2.4e-6)= 416.666667`
    - optimizing is only a 4x speedup

### Calculate without inline
- total cycle time *without inline* is:
- call time + execution time = 24 cycles
    - call time is 10 cycles:
        - call 4
        - lds 2
        - lds 2
        - rjmp 2
    - execution time is 14 cycles:
        - movw 1
        - ldd  2
        - ldd  2
        - ldi  1
        - or   1
        - breq 2
        - ldi 1
        - ret 4

### Calculate *with inline*
- there is no call, so call time is 0 cycles
- execution time is 7 cycles:
    - ldd 2
    - ldd 2
    - or 1
    - breq 2

#### quick inline plan
- commit any changes because the inline work is throwaway
- do not add the word `inline`
- `avr-gcc` does `inline` automatically as long as all source
  code in `Queue.c` is in the same translation unit as
  `vis-spi-out.c`
- git reset to HEAD after the inline test:

```bash
$ git reset --hard HEAD
```

#### inline Queue with include .c
- delete lib from list of libs in Makefile
- rebuild: build should fail for undefined references
- change `.h` in `#include "Queue.h"` to `.c` in `vis-spi-out.c`
- rebuild
- last check build size was 284 bytes
- build size shrinks to 272 bytes

#### inspect disassembly
- look at .avra to see what changed
    - the compiler automatically inlined everything Queue does
- entire `main` is below
- I split it up at:
    - the call to SpiSlaveInit since that marks the start of
      Queue code
    - the check for Queue empty

```asm
000000a6 <main>:
inline void SetBit(register_address reg_addr, bit_index bit)
{
    /** SetBit behavior:\n 
      * - sets bit in register\n 
      * */
    *reg_addr |= 1<<bit;
  a6:	38 9a       	sbi	0x07, 0	; 7
  a8:	39 9a       	sbi	0x07, 1	; 7
inline void ClearBit(register_address reg_addr, bit_index bit)
{
    /** ClearBit behavior:\n 
      * - clears bit in register\n 
      * */
    *reg_addr &= ~(1<<bit);
  aa:	40 98       	cbi	0x08, 0	; 8
  ac:	41 98       	cbi	0x08, 1	; 8
    BiColorLedOn(led_0); // sbi	0x07, 0
    BiColorLedOn(led_1); // sbi	0x07, 1
    BiColorLedGreen(led_0); // cbi	0x08, 0
    BiColorLedGreen(led_1); // cbi	0x08, 1
    // Configure as SPI slave, interrupts run `ISR(SPI_STC_vect)`
    SpiSlaveInit(); // call	0xaa	; 0xaa <SpiSlaveInit>
  ae:	0e 94 78 00 	call	0xf0	; 0xf0 <SpiSlaveInit>
```

- inline Queue code starts here:
```asm
      * - initializes Queue with length 0\n 
      * */
    // Create a pointer to the global Queue.
    volatile Queue_s * pq = &Queue;
    // Assign Queue to access the array
    pq->buffer = buffer;
  b2:	8a e0       	ldi	r24, 0x0A	; 10
  b4:	91 e0       	ldi	r25, 0x01	; 1
  b6:	90 93 01 01 	sts	0x0101, r25	; 0x800101 <__data_end+0x1>
  ba:	80 93 00 01 	sts	0x0100, r24	; 0x800100 <__data_end>
    // Store array size (this is the maximum length of the Queue)
    pq->max_length = buffer_size_in_bytes;
  be:	8b e0       	ldi	r24, 0x0B	; 11
  c0:	90 e0       	ldi	r25, 0x00	; 0
  c2:	90 93 07 01 	sts	0x0107, r25	; 0x800107 <__data_end+0x7>
  c6:	80 93 06 01 	sts	0x0106, r24	; 0x800106 <__data_end+0x6>
    // Empty the Rx Buffer: by setting head/tail index to first byte
    pq->head = 0;
  ca:	10 92 02 01 	sts	0x0102, r1	; 0x800102 <__data_end+0x2>
    pq->tail = 0;
  ce:	10 92 03 01 	sts	0x0103, r1	; 0x800103 <__data_end+0x3>
    // queue length is 0
    pq->length = 0;
  d2:	10 92 05 01 	sts	0x0105, r1	; 0x800105 <__data_end+0x5>
  d6:	10 92 04 01 	sts	0x0104, r1	; 0x800104 <__data_end+0x4>
    // Queue incoming SPI bytes in a FIFO buffer.
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
  da:	80 e0       	ldi	r24, 0x00	; 0
  dc:	91 e0       	ldi	r25, 0x01	; 1
  de:	90 93 09 01 	sts	0x0109, r25	; 0x800109 <SpiFifo+0x1>
  e2:	80 93 08 01 	sts	0x0108, r24	; 0x800108 <SpiFifo>
```

- check if Queue is empty starts here:
```
{ //! Return true if Queue is empty
    /** QueueIsEmpty behavior:\n 
      * - returns true if Queue is empty\n 
      * - returns false if Queue is not empty\n 
      * */
    if (pq->length == 0) return true;
  e6:	80 91 04 01 	lds	r24, 0x0104	; 0x800104 <__data_end+0x4>
  ea:	90 91 05 01 	lds	r25, 0x0105	; 0x800105 <__data_end+0x5>
  ee:	fb cf       	rjmp	.-10     	; 0xe6 <main+0x40>
```

#### confirm Queue is inlined
- I did not have to add `inline`
- by putting all Queue code in the same translation unit as the caller
  `vis-spi-out.c`, avr-gcc did the inline for me
- the code size reduction of 12 bytes is from eliminating the two calls
- but more importantly, the execution time to check the Queue is much
  shorter

#### calculate number of cycles
- there is no call, so call time is 0 cycles
- execution time is 6 cycles:
    - lds 2
    - lds 2
    - rjmp 2

#### of course the actual use is in a loop
- the assembly looks weird because there is no loop
- I am basically trusting the compiler to identify the section
  for me by prefacing it with the relevant C code
- I can do better by giving the loop something to execute if it
  ever exits:

```c
    while (QueueIsEmpty(SpiFifo));
        BiColorLedRed(led_0); // sbi	0x08, 0
```

- the resulting code is *very* similar
- instead of an `rjmp` there is a `breq` which is also 2 cycles
- and to decide whether to branch, there is an `or`
    - the processor checks to see if it breaks out of the loop
    - `or` two registers

```asm
ldd	r24, Z+4	; 0x04
ldd	r25, Z+5	; 0x05
or	r24, r25
breq	.-8      	; 0xe8 <main+0x42>
```


## But Queue calls are also made in the ISR so make it inline
- the Queue needs to be snappy because it is also called in the
  ISR

```c
// src/vis-spi-out.c
//
ISR(SPI_STC_vect)
{
    if (QueueIsFull(SpiFifo)) DEBUG_LedsShowError(); // TODO: add error handler
    else
    {
        QueuePush(SpiFifo, *Spi_spdr); // "client" must pop data from SpiFifo queue
    }
}
```

- I like my abstract datatype, but I have to sacrifice clean code
  for speed
- [x] Save existing `Queue` lib as `Queue_slow`
- [x] Move code from `Queue.c` to `Queue.h`:
    - goal is to make `inline` possible via `#include` in the
      `vis-spi-out.c` translation unit
    - move function bodies into `.h` with keyword `inline`
    - move function declarations into `.c`
    - but leave the allocation of memory for globals in
      `Queue.c`
    - this way if `Queue.h` is included in more than one file,
      there is not a multiple definition error
    - the only global defined in `Queue.o` is the singleton
      `Queue_s` Queue
- [x] Confirm this *proper* way to inline results in the exact
  same outcome as the quick-and-dirty inline via `#include` .c
    - size is 272 bytes
    - [x] diff the two .avra files
    - the code is identical except for the choice of addresses
      for the globals in static memory
    - in terms of instructions, the code is identical
    - example:

- include .c file to make a giant translation unit kludge:
```asm
    pq->buffer = buffer;
  b2:	8a e0       	ldi	r24, 0x0A	; 10
  b4:	91 e0       	ldi	r25, 0x01	; 1
  b6:	90 93 01 01 	sts	0x0101, r25	; 0x800101 <__data_end+0x1>
  ba:	80 93 00 01 	sts	0x0100, r24	; 0x800100 <__data_end>
    // Store array size (this is the maximum length of the Queue)
    pq->max_length = buffer_size_in_bytes;
  be:	8b e0       	ldi	r24, 0x0B	; 11
  c0:	90 e0       	ldi	r25, 0x00	; 0
  c2:	90 93 07 01 	sts	0x0107, r25	; 0x800107 <__data_end+0x7>
  c6:	80 93 06 01 	sts	0x0106, r24	; 0x800106 <__data_end+0x6>
```

- move function body to Queue.h and declare Queue global defined
  in Queue.c as extern in Queue.h:
```asm
    pq->buffer = buffer;
  b2:	82 e0       	ldi	r24, 0x02	; 2
  b4:	91 e0       	ldi	r25, 0x01	; 1
  b6:	90 93 0e 01 	sts	0x010E, r25	; 0x80010e <Queue+0x1>
  ba:	80 93 0d 01 	sts	0x010D, r24	; 0x80010d <Queue>
    // Store array size (this is the maximum length of the Queue)
    pq->max_length = buffer_size_in_bytes;
  be:	8b e0       	ldi	r24, 0x0B	; 11
  c0:	90 e0       	ldi	r25, 0x00	; 0
  c2:	90 93 14 01 	sts	0x0114, r25	; 0x800114 <Queue+0x7>
  c6:	80 93 13 01 	sts	0x0113, r24	; 0x800113 <Queue+0x6>
```

# keyword const is required for avr-gcc optimal assembly 
- avr-gcc needs a `const` variable passed to a function to use the
  optimal assembly for that function
- it is not sufficient for the function definition to declare a
  `const` input argument
- the variable passed must be `const` as well

## const example
- example: `BiColorLedOn(led_0)` takes variable `led_0`
- if `led_0` is not `const`, then avr-gcc does not know at
  compile time if `led_0` will change value, so it does not know
  if it is OK to use the `sbi` instruction

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

# avr-gcc outputs short assembly if all definitions are in headers
- the main file calls an inline function
- the inline function is in a lib header
- the inline function sets a bit in an io register
- the main file includes headers that define the bit and the io
  register with const values
- result: the main.c translation unit translates the inline call
  as an `sbi` instruction

## details
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

## example
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
	rm -f ${lib_o}
	rm -f ${test_lib_o}
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
lib_o := $(addsuffix .o,${lib_src})
lib_o := $(addprefix ../lib/build/,${lib_o})
 # define a pattern rule to build these object files
${lib_o}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
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
    ${lib_o}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
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
# assume function *is* `inline`

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

## Create a -HardwareFake.h in lib/test/
- create the file:
    - a `-HardwareFake.h` goes in `firmware/lib/test/`
    - the missing dependency is fixed
- try building the test again with `;mkf`
- make reports which files are missing:

```bash
|| make: *** No rule to make target 'test/test_SpiSlave.c', needed by 'build/test_SpiSlave.o'.  Stop.
```

## Create a `test_BlahLibName.c` in lib/test/
- the `test_BlahLibName.c` goes in `firmware/lib/test/`
- create the file:
    - make the `.c` file with a placeholder function
    - use `;fh` to auto-generate the header
- the missing dependency is fixed
- try building the test again with `;mkf`
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

## Create a `BlahLibName.c` and `BlahLibName.h` in lib/src/
- the `BlahLibName.c` goes in firmware/lib/src/
- create the file:
    - make the `.c` file with a placeholder function
    - use `;fh` to auto-generate the header
- the missing dependency is fixed
- try building the test again with `;mkf`
- this should succeed
- erase the placeholder functions
- the empty test runner should build now with `;mktgc`

## Create an empty test suite
Start with an empty test suite:

```c
// test/test_runner.c
//
void UartSpi_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
    }
}
```

## Write the first test in `test_BlahLibName.c`
- write the first test in the `test_BlahLibName.c` file
    - `ttt` in insert mode in the function body to get
      `TEST_FAIL_MESSAGE("Implement test.");`
- `;fh` to list the test in the header for visibility to the
  `test_runner`
- `;yt` to add the `RUN_TEST` call to the `test_runner`

## Add a call to the test to drive writing code in the lib
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

## if lib reg and pin vals are extern how is assembly optimized?
- `extern` registers and pins works for `inline` functions
  because the main.c translation unit has the definitions
- it does not matter that the lib.c translation unit is missing
  definitions because main.o does not link with lib.o

### details
- main.elf
    - main.c includes the lib header
    - main.c includes definitions of registers and pins
    - the main.c translation unit has the values to substitute
      for register and pin variables used in the inline function
      in lib.h
    - the main.c translation unit does not have to compile with
      symbols for linking with lib.o
- TestSuite.exe
    - the lib.c translation unit does not have the definitions
    - but the linker does not need lib.o when building the .elf
      from main.o
    - lib.o is only used by the `test_runner` and for testing
      purposes it does not matter if the lib translation unit has
      the definitions

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
build/${board-name}.elf: build/${board-name}.o ${app_build_src} ${lib_o}
	avr-gcc $(CFLAGS) $^ -o $@ $(LFLAGS)
```

- and one of these object files *is* `../lib/build/SpiSlave.o`

```make
hw_lib_src := BiColorLed SpiSlave
lib_src := ${hw_lib_src} ReadWriteBits
lib_o := $(addsuffix .o,${lib_src})
lib_o := $(addprefix ../lib/build/,${lib_o})
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

## check translation for `inline` body in the `.h`
- translation to single instructions should be no problem if the
  function body is `inline` in the `.h`
- if `avr-gcc` fails to use single instructions:
    - make sure the variables are `const`

# but if you decide not to `inline` the function
- lib needs to build with:
    - actual hardware definitions for avr
    - fake definitions for unit-test
- "single compilation unit" solution:
    - build rule uses `-include` to include hardware headers for
      avr builds

## check translation when lib is used in avr-target
- the procedure for making the lib is similar to `inline` version
- obviously, drop the check for `inline`
- still check translated assembly uses the correct instructions
- for avr-target, the lib translation unit needs to know the reg
  and pin values to compile with optimal assembly
- for unit-tests, it's OK for the values to remain as symbols
  undefined until linking

## how are reg and pin variables and avr function macros defined?

### avr-gcc lib-object rule uses -include for reg and pin variables
- reg and pin variables are still declared extern
- so how can `avr-gcc` possibly select the correct instructions?
- the Makefile handles including the definitions in the
  translation unit when compiling with `avr-gcc`
- the `lib/Makefile` only uses `gcc` so it is not affected
- the Makefiles that have builds for both `avr-gcc` and `gcc`
  list the libs separately
- this is to differentiate between two kinds of hardware libs:
    - hardware libs that need the definitions
        - without the definitions the compiler creates symbols
          for the linker to resolve when linking to create the
          `.elf`, which means the compiler cannot optimize the
          instructions
    - hardware libs that do not need the definitions
        - all of the hardware dependencies are inside of `inline`
          function bodies
        - the lib object file is ignored when linking to create
          the `.elf`
        - the main.c translation unit includes the definitions
          and it includes the lib header

### gcc lib-object rule uses -include to fake avr function macros
- avr function macros are macros defined in avr headers
    - avr-gcc knows where these header files are
    - gcc does not know where these header files are
- an example is `avr/interrupt.h` which defines `cli()` and
  `sei()`
- since these are macros:
    - when compiling with `avr-gcc`, the header can be included
      in different translation units without the
      multiple-definition problem that happens when the header
      defines variables or functions
    - when compiling with `gcc`, the macro function is easily
      stubbed out by a `#define`

### details: lib Makefile is not affected
- the `lib/Makefile` is unaffected because it never uses
  `avr-gcc`
- in fact, all of its libs are compiled the same way
- below we see all of the libs end up in the same list of lib
  objects:
    - `lib/build/lib.o`
    - `lib/build/test_lib.o`
- the only reason the libs are separated as hardware and
  non-hardware is to generate a list of pre-requisites for the
  hardware fakes
    - `lib/test/lib-HardwareFake.h`
- the lists of fakes and object files are defined:
```make
 # lib/Makefile
 #
 # ---add hardware libs here to create dependency on fakes---
hw_lib_src := BiColorLed SpiSlave
HardwareFakes := ${hw_lib_src}
HardwareFakes := $(addsuffix -HardwareFake.h,${HardwareFakes})
HardwareFakes := $(addprefix test/,${HardwareFakes})
 # ---add non-hardware libs here---
lib_src := ${hw_lib_src} ReadWriteBits
lib_o := $(addsuffix .o,${lib_src})
lib_o := $(addprefix build/,${lib_o})
test_lib_o := $(addsuffix .o,${lib_src})
test_lib_o := $(addprefix build/test_,${test_lib_o})
```

- the hardware fakes are pre-requisites for `test_runner.o`:

```make
 # lib/Makefile
 #
 # test_runner translation unit requires fake hardware definitions
build/test_runner.o: test/test_runner.c test/HardwareFake.h ${HardwareFakes}
```

- the lib-objects are all linked with `test_runner.o`:

```make
 # lib/Makefile
 #
build/TestSuite.exe: build/test_runner.o ${unittest_o} ${test_lib_o} ${lib_o}
```

- and the lib-object build rule is the same for all libs:

```make
 # lib/Makefile
${lib_o}: build/%.o: src/%.c src/%.h ${FakeAvrHeaders}
	${compiler} ${IncludeFakeAvrHeaders} $(CFLAGS) -c $< -o $@
```

- the one unexplained bit is pre-requisitie `FakeAvrHeaders` and
  compile flag `IncludeFakeAvrHeaders`
- `IncludeFakeAvrHeaders` is a `-include` flag for each
  fake of an avr header file
    - these fake header files just contain stubs of function-like
      macros defined in the avr headers
    - the stubs effectively make `gcc` ignore the macros
- `FakeAvrHeaders` is a list of fake AVR header names
    - this is a pre-requisite to the lib object to make sure the
      lib object is rebuilt if any fake avr header file is
      updated or created

### details: vis-spi-out Makefile is complicated
- No, this is too important, I am rethinking it. See level-1
  headings:
    - `libs separate into two lists`
    - `two types of libs`

# libs separate into two or three lists
- there are three kinds of libs as far as Makefile variables are
  concerned:
    - `app` (application) libs
    - `hw` (hardware) libs 
    - `nohw` (non-hardware) libs

- `applib` libs are listed separately because their path is in the
  *application* folder while the other two lib types are in the
  *lib* folder
- app libs contain application code that I want to unit test
    - application contains code that is not in the libs
    - I cannot unit test the file that contains `main()`
    - so I put most of it in application libs for testing
- `hwlib` and `nohwlib` libs need to be separated because the
  `hwlib` libs have Hardware headers that I list as
  pre-requisites in the build rules:
    - I edit either the `libname-Hardware.h` file or the
      `libname-HardwareFake.h` file
    - I want this edit to trigger a rebuild of whatever the obj
      file that depends on this file
        - the board-name obj file depends on `libname-Hardware.h`
        - the `test_runner` obj file depends on
          `libname-HardwareFake.h`
    - so I make these files pre-requisites

# Examples lib lists
- application libs:

```make
applib := Example
test_app := $(addprefix test_,${applib})
test_app_o := $(addsuffix .o,${test_app})
test_app_o := $(addprefix build/,${test_app_o})
app_o := $(addsuffix .o,${applib})
app_o := $(addprefix build/,${app_o})
```

- hardware libs:

```make
hwlib := BiColorLed SpiSlave UartSpi Lis
```

- non-hardware libs:

```make
nohwlib := ReadWriteBits Queue
```

- group hw and nohw libs to make pre-reqs:

```make
Hardware := ${hwlib}
HardwareFakes := ${Hardware}
Hardware := $(addsuffix -Hardware.h,${Hardware})
Hardware := $(addprefix src/,${Hardware})
HardwareFakes := $(addsuffix -HardwareFake.h,${HardwareFakes})
HardwareFakes := $(addprefix ../lib/test/,${HardwareFakes})
```

- group hw and nohw libs to make build targets and header
  names

```make
all_libs := ${hwlib} ${nohwlib}
lib_o := $(addsuffix .o,${all_libs})
lib_o := $(addprefix ../lib/build/,${lib_o})
lib_headers := $(addsuffix .h,${all_libs})
lib_headers := $(addprefix ../lib/src/,${lib_headers})
```



# non-hardware libs do not have hardware dependencies
- `ReadWriteBits` does not have hardware dependencies
    - it has inline functions
    - these functions are not defined with hardware dependencies
- but these functions are usually called with hardware
  dependencies as the input arguments
    - in effect, these functions *do* have hardware dependencies
      after inlining because the caller inlines the function with
      the hardware dependencies
    - but from the perspective of Makefile concerns, there are no
      hardware dependencies

# do not do this
## optimize assembly with function body in lib `.c`
- function uses hardware but does not need to be `inline`
    - example `SpiSlaveInit` does not need to be `inline`
    - but the functions it calls should still be `inline`
    - and the instructions should translate to single
      instructions
- `SpiSlaveInit` function body is in the `.c`

### two options
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

### single compilation unit - first failed attempt
- ~build for avr-target does *not* compile the
  `../lib/src/BlahLibName.c`~
- ~instead, the libs build for the avr-target by compiling
src/BlahLibName.c~
    - this file is a short .c with no code except #include
      statements to include the ../lib/src/BlahLibName.c, avr/io,
      and the src/BlahLibName-Hardware.h file
- ...try that again...

## single compilation unit
- let the build target handle which headers to include
- target is still `../lib/build/SpiSlave.o`
    - avoid `make` complaints about overriding target by putting
      the build for `SpiSlave.o` in an if-else that checks if the
      compiler is avr-gcc
- build for avr-target and unit-test are similar except for the
  `-include` flag
- below I go through each case to show the `-include` flag

### build for lib tests in `../lib/test`
- two goals:
    1. stub function-like macros from avr headers
    2. include HardwareFake headers as pre-requisities for
       `test_runner` object file but let `test_runner.c` include
       the files if it needs them

- so to unit-test a lib in `../lib/src`:

1. compile each lib-object with flag `-include
  ../test/FakeAvr/interrupt.h`
    - now the lib compilation unit has stubbed definitions for
      function-like macros like `cli()` and `sei()`
    - if the lib does not use those macros, no harm done
2. compile the `test_runner.o` object with `HardwareFakes` in the
   pre-requisite list
    - this is a convenience to force `make` to rebuild
      `test_runner.o` after edits to a `HardwareFake` header

- update the `lib/Makefile` to include fake avr macros:

```make
 # lib/Makefile
 # 
 # ---stub function-like macros in avr headers---
AvrHeaders := interrupt io
AvrHeaders := $(addsuffix .h,${AvrHeaders})
FakeAvrHeaders := ${AvrHeaders}
FakeAvrHeaders := $(addprefix test/FakeAvr/,${FakeAvrHeaders})
IncludeFakeAvrHeaders := $(addprefix -include,${FakeAvrHeaders})
```

- use `-include` to include the stubs in the `lib` translation
  units

```make
 # stub macros for lib translation units that include function-like avr macros
${lib_o}: build/%.o: src/%.c src/%.h
	${compiler} ${IncludeFakeAvrHeaders} $(CFLAGS) -c $< -o $@
```

- list `HardwareFakes` as a requirement but let `test_runner.c`
  include the ones it needs (do not use a `-include` flag here)
- similarly, list the `FakeAvrHeaders` as a requirement
    - (so do not use the `-include` flag or `make` thinks it
      needs a target named `-includetest/FakeAvr/blah`)

```make
 # test_runner translation unit requires fake hardware definitions exist
build/test_runner.o: test/test_runner.c test/HardwareFake.h ${HardwareFakes} ${FakeAvrHeaders}
	${compiler} $(CFLAGS) -c $< -o $@
```

### there is no build for `../lib` tests in `vis-spi-out/test`
- the `vis-spi-out` test suite does not test code in `../lib/src`
- this code is already tested in `../lib/test/`

### build for local lib tests in `vis-spi-out/test`
- the `vis-spi-out` test suite tests libs that are local to
  `vis-spi-out`
- `vis-spi-out` has local libs because it is impossible to
  test functions defined in the main() file
    - I can test by copy-paste, but then I have no guarantee
      the version in the test is the same version in the
      main() file
- so anything I want to test has to go in a separate file
- lib `Example` is an example of such a local lib
- it's a toy example, so I don't know yet which headers I
  need to include via the `-include` flag
- all I know so far is I want the tests to include:

1. `../lib/test/{lib}-HardwareFake`
    - these headers define fake values for the registers and
      pins in any `../lib/src` libs that the test depends on
2. `../test/FakeAvr/{avrmacros}.h`
    - these headers stub the function-like macros from the
      avr includes

### build for avr-target includes -Hardware.h
- compile with flag `-include src/SpiSlave-Hardware.h`
- now the lib compilation unit has the hardware info

### example build for local lib tests in `vis-spi-out/test`
- make the list of local libs: `app_src`
- create lists of object files for the tests: `app_build_test`
- create lists of object files for the source: `app_build_src`

```make
 # vis-spi-out/Makefile
 #
app_src := Example
app_test := $(addprefix test_,${app_src})
app_build_test := $(addsuffix .o,${app_test})
app_build_test := $(addprefix build/,${app_build_test})
app_build_src := $(addsuffix .o,${app_src})
app_build_src := $(addprefix build/,${app_build_src})
```

- define the rules for object files listed in `app_build_test`
  and `app_build_src`

```make
 # vis-spi-out/Makefile
 #
${app_build_test}: build/%.o: test/%.c test/%.h ../lib/test/HardwareFake.h ${HardwareFakes} ${IncludeFakeAvrHeaders}
	${compiler} $(CFLAGS) -c $< -o $@
 #
${app_build_src}: build/%.o: src/%.c src/%.h
	${compiler} $(CFLAGS) -c $< -o $@
```

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

## new problem when building BiColorLed.o like SpiSlave.o
### hardware values are defined twice
- vis-spi-out.c calls `BiColorLedOn(led_0)`
- this is an inline function
- so the compiler copies the source code to vis-spi-out.c
- vis-spi-out.c used to include BiColorLed-Hardware.h
- so it could translate the copied source code correctly
- but after the *single compilation unit* fix, BiColorLed is
  built with BiColorLed-Hardware.h
- it's a hardware library
- so it gets built with the hardware header
- but now two translation units are built with the same hardware
  header
- the hardware header defines values
- the values are defined twice

### use macros instead of variables? no
- I tried changing all hardware values in BiColorLed to macros
  and it was a mess
- but it's not obvious how to make registers in libraries macros

### a hardware definition is owned by one and only one unit
- I am back to the hardware header multiple define problem
    - I can only include the hardware header once
    - otherwise the linker ends up with two objects files that
      each claim to have the definitions for the registers and
      pins
- takeaway:
    - a single executable must never have more than one
      translation unit that needs the hardware definitions
- right now I have two units that need it, but BiColorLed.o does
  not *really* need it, I was just copying how I built SpiSlave.o
- vis-spi-out.o is the only object file that *really* needs the
  hardware definitions
- if I make the function inline by putting it in lib .h, I can
  make the variables extern in the lib .h and that hardware lib
  does not get built with the hardware-header
- vis-spi-out.o is built with the hardware header
- but if I put the function body in lib .c
- then the lib must be built with the hardware-header included,
  otherwise the object file will not have optimal code
- and that means vis-spi-out.o *cannot* be built with the
  hardware-header (or I have a multiple definition error)

### split a lib in two if it needs both inline and not inline
- the two options for a hardware lib are:
    - define lib with inline function bodies in the header and
      build the lib object file without including the hardware
      header (the translation unit that calls the inline
      functions is the unit that gets to include the hardware
      header), example: BiColorLed.o
    - or define lib without inline functions bodies in the header
      and build the lib object file with the hardware header
      included, example: SpiSlave.o
- if one lib has some functions that need to be inline and some
  functions that need to be *not* inline, then split it into two
  libs

# unit test libs using -DMacro to define fakes

## concept
- want to test `InitSpiSlave` in lib `SpiSlave`
    - most functionality is encapsulated in a one-liner that
      manipulates a bit in a hardware register
    - just check the hardware register
- but there is too much stuff in the function `EnableSpiInterrupt`
    - so I only want to check the function calls
      `EnableSpiInterrupt`
    - I don't want to check all the hardware registers it
      manipulates, or test whether it calls avr function-like
      macros
- I'll make smaller tests for the functions inside
  `EnableSpiInterrupt`

## details
- add to `lib/Makefile`:
    - CFLAGS: `-DSPISLAVE_FAKED`
    - TestSuite.exe links against `build/SpiSlave_faked.o`
    - add target:
        - `build/SpiSlave_faked.o`
    - target pre-requisites:
        - `test/SpiSlave_faked.c`
        - `test/SpiSlave_faked.h`
    - target command:
        - compile `test/SpiSlave_faked.c`
        - output `build/SpiSlave_faked.o`
- create `faked` files:
    - `lib/test/SpiSlave_faked.h`
    - `lib/test/SpiSlave_faked.c`

## example `SpiSlave_faked.h`

```c
// test/SpiSlave_faked.h
//
#ifndef _SPISLAVE_FAKED_H
#define _SPISLAVE_FAKED_H

#ifdef SPISLAVE_FAKED
#define EnableSpiInterrupt EnableSpiInterrupt_fake
#endif

#endif // _SPISLAVE_FAKED_H
```

## example `SpiSlave_faked.c`

- if the fake is just a stub, it's a simple empty function
- but if I am testing that the function is called, I need the
  fake to record itself
- that's what "mock" is for
- I used to put this mock stuff in its own file `mock_SpiSlave`
- then I switched to putting it in the `test_SpiSlave` file
- now it conveniently sits in this `SpiSlave_faked` file
- here is the simple example of faking a function that takes no
  arguments and returns nothing

```c
// test/SpiSlave_faked.c
//
#include "Mock.h" // record call history in "mock"
/* =====[ Mock EnableSpiInterrupt() ]===== */
static RecordedCall * Record_EnableSpiInterrupt(void)
{ // Define call recorded when func-under-test calls mocked function.
    char const *call_name = "EnableSpiInterrupt";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void EnableSpiInterrupt_fake(void)
{
    RecordActualCall(mock, Record_EnableSpiInterrupt());
}
```

- by including "Mock.h", the `SpiSlave_faked.c` file is declaring
  the "mock" as an `extern` (a global defined in another
  translation unit) of type `Mock_s`
- so I am free to use "mock" all I want in this file and the
  linker will resolve it's actual memory location later
- in fact, "mock" is on the heap, so the `test_runner` is
  creating and destroying "mock" for each test where "mock" is
  used

## example `SpiSlave.c`

- include `SpiSlave_faked.h` *before* including `SpiSlave.h`
    - if any faked functions are mentioned in the header, this
      appends their names with `_fake` in the header too
- put the include for `_faked` inside `#ifdef`
    - this way the Makefile for the avr-target can ignore the
      fact that the unit-test target needs this `_faked` header
    - otherwise avr-gcc complains it cannot find file
      `SpiSlave_faked.h`
    - I thought the preprocessor would ignore this `#include`
      because the targets built with avr-gcc do not include the
      `-DSPISLAVE_FAKED` in its CFLAGS, but unfortunately that is
      not the case

```c
// src/SpiSlave.c
//
#ifdef SPISLAVE_FAKED
#include "SpiSlave_faked.h"
#endif
#include "SpiSlave.h"
```

- define EnableSpiInterrupt using `#ifndef` to select the `real`
  version if the macro `SPISLAVE_FAKED` is not defined by the
  CFLAGS in the `lib/Makefile`

```c
// src/SpiSlave.c
//
#ifndef SPISLAVE_FAKED
static void EnableSpiInterrupt(void)
{
    cli(); // cli
    ReadSpiStatusRegister(); // in	r24, 0x2d
    ReadSpiDataRegister(); // in	r24, 0x2e
    SetBit(Spi_spcr, Spi_InterruptEnable);
    sei(); // sei
}
#else
void EnableSpiInterrupt(void);
#endif
```

- building the unit-tests results in the `_fake` version of the
  function being called
- this means the `static` functions called by the real function
  are not used
- this triggers the `-Wunused-function` warning
- eliminate this nuisance warning by making the `static`
  functions `static inline` functions

```c
// src/SpiSlave.c
//
// ---Private Functions---
static inline uint8_t ReadSpiDataRegister(void) { return *Spi_spdr; }
static inline uint8_t ReadSpiStatusRegister(void) { return *Spi_spsr; }
static void EnableSpiModule(void) { SetBit(Spi_spcr, Spi_Enable); }
```

- `EnableSpiInterrupt` is the only function that calls
  `ReadSpiDataRegister` and `ReadSpiStatusRegister`, so I made
  those `inline` to kill the warning
- `EnableSpiModule` is called whether or not I fake
  `EnableSpiInterrupt`, so there is no reason to make it `inline`
- in this instance, `inline` has no impact other than to suppress
  the `-Wunused-function` warning

## example: `test_SpiSlave.c`

- the test translation unit does not know about the
  `SpiSlave_faked.h` file
- as far any test knows, `SpiSlaveInit` calls
  `EnableSpiInterrupt`
- in reality:
    - `SpiSlave_faked.h` renames the function
      `EnableSpiInterrupt_fake`
    - `SpiSlave_faked.c` defines the `_fake` version to record
      call name `EnableSpiInterrupt` in the call history
- the test includes `Mock.h` because the "mock" object is needed
  to record and play back the call history

```c
#include "unity.h"
#include "test_SpiSlave.h"
#include "SpiSlave.h"
#include "Mock.h" // record call history in "mock"
```

- simple hardware tests do not need to look at the "mock" object:

```c
void SpiSlaveInit_enables_SPI(void)
{
    /* =====[ Setup ]===== */
    *Spi_spcr = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_Enable,
        *Spi_spcr,
        "Cannot run test: must start with bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Enable,
        *Spi_spcr,
        "Expect bit 6 HIGH to enable SPI module."
        );
}
```

- but "mock" is used when testing if `SpiSlaveInit` calls `EnableSpiInterrupt`

```c
void SpiSlaveInit_enables_SPI_interrupt(void)
{
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "EnableSpiInterrupt"),
        "Expect SpiSlaveInit enables the SPI interrupt."
        );
}
```

# Colorschemes for diff, avra, and read-only C
- created `asmwolf.vim` to make a nice .avra and .avra diff color
  scheme
- [x] make `greywolf.vim`
    - badwolf with a lighter background to indicate when C file
      is RO

