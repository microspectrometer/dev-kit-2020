board-name := vis-spi-out

# =====[ Build targets ]=====
#
# Vim ;mktgc builds unit-test target using gcc
unit-test: build/TestSuite-results.md
# .md opens in new bottom Vim window
#
# Vim ;mka builds avr target using avr-gcc
# Vim ;fa downloads .elf to microcontroller
avr-target: build/${board-name}.elf build/${board-name}.lst
# .elf is the executable to load into flash memory
# .lst is the disassembly for analysis

#=====[ AVR library from AtmelStudio installation ]=====
# atmega328_lib has the .o and .a lib files and the spec file.
atmega328_lib = '/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'

#=====[ AVR Compiler and Linker flags ]=====
CFLAGS_for_avr = -Isrc -I../lib/src \
-g -Wall -Wextra -pedantic -Winline -Wno-pointer-sign\
-O3 -ffunction-sections -fdata-sections -fshort-enums \
-mmcu=atmega328p
# -fearly-inlining -findirect-inlining -finline-functions \
#Used to be -O1, not sure why.
#The avr ld.exe does not recognize any of these
#LFLAGS = -lglib-2.0 -lintl -L/usr/lib/glib-2.0
LFLAGS_for_avr := -B ${atmega328_lib}

#=====[ Unit Test Compiler and Linker flags ]=====
path_shared-lib_mock-c := /cygdrive/c/chromation-dropbox/Dropbox/c/TddFramework/mock-c/
CFLAGS_for_cygwin = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include \
	-I${path_shared-lib_mock-c}include -I${path_shared-lib_mock-c}test/unity \
	-Iinclude -I../lib/src -Isrc \
	-Itest -I../lib/test \
	-g -Wall -Wextra -pedantic -Winline
LFLAGS_for_cygwin = -lglib-2.0 -lintl -L/usr/lib/glib-2.0
# -Itest is "include folder `test`". This finds `test_runner.h` from `#include "test_runner.h"`
# -I../lib/test includes the project libs `test` folder.
#    This picks up the headers for project lib mocks and the hardware-dependency fakes.

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


# =====[ AVR and Unit Tests use the same *lib* object files ]=====
# Targets `avr-target` and `unit-test` find these in the same location!
# *clean* the builds out, therefore, when you switch between targets, or you get
# a linker error.

#=====[ Project Libs ]=====
# TODO: fix special treatment of test_Spi.c
#   For now, build and link test_Spi.c for mocking SpiSlave hardware calls.
# List the project libs (for dependencies, not for testing):
names-of-prj-libs := ReadWriteBits BiColorLed Spi UartSpi Lis Pwm Queue
# List the application libs (for unit testing and for dependencies):
names-of-app-libs := SensorVis
# name convention for application lib tests
name_app-libs-tested   := $(addprefix test_,${names-of-app-libs})
# List project/application libs that are mocked out in application lib tests.
mocked-prj-libs := Ft1248 Spi UartSpi Lis Pwm
# name convention for mocked lib source files and object files
name_prj-libs-mocked := $(addprefix mock_,${mocked-prj-libs})
# List project libs that use mocks in their test.
# names-of-libs-tested-with-mocks := Ft1248 Usb Spi UartSpi Lis
names-of-libs-tested-with-mocks := Spi
# name convention for project libs that use mocks in their tests
name_libs-tested-with-mocks := $(addsuffix _MockUps,${names-of-libs-tested-with-mocks})
name_libs-tested-with-mocks := $(addprefix test_,${name_libs-tested-with-mocks})

# ---List object file prerequisites---
# prerequisite *project libs*:
obj_prj-libs := $(addsuffix .o,${names-of-prj-libs})
obj_prj-libs := $(addprefix build/,${obj_prj-libs})
# prerequisite *application libs*:
obj_app-libs := $(addsuffix .o,${names-of-app-libs})
obj_app-libs := $(addprefix build/,${obj_app-libs})
# prerequisite application lib *tests*
obj_app-libs-tested := $(addsuffix .o,${name_app-libs-tested})
obj_app-libs-tested := $(addprefix build/,${obj_app-libs-tested})
# prerequisite *mocked* project libs
obj_prj-libs-mocked := $(addsuffix .o,${name_prj-libs-mocked})
obj_prj-libs-mocked := $(addprefix build/,${obj_prj-libs-mocked})
# prerequisite project libs that use mocks in their tests
obj_libs-tested-with-mocks := $(addsuffix .o,${name_libs-tested-with-mocks})
obj_libs-tested-with-mocks := $(addprefix build/,${obj_libs-tested-with-mocks})

# ---Explicit rule for project lib *object files*---
#  Needs LFLAGS to link against atmega328_lib when building object files with
#  avr-gcc.
#  obj_prj-libs used to be called obj_dev-libs
${obj_prj-libs}: build/%.o: ../lib/src/%.c
	${compiler} -c $^ -o $@ $(CFLAGS) $(LFLAGS)
# ---Explicit rule for application lib *object files*---
#  Needs LFLAGS to link against atmega328_lib
${obj_app-libs}: build/%.o: src/%.c
	${compiler} $(CFLAGS) -c $^ -o $@ $(LFLAGS)
# ---Explicit rule application lib test *object files*---
${obj_app-libs-tested}: build/%.o: test/%.c
	${compiler} $(CFLAGS) -c $^ -o $@
# ---Explicit rule for Spi because test_Spi.c contains the hardware fake---
../lib/build/test_Spi.o: ../lib/test/test_Spi.c
	${compiler} $(CFLAGS) -c $^ -o $@
# ---Explicit rule for obj_prj-libs-mocked---
${obj_prj-libs-mocked}: build/%.o: ../lib/test/%.c
	${compiler} $(CFLAGS) -c $^ -o $@
# ---Explicit rule for object files for testing project libs with mocks---
${obj_libs-tested-with-mocks}: build/%.o: ../lib/test/%.c
	${compiler} $(CFLAGS) -c $^ -o $@

#=====[ Unit Tests also have Test Framework Object Files ]=====
# Behold my static library hack:
# list object files for unit testing with shared lib `mock-c`
mock-c-libs    := Mock RecordedCall RecordedArg ReturnValues
mock-c-objects := $(addsuffix .o,${mock-c-libs})
mock-c-objects := $(addprefix ${path_shared-lib_mock-c}build/,${mock-c-objects})
obj_unity := ${path_shared-lib_mock-c}build/unity.o
obj_shared-lib_mock-c := ${mock-c-objects} ${obj_unity}
# TODO: add target to rebuild test framework to match the Cygwin compiler
# For now, rebuild for the desired compiler with:
#     ;nb mock-c
#         open the test framework project
#     ;mca
#         clean all builds
# Then, either:
#     ;mkgc (build with gcc)
#     or
#     ;mkcl (build with clang)

# =====[ Build recipes ]=====
#
build/TestSuite-results.md: build/TestSuite.exe
	$^ > $@

# [ ] build the application lib object files.
# [ ] build the project lib object files.
# TODO: [ ] *clean up* test_Spi and so it does not need special treatment here.
# Right now, there is no object file group it neatly slots into.
test-runner := test/test_runner.c
fake-avr-asm-macros := ../lib/test/fake/AvrAsmMacros.c
# Target `unit-test` ends up here:
build/TestSuite.exe: ${test-runner} ${fake-avr-asm-macros} \
${obj_prj-libs} ${obj_app-libs} ${obj_app-libs-tested} ${obj_prj-libs-mocked} \
../lib/build/test_Spi.o ${obj_libs-tested-with-mocks} \
${obj_shared-lib_mock-c}
	${compiler} $(CFLAGS) $^ -o $@ $(LFLAGS)

avr-asm-macros := src/AvrAsmMacros.c
# Target `avr-target` ends up here:
# build/%.elf: ${obj_prj-libs} ${obj_app-libs} src/%.c ${avr-asm-macros}
# Do NOT build SensorVis.o (this is the only file in obj_app-libs)
# ReadWriteBits BiColorLed Spi UartSpi Lis Pwm Queue
# Remove: Lis.o, Pwm.o, UartSpi.o, AvrAsmMacros.o
# Include lib *only in vis-spi-out.c* with #include <lib.c>
# build/%.elf: ${obj_prj-libs} src/%.c ${avr-asm-macros}
build/%.elf: build/ReadWriteBits.o build/BiColorLed.o build/Spi.o build/Queue.o src/%.c
	avr-gcc $(CFLAGS) $^ -o $@ $(LFLAGS) \
		-Wl,-Map="build/$*.map" -Wl,--gc-sections
# avr-size writes a summary to stdout with the size of each section.
	avr-size $@
#	# avr-size $@ > build/avr-size_vis-spi-out.log
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
	# atprogram.exe --tool atmelice --interface isp \
	# 	--device atmega328p program --chiperase --verify --file $^
	# avr-size $^
	atprogram.exe --tool avrispmk2 --interface isp \
		--device atmega328p program --chiperase --verify --file $^
	avr-size $^

.PHONY: other_download_flash_commands
	# atprogram.exe --tool avrispmk2 --interface isp \
		--device atmega328p program --chiperase --verify --file $^
	# avr-size $^
#	# atprogram.exe --verbose --tool atmelice --interface isp \
#	# 	--device atmega328p program --chiperase --verify --file \
#	# 	$^ > \
#	# 	build/atprogram-download_flash-stdout.log 2> \
#	# 	build/atprogram-download_flash-stderr.log

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

#;mc
.PHONY: clean-all-builds
clean-all-builds:
	rm -f build/${board-name}.elf
	rm -f build/${board-name}.lst
	rm -f build/${board-name}.map
	rm -f ${obj_prj-libs}
	rm -f ${obj_app-libs}
	rm -f ${obj_prj-libs-mocked}
	rm -f ${obj_libs-tested-with-mocks}
	rm -f build/atprogram-download*.log
	rm -f build/avr-size_*.log
