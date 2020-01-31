# board-name: the PCB that uses this firmware{{{
board-name := usb-bridge
# board-name shows up as:
# - the main() `.c` file for the "avr-target"
# - the EAGLE .sch and .brd files
# - the Fusion360 model
# - the MacroFab/PCB:NG project
# }}}
# =====[ Build targets ]=====
# ---"unit-test" runs on PC with Cygwin---{{{
# Vim ;mktgc builds unit-test target using gcc
unit-test: build/TestSuite-results.md
# Vim opens .md opens in new bottom window
# }}}
# ---"avr-target" runs on ${board-name}---{{{
# Vim ;mka builds avr target using avr-gcc
# Download to flash:
# Vim ;fa downloads .elf to microcontroller
# Analyze:
# Vim ;ds pastes memory usage with time-stamp
# Disassembly:
# Vim `:e build/vis-spi-out.avra` and select block to analyze
# Vim: ;avrt measures cycles, ;avra extracts clean assembly
avr-target: build/${board-name}.elf build/${board-name}.avra
# }}}
#=====[ "avr-target" library (from AtmelStudio installation) ]=====
# atmega328_lib has the .o and .a lib files and the spec file.{{{
atmega328_lib = '/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'
# }}}
# AVR headers{{{
AvrHeaders := interrupt io
AvrHeaders := $(addsuffix .h,${AvrHeaders})
FakeAvrHeaders := ${AvrHeaders}
AvrHeaders := $(addprefix avr/,${AvrHeaders})
IncludeAvrHeaders := $(addprefix -include,${AvrHeaders})
FakeAvrHeaders := $(addprefix ../lib/test/FakeAvr/,${FakeAvrHeaders})
IncludeFakeAvrHeaders := $(addprefix -include,${FakeAvrHeaders})
# Hardware definitions for registers and bits:
# /cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include/avr/iom328p.h
# `AvrHeaders` are in the same folder.
# }}}
#=====[ "avr-target" Compiler and Linker flags ]=====
# CFLAGS{{{
# Compile hwlib obj files with AvrHeaders for avr-target.
CFLAGS_for_avr = -Isrc -I../lib/src ${IncludeAvrHeaders} \
-g -Wall -Wextra -pedantic -Winline -Wno-pointer-sign\
-O3 -ffunction-sections -fdata-sections -fshort-enums \
-mmcu=atmega328p
# }}}
# LFLAGS{{{
LFLAGS_for_avr := -B ${atmega328_lib}
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
	-Isrc -I../lib/src \
	-Itest -I../lib/test ${IncludeFakeAvrHeaders} \
	-g -Wall -Wextra -pedantic -Winline
# }}}
# LFLAGS{{{
LFLAGS_for_cygwin = -lglib-2.0 -lintl -L/usr/lib/glib-2.0
# }}}
# =====[ Use compiler to pick which flags to use ]=====
# avr-gcc{{{
ifeq ($(compiler),avr-gcc)
	CFLAGS := $(CFLAGS_for_avr)
	LFLAGS := $(LFLAGS_for_avr)
# }}}
# gcc/clang{{{
else
	CFLAGS := $(CFLAGS_for_cygwin)
	LFLAGS := $(LFLAGS_for_cygwin)
endif
# }}}
# no compiler, just print build rules{{{
ifeq ($(compiler),fake)
	CFLAGS := 
	LFLAGS := 
