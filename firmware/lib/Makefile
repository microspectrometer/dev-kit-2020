help:
	@echo
	@echo
	@echo
	@echo "----------------"
	@echo "|     HELP     |"
	@echo "----------------"
	@echo
	@echo "TODO: clean up this Makefile"
	@echo
	@echo "  For now, do not run make here."
	@echo "  Run make from folder usb-bridge"
	@echo "  or from folder vis-spi-out."

# =====[ Build targets ]=====
# ---"unit-test" runs on PC with Cygwin---{{{
# Vim ;mktgc builds unit-test target using gcc
unit-test: build/TestSuite-results.md
# Vim opens .md opens in new bottom window
# }}}
#=====[ hardware library fake ]=====
# AVR headers{{{
# FakeAvrHeaders stub the AVR function-like-macros, e.g., cli()
FakeAvrHeaders := interrupt io
FakeAvrHeaders := $(addsuffix .h,${FakeAvrHeaders})
FakeAvrHeaders := $(addprefix test/FakeAvr/,${FakeAvrHeaders})
IncludeFakeAvrHeaders := $(addprefix -include,${FakeAvrHeaders})
# Hardware definitions for registers and bits:
# /cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/iom328p.h
# `AvrHeaders` are in the same folder.
# }}}
#=====[ "unit-test" library (from ThrowTheSwitch and Chromation) ]=====
# Paths{{{
path_tdd := /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/
path_unity  := ${path_tdd}mock-c/test/unity
path_mock-c := ${path_tdd}mock-c/
# }}}
# Pre-compiled object files{{{
unity_libs  := unity
unity_libo  := $(addsuffix .o,${unity_libs})
unity_libo  := $(addprefix ${path_tdd}mock-c/build/,${unity_libo})
mock-c_libs := Mock RecordedCall RecordedArg ReturnValues
mock-c_libo := $(addsuffix .o,${mock-c_libs})
mock-c_libo := $(addprefix ${path_tdd}mock-c/build/,${mock-c_libo})
unittest_o  := ${unity_libo} ${mock-c_libo}
# }}}
#=====[ "unit-test" Compiler and Linker flags ]=====
# CFLAGS{{{
# Compile hwlib obj files with FakeAvrHeaders for unit tests.
CFLAGS_for_cygwin = -I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-DUSE_FAKES \
	-I${path_mock-c}include -I${path_unity} \
	-Isrc \
	-Itest ${IncludeFakeAvrHeaders} \
	-g -Wall -Wextra -pedantic -Winline
# }}}
# LFLAGS{{{
LFLAGS_for_cygwin = -lglib-2.0 -lintl -L/usr/lib/glib-2.0
# }}}
# =====[ Use compiler to pick which flags to use ]=====
# no compiler, just print build rules{{{
ifeq ($(compiler),fake)
	CFLAGS := 
	LFLAGS := 
# }}}
# gcc/clang{{{
else
	CFLAGS := $(CFLAGS_for_cygwin)
	LFLAGS := $(LFLAGS_for_cygwin)
endif
# }}}
# =====[ Libs ]=====
# Add hardware libs here{{{
# Hardware libs depend on AVR headers (register/bit definitions, macros, etc.).
# List separate from non-hardware libs to generate libname-Harware.h pre-reqs.
hwlib := BiColorLed Spi SpiSlave SpiMaster UartSpi Lis Flag Usb
# }}}
# Add non-hardware libs here{{{
# Non-hardware libs do not depend on AVR headers.
nohwlib := ReadWriteBits Queue StatusCode
# }}}
# Group libs into build *pre-requisites*{{{
# Hardware libs add libname-HardwareFake.h file pre-requisites to build rules.
HardwareFakes := ${hwlib}
HardwareFakes := $(addsuffix -HardwareFake.h,${HardwareFakes})
HardwareFakes := $(addprefix test/,${HardwareFakes})
# }}}
# Group libs into *.o build targets* and *lib headers*{{{
all_libs := ${hwlib} ${nohwlib}
lib_o := $(addsuffix .o,${all_libs})
lib_o := $(addprefix build/,${lib_o})
test_lib_o := $(addsuffix .o,${all_libs})
test_lib_o := $(addprefix build/test_,${test_lib_o})
lib_headers := $(addsuffix .h,${all_libs})
lib_headers := $(addprefix src/,${lib_headers})
# List libs that USE_FAKES in tests
lib_faked := Spi SpiSlave Lis Usb
lib_faked_o := $(addsuffix _faked.o,${lib_faked})
lib_faked_o := $(addprefix build/,${lib_faked_o})
# }}}
# =====[ Build recipes ]=====
# ;mct - `:make clean-TestSuite` (to force a rebuild){{{
.PHONY: clean-TestSuite
clean-TestSuite:
	rm -f build/TestSuite.exe
	rm -f build/test_runner.o
# }}}
# ;mca - remember to clean all builds when switching build targets{{{
# "avr-target" and "unit-test" builds share same *lib* object file paths,
# but avr-gcc and gcc object files are obviously not compatible.
# ;mca - `:make clean`{{{
.PHONY: clean
clean:
	rm -f build/TestSuite-results.md
	rm -f build/TestSuite.exe
	rm -f build/test_runner.o
	rm -f ${lib_o}
	rm -f ${test_lib_o}
	rm -f ${lib_faked_o}
# }}}
# }}}
# ;mktgc - "unit-test" target: `TestSuite-results.md`{{{
build/TestSuite-results.md: build/TestSuite.exe
	$^ > $@
build/TestSuite.exe: build/test_runner.o ${unittest_o} \
${lib_o} ${test_lib_o} ${lib_faked_o}
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)
# test-runner rebuilds if fake hardware definitions change
build/test_runner.o: test/test_runner.c test/HardwareFake.h ${HardwareFakes} ${lib_headers}
	${compiler} $(CFLAGS) -c $< -o $@
# unit-test translation units must #include "unity.h" and "Mock.h"
${test_lib_o}: build/%.o: test/%.c test/%.h
	${compiler} $(CFLAGS) -c $< -o $@
# fake functions
${lib_faked_o}: build/%.o: test/%.c test/%.h
	${compiler} $(CFLAGS) -c $< -o $@
# For tests: stub function-like macros used in libs in FakeAvrHeaders
${lib_o}: build/%.o: src/%.c src/%.h ${FakeAvrHeaders}
	${compiler} $(CFLAGS) -c $< -o $@
# }}}
# =====[ Check Makefile variables ]=====
# ;mpv - paste variables into documentation{{{
.PHONY: print_vars
print_vars:
	echo hwlib:
	echo - ${hwlib}
	echo
	echo nohwlib:
	echo - ${nohwlib}
	echo
	echo HardwareFakes:
	echo - ${HardwareFakes}
	echo
	echo lib_o:
	echo - ${lib_o}
	echo
	echo lib_faked_o:
	echo - ${lib_faked_o}
	echo
	echo lib_headers:
	echo - ${lib_headers}
	echo
	echo IncludeFakeAvrHeaders:
	echo - ${IncludeFakeAvrHeaders}
	echo
# }}}
# vim:set fdm=marker:
