avr-elf: clean build/vis-spi-out.elf build/vis-spi-out.avra sensor.txt.sha1
unit-test: build/test_runner.md
.PHONY: get-commit
get-commit:
	git log --format="%h" -n1
.PHONY: view-in-terminal
S13131-elf: view-in-terminal

# sensor ?= LIS
sensor ?= S13131

ifeq ($(sensor),LIS)
	sensor-hash := 0x351ea9
endif
ifeq ($(sensor),S13131)
	sensor-hash := 0x91d318
endif

check-sensor-hash: sensor.txt.sha1
	sha1sum.exe -c sensor.txt.sha1

# Tests{{{
tests := test/test_example.c \
		test/test_VisCmd.c \
		test/test_AutoExpose.c
# }}}
# Object files{{{
mock-c_o := ../mock-c/build/unity.o \
			../mock-c/build/Mock.o \
			../mock-c/build/ReturnValues.o \
			../mock-c/build/RecordedArg.o \
			../mock-c/build/RecordedCall.o
vis-spi-out_o-test := build/test/AutoExpose.o \
				build/test/Example.o \
				build/test/VisCmd.o
vis-spi-out_o-avr := build/avr/AutoExpose.o \
				build/avr/Example.o \
				build/avr/VisCmd.o
lib_o-test := ../lib/build/test/BiColorLed.o \
			../lib/build/test/ReadWriteBits.o \
			../lib/build/test/StatusCode.o \
			../lib/build/test/Queue.o \
			../lib/build/test/UartSpi.o \
			../lib/build/test/UartSpi_faked.o \
			../lib/build/test/Spi.o \
			../lib/build/test/SpiSlave.o \
			../lib/build/test/Spi_faked.o \
			../lib/build/test/SpiSlave_faked.o

ifeq ($(sensor),LIS)
	sensor_o-test := ../lib/build/test/Lis.o \
				../lib/build/test/Lis_faked.o
endif
ifeq ($(sensor),S13131)
	sensor_o-test := ../lib/build/test/S13131.o \
				../lib/build/test/S13131_faked.o
endif

lib_o-avr := ../lib/build/avr/BiColorLed.o \
			../lib/build/avr/ReadWriteBits.o \
			../lib/build/avr/StatusCode.o \
			../lib/build/avr/Queue.o \
			../lib/build/avr/UartSpi.o \
			../lib/build/avr/Spi.o \
			../lib/build/avr/SpiSlave.o

ifeq ($(sensor),LIS)
	sensor_o-avr := ../lib/build/avr/Lis.o
endif
ifeq ($(sensor),S13131)
	sensor_o-avr := ../lib/build/avr/S13131.o
endif

# }}}
# Compiler/Linker FLAGS for AVR{{{
CFLAGS_for_AVR := -includeavr/interrupt.h -includeavr/io.h -mmcu=atmega328p \
					-D$(sensor)=$(sensor-hash) \
					-Isrc -I../lib/src \
					-O3 -ffunction-sections -fdata-sections -fshort-enums \
					-g -Wall -Wextra -pedantic -Winline -Wno-pointer-sign
shrink_the_elf := -Wl,--gc-sections
output_a_map := -Wl,-Map="build/vis-spi-out.map"
LFLAGS_for_AVR := $(shrink_the_elf) $(output_a_map)
# }}}
# Compiler/Linker FLAGS for unit tests{{{
CFLAGS_for_tests := -include../lib/test/FakeAvr/interrupt.h \
					-include../lib/test/FakeAvr/io.h \
					-includestdbool.h \
					-D$(sensor)=$(sensor-hash) \
					-DUSE_FAKES \
					-Isrc -I../lib/src \
					-Itest -I../lib/test  -I../mock-c/code \
					-I/usr/include/glib-2.0 \
					-I/usr/lib/glib-2.0/include \
					-g
					# -g -Wall -Wextra -pedantic -Winline

