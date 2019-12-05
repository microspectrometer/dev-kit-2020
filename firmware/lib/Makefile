# Vim ;mktgc builds unit-test target using gcc
unit-test: build/TestSuite-results.md
# .md opens in new bottom Vim window

#=====[ Unit Test Framework Paths ]=====
path_tdd := /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/
path_unity  := ${path_tdd}mock-c/test/unity
path_mock-c := ${path_tdd}mock-c/
#=====[ Unit Test Framework Files ]=====
unity_libs  := unity
unity_libo  := $(addsuffix .o,${unity_libs})
unity_libo  := $(addprefix ${path_tdd}mock-c/build/,${unity_libo})
mock-c_libs := Mock RecordedCall RecordedArg ReturnValues
mock-c_libo := $(addsuffix .o,${mock-c_libs})
mock-c_libo := $(addprefix ${path_tdd}mock-c/build/,${mock-c_libo})
unittest_o  := ${unity_libo} ${mock-c_libo}
#=====[ Unit Test runs on PC with Cygwin ]=====
lib_src := BiColorLed
lib_build_src := $(addsuffix .o,${lib_src})
lib_build_src := $(addprefix build/,${lib_build_src})
HardwareFake := ${lib_src}
HardwareFake := $(addsuffix -HardwareFake.h,${HardwareFake})
lib_build_test := $(addsuffix .o,${lib_src})
lib_build_test := $(addprefix build/test_,${lib_build_test})

#=====[ Unit Test Compiler and Linker flags ]=====
CFLAGS_for_cygwin = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include \
	-I${path_mock-c}include -I${path_unity} \
	-Isrc -Itest \
	-g -Wall -Wextra -pedantic -Winline
LFLAGS_for_cygwin = -lglib-2.0 -lintl -L/usr/lib/glib-2.0
# -I../lib/test includes the project libs `test` folder.
#    This picks up the headers for project lib mocks and the hardware-dependency fakes.
#
# =====[ Use compiler to pick which flags to use ]=====
ifeq ($(compiler),fake)
	CFLAGS := 
	LFLAGS := 
else
	CFLAGS := $(CFLAGS_for_cygwin)
	LFLAGS := $(LFLAGS_for_cygwin)
endif

# Target `unit-test` ends up here:
build/TestSuite-results.md: build/TestSuite.exe
	$^ > $@
#
build/TestSuite.exe: test/test_runner.o ${unittest_o} ${lib_build_test} ${lib_build_src}
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)
#
# translation units with tests must #include "unity.h" and "Mock.h"
test/test_runner.o: test/test_runner.c
	${compiler} $(CFLAGS) -c $< -o $@
# tests depend on fake hardware definitions
${lib_build_test}: build/%.o: test/%.c test/%.h test/${HardwareFake} test/HardwareFake.h
	${compiler} $(CFLAGS) -c $< -o $@
#
${lib_build_src}: build/%.o: src/%.c src/%.h
	${compiler} $(CFLAGS) -c $< -o $@

# ;mca clean-all-builds
# ;mct clean-TestSuite (to force a rebuild)
.PHONY: clean-all-builds clean-TestSuite
clean-TestSuite:
	rm -f build/TestSuite.exe
clean-all-builds:
	rm -f build/TestSuite-results.md
	rm -f build/TestSuite.exe
	rm -f ${lib_build_src}
	rm -f ${lib_build_test}
