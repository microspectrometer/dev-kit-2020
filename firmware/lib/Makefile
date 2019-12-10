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
# ---stub function-like macros in avr headers---
AvrHeaders := interrupt io
AvrHeaders := $(addsuffix .h,${AvrHeaders})
FakeAvrHeaders := ${AvrHeaders}
FakeAvrHeaders := $(addprefix test/FakeAvr/,${FakeAvrHeaders})
IncludeFakeAvrHeaders := $(addprefix -include,${FakeAvrHeaders})
#=====[ Unit Test Compiler and Linker flags ]=====
CFLAGS_for_cygwin = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include \
	-DSPISLAVE_FAKED \
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
build/TestSuite.exe: build/test_runner.o ${unittest_o} ${lib_build_test} ${lib_build_src} build/SpiSlave_faked.o
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)
#
# test translation units must #include "unity.h" and "Mock.h"
# test_runner translation unit requires fake hardware definitions exist
build/test_runner.o: test/test_runner.c test/HardwareFake.h ${HardwareFakes}
	${compiler} $(CFLAGS) -c $< -o $@
# test translation units do not depend on fake hardware definitions
${lib_build_test}: build/%.o: test/%.c test/%.h
	${compiler} $(CFLAGS) -c $< -o $@
# stub macros for lib translation units that include function-like avr macros
${lib_build_src}: build/%.o: src/%.c src/%.h ${FakeAvrHeaders}
	${compiler} ${IncludeFakeAvrHeaders} $(CFLAGS) -c $< -o $@
# fake functions
build/SpiSlave_faked.o: build/%.o: test/%.c test/%.h
	${compiler} $(CFLAGS) -c $< -o $@

# ;mca clean-all-builds
# ;mct clean-TestSuite (to force a rebuild)
.PHONY: clean-all-builds clean-TestSuite
clean-TestSuite:
	rm -f build/TestSuite.exe
	rm -f build/test_runner.o
clean-all-builds:
	rm -f build/TestSuite-results.md
	rm -f build/TestSuite.exe
	rm -f build/test_runner.o
	rm -f ${lib_build_src}
	rm -f ${lib_build_test}
	rm -f build/SpiSlave_faked.o
