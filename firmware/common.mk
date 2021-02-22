# {{{ HELP
help-fuses:
	@echo
	@echo
	@echo
	@cat ../help-fuses.man | less

help-flash:
	@echo
	@echo
	@echo
	@cat ../help-flash.man | less
# }}}


# {{{ Build Targets
# .md is the unit test output
# Vim: ;mktgc -- make -w unit-test compiler=gcc
unit-test: build/TestSuite-results.md

# .elf is the executable to download to flash
# Vim: ;mka, ;fa
# .avra is the disassembly for analysis
# Vim ;ds pastes memory usage with time-stamp
# Disassembly:
# Vim `:e build/vis-spi-out.avra` and select block to analyze
# Vim: ;avrt measures cycles, ;avra extracts clean assembly
avr-elf: build/${board-name}.elf build/${board-name}.avra
# }}}

# {{{ Unit Test Framework
# TODO: move framework off of Dropbox into a repository
# TODO: include framework as a static library
unittest-c := /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/
# unity is made by ThrowTheSwitch
# TODO: check unity software license
# mock-c is made by Chromation
path_unity  := ${unittest-c}mock-c/test/unity
path_mock-c := ${unittest-c}mock-c/
# Pre-compiled object files
unity_libs  := unity
unity_libo  := $(addsuffix .o,${unity_libs})
unity_libo  := $(addprefix ${unittest-c}mock-c/build/,${unity_libo})
mock-c_libs := Mock RecordedCall RecordedArg ReturnValues
mock-c_libo := $(addsuffix .o,${mock-c_libs})
mock-c_libo := $(addprefix ${unittest-c}mock-c/build/,${mock-c_libo})
unittest_o  := ${unity_libo} ${mock-c_libo}
# }}}

# {{{ AVR Headers
# Hardware definitions for registers and bits are in "iom328p.h":
# Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/iom328p.h
# "iom328p.h" is included by including AvrHeader "avr/io".
# Builds for "avr" use AvrHeaders.
# Builds for "tests" use FakeAvrHeaders.
# AvrHeaders are included by {board-name} code.
# FakeAvrHeaders are included by unit-test code.
# For every AvrHeader, a FakeAvrHeader is required for tests to build, even if
# the FakeAvrHeader is simply an empty file.
AvrHeaders := interrupt io
AvrHeaders := $(addsuffix .h,${AvrHeaders})
FakeAvrHeaders := ${AvrHeaders}
AvrHeaders := $(addprefix avr/,${AvrHeaders})
IncludeAvrHeaders := $(addprefix -include,${AvrHeaders})
FakeAvrHeaders := $(addprefix ../lib/test/FakeAvr/,${FakeAvrHeaders})
IncludeFakeAvrHeaders := $(addprefix -include,${FakeAvrHeaders})
# }}}

# {{{ Compiler and Linker flags
CFLAGS_for_avr = -Isrc -I../lib/src ${IncludeAvrHeaders} \
	-g -Wall -Wextra -pedantic -Winline -Wno-pointer-sign\
	-O3 -ffunction-sections -fdata-sections -fshort-enums \
	-mmcu=atmega328p
LFLAGS_for_avr := -B ${atmega328_lib}
CFLAGS_for_tests = -I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-DUSE_FAKES \
	-I${path_mock-c}include -I${path_unity} \
	-Isrc -I../lib/src \
	-Itest -I../lib/test ${IncludeFakeAvrHeaders} \
	-g -Wall -Wextra -pedantic -Winline
LFLAGS_for_tests = -lglib-2.0 -lintl -L/usr/lib/glib-2.0

# define compiler as avr-gcc if not defined when make is called
compiler ?= avr-gcc

# avr-gcc{{{
ifeq ($(compiler),avr-gcc)
	CFLAGS := $(CFLAGS_for_avr)
	LFLAGS := $(LFLAGS_for_avr)
# }}}
# gcc/clang{{{
else
	CFLAGS := $(CFLAGS_for_tests)
	LFLAGS := $(LFLAGS_for_tests)
endif
# }}}
# no compiler, just print build rules{{{
ifeq ($(compiler),fake)
	CFLAGS := 
	LFLAGS := 
endif
# }}}
# }}}

#{{{ Lib Object Files and Prerequisites
# applib, hwlib, and nohwlib are defined in ${board-name}/Makefile
test_app := $(addprefix test_,${applib})
test_app_o := $(addsuffix .o,${test_app})
test_app_o := $(addprefix build/,${test_app_o})
app_o := $(addsuffix .o,${applib})
app_o := $(addprefix build/,${app_o})
Hardware := ${hwlib}
HardwareFakes := ${Hardware}
Hardware := $(addsuffix -Hardware.h,${Hardware})
Hardware := $(addprefix src/,${Hardware})
HardwareFakes := $(addsuffix -HardwareFake.h,${HardwareFakes})
HardwareFakes := $(addprefix ../lib/test/,${HardwareFakes})
all_libs := ${hwlib} ${nohwlib}
lib_o := $(addsuffix .o,${all_libs})
lib_o := $(addprefix ../lib/build/,${lib_o})
lib_headers := $(addsuffix .h,${all_libs})
lib_headers := $(addprefix ../lib/src/,${lib_headers})
lib_faked_o := $(addsuffix _faked.o,${lib_faked})
lib_faked_o := $(addprefix ../lib/build/,${lib_faked_o})
#}}}

