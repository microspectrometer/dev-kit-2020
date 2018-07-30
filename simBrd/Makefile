board-name := simBrd

# variable for my test output name that is stupidly coupled to my .vimrc
test-output := build/TestSuite-results.md

#=====[ Targets (the first target is the default target) ]=====
#TODO: define a build for unit-tests -- add this when the need arises.
unit-test: ${test-output}

#=====[ Compiler and Linker flags ]=====
CFLAGS = -I../lib/src -Isrc \
	-g -Wall -Wextra -pedantic \
	-O1 -ffunction-sections -fdata-sections -fshort-enums \
		-mmcu=atmega328p -B ${atmega328_lib}
#The avr ld.exe does not recognize any of these
#LFLAGS = -lglib-2.0 -lintl -L/usr/lib/glib-2.0

#=====[ Project Libs ]=====
# List the project libs:
names-of-dev-libs := ReadWriteBits DebugLed Ft1248 Usb SpiMaster
# list of object prerequisites for dev libs
obj_dev-libs := $(addsuffix .o,${names-of-dev-libs})
obj_dev-libs := $(addprefix build/,${obj_dev-libs})

#=====[ Explicit rule for obj_dev-libs ]=====
# avr-gcc has to build the lib code into its own object files
# the `ld` linker does not recognize the file format.
${obj_dev-libs}: build/%.o: ../lib/src/%.c
	avr-gcc -c $^ -o $@ $(CFLAGS)

#=====[ AVR ]=====
# atmega328_lib has the .o and .a lib files and the spec file.
atmega328_lib = '/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'

# Executable for uploading to the microcontroller. Eliminate unused code.
# .elf is the executable to download to flash
# .lst is the disassembly for analysis
avr-target: build/${board-name}.elf build/${board-name}.lst

build/%.elf: ${obj_dev-libs} src/%.c
	avr-gcc $^ -o $@ $(CFLAGS)  \
		-Wl,-Map="build/$*.map" -Wl,--gc-sections
# avr-size writes a summary to stdout with the size of each section.
	avr-size $@ > build/avr-size_simBrd.log
#	# 	-I../lib/src \

# Convert .elf to .lst for disassembly.
#  -h: list space used by each section
#  -S: output the binary with source code
build/%.lst: build/%.elf
	avr-objdump -h -S $^ > $@

#--------------------------------------------------------------
# ;fa or ;mfa
# TODO: figure out how to replace name 'simBrd' with a variable.
.PHONY: download_flash
download_flash: build/${board-name}.elf
	atprogram.exe --verbose --tool atmelice --interface isp \
		--device atmega328p program --chiperase --verify --file \
		$^ > \
		build/atprogram-download_flash-stdout.log 2> \
		build/atprogram-download_flash-stderr.log

#---Development---
# ;mkp
# Test USB communication with the AVR programmer.
.PHONY: test_programmer_is_connected
test_programmer_is_connected:
	atprogram.exe --tool atmelice --interface isp --device atmega328p info
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