endif
# }}}
# =====[ Libs ]=====
# Add app libs here{{{
# App libs are application code with test coverage.
# List separate from other libs because path is different.
applib := Example
test_app := $(addprefix test_,${applib})
test_app_o := $(addsuffix .o,${test_app})
test_app_o := $(addprefix build/,${test_app_o})
app_o := $(addsuffix .o,${applib})
app_o := $(addprefix build/,${app_o})
# }}}
# Add hardware libs here{{{
# Hardware libs depend on AVR headers (register/bit definitions, macros, etc.).
# List separate from non-hardware libs to generate libname-Harware.h pre-reqs.
hwlib := BiColorLed
# }}}
# Add non-hardware libs here{{{
# Non-hardware libs do not depend on AVR headers.
nohwlib := ReadWriteBits
# }}}
# Group libs into build *pre-requisites*{{{
# Hardware libs add libname-Hardware.h file pre-requisites to build rules.
Hardware := ${hwlib}
HardwareFakes := ${Hardware}
Hardware := $(addsuffix -Hardware.h,${Hardware})
Hardware := $(addprefix src/,${Hardware})
HardwareFakes := $(addsuffix -HardwareFake.h,${HardwareFakes})
HardwareFakes := $(addprefix ../lib/test/,${HardwareFakes})
# }}}
# Group libs into *.o build targets* and *lib headers*{{{
all_libs := ${hwlib} ${nohwlib}
lib_o := $(addsuffix .o,${all_libs})
lib_o := $(addprefix ../lib/build/,${lib_o})
lib_headers := $(addsuffix .h,${all_libs})
lib_headers := $(addprefix ../lib/src/,${lib_headers})
# List libs that USE_FAKES in tests
lib_faked :=
lib_faked_o := $(addsuffix _faked.o,${lib_faked})
lib_faked_o := $(addprefix ../lib/build/,${lib_faked_o})
# }}}
# =====[ Build recipes ]=====
# ;mca - remember to clean all builds when switching build targets{{{
# "avr-target" and "unit-test" builds share same *lib* object file paths,
# but avr-gcc and gcc object files are obviously not compatible.
# ;mca - `:make clean-all-builds`{{{
.PHONY: clean-all-builds
clean-all-builds:
	rm -f build/${board-name}.elf
	rm -f build/${board-name}.avra
	rm -f build/${board-name}.map
	rm -f build/${board-name}.o
	rm -f build/TestSuite-results.md
	rm -f build/TestSuite.exe
	rm -f build/test_runner.o
	rm -f ${test_app_o}
	rm -f ${app_o}
	rm -f ${lib_o}
	rm -f ${lib_faked_o}
# }}}
# }}}
# ;mktgc - "unit-test" target: `TestSuite-results.md`{{{
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
# ;mka - "avr-target": `vis-spi-out.elf`{{{
# `usb-bridge.o`{{{
build/${board-name}.o: src/${board-name}.c src/Hardware.h ${Hardware} ${lib_headers} ../lib/src/StatusCode.h ../lib/src/StatusCodes.h ../lib/src/LisConfig.h ../lib/src/LisConfigs.h
	${compiler} $(CFLAGS) -c $< -o $@
# }}}
# `usb-bridge.elf`{{{
build/${board-name}.elf: build/${board-name}.o ${app_o} ${lib_o}
	avr-gcc $(CFLAGS) $^ -o $@ $(LFLAGS) \
		-Wl,-Map="build/${board-name}.map" -Wl,--gc-sections
# avr-size writes a summary to stdout with the size of each section.
	avr-size $@
# }}}
# `usb-bridge.avra`{{{
# generate .avra for disassembly analysis
build/%.avra: build/%.elf
	avr-objdump -h -S $^ > $@
# .avra: AVR flavor of .lst and .asm
#  -h: list space used by each section
#  -S: output the binary with source code
#}}}
# TODO: run `avr-nm`{{{
# avr-nm lists size of each function
#}}}
# TODO: analyze individual object files as well as final .elf{{{
# }}}
#}}}
# =====[ AVR programmer ]=====
# ;fa - download .elf to flash memory{{{
# ;mfa - make "avr-target", then download .elf to flash memory
.PHONY: download_flash
download_flash: build/${board-name}.elf
	atprogram.exe --tool atmelice --interface isp \
		--device atmega328p program --chiperase --verify --file $^
	avr-size $^
	# atprogram.exe --tool avrispmk2 --interface isp \
	# 	--device atmega328p program --chiperase --verify --file $^
	# avr-size $^
# }}}
# ;mkp - test USB communication with the AVR programmer{{{
.PHONY: test_programmer_is_connected
test_programmer_is_connected:
	atprogram.exe --tool atmelice --interface isp --device atmega328p info
	# atprogram.exe --tool avrispmk2 --interface isp --device atmega328p info
# }}}
# ;mkv - check voltage on target PCB{{{
.PHONY: display_target_voltage
display_target_voltage:
	atprogram.exe --tool atmelice --interface isp \
		--device atmega328p parameters --voltage
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
	echo Hardware:
	echo - ${Hardware}
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
	echo IncludeAvrHeaders:
	echo - ${IncludeAvrHeaders}
	echo
	echo IncludeFakeAvrHeaders:
	echo - ${IncludeFakeAvrHeaders}
	echo
# }}}
# vim:set fdm=marker:

