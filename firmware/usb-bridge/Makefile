sensor ?= LIS
avr-elf: build/usb-bridge.elf build/usb-bridge.avra
unit-test: build/test_runner.md

# Object files{{{
mock-c_o := ../mock-c/build/unity.o \
			../mock-c/build/Mock.o \
			../mock-c/build/ReturnValues.o \
			../mock-c/build/RecordedArg.o \
			../mock-c/build/RecordedCall.o
usb-bridge_o-test := build/test/UsbCmd.o
usb-bridge_o-avr := build/avr/UsbCmd.o
lib_o-test := ../lib/build/test/ReadWriteBits.o \
			../lib/build/test/BiColorLed.o \
			../lib/build/test/Usb.o \
			../lib/build/test/Usb_faked.o \
			../lib/build/test/Spi.o \
			../lib/build/test/Spi_faked.o \
			../lib/build/test/SpiMaster.o \
			../lib/build/test/StatusCode.o
lib_o-avr := ../lib/build/avr/ReadWriteBits.o \
			../lib/build/avr/BiColorLed.o \
			../lib/build/avr/Usb.o \
			../lib/build/avr/Spi.o \
			../lib/build/avr/SpiMaster.o \
			../lib/build/avr/StatusCode.o
# }}}
# Compiler/Linker FLAGS for AVR{{{
CFLAGS_for_AVR := -includeavr/interrupt.h -includeavr/io.h -mmcu=atmega328p \
					-D$(sensor) \
					-Isrc -I../lib/src \
					-O3 -ffunction-sections -fdata-sections -fshort-enums \
					-g -Wall -Wextra -pedantic -Winline -Wno-pointer-sign
shrink_the_elf := -Wl,--gc-sections
output_a_map := -Wl,-Map="build/usb-bridge.map"
LFLAGS_for_AVR := $(shrink_the_elf) $(output_a_map)
# }}}
# Compiler/Linker FLAGS for unit tests{{{
CFLAGS_for_tests = -I../lib/src -I../lib/test -Isrc \
				-I../mock-c/code \
				-I/usr/include/glib-2.0 \
				-I/usr/lib/glib-2.0/include \
				-DUSE_FAKES \
				-g -Wall -Wextra -pedantic -Winline
LFLAGS_for_tests := -lglib-2.0 -lintl
# }}}
# {{{ AVR FLASH and other programmer Recipes
# $ make flash {{{
.PHONY: flash
flash: build/usb-bridge.elf
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
build/usb-bridge.avra: build/usb-bridge.elf
	avr-objdump -h -S $^ > $@
build/usb-bridge.elf: src/usb-bridge.c \
						$(usb-bridge_o-avr) \
						$(lib_o-avr)
	avr-gcc $^ -o $@ $(CFLAGS_for_AVR) $(LFLAGS_for_AVR)
	avr-size $@
build/avr/%.o: src/%.c src/%.h src/Hardware.h src/BiColorLed-Hardware.h src/SpiMaster-Hardware.h src/Usb-Hardware.h
	avr-gcc -c $< -o $@ $(CFLAGS_for_AVR)
../lib/build/avr/%.o: ../lib/src/%.c ../lib/src/%.h ../lib/src/StatusCodes.h ../lib/src/LisConfigs.h
	avr-gcc -c $< -o $@ $(CFLAGS_for_AVR)
# }}}
# Test Recipes {{{
build/test_runner.md: build/test_runner.exe
	$< > $@
build/test_runner.exe: test/test_runner.c test/test_UsbCmd.c \
						$(mock-c_o) \
						$(usb-bridge_o-test) \
						$(lib_o-test)
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

# vim:set fdm=marker:
