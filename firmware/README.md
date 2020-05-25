# Git exclude

After cloning this repository, please add the following
exclusions to `.git/info/exclude`:

```
*~
*.sw[mnop]
__pycache__
*.elf
*.o
*.map
*.log
*.avra
```

# Dev-kit programming cheatsheet

## Overview

Use `make` to program:

- the FTDI FT221X EEPROM
    - the kit serial number
    - configuration for how the USB Bridge IC behaves
- the two ATmega328 microcontrollers
    - Flash memory
    - fuses

First time setup of a dev kit requires:

- two `make` commands for the FTDI chip
    - `$ make kitnum`
    - `$ make progkitnum`
- two `make` commands for each microcontroller.
    - `$ make fuses`
    - `$ make flash`

Once a kit has been programmed, updating firmware is just
the Flash:

- `$ make flash`

## Enter the repo

Open bash (the Cygwin mintty terminal):

```powershell
> bash
```

Enter the local directory for this repo

```bash
$ cd ~/chromation/dev-kit-2020/
```

Enter the `firmware` folder

```bash
$ cd firmware/
```

## Program the EEPROM

Hardware setup:

- connect the dev-kit over USB
- if AtmelICE is connected, power it OFF

Check the dev-kit is visible:

```bash
$ make scan
Scanning connected USB devices...
Device 0: FT X Series, ChromationSpect-0911-03, CHROMATION091103
OK: One device found.
```

Program the serial number and configure the FTDI EEPROM:

```bash
$ make kitnum

Serial number: 1182-XX
Enter two digits to replace XX
...
```

*`make kitnum` creates a sub-folder in `kits/` and the EEPROM
config file for this kit*

```bash
$ make progkitnum
...
Enter ENTIRE kit serial number, for example: 1182-01
XXXX-XX: 1182-03
...
```

## Program the fuses and the Flash

- connect the *Atmel-ICE* programmer over USB and ISP
- flip the switches to program the `usb-bridge`
    - slide black switch ''left''
    - slide white switch ''right''

Program the usb-bridge microcontroller:

```bash
$ cd usb-bridge/
$ make fuses

atprogram.exe --tool atmelice --interface isp --device atmega328p \
        write --verify --fuses --values F7D9FF
Firmware check OK
Verification of write OK
Write completed successfully.

$ make flash

atprogram.exe --tool atmelice --interface isp \
        --device atmega328p program --chiperase --verify --file build/usb-bridge.elf
Firmware check OK
Programming and verification completed successfully.
avr-size build/usb-bridge.elf
   text    data     bss     dec     hex filename
   2980      96    1577    4653    122d build/usb-bridge.elf
```

Hardware setup:

- flip the switches to program the `vis-spi-out`
    - slide white switch ''left''

Program the vis-spi-out microcontroller:

```bash
$ cd ../vis-spi-out/

$ make fuses

atprogram.exe --tool atmelice --interface isp --device atmega328p \
        write --verify --fuses --values F7D9FF
Firmware check OK
Verification of write OK
Write completed successfully.

$ make flash

atprogram.exe --tool atmelice --interface isp \
        --device atmega328p program --chiperase --verify --file build/vis-spi-out.elf
Firmware check OK
Programming and verification completed successfully.
avr-size build/vis-spi-out.elf
   text    data     bss     dec     hex filename
   3754     352    1577    5683    1633 build/vis-spi-out.elf
```

For first time setup, see the **Setup instructions** below.

# Setup instructions are in chromation-notebook

:rainbow: See **chromation-notebook**.

Open tiddler **Use AtmelStudio**:

![Screenshot of chromation-notebook TiddlyWiki](open-tiddler.PNG)

* type *Use AtmelStudio* in the search bar on the right
* click on the link **Use AtmelStudio** in the search results
* the tiddler opens on the left, as shown

This tiddler is the starting point for downloading the old
firmware onto the dev-kit.

The tiddler contains links to external websites and links to
other tiddlers in chromation-notebook.

Also see tiddler **Program FTDI Chips with FT_PROG**.

# Access chromation-notebook

```bash
$ mkdir ~/tiddlywiki
$ cd ~/tiddlywiki/
$ git clone https://github.com/sustainablelab/chromation-notebook.git
```

This is a private repo. Request permission from Mike if you are
unable to clone this repo using your GitHub credentials.

If you already have the repo, see if **chromation-notebook** has
changed:

```bash
$ cd ~/tiddlywiki/chromation-notebook/
$ git remote -v update
$ git status
```

If your local copy is behind, update it with:

```bash
$ git pull
```
