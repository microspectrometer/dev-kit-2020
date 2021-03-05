sensor ?= LIS
build/test_runner.md: build/test_runner.exe
	$< > $@

# {{{ unit test source code
tests := test/test_BiColorLed.c \
		test/test_ReadWriteBits.c \
		test/test_Flag.c \
		test/test_Spi.c \
		test/test_SpiSlave.c \
		test/test_SpiMaster.c \
		test/test_Queue.c \
		test/test_UartSpi.c \
		test/test_Lis.c \
		test/test_Usb.c \
		test/test_StatusCode.c \

# }}}
# {{{mock-c object files
mock-c_o := ../mock-c/build/unity.o \
			../mock-c/build/Mock.o \
			../mock-c/build/RecordedArg.o \
			../mock-c/build/RecordedCall.o \
			../mock-c/build/ReturnValues.o
# }}}
# lib object files {{{
lib_o := build/BiColorLed.o \
		build/ReadWriteBits.o \
		build/Spi.o \
		build/Spi_faked.o \
		build/SpiSlave.o \
		build/SpiSlave_faked.o \
		build/SpiMaster.o \
		build/Queue.o \
		build/UartSpi.o \
		build/Lis.o \
		build/Lis_faked.o \
		build/Usb.o \
		build/Usb_faked.o \
		build/StatusCode.o \

# }}}
# Compiler and Linker flags {{{
CFLAGS := -I../mock-c/code -includestdbool.h -Isrc -Itest\
			-DUSE_FAKES \
			-D$(sensor) \
			-include./test/FakeAvr/interrupt.h \
			-I/usr/include/glib-2.0 \
			-I/usr/lib/glib-2.0/include \
			-g -Wall -Wextra -pedantic -Winline
LFLAGS := -lglib-2.0 -lintl
# }}}
build/test_runner.exe: test/test_runner.c $(tests) \
						$(mock-c_o) \
						$(lib_o)
	gcc $^ -o $@ $(CFLAGS) $(LFLAGS)

build/%.o: src/%.c src/%.h
	gcc $< -c -o $@ $(CFLAGS)

build/%_faked.o: test/%_faked.c test/%_faked.h
	gcc $< -c -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -f build/*.o
	rm -f build/*.exe

# vim:set fdm=marker:
