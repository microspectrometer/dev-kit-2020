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
    - everything was going great with setting this up
    - but I can't figure out why `AssertCall` is returning
      `false` when it should *clearly* be returning true
    - come back to this by inserting print statements all
      the fuck over `AssertCall`, rebuilding, and remaking
      the tests to find out where things are fucking up
    - OK, I still don't know why it failed
    - but I fixed it by replacing the `==` comparison with a
      `g_strcmp0` comparison
- [ ] move more code from old vis-spi-out to new vis-spi-out
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
    - [ ] am I encountering new cases?

- [ ] split common Spi stuff from SpiSlave into a Spi lib

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
- [ ] leave off here
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
lib_build_src := $(addsuffix .o,${lib_src})
lib_build_src := $(addprefix build/,${lib_build_src})
lib_build_test := $(addsuffix .o,${lib_src})
lib_build_test := $(addprefix build/test_,${lib_build_test})
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
build/TestSuite.exe: build/test_runner.o ${unittest_o} ${lib_build_test} ${lib_build_src}
```

- and the lib-object build rule is the same for all libs:

```make
 # lib/Makefile
${lib_build_src}: build/%.o: src/%.c src/%.h ${FakeAvrHeaders}
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

#### libs separate into three lists
- the `vis-spi-out/Makefile` is affected since it includes rules
  for compiling with `avr-gcc`
- there are three kinds of libs:
    - `hw` (hardware) libs 
    - `inlhw` (inline hardware) libs
    - `nonhw` (non-hardware) libs

#### `hw` and `inlhw` form a list for pre-requisites
- `hw` libs and `inlhw` libs form a list of `-Hardware.h`
  pre-requisites (`src/lib-Hardware.h`) for building the main
  object file
- `hw` libs and `inlhw` libs form a list of `-HardwareFake.h`
   pre-requisites (`../lib/test/lib-HardwareFake.h`) for the test
   object files

#### `hw` lib-objects build rules depend on compiler

#### `hw` build rules for avr-gcc
- `hw` libs form a list of lib-objects with the build rule to
  *include the hardware definition in the translation unit* when
  compiling with `avr-gcc`:

    - this is done with the flag `-include src/$*-Hardware.h`

    ```make
    ${hw_lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h src/%-Hardware.h
        ${compiler} -include src/$*-Hardware.h $(CFLAGS) -c $< -o $@
    ```

#### `hw` build rules for gcc
- that same list of `hw` lib-objects does *not* include the
  hardware definition when compiling with `gcc`, but does include
  *fake AVR macro definitions*

   - the `-include` flag for AVR and fake AVR macro definitions
     is yet another list:

    ```make
    AvrHeaders := interrupt io
    AvrHeaders := $(addsuffix .h,${AvrHeaders})
    FakeAvrHeaders := ${AvrHeaders}
    FakeAvrHeaders := $(addprefix ../test/FakeAvr/,${FakeAvrHeaders})
    IncludeFakeAvrHeaders := $(addprefix -include,${FakeAvrHeaders})
    ```

    - and here is the build rule for `gcc` showing the fakes are
      used:

    ```make
    ${hw_lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
        ${compiler} ${IncludeFakeAvrHeaders} $(CFLAGS) -c $< -o $@
    ```

#### check compiler to resolve conflicting build rules
- `make` does not allow conflicting rules for the same target
- this is handled by checking which compiler is used, as shown
  in the examples below

#### `inlhw` and `nonhw` build rules do not depend on compiler
- `inlhw` and `nonhw` libs form a list of lib-objects with the
  same build rule

    - group them into a list:

    ```make
    nonhw_lib_src := ${inlhw_lib_src} ReadWriteBits
    ```

    - create the list of lib-ojects:

    ```make
    nonhw_lib_build_src := $(addsuffix .o,${nonhw_lib_src})
    nonhw_lib_build_src := $(addprefix ../lib/build/,${nonhw_lib_build_src})
    ```

- here is the build rule:

```make
${nonhw_lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
	${compiler} $(CFLAGS) -c $< -o $@
```

### examples from vis-spi-out Makefile
- the example below shows how three libs, one for each type, form
  the lists for pre-requisites and compiler-dependent build
  rules
- the examples also cover the three compiler cases:
    - `avr-gcc` compiles libs for linking to make the `.elf`
    - `gcc` compiles libs for linking to make the `TestSuite.exe`
      for `vis-spi-out` unit-tests
    - `gcc` compiles libs for linking to make the `TestSuite.exe`
      for `lib` unit-tests

#### three libs each representing a different case
- `SpiSlave` has function definitions with hardware dependencies
  and these functions are *not* inline
- `BiColorLed` has has function definitions with hardware
  dependencies but all of these functions are `inline`
- `ReadWriteBits` does not have hardware dependencies
    - it has inline functions
    - these functions are not defined with hardware dependencies
    - but these functions are usually called with hardware
      dependencies as the input arguments
    - in effect, these functions *do* have hardware dependencies
      after inlining because the caller inlines the function with
      the hardware dependencies

```make
 # vis-spi-out/Makefile
 #
 # ---add hardware libs here that do not put inline in .h---
 #  avr-gcc builds these with hardware defs via -include lib-Hardware.h
hw_lib_src := SpiSlave
 # ---add hardware libs here that put inline in .h---
 #  avr-gcc builds these libs without needing hardware defs
inlhw_lib_src := BiColorLed
 # ---add nonhardware libs here---
nonhw_lib_src := ${inlhw_lib_src} ReadWriteBits
```

#### when `avr-gcc` compiles `SpiSlave`

#### when `avr-gcc` compiles `BiColorLed`

#### when `avr-gcc` compiles `ReadWriteBits`

#### when `gcc` compiles `SpiSlave` for unit-tests in `vis-spi-out`

#### when `gcc` compiles `BiColorLed` for unit-tests in `vis-spi-out`

#### when `gcc` compiles `ReadWriteBits` for unit-tests in `vis-spi-out`

#### when `gcc` compiles `SpiSlave` for unit-tests in `lib`

#### when `gcc` compiles `BiColorLed` for unit-tests in `lib`

#### when `gcc` compiles `ReadWriteBits` for unit-tests in `lib`

## why not make all lib functions `inline`
- I don't know
- it would certainly make this all a lot simpler

## translation for body in the `.c`
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
${lib_build_src}: build/%.o: src/%.c src/%.h
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