# Build Recipes {{{
# $ make clean {{{
# Clean all builds when switching build targets.
# If not, expect linker error: "error adding symbols: file in wrong format"
# Vim ;mca - clean all builds
.PHONY: clean
clean:
	rm -f build/${board-name}.elf
	rm -f build/${board-name}.avra
	rm -f build/${board-name}.map
	rm -f build/${board-name}.o
	rm -f sig.log
	rm -f fuses.log
	rm -f build/TestSuite-results.md
	rm -f build/TestSuite.exe
	rm -f build/test_runner.o
	rm -f ${test_app_o}
	rm -f ${app_o}
	rm -f ${lib_o}
	rm -f ${lib_faked_o}
# }}}
# $ make unit-test {{{
# Vim ;mktgc - make tests with gcc
build/TestSuite-results.md: build/TestSuite.exe
	$^ > $@
build/TestSuite.exe: build/test_runner.o ${unittest_o} \
${lib_o} ${test_app_o} ${app_o} ${lib_faked_o}
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)
# test-runner rebuilds if fake hardware definitions change
build/test_runner.o: test/test_runner.c ../lib/test/HardwareFake.h ${HardwareFakes} ${lib_headers} ../lib/src/StatusCode.h ../lib/src/StatusCodes.h ../lib/src/LisConfig.h ../lib/src/LisConfigs.h
	${compiler} $(CFLAGS) -c $< -o $@
# unit-test translation units must #include "unity.h" and "Mock.h"
${test_app_o}: build/%.o: test/%.c test/%.h
	${compiler} $(CFLAGS) -c $< -o $@
${app_o}: build/%.o: src/%.c src/%.h
	${compiler} $(CFLAGS) -c $< -o $@
# For tests: stub function-like macros used in libs in FakeAvrHeaders
${lib_o}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h ${FakeAvrHeaders}
	${compiler} $(CFLAGS) -c $< -o $@
# fake functions
${lib_faked_o}: ../lib/build/%.o: ../lib/test/%.c ../lib/test/%.h
	${compiler} $(CFLAGS) -c $< -o $@
# }}}
# $ make avr-elf {{{
# `board-name.o`{{{
build/${board-name}.o: src/${board-name}.c src/Hardware.h ${Hardware} ${lib_headers} ../lib/src/StatusCode.h ../lib/src/StatusCodes.h ../lib/src/LisConfig.h ../lib/src/LisConfigs.h
	${compiler} $(CFLAGS) -c $< -o $@
# }}}
# `board-name.elf`{{{
build/${board-name}.elf: build/${board-name}.o ${app_o} ${lib_o}
	avr-gcc $(CFLAGS) $^ -o $@ $(LFLAGS) \
		-Wl,-Map="build/${board-name}.map" -Wl,--gc-sections
# avr-size writes a summary to stdout with the size of each section.
	avr-size $@
# }}}
# `board-name.avra`{{{
# generate .avra for disassembly analysis
build/%.avra: build/%.elf
	avr-objdump -h -S $^ > $@
# .avra: AVR flavor of .lst and .asm
#  -h: list space used by each section
#  -S: output the binary with source code
# }}}
# }}}
# TODO: run `avr-nm`{{{
# avr-nm lists size of each function
#}}}
# TODO: analyze individual object files as well as final .elf{{{
# }}}
# }}}

# Flash Programmer Recipes {{{
# $ make flash {{{
# Vim ;fa -- load .elf on microcontroller
# Vim ;mfa -- make avr-elf, then load .elf on microcontroller
.PHONY: flash
flash: build/${board-name}.elf
	atprogram.exe --tool atmelice --interface isp --device atmega328p \
		program --chiperase --verify --file $^
	avr-size $^
# }}}
# $ make fuses {{{
.PHONY: fuses
fuses:
	atprogram.exe --tool atmelice --interface isp --device atmega328p \
		write --verify --fuses --values F7D9FF
# }}}
# $ make log-fuses {{{
.PHONY: log-fuses
log-fuses:
	@echo Reading fuses...
	@atprogram.exe --tool atmelice --interface isp --device atmega328p \
		read --format hex --fuses \
		> fuses.log
	@cat fuses.log
	@echo Fuses logged in file 'fuses.log'
# }}}
# $ make log-signature {{{
.PHONY: log-signature
log-signature:
	atprogram.exe --tool atmelice --interface isp --device atmega328p \
		read --format hex --signatures \
		> sig.log
# }}}
# $ make test-isp{{{
.PHONY: test-isp
test-isp:
	atprogram.exe --tool atmelice --interface isp --device atmega328p \
		info
# }}}
# $ make test-voltage{{{
# Vim ;mkv
.PHONY: test-voltage
test-voltage:
	atprogram.exe --tool atmelice --interface isp --device atmega328p \
		parameters --voltage
# }}}
# reset all microcontrollers {{{
# Vim ;mkr
.PHONY: reset
reset:
	atprogram.exe --tool atmelice --interface isp --device atmega328p \
		reset
# }}}
# }}}

# {{{ Debug Makefile
# $ make print-vars
# Vim ;mpv -- paste variables into buffer
.PHONY: print-vars
print-vars:
	@echo sensor:
	@echo - ${sensor}
	@echo
	@echo hwlib:
	@echo - ${hwlib}
	@echo
	@echo nohwlib:
	@echo - ${nohwlib}
	@echo
	@echo Hardware:
	@echo - ${Hardware}
	@echo
	@echo HardwareFakes:
	@echo - ${HardwareFakes}
	@echo
	@echo lib_o:
	@echo - ${lib_o}
	@echo
	@echo lib_faked_o:
	@echo - ${lib_faked_o}
	@echo
	@echo lib_headers:
	@echo - ${lib_headers}
	@echo
	@echo IncludeAvrHeaders:
	@echo - ${IncludeAvrHeaders}
	@echo
	@echo IncludeFakeAvrHeaders:
	@echo - ${IncludeFakeAvrHeaders}
	@echo
#}}}

# vim:set fdm=marker:

