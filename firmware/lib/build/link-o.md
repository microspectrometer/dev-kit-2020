# Example using a mock object
## Print the *Recorded Call* Lists
- Expected calls:
    - "TakesOneArg", 0x12
- Actual calls:
    - "TakesOneArg", 0x21

## Print the *Why it failed* message
Why it failed: Call #1: expected '0x12', was '0x21'. 
# Linking object files
## The right way

The right way to link in the unit test libs I created in `mock-c`
is to turn those `.o` files into a *Static Library*:
https://medium.com/@meghamohan/everything-you-need-to-know-about-libraries-in-c-e8ad6138cbb4
and link that library with `-llibrary`.
Note there is *no space* between `-l` and the library name.
Here is *GNU* documentation on the `-l` flag for `gcc`:
> Search the library named library when linking.
> 
> The linker searches a standard list of directories for the
> library, which is actually a file named liblibrary.a.  The linker
> then uses this file as if it had been specified precisely by name.
> The directories searched include several standard system
> directories plus any that you specify with -L.
> 
> Normally the files found this way are library files---archive
> files whose members are object files.  The linker handles an
> archive file by scanning through it for members which define
> symbols that have so far been referenced but not defined.

## The temporary hack way

The *Makefile* recipe lists the object files as prerequisites:
```make
build/%.exe: src/%.c ${unit-test-lib-objects}
    ${compiler} $^ -o $@ $(LFLAGS) $(CFLAGS) -I src/
```
and `CFLAGS` has the include paths for the header files:
```make
CFLAGS = -I ${mock-c-lib}include/
```
## Linking aginst the unity object file requires definitions of setUp and tearDown.
nThe *Makefile* recipe lists `unity.o` as a prerequisite.
This is part of `${unit-test-lib-objects}`.
The `unity.o` filename,
and the path to this file in my `mock-c` project,
are part of the `${unit-test-lib-objects}` variable.

The `unity.o` object file has undefined symbols for
`setUp` and `tearDown`. This is because
my modification of `unity.c` gives `setUp` and `tearDown`
*external linkage* and makes them function pointers.
Interestingly, the mere act of linking against `unity.o`
causes *clang* to throw an *undefined reference* error.
Therefore, even without including `unity.h` in this file,
these function pointers must be *defined and assigned*.

I *define* them globally:
```c
void (*setUp)(void); void (*tearDown)(void);
```

And inside `main()` I *assign* them to definitions:
```c
setUp = SetUp_Mock; tearDown = TearDown_Mock;
```
