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
mock-c-lib := /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/
CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include \
	-I ${mock-c-lib}include/ \
	-g -Wall -Wextra -pedantic
LFLAGS = -lglib-2.0 -lintl -L/usr/lib/glib-2.0
#	-L ${mock-c-lib}build/
#	For this to work, you need to make a static library:
#https://medium.com/@meghamohan/everything-you-need-to-know-about-libraries-in-c-e8ad6138cbb4
#Hack for now: create a variable to the path of the .o files to put in recipe

mock-c-lib-names := Mock RecordedCall RecordedArg
mock-c-lib-objects := $(addsuffix .o,${mock-c-lib-names})
mock-c-lib-objects := $(addprefix ${mock-c-lib}build/,${mock-c-lib-objects})
unity-lib-object := ${mock-c-lib}build/unity.o
unit-test-lib-objects := ${mock-c-lib-objects} ${unity-lib-object}

#=====[ generic build recipe for ${file-stem} target above ]=====
build/%.md: build/%.exe
	$^ > $@
build/%.exe: src/%.c ${unit-test-lib-objects}
	${compiler} $^ -o $@ $(LFLAGS) $(CFLAGS) \
		-I src/


#;mc
.PHONY: clean-all-builds
clean-all-builds:
	rm -f build/${file-stem}.exe
	rm -f build/TestSuite.exe
	rm -f ${lib-objects}
	rm -f ${lib-test-objects}


