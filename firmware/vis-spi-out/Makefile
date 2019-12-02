board-name := vis-spi-out
#
# =====[ Build targets ]=====
#
# Vim ;mktgc builds unit-test target using gcc
unit-test: build/TestSuite-results.md
# .md opens in new bottom Vim window
#
# Vim ;mka builds avr target using avr-gcc
# Vim ;fa downloads .elf to microcontroller
avr-target: build/${board-name}.elf build/${board-name}.lst

#=====[ AVR library from AtmelStudio installation ]=====
# atmega328_lib has the .o and .a lib files and the spec file.
atmega328_lib = '/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'
#
#=====[ AVR Compiler and Linker flags ]=====
CFLAGS_for_avr = -Isrc -I../lib/src \
-g -Wall -Wextra -pedantic -Winline -Wno-pointer-sign\
-O3 -ffunction-sections -fdata-sections -fshort-enums \
-mmcu=atmega328p
LFLAGS_for_avr := -B ${atmega328_lib}
#

#=====[ Unit Test Framework Paths ]=====
path_tdd := /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/
path_unity  := ${path_tdd}mock-c/test/unity
path_mock-c := ${path_tdd}mock-c/
#=====[ Unit Test Object Files ]=====
unity_libs  := unity
unity_libo  := $(addsuffix .o,${unity_libs})
unity_libo  := $(addprefix ${path_tdd}mock-c/build/,${unity_libo})
mock-c_libs := Mock RecordedCall RecordedArg ReturnValues
mock-c_libo := $(addsuffix .o,${mock-c_libs})
mock-c_libo := $(addprefix ${path_tdd}mock-c/build/,${mock-c_libo})
unittest_o  := ${unity_libo} ${mock-c_libo}

# =====[ App ]=====
app_libs := Example
app_lib_tests := $(addprefix test_,${app_libs})
app_lib_testo := $(addsuffix .o,${app_lib_tests})
app_lib_testo := $(addprefix build/,${app_lib_testo})
app_libo := $(addsuffix .o,${app_libs})
app_libo := $(addprefix build/,${app_libo})

#=====[ Unit Test Compiler and Linker flags ]=====
CFLAGS_for_cygwin = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include \
	-I${path_mock-c}include -I${path_unity} \
	-I../lib/src -Isrc \
	-Itest -I../lib/test \
	-g -Wall -Wextra -pedantic -Winline
LFLAGS_for_cygwin = -lglib-2.0 -lintl -L/usr/lib/glib-2.0
# -I../lib/test includes the project libs `test` folder.
#    This picks up the headers for project lib mocks and the hardware-dependency fakes.
#
# =====[ Use compiler to pick which flags to use ]=====
ifeq ($(compiler),avr-gcc)
	CFLAGS := $(CFLAGS_for_avr)
	LFLAGS := $(LFLAGS_for_avr)
else
	CFLAGS := $(CFLAGS_for_cygwin)
	LFLAGS := $(LFLAGS_for_cygwin)
endif
ifeq ($(compiler),fake)
	CFLAGS := 
	LFLAGS := 
endif

# =====[ Build recipes ]=====
# AVR and Unit Test builds use the same *lib* object file paths.
# Clean vis-spi-out/build/ (;mca) when you switch between targets.
#

# Target `unit-test` ends up here:
build/TestSuite-results.md: build/TestSuite.exe
	$^ > $@
#
build/TestSuite.exe: test/test_runner.c ${unittest_o} ${app_lib_testo} ${app_libo}
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)
#
# lib tests #include "unity.h" and "Mock.h"
${app_lib_testo}: build/%.o: test/%.c
	${compiler} $(CFLAGS) -c $^ -o $@
#
${app_libo}: build/%.o: src/%.c
	${compiler} $(CFLAGS) -c $^ -o $@
# LFLAGS is for avr-target to link against atmega328_lib
# ${compiler} $(CFLAGS) -c $^ -o $@ $(LFLAGS)

# Target `avr-target` ends up here:
# build/${board-name}.elf: src/%.c ${app_libo}
build/%.elf: src/%.c ${app_libo}
	avr-gcc $(CFLAGS) $^ -o $@ $(LFLAGS) \
		-Wl,-Map="build/$*.map" -Wl,--gc-sections
# avr-size writes a summary to stdout with the size of each section.
	avr-size $@
#
# Convert .elf to .lst for disassembly.
#  -h: list space used by each section
#  -S: output the binary with source code
build/%.lst: build/%.elf
	avr-objdump -h -S $^ > $@

#--------------------------------------------------------------
# ;fa or ;mfa
# Get rid of --verbose to just display `Firmware check OK`
# Add `avr-size blah.elf` to see how much memory is used.
.PHONY: download_flash
download_flash: build/${board-name}.elf
	atprogram.exe --tool atmelice --interface isp \
		--device atmega328p program --chiperase --verify --file $^
	avr-size $^
	# atprogram.exe --tool avrispmk2 --interface isp \
	# 	--device atmega328p program --chiperase --verify --file $^
	# avr-size $^
#
#---Development---
# ;mkp
# Test USB communication with the AVR programmer.
.PHONY: test_programmer_is_connected
test_programmer_is_connected:
	atprogram.exe --tool atmelice --interface isp --device atmega328p info
	# atprogram.exe --tool avrispmk2 --interface isp --device atmega328p info
# ;mkv
# Check voltage on target PCB. 2018-03-20: I see 2.94V.
# This is included in the output of 'info'.
.PHONY: display_target_voltage
display_target_voltage:
	atprogram.exe --tool atmelice --interface isp \
		--device atmega328p parameters --voltage
#----------------------------------------------------------------------

#;mca
.PHONY: clean-all-builds
clean-all-builds:
	rm -f build/${board-name}.elf
	rm -f build/${board-name}.lst
	rm -f build/${board-name}.map
	rm -f build/TestSuite-results.md
	rm -f build/TestSuite.exe
	rm -f ${app_lib_testo}
	rm -f ${app_libo}
