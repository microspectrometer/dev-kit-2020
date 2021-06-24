include uservars.mk

help:
	@echo
	@cat help.man | less

help-fuses:
	@echo
	@echo
	@echo
	@cat help-fuses.man | less

help-flash:
	@echo
	@echo
	@echo
	@cat help-flash.man | less

help-setup:
	@echo
	@echo
	@echo
	@echo "--------------"
	@echo "| HELP-SETUP |"
	@echo "--------------"
	@echo
	@echo Makefiles:
	@echo "  -----------------------------------------------------------------------------------"
	@echo "  | Edit machine-dependent path for ATmega_DFP in dev-kit-2020/firmware/uservars.mk |"
	@echo "  -----------------------------------------------------------------------------------"
	@echo
	@echo "  uservars.mk currently has ATmega_DFP path as:"
	@echo "    $(ATmega_DFP)"
	@echo "  and final path to vendor lib dependencies (atmega328_lib) as:"
	@echo "    $(atmega328_lib)"
	@echo
	@cat help-setup.man

help-build:
	@echo
	@echo
	@echo
	@cat help-build.man

help-ftdi:
	@echo
	@echo
	@echo
	@echo
	@cat help-ftdi.man | less

troubleshoot-ftdi:
	@echo
	@echo
	@cat troubleshoot-ftdi.man | less


kitnum: ftcfg.xml
	@. ./kitnum.sh

.PHONY: scan
scan:
	@echo Scanning connected USB devices...
	@"$$ftdi" scan | cat > devscan
	@. ./devcnt.sh
	@rm -f devscan

.PHONY: cycle
cycle:
	@"$$ftdi" scan cycl '0'

.PHONY: progkitnum
progkitnum:
	@echo Checking only one FT X Series is USB-connected...
	@"$$ftdi" scan | cat > devscan
	@. ./devcnt.sh # stop if num devices is not 1
	@rm -f devscan
	@. ./progkitnum.sh
