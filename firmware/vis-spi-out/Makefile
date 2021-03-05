# TODO(sustainablelab): Don't forget -O3 (correctly optimized assembly) when I add the .elf recipe
sensor ?= LIS
unit-test: build/test_runner.md
# Tests{{{
tests := test/test_example.c \
		test/test_VisCmd.c \
		test/test_AutoExpose.c
# }}}
# Object files{{{
vis-spi-out_o := build/AutoExpose.o \
				build/Example.o \
				build/VisCmd.o
mock-c_o := ../mock-c/build/unity.o \
			../mock-c/build/Mock.o \
			../mock-c/build/ReturnValues.o \
			../mock-c/build/RecordedArg.o \
			../mock-c/build/RecordedCall.o
lib_o := ../lib/build/BiColorLed.o \
		../lib/build/ReadWriteBits.o \
		../lib/build/StatusCode.o \
		../lib/build/Queue.o \
		../lib/build/Spi.o \
		../lib/build/SpiSlave.o \
		../lib/build/Lis.o \
		../lib/build/Spi_faked.o \
		../lib/build/SpiSlave_faked.o \
		../lib/build/Lis_faked.o
# }}}
# Compiler/Linker FLAGS for unit tests{{{
CFLAGS_for_tests = -include../lib/test/FakeAvr/interrupt.h \
					-include../lib/test/FakeAvr/io.h \
					-includestdbool.h \
				-D$(sensor) \
				-DUSE_FAKES \
				-Isrc -I../lib/src \
				-Itest -I../lib/test  -I../mock-c/code \
				-I/usr/include/glib-2.0 \
				-I/usr/lib/glib-2.0/include \
				-g -Wall -Wextra -pedantic -Winline
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
	@echo vis-spi-out object files:
	@echo "    "$(vis-spi-out_o)
	@echo
	@echo mock-c object files:
	@echo "    "$(mock-c_o)
	@echo
	@echo lib object files:
	@echo "    "$(lib_o)
	@echo
	@echo unit test CFLAGS:
	@echo "    "$(CFLAGS_for_tests)
	@echo
	@echo unit test LFLAGS:
	@echo "    "$(LFLAGS_for_tests)
# }}}
# {{{ Recipes
build/test_runner.md: build/test_runner.exe
	$< > $@
build/test_runner.exe: test/test_runner.c $(tests) \
					$(vis-spi-out_o) \
					$(mock-c_o) \
					$(lib_o)
	gcc $^ -o $@ $(CFLAGS_for_tests) $(LFLAGS_for_tests)
build/%.o: src/%.c src/%.h
	gcc -c $< -o $@ $(CFLAGS_for_tests)
../mock-c/build/%.o: ../mock-c/code/%.c ../mock-c/code/%.h ../mock-c/code/unity_internals.h
	gcc -c $< -o $@ $(CFLAGS_for_tests)
../lib/build/%.o: ../lib/src/%.c ../lib/src/%.h
	gcc -c $< -o $@ $(CFLAGS_for_tests)
../lib/build/%_faked.o: ../lib/test/%_faked.c ../lib/test/%_faked.h
	gcc -c $< -o $@ $(CFLAGS_for_tests)

.PHONY: clean
clean:
	rm -f build/*.o
	rm -f ../lib/build/*.o

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
# }}}
# vim:set fdm=marker:
