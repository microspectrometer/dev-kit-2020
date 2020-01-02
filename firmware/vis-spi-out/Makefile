# board-name is the main() file
board-name := vis-spi-out
#
# =====[ Build targets ]=====
#
# Vim ;mktgc builds unit-test target using gcc
unit-test: build/TestSuite-results.md
# Vim opens .md opens in new bottom window
#
# Vim ;mka builds avr target using avr-gcc
# Vim ;fa downloads .elf to microcontroller
avr-target: build/${board-name}.elf build/${board-name}.avra

#=====[ AVR library from AtmelStudio installation ]=====
# atmega328_lib has the .o and .a lib files and the spec file.
atmega328_lib = '/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'
#
AvrHeaders := interrupt io
AvrHeaders := $(addsuffix .h,${AvrHeaders})
FakeAvrHeaders := ${AvrHeaders}
AvrHeaders := $(addprefix avr/,${AvrHeaders})
IncludeAvrHeaders := $(addprefix -include,${AvrHeaders})
FakeAvrHeaders := $(addprefix ../lib/test/FakeAvr/,${FakeAvrHeaders})
IncludeFakeAvrHeaders := $(addprefix -include,${FakeAvrHeaders})

#=====[ AVR Compiler and Linker flags ]=====
CFLAGS_for_avr = -Isrc -I../lib/src ${IncludeAvrHeaders} \
-g -Wall -Wextra -pedantic -Winline -Wno-pointer-sign\
-O3 -ffunction-sections -fdata-sections -fshort-enums \
-mmcu=atmega328p
LFLAGS_for_avr := -B ${atmega328_lib}
#

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
# =====[ App runs on ${board-name} ]=====
app_src := Example
app_test := $(addprefix test_,${app_src})
app_build_test := $(addsuffix .o,${app_test})
app_build_test := $(addprefix build/,${app_build_test})
app_build_src := $(addsuffix .o,${app_src})
app_build_src := $(addprefix build/,${app_build_src})
# ---add hardware libs here that do not put inline in .h---
#  avr-gcc builds these with hardware defs via -include lib-Hardware.h
hw_lib_src := SpiSlave UartSpi Lis
# ---add hardware libs here that put inline in .h---
#  avr-gcc builds these libs without needing hardware defs
inlhw_lib_src := BiColorLed
# ---add nonhardware libs here---
nonhw_lib_src := ${inlhw_lib_src} ReadWriteBits Queue
# hardware libs add hardware file pre-requisites to the rule
Hardware := ${hw_lib_src} ${inlhw_lib_src}
HardwareFakes := ${Hardware}
Hardware := $(addsuffix -Hardware.h,${Hardware})
Hardware := $(addprefix src/,${Hardware})
HardwareFakes := $(addsuffix -HardwareFake.h,${HardwareFakes})
HardwareFakes := $(addprefix ../lib/test/,${HardwareFakes})
# lib_src := ${hw_lib_src} ${nonhw_lib_src}
# lib_build_src := $(addsuffix .o,${lib_src})
# lib_build_src := $(addprefix ../lib/build/,${lib_build_src})
hw_lib_build_src := $(addsuffix .o,${hw_lib_src})
hw_lib_build_src := $(addprefix ../lib/build/,${hw_lib_build_src})
nonhw_lib_build_src := $(addsuffix .o,${nonhw_lib_src})
nonhw_lib_build_src := $(addprefix ../lib/build/,${nonhw_lib_build_src})
lib_headers := ${hw_lib_src} ${inlhw_lib_src} ${nonhw_lib_src}
lib_headers := $(addsuffix .h,${lib_headers})
lib_headers := $(addprefix ../lib/src/,${lib_headers})

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
# build/TestSuite.exe: build/test_runner.o ${unittest_o} \
# ${app_build_test} ${app_build_src} ${lib_build_src} ../lib/build/SpiSlave.o
# 	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)
build/TestSuite.exe: build/test_runner.o ${unittest_o} \
${app_build_test} ${app_build_src} ${nonhw_lib_build_src} ${hw_lib_build_src}
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)
#
# translation units with tests must #include "unity.h" and "Mock.h"
build/test_runner.o: test/test_runner.c
	${compiler} $(CFLAGS) -c $< -o $@
# tests depend on fake hardware definitions
${app_build_test}: build/%.o: test/%.c test/%.h ../lib/test/HardwareFake.h ${HardwareFakes} ${FakeAvrHeaders}
	${compiler} $(CFLAGS) -c $< -o $@
#
${app_build_src}: build/%.o: src/%.c src/%.h
	${compiler} $(CFLAGS) -c $< -o $@
ifeq (0,1)
# unit-tests build libs without any Hardware definitions
../lib/build/SpiSlave.o: ../lib/src/SpiSlave.c ../lib/src/SpiSlave.h
	${compiler} $(CFLAGS) -c $< -o $@
# avr-target builds lib with lib-Hardware.h definitions
build/SpiSlave.o: ../lib/src/SpiSlave.c ../lib/src/SpiSlave.h src/SpiSlave-Hardware.h
	${compiler} -include src/SpiSlave-Hardware.h $(CFLAGS) -c $< -o $@
endif
${nonhw_lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
	${compiler} $(CFLAGS) -c $< -o $@
ifeq ($(compiler),avr-gcc)
${hw_lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h src/%-Hardware.h
	${compiler} -include src/$*-Hardware.h $(CFLAGS) -c $< -o $@
else
${hw_lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
	${compiler} ${IncludeFakeAvrHeaders} $(CFLAGS) -c $< -o $@
endif
# ${lib_build_src}: ../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
# 	${compiler} $(CFLAGS) -c $< -o $@
# Target `avr-target` ends up here:
build/${board-name}.o: src/${board-name}.c ${Hardware} ${lib_headers}
	${compiler} $(CFLAGS) -c $< -o $@
#
# build/${board-name}.elf: src/%.c ${app_build_src}
# build/${board-name}.elf: build/${board-name}.o ${app_build_src} ${lib_build_src} ../lib/build/SpiSlave.o
build/${board-name}.elf: build/${board-name}.o ${app_build_src} ${nonhw_lib_build_src} ${hw_lib_build_src}
	avr-gcc $(CFLAGS) $^ -o $@ $(LFLAGS) \
		-Wl,-Map="build/${board-name}.map" -Wl,--gc-sections
# avr-size writes a summary to stdout with the size of each section.
	avr-size $@
#
# Convert .elf to .avra for disassembly (avra: AVR version of lst).
#  -h: list space used by each section
#  -S: output the binary with source code
build/%.avra: build/%.elf
	avr-objdump -h -S $^ > $@
# Add avr-nm output. This lists size of each function!
# avr-nm 
# Also try using these on object files instead of final .elf

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
	rm -f build/${board-name}.avra
	rm -f build/${board-name}.map
	rm -f build/${board-name}.o
	rm -f build/TestSuite-results.md
	rm -f build/TestSuite.exe
	rm -f build/test_runner.o
	rm -f ${app_build_test}
	rm -f ${app_build_src}
	# rm -f ${lib_build_src}
	rm -f ${hw_lib_build_src}
	rm -f ${nonhw_lib_build_src}
	rm -f build/SpiSlave.o
	rm -f ../lib/build/SpiSlave.o
