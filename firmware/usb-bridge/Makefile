board-name := usb-bridge

# variable for my test output name that is stupidly coupled to my .vimrc
test-output := build/TestSuite-results.md

#=====[ Targets (the first target is the default target) ]=====
# Build unit-test targets using Vim shortcuts starting with ;mkt
unit-test: ${test-output}
# Build avr targets using Vim shortcut ;mka
# Create executable for uploading to the microcontroller. Eliminate unused code.
# .elf is the executable to download to flash
# .lst is the disassembly for analysis
avr-target: build/${board-name}.elf build/${board-name}.lst


#=====[ AVR Compiler and Linker flags ]=====
CFLAGS_for_avr = -I../lib/src -Isrc \
	-g -Wall -Wextra -pedantic -Wno-pointer-sign\
	-O3 -ffunction-sections -fdata-sections -fshort-enums \
		-mmcu=atmega328p -B ${atmega328_lib}
#Used to be -O1, not sure why.
#The avr ld.exe does not recognize any of these
#LFLAGS = -lglib-2.0 -lintl -L/usr/lib/glib-2.0
LFLAGS_for_avr := 

#=====[ Unit Test Compiler and Linker flags ]=====
path_shared-lib_mock-c := /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/
CFLAGS_for_cygwin = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include \
	-I${path_shared-lib_mock-c}include -I${path_shared-lib_mock-c}test/unity \
	-Iinclude -I../lib/src -Isrc \
	-g -Wall -Wextra -pedantic -Winline
LFLAGS_for_cygwin = -lglib-2.0 -lintl -L/usr/lib/glib-2.0

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


#=====[ Project Libs: objects built for AVR ]=====
# List the project libs:
names-of-dev-libs := ReadWriteBits BiColorLed Ft1248 Usb Spi Lis
# list of object prerequisites for dev libs
obj_dev-libs := $(addsuffix .o,${names-of-dev-libs})
obj_dev-libs := $(addprefix build/,${obj_dev-libs})

#=====[ Explicit rule for objects for AVR ]=====
# avr-gcc has to build the lib code into its own object files
# the `ld` linker does not recognize the file format.
${obj_dev-libs}: build/%.o: ../lib/src/%.c
	avr-gcc -c $^ -o $@ $(CFLAGS)

#=====[ Project Libs: objects built for Unit Tests ]=====
#
# ---Unity (unit test framework) object file.---
# Behold my static library hack:
# list of objects for unit testing with shared lib mock-c.
mock-c-libs    := Mock RecordedCall RecordedArg ReturnValues
mock-c-objects := $(addsuffix .o,${mock-c-libs})
mock-c-objects := $(addprefix ${path_shared-lib_mock-c}build/,${mock-c-objects})
obj_unity := ${path_shared-lib_mock-c}build/unity.o
obj_shared-lib_mock-c := ${mock-c-objects} ${obj_unity}
# TODO: add target to rebuild test framework to match compiler
#
# ---Libs under test?---
# Find the `build` files in the project's `lib/build` folder.
# Do not touch the `build` files in the application's `build` folder.
test-obj_dev-libs := $(addsuffix .o,${names-of-dev-libs})
test-obj_dev-libs := $(addprefix ../lib/build/,${test-obj_dev-libs})

#=====[ Explicit rule for objects for Unit Tests ]=====
# Find the `src` and `build` files in the project's `lib/build` folder.
${test-obj_dev-libs}: ../lib/build/%.o: ../lib/src/%.c
	${compiler} $(CFLAGS) -c $^ -o $@ $(LFLAGS)


#=====[ Test Suite ]=====
${test-output}: build/TestSuite.exe
	$^ > $@

# Does this recipe work? Avoid the objects built for avr-gcc!
# TODO: [ ] build the project lib object files.
# TODO: [ ] build the application lib object files.
# Target unit-test ends up here:
build/TestSuite.exe: test/test_runner.c \
../lib/test/fake/AvrAsmMacros.c ${test-obj_dev-libs} ${obj_shared-lib_mock-c}
	${compiler} $(CFLAGS) -Itest $^ -o $@ $(LFLAGS)

#=====[ AVR ]=====
# atmega328_lib has the .o and .a lib files and the spec file.
atmega328_lib = '/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'

avr-asm-macros := src/AvrAsmMacros.c
build/%.elf: ${obj_dev-libs} src/%.c ${avr-asm-macros}
	avr-gcc $^ -o $@ $(CFLAGS)  \
		-Wl,-Map="build/$*.map" -Wl,--gc-sections
# avr-size writes a summary to stdout with the size of each section.
	avr-size $@
#	# avr-size $@ > build/avr-size_usb-bridge.log
#	# 	-I../lib/src \

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
# atprogram.exe --verbose --tool atmelice --interface isp \
#		# --device atmega328p program --chiperase --verify --file \
#		# $^ > \
#		# build/atprogram-download_flash-stdout.log 2> \
#		# build/atprogram-download_flash-stderr.log
#	atprogram.exe --tool avrispmk2 --interface isp \
#		--device atmega328p program --chiperase --verify --file $^
#	avr-size $^

#---Development---
# ;mkp
# Test USB communication with the AVR programmer.
.PHONY: test_programmer_is_connected
test_programmer_is_connected:
	atprogram.exe --tool atmelice --interface isp --device atmega328p info
	# atprogram.exe --tool avrispmk2 --interface isp --device atmega328p info
# Check voltage on target PCB. 2018-03-20: I see 2.94V.
# This is included in the output of 'info'.
.PHONY: display_target_voltage
display_target_voltage:
	atprogram.exe --tool atmelice --interface isp \
		--device atmega328p parameters --voltage
#----------------------------------------------------------------------

#;mc
.PHONY: clean-all-builds
clean-all-builds:
	rm -f build/${board-name}.elf
	rm -f build/${board-name}.lst
	rm -f build/${board-name}.map
	rm -f ${obj_dev-libs}
	rm -f build/atprogram-download*.log
	rm -f build/avr-size_*.log
