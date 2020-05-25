# Name the PCB to program with this firmware.
board-name := usb-bridge

# Name libs in usb-bridge/src.
applib := Example UsbCmd

# Name HARDWARE libs in lib/src.
hwlib := BiColorLed Usb Spi SpiMaster

# Name NON-hardware libs in lib/src.
nohwlib := ReadWriteBits StatusCode

# Name libs that USE_FAKES in tests
lib_faked := Usb Spi

help:
	@echo
	@echo
	@echo
	@echo "----------------"
	@echo "|     HELP     |"
	@echo "----------------"
	@echo
	@echo "USB Bridge PCB name: $(board-name)"
	@echo
	@echo Setup:
	@echo "  Power-on the AtmelICE."
	@echo "  Orient PCB with 6-pin programming header on the RIGHT."
	@echo "  (Slide white switch RIGHT, black switch LEFT)"
	@echo
	@cat ../help-board-common.man
	@echo
	@echo --- Vendor Dependency Path ---
	@echo atmega328_lib: "$(atmega328_lib)"

include ../uservars.mk
include ../common.mk
