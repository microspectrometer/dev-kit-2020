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
slib-mock-c := /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/
CFLAGS = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include \
	-I${slib-mock-c}include -I${slib-mock-c}test/unity \
	-Iinclude -Isrc \
	-g -Wall -Wextra -pedantic
LFLAGS = -lglib-2.0 -lintl -L/usr/lib/glib-2.0

# List the libs under dev:
dev-libs := ReadWriteBits
# List the dev libs that have mocked versions to remove dependencies:
mocked-dev-libs := 

# variables for             dev lib      object prerequisites
dev-lib-objects := $(addsuffix .o,${dev-libs})
dev-lib-objects := $(addprefix build/,${dev-lib-objects})
# name convention for dev libs that are mocked
mocked-names := $(addprefix mock_,${mocked-dev-libs})
# variables for      mocked dev lib      object prerequisites
mocked-lib-objects := $(addsuffix .o,${mocked-names})
mocked-lib-objects := $(addprefix build/,${mocked-lib-objects})
# name convention for dev lib tests
test-names   := $(addprefix test_,${dev-libs})
# variables for             dev lib test object prerequisites
test-lib-objects := $(addsuffix .o,${test-names})
test-lib-objects := $(addprefix build/,${test-lib-objects})

# behold my static library hack:
# variables for unit testing with shared lib mock-c
mock-c-libs    := Mock RecordedCall RecordedArg
mock-c-objects := $(addsuffix .o,${mock-c-libs})
mock-c-objects := $(addprefix ${slib-mock-c}build/,${mock-c-objects})
unity-object := ${slib-mock-c}build/unity.o
slib-mock-c-objects := ${mock-c-objects} ${unity-object}

#=====[ generic build recipe for file in current window ]=====
build/%.md: build/%.exe
	$^ > $@
build/%.exe: src/%.c ${slib-mock-c-objects}
	${compiler} $^ -o $@ $(LFLAGS) $(CFLAGS)

#=====[ unit-test recipe ]=====
${test-output}: build/TestSuite.exe
	$^ > $@
test-runner := test/test_runner.c
build/TestSuite.exe: ${test-runner} \
${test-lib-objects} ${dev-lib-objects} ${mocked-lib-objects} ${slib-mock-c-objects}
	${compiler} $^ -o $@ $(LFLAGS) $(CFLAGS) \
		-Itest

#=====[ Explicit rule for test-lib-objects ]=====
${test-lib-objects}: build/%.o: test/%.c
	${compiler} -c $^ -o $@ $(CFLAGS)

#=====[ Explicit rule for dev-lib-objects ]=====
${dev-lib-objects}: build/%.o: src/%.c
	${compiler} -c $^ -o $@ $(CFLAGS)

#;mc
.PHONY: clean-all-builds
clean-all-builds:
	rm -f build/${file-stem}.exe
	rm -f build/TestSuite.exe
	rm -f ${dev-lib-objects}
	rm -f ${test-lib-objects}
	rm -f ${mocked-lib-objects}

