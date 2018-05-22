# Linking object files.

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

The *Makefile* recipe lists `unity.o` as a prerequisite.
My modification of `unity.c` gives `setUp` and `tearDown`
*external linkage* and makes them function pointers.
Even without including `unity.h` in this file,
these function pointers must be defined *and* assigned.

I define them globally:
```c
void (*setUp)(void); void (*tearDown)(void);
```

And inside `main()` I assign them to definitions:
```c
setUp = SetUp_Mock; tearDown = TearDown_Mock;
```
