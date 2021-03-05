# TODO(sustainablelab): Don't forget -O3 (correctly optimized assembly) when I add the .elf recipe
unit-test: build/test_runner.md

# Compiler/Linker FLAGS for unit tests{{{
CFLAGS_for_tests = -I../lib/src -I../lib/test -Isrc \
				-I../mock-c/code \
				-I/usr/include/glib-2.0 \
				-I/usr/lib/glib-2.0/include \
				-DUSE_FAKES \
				-g -Wall -Wextra -pedantic -Winline
LFLAGS_for_tests := -lglib-2.0 -lintl
# }}}
# Object files{{{
usb-bridge_o := build/UsbCmd.o
lib_o := ../lib/build/ReadWriteBits.o \
			../lib/build/BiColorLed.o \
			../lib/build/Usb.o \
			../lib/build/Usb_faked.o \
			../lib/build/Spi.o \
			../lib/build/Spi_faked.o \
			../lib/build/SpiMaster.o \
			../lib/build/StatusCode.o
mock-c_o := ../mock-c/build/unity.o \
			../mock-c/build/Mock.o \
			../mock-c/build/ReturnValues.o \
			../mock-c/build/RecordedArg.o \
			../mock-c/build/RecordedCall.o
# }}}
build/test_runner.md: build/test_runner.exe
	$< > $@
build/test_runner.exe: test/test_runner.c test/test_UsbCmd.c \
						$(mock-c_o) \
						$(usb-bridge_o) \
						$(lib_o)
	gcc $^ -o $@ $(CFLAGS_for_tests) $(LFLAGS_for_tests)
build/%.o: src/%.c src/%.h
	gcc -c $< -o $@ $(CFLAGS_for_tests)
../lib/build/%.o: ..lib/src/%.c ..lib/src/%.h
	gcc -c $< -o $@ $(CFLAGS_for_tests)

.PHONY: clean
clean:
	rm -f build/*.o

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