LFLAGS_for_tests := -lglib-2.0 -lintl
# }}}
# make print-vars -- Print the variables in this Makefile{{{
print-vars:
	@echo sensor: $(sensor)
	@echo
	@echo unit test files:
	@echo "    "$(tests)
	@echo
	@echo unit test files:
	@echo "    "$(tests)
	@echo
	@echo mock-c object files:
	@echo "    "$(mock-c_o)
	@echo
	@echo vis-spi-out object files for tests:
	@echo "    "$(vis-spi-out_o-test)
	@echo
	@echo vis-spi-out object files for AVR:
	@echo "    "$(vis-spi-out_o-avr)
	@echo
	@echo lib object files for tests:
	@echo "    "$(lib_o-test)
	@echo
	@echo lib object files for AVR:
	@echo "    "$(lib_o-avr)
	@echo
	@echo sensor object files for tests:
	@echo "    "$(sensor_o-test)
	@echo
	@echo sensor object files for AVR:
	@echo "    "$(sensor_o-avr)
	@echo
	@echo unit test CFLAGS:
	@echo "    "$(CFLAGS_for_tests)
	@echo
	@echo unit test LFLAGS:
	@echo "    "$(LFLAGS_for_tests)
	@echo
	@echo AVR CFLAGS:
	@echo "    "$(CFLAGS_for_AVR)
	@echo
	@echo AVR LFLAGS:
	@echo "    "$(LFLAGS_for_AVR)
# }}}
# {{{ AVR FLASH and other programmer Recipes
# $ make flash {{{
.PHONY: flash
flash: build/vis-spi-out.elf
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
# {{{ AVR ELF and Assembly Recipes
view-in-terminal: ../lib/src/S13131.c
	avr-gcc $< -S -o - -includeavr/interrupt.h -Isrc -I../lib/src -O3 -includeavr/io.h -mmcu=atmega328p -D$(sensor) 

sensor.txt.sha1:
	@echo This firmware is for spectrometers using the $(sensor).
	@echo Identify sensor by the first six characters of the SHA-1 hash.
	echo $(sensor) > sensor.txt
	sha1sum.exe sensor.txt > sensor.txt.sha1
	@echo "If you need to check the SHA-1: make check-sensor-hash"
build/vis-spi-out.avra: build/vis-spi-out.elf
	avr-objdump -h -S $^ > $@
build/vis-spi-out.elf: src/vis-spi-out.c \
						$(vis-spi-out_o-avr) \
						$(lib_o-avr) \
						$(sensor_o-avr)
	avr-gcc $^ -o $@ $(CFLAGS_for_AVR) $(LFLAGS_for_AVR)
	avr-size $@

build/avr/%.o: src/%.c src/%.h src/Hardware.h src/BiColorLed-Hardware.h src/UartSpi-Hardware.h src/Lis-Hardware.h src/S13131-Hardware.h
	avr-gcc -c $< -o $@ $(CFLAGS_for_AVR)
../lib/build/avr/%.o: ../lib/src/%.c ../lib/src/%.h ../lib/src/StatusCodes.h ../lib/src/LisConfigs.h
	avr-gcc -c $< -o $@ $(CFLAGS_for_AVR)
# }}}
# {{{ Test Recipes

build/test_runner.md: build/test_runner.exe
	$< > $@
build/test_runner.exe: test/test_runner.c $(tests) \
					$(vis-spi-out_o-test) \
					$(mock-c_o) \
					$(lib_o-test) \
					$(sensor_o-test)
	gcc $^ -o $@ $(CFLAGS_for_tests) $(LFLAGS_for_tests)
build/test/%.o: src/%.c src/%.h
	gcc -c $< -o $@ $(CFLAGS_for_tests)
../mock-c/build/%.o: ../mock-c/code/%.c ../mock-c/code/%.h ../mock-c/code/unity_internals.h
	gcc -c $< -o $@ $(CFLAGS_for_tests)
../lib/build/test/%.o: ../lib/src/%.c ../lib/src/%.h
	gcc -c $< -o $@ $(CFLAGS_for_tests)
../lib/build/test/%_faked.o: ../lib/test/%_faked.c ../lib/test/%_faked.h
	gcc -c $< -o $@ $(CFLAGS_for_tests)
# }}}
.PHONY: clean
clean:
	rm -f build/test/*.o
	rm -f ../lib/build/test/*.o
	rm -f build/avr/*.o
	rm -f ../lib/build/avr/*.o
	rm -f sensor.txt.sha1
	rm -f sensor.txt

.PHONY: lib-tags
lib-tags:
	gcc -M $(CFLAGS_for_tests) test/test_runner.c | \
		sed -e 's/[\\ ]/\n/g' | \
		sed -e '/^ *$$/d' | \
		sed -e '/\.o:/d'  | \
		sed -e '/\.c$$/d' \
		> lib-tags.txt
	ctags -f $@ --c-kinds=+p -L lib-tags.txt
	rm lib-tags.txt

.PHONY: what-is-__FILE__
what-is-__FILE__: build/what-is-__FILE__.exe
	$<
build/what-is-__FILE__.exe: what-is-__FILE__.c
	gcc $< -o $@

# vim:set fdm=marker:
