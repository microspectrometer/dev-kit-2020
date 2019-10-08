# variable for my test output name that is stupidly coupled to my .vimrc
test-output := build/TestSuite-results.md

#=====[ Targets (the first target is the default target) ]=====
unit-test: ${test-output}
file-in-current-window: build/${file-stem}.md

#=====[ Default: build and run stand-alone .c files, view .md output ]=====
    # move the file to be the first target in the list above
    #;mkc / ;mkl / ;mk+ to build
    #;re to open markdown in window, ;r<Space> to close markdown window
    #;mrc / ;mrl / ;mr+ to build *and* view markdown
    # variables I made in the Vim scripts that are up for use:
    # ${file-stem} to get the file in the current window
    # ${compiler} to pass the compiler (this is how `c` `l` `+` work)

#=====[ Compiler and Linker flags ]=====
path_shared-lib_mock-c := /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/
CFLAGS_for_cygwin := -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include \
	-I${path_shared-lib_mock-c}include -I${path_shared-lib_mock-c}test/unity \
	-Iinclude -Isrc \
	-g -Wall -Wextra -pedantic -Winline
LFLAGS_for_cygwin := -lglib-2.0 -lintl -L/usr/lib/glib-2.0

ifeq ($(compiler),fake)
	CFLAGS := 
	LFLAGS := 
else
	CFLAGS := $(CFLAGS_for_cygwin)
	LFLAGS := $(LFLAGS_for_cygwin)
endif

# List the libs under dev:
names-of-dev-libs := ReadWriteBits BiColorLed Ft1248 Usb Spi UartSpi Lis Pwm Auto
# All libs under dev will be tested!
# All libs under test need a `test_dev-lib.c` for the tests to build.
#
# But only some tests use mocks.
# List the dev libs tested with mocks:
names-of-libs-tested-with-mocks := Ft1248 Usb Spi UartSpi Lis

# And only some libs require mocking.
# List the dev libs that have mocked versions to remove dependencies:
mocked-dev-libs := Ft1248 Spi UartSpi Lis Pwm

# list of object prerequisites for      dev libs
obj_dev-libs := $(addsuffix .o,${names-of-dev-libs})
obj_dev-libs := $(addprefix build/,${obj_dev-libs})
# name convention for dev libs that are mocked
name_libs-mocked := $(addprefix mock_,${mocked-dev-libs})
# list of object prerequisites for      mocked dev libs
obj_libs-mocked := $(addsuffix .o,${name_libs-mocked})
obj_libs-mocked := $(addprefix build/,${obj_libs-mocked})
# name convention for dev lib tests
name_libs-tested   := $(addprefix test_,${names-of-dev-libs})
# list of object prerequisites for      dev lib tests
obj_libs-tested := $(addsuffix .o,${name_libs-tested})
obj_libs-tested := $(addprefix build/,${obj_libs-tested})
# name convention for dev lib tests that use mocks
name_libs-tested-with-mocks := $(addsuffix _MockUps,${names-of-libs-tested-with-mocks})
name_libs-tested-with-mocks := $(addprefix test_,${name_libs-tested-with-mocks})
# list of object prerequisites for      dev libs tested with mocks
obj_libs-tested-with-mocks := $(addsuffix .o,${name_libs-tested-with-mocks})
obj_libs-tested-with-mocks := $(addprefix build/,${obj_libs-tested-with-mocks})

# behold my static library hack:
# list of objects for unit testing with shared lib mock-c
mock-c-libs    := Mock RecordedCall RecordedArg ReturnValues
mock-c-objects := $(addsuffix .o,${mock-c-libs})
mock-c-objects := $(addprefix ${path_shared-lib_mock-c}build/,${mock-c-objects})
obj_unity := ${path_shared-lib_mock-c}build/unity.o
obj_shared-lib_mock-c := ${mock-c-objects} ${obj_unity}

#=====[ generic build recipe for file in current window ]=====
build/%.md: build/%.exe
	$^ > $@
build/%.exe: src/%.c ${obj_shared-lib_mock-c}
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)

#=====[ unit-test recipe ]=====
${test-output}: build/TestSuite.exe
	$^ > $@
test-runner := test/test_runner.c
avr-asm-macros := test/fake/AvrAsmMacros.c
build/TestSuite.exe: ${test-runner} ${avr-asm-macros} ${obj_dev-libs} ${obj_libs-tested} \
${obj_libs-tested-with-mocks} ${obj_libs-mocked} ${obj_shared-lib_mock-c}
	${compiler} $(CFLAGS) -Itest $^ -o $@ $(LFLAGS)

# =====[ Explicit rule to update mock-c object files if that lib changes ]=====
# ${obj_shared-lib_mock-c}: ${path_shared-lib_mock-c}build/%.o: src/%.c

#=====[ Explicit rule for obj_dev-libs ]=====
${obj_dev-libs}: build/%.o: src/%.c
	${compiler} $(CFLAGS) -c $^ -o $@

#=====[ Explicit rule for obj_libs-tested ]=====
${obj_libs-tested}: build/%.o: test/%.c
	${compiler} $(CFLAGS) -c $^ -o $@

#=====[ Explicit rule for obj_libs-tested-with-mocks ]=====
${obj_libs-tested-with-mocks}: build/%.o: test/%.c
	${compiler} $(CFLAGS) -c $^ -o $@

#=====[ Explicit rule for obj_libs-mocked ]=====
${obj_libs-mocked}: build/%.o: test/%.c
	${compiler} $(CFLAGS) -c $^ -o $@

# unused:
# rm -f build/${file-stem}.exe
# ;mct to clean out test only
# ;mca to clean all
.PHONY: clean-BiColorLed clean-TestSuite clean-all-builds
clean-BiColorLed:
	rm -f build/BiColorLed.o
clean-TestSuite:
	rm -f build/TestSuite.exe
clean-all-builds:
	rm -f build/TestSuite.exe
	rm -f ${obj_dev-libs}
	rm -f ${obj_libs-tested}
	rm -f ${obj_libs-tested-with-mocks}
	rm -f ${obj_libs-mocked}

