#=====[ Targets ]=====
${file-stem}: build/${file-stem}.md

#=====[ Default: build and run stand-alone .c files, view .md output ]=====
    # move the file to be the first target in the list above
    #;mkc / ;mkl / ;mk+ to build
    #;re to open markdown in window, ;r<Space> to close markdown window
    #;mrc / ;mrl / ;mr+ to build *and* view markdown
    # variables I made in the Vim scripts that are up for use:
    # ${file-stem} to get the file in the current window
    # ${compiler} to pass the compiler (this is how `c` `l` `+` work)

#=====[ Compiler and Linker flags ]=====
CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include \
	-g -Wall -Wextra -pedantic
LFLAGS = -lglib-2.0 -lintl -L/usr/lib/glib-2.0

#=====[ generic build recipe for ${file-stem} target above ]=====
build/%.md: build/%.exe
	$^ > $@
build/%.exe: src/%.c
	${compiler} $^ -o $@ $(LFLAGS) $(CFLAGS) \
		-I src/


#;mc
.PHONY: clean-all-builds
clean-all-builds:
	rm -f build/${file-stem}.exe
	rm -f build/TestSuite.exe
	rm -f ${lib-objects}
	rm -f ${lib-test-objects}


