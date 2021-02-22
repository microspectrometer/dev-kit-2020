# Name the PCB to program with this firmware.
board-name := vis-spi-out

# Name libs in vis-spi-out/src.
applib := Example VisCmd AutoExpose

# Name HARDWARE libs in lib/src.
hwlib := BiColorLed Spi UartSpi Lis Flag

# Name NON-hardware libs in lib/src.
nohwlib := ReadWriteBits StatusCode Queue SpiSlave

# Name libs that USE_FAKES in tests
lib_faked := Spi SpiSlave Lis

help:
	@echo
	@echo
	@echo
	@echo "----------------"
	@echo "|     HELP     |"
	@echo "----------------"
	@echo
	@echo "Sensor Interface PCB name: $(board-name)"
	@echo
	@echo Setup:
	@echo "  Power-on the AtmelICE."
	@echo "  Orient PCB with 6-pin programming header on the RIGHT."
	@echo "  (Slide white switch LEFT, black switch LEFT)"
	@echo
	@cat ../help-board-common.man
	@echo
	@echo --- Vendor Dependency Path ---
	@echo atmega328_lib: "$(atmega328_lib)"

include ../sensor.mk
include ../uservars.mk
include ../common.mk
