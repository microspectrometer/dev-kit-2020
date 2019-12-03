- [x] want to see definition of variables in a lib optimized correctly
    - BiColorLed is a lib
    - it uses a register and pins
    - these are variables that the compiler needs to know the value of when
      compiling to use the optimal instructions
- well, that was useful but it did not prove what I need in general
    - BiColorLed is not its own translation unit
    - I do compile it separately, but:
    - all the function definitions are in the .h file
    - so the application includes those definitions and only a single
      translation unit is required
    - i.e., I could eliminate BiColorLed.o and it would make no difference to
      the linker when making `vis-spi-out.elf`
- [ ] create a lib that has function definitions in its .c
    - this lib has its own translation unit
    - the resulting .o file is needed by the linker
    - the compiler needs to know the values to use when it makes the translation
      unit

# keyword const is required for avr-gcc optimal assembly 
- avr-gcc needs a const variable passed to a function to use the optimal
  assembly for that function
- it is not sufficient for the function definition to declare a const input
  argument
- the variable passed must be const as well

## example
- example: `BiColorLedOn(led_0)` takes variable `led_0`
- if `led_0` is not `const`, then avr-gcc does not know at compile time if
  `led_0` will change value, so it does not know if it is OK to use the `sbi`
  instruction

## example details
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

## recommendation
- use `typedef` instead of remembering `const`
- put `typedef` in library header
- match type name with library name

## recommendation example

- define the type in the library header:

```c
// BiColorLed.h
typedef uint8_t const bicolorled;
```

- define the variable using the type:

```c
// main.c
bicolorled led_0 = 0;
```

# avr-gcc automatically inlines functions if they only call inline functions
- avr-gcc tries to inline a function that calls inline functions, even if the
  function is not inline or static

## example
- example: `setup` only calls `inline` functions, so the call to `setup` is
  inlined even without the `inline` keyword

## example details
- `main` calls `setup` and `loop`
- `setup` is inlined but `loop` is not
- it is because:
    - `setup` only calls inline functions
    - and `loop` calls one function that is *not* inline
- it is *not* because:
    - `setup` is `static`
    - `loop` is called in a `while` loop

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

# Makefile

## target rules
- target rule syntax is:

```make
target: dependency
	command
```

- `dependency` is an optional space-separated list of dependencies
- `command` is an optional list of commands, each one prefaced by a `tab`
  character


## pattern rules work on a list of file names
- *target* uses `%` to extract the file name `stem`
- think of the `stem` as the file name without its path or extension
- extracting a `stem` is useful for working on a list of files
- each *dependency* uses `%` to wrap the `stem` in the dependency path
- each *command* uses `$*` to access the `stem`

## example

```make
# make a list of object file names
lib_src := Led BiColorLed TriColorLed
lib_build_src := $(addsuffix .o,${lib_src})
lib_build_src := $(addprefix ../lib/build/,${lib_build_src})
# define a pattern rule to build these object files
${lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
	${compiler} $(CFLAGS) -c ../lib/src/$*.c -o $@
```

#### Automatic Variables
<https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html>

- `$@`
    - file name of target
    - if multiple targets, whichever target caused the recipe to run
- `$^`
    - name of all prerequisites with spaces between them
- `$<`
    - name of first prerequisite
    - useful for listing header dependencies
    - example: stem.o depends on stem.c and stem.h, but only compile stem.c

    ```make
    ${lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
        ${compiler} $(CFLAGS) -c $< -o $@
    ```

- `$?`
    - name of all prerequisites *newer than the target* with spaces between them
