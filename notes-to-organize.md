Notes while working on the LIS-770i interface.

# Communication Links to code

```bash
--------        --------        ------
|hostPC|==usb===|simBrd|==spi===|mBrd|
--------        --------        ------
                    |            |  |
                    |            |  |
                    i2c          |  lis_io/clk
                    |            |  |
                    |            |  |   -----
               -----------       |  ----|Lis|
               |LedDriver|       |      -----
               -----------       |
                                 adc_spi
                                 |
                                 |      -----
                                 -------|Adc|
                                        -----

┌─────────┐                 ┌────────┐                    ┌───────────┐
│ UsbHost │=====[ Usb ]=====│ FT221X │=====[ Ft1248 ]=====│ ATmega328 │
└─────────┘                 └────────┘                    └───────────┘
```
```
The labels on the communication links shall be turned into code libraries.

- Implement the libraries in this order:
    - Usb (Ft1248)
    - I2c
    - Spi,
    - Adc-spi
    - Lis-io

The implementation order is bridge-building: it is the necessary order to have a
working link from the *hostPC* to each *endpoint*. The `i2c` is not necessary,
but it will be an improved debug display compared with the single red/green LED
on the `simBrd`.

#Background
    Abbreviations
        PCB     Printed Circuit Board
        dBrd    daughterboard with line scan sensor
        mBrd    motherboard: interfaces line scan sensor
        simBrd  SPI simulant for a host PC to communicate with the mBrd
        rgbBrd  PCB with an RGB LED
        ISP     In System Programmer: either the AVRISPmkII or ATMEL-ICE
        FFC     flat-flexible cable
        ZIF     Zero Insertion Force
        2.5V    2.5 Volts
        3.0V    3.0 Volts
        ADC     Analog-to-Digital Converter
        SAR     Successive Approximation Register
        XTAL    Crystal
        MCU     Microcontroller
        GPIO    General Purpose I/O (input/output)
        RST     Reset
        CS      Chip Select
        USART   Universal Synchronous/Asynchronous Receiver/Transmitter
        UART    USART is often just written as UART (I try to use USART)
        XCK     USART clock
        RxD     USART data line: receive
        TxD     USART data line: transmit
        SPI     Serial Peripheral Interface
        SCK     SPI clock
        MOSI    SPI data line: Master Out Slave In
        MISO    SPI data line: Master In Slave Out
        SS      SPI Slave Select
                SS is a special function pin on the SPI slave.
                SS is implemented with a GPIO pin on the SPI master.

    Names of important electronic components
        ATmega328   A popular 8-bit MCU manufactured by Atmel and used on
                    many of the early Arduinos.  This embedded code targets a
                    32-pin ATmega328.  The QFN (no lead) and TQFP (leaded quad)
                    have the same pinout.

        LIS-770i    A line scan sensor manufactured by Dynamax.
                    The MCU clocks the LIS-770i to collect frames of pixel data.
                    Each pixel is an analog voltage.  The LIS-770i is off-board
                    on its own daughterboard (dBrd).  The MCU and all other
                    components are on the motherboard (mBrd).  The dBrd and mBrd
                    connect via a flat-flexible cable (FFC).  The flat-flex
                    plugs into a ZIF (zero insertion force) connector.  The ZIF
                    on the dBrd and mBrd are identical, but note that the pin
                    mapping is mirrored: M1 -> D8, M2 -> D7, ... M8 -> D1.  The
                    8 pins on the LIS-770i do not map in an obvious way to the 8
                    pins on the ZIF (e.g., it is not pin 1 to pin 1, pin 2 to
                    pin 2, etc.). Pin 1 on the ZIF is marked by a triangle.

        LTC1864L    A 16-bit SAR ADC with a differential input and
                    a unipolar transfer function. It uses a 2.5V external
                    reference.  Unipolar transfer function means the allowed
                    analog voltages range from 0V to 2.5V.  Differential input
                    means the analog voltage is the difference between inputs
                    IN+ and IN-. IN- connects to the LIS-770i VREF (dark
                    reference) output, so the difference between IN+ and IN- is
                    the dark-corrected pixel voltage.  The MCU communicates with
                    this ADC over SPI to read the pixel voltages as 16-bit
                    values. The MCU uses its USART (XCK, RxD, and a GPIO CS) to
                    act the SPI master. The ADC is the SPI slave.


    Existing project code:
        < > is an empty folder; <x> means not empty or a file
        PCB project files for the camera interface PCB:
            C:\chromation-dropbox\Dropbox\design files and protocols\circuits\
            pcb design\eagle\projects\Chromation\20151020_LIS-770i_mBrd
        The PCB is for evaluating the LIS-770i line scan sensor. This PCB acts
        as a SPI slave component: it receives a frame request from a SPI master,
        and then initiates a measurement sequence, transmitting frame data back
        to the SPI master.
        Code for the camera interface PCB:
        <x> ./20151020_LIS-770i_mBrd/src/main.c
        <x> ./20151020_LIS-770i_mBrd/src/pins.h
        < > ./20151020_LIS-770i_mBrd/src/lib/adc_spi/.
        < > ./20151020_LIS-770i_mBrd/src/lib/debug_led/.
        < > ./20151020_LIS-770i_mBrd/src/lib/i2c_master/.
        < > ./20151020_LIS-770i_mBrd/src/lib/lis_clk/.
        < > ./20151020_LIS-770i_mBrd/src/lib/lis_io/.
        < > ./20151020_LIS-770i_mBrd/src/lib/spi_slave/.
        PCB project files for the host PC interface PCB (SPI simulant PCB):
            C:\chromation-dropbox\Dropbox\design files and protocols\circuits\
            pcb design\eagle\projects\Chromation\20160219_SPI_simulant
        The host PC interface PCB simulates a SPI master that the customer
        (originally FLIR) would instantiate in their existing hardware.
        Code for the SPI simulant PCB:
        <x> ./20160711_SPI_simulant/src/main.c
        <x> ./20160711_SPI_simulant/src/pins.h
        <x> ./20160711_SPI_simulant/src/lib/ft1248/ft1248.c
        <x> ./20160711_SPI_simulant/src/lib/ft1248/ft1248.h
        <x> ./20160711_SPI_simulant/src/lib/i2c_master/i2c_master.c
        <x> ./20160711_SPI_simulant/src/lib/i2c_master/i2c_master.h
        < > ./20160711_SPI_simulant/src/lib/spi_master/.
        < > ./20160711_SPI_simulant/src/lib/test_points/.

#First crack
USB communication
-----------------
[ ] What has to be coded to set up USB communication?
    [x] This is in my ft1248 lib.
        /cygdrive/c/chromation-dropbox/Dropbox/design files and
        protocols/circuits/mcu/Atmel Studio/LIS-770i_Interface/
        ./20160711_SPI_simulant/src/lib/ft1248/ft1248.c
        ./20160711_SPI_simulant/src/lib/ft1248/ft1248.h
    [ ] What did I have so far?
        path:
        /cygdrive/c/chromation-dropbox/Dropbox/design files and protocols/circuits/mcu/Atmel Studio/LIS-770i_Interface/20160711_SPI_simulant/src/lib/ft1248
        Version 0.3: ATmega328 is the FT1248 bus master.
        ft1248_write() is 8-bit wide
            This is the code from lines 52 to 311.  The rest of the code is the
            4-bit wide version.
        Private functions:
            break_up_data
        Public functions:
            ft1248_setup
            ft1248_available
            ft1248_write
            ft1248_read
        Function-like macros:
            Common:
                set_bit (why is this not in the tags file?)
            ft1248:
                pin control:
                    cfg_miosio_port_as_output8
                    cfg_miosio_port_as_input
                control lines:
                    cfg_ft1248_sck_pin_as_output
                    ft1248_sck_pin_high
                    ft1248_sck_pin_low
                    cfg_ft1248_ss_pin_as_output
                    ft1248_ss_pin_high
                    ft1248_ss_pin_low
                    cfg_ft1248_miso_pin_as_input
                    en_ft1248_miso_pin_pull_up
                    dis_ft1248_miso_pin_pull_up
        Screaming-CAPS macros:
            ft148 protocol:
                flow control:
                    RX_BUFFER_HAS_INCOMING_DATA
                    TX_BUFFER_IS_FULL
                    TX_BUFFER_IS_NOT_FULL
                    FT1248_ACK
                    FT1248_NACK
                8-bit bus width:
                    FT1248_CMD_WRITE8
                    FT1248_CMD_READ8
    [ ] Test this
        How do I test this?
        Start a temporary main file here:
        /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/testbox/src/main.c
        Make the minimum viable test file.
        Compile as you build it up to this minimum viable.
            $ avr-gcc ./src/main.c -o testbox
            In file included from c:\program files (x86)\atmel\studio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\avr\interrupt.h:38:0,
                             from ./src/main.c:2:
            c:\program files (x86)\atmel\studio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\avr\io.h:623:6: warning: #warning "device type not defined" [-Wcpp]
             #    warning "device type not defined"
                  ^
        Try the following line.
        $ avr-gcc -mmcu=atmega328p ./src/main.c -o testbox
        In file included from c:\program files (x86)\atmel\studio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\avr\interrupt.h:38:0,
                         from ./src/main.c:2:
        c:\program files (x86)\atmel\studio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\avr\io.h:623:6: warning: #warning "device type not defined" [-Wcpp]
         #    warning "device type not defined"
              ^
        c:/program files (x86)/atmel/studio/7.0/toolchain/avr8/avr8-gnu-toolchain/bin/../lib/gcc/avr/5.4.0/../../../../avr/bin/ld.exe: cannot find crtatmega328p.o: No such file or directory
        c:/program files (x86)/atmel/studio/7.0/toolchain/avr8/avr8-gnu-toolchain/bin/../lib/gcc/avr/5.4.0/../../../../avr/bin/ld.exe: cannot find -latmega328p
        collect2.exe: error: ld returned 1 exit status

        OK, that didn't help with the 'device type not defined' and I got a
        bunch more errors.

        I need to give the -B and -I flags.
        -I lets me include files.
        -B lets me add to the compiler's search path.
        What's the difference?

        $ avr-gcc -mmcu=atmega328p -B /home/packs/Atmel.ATmega_DFP.1.0.86/gcc/dev/atmega328p/ -I /home/packs/Atmel.ATmega_DFP.1.0.86/include/ ./src/main.c -o testbox
        But I need to use my paths, not the example paths:
C:\Program Files (x86)\Atmel\Studio\7.0\packs\atmel\ATmega_DFP\1.2.203
        $ atmega_packs='/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203'
        $ echo "$atmega_packs"/
        $ avr-gcc -mmcu=atmega328p -B "$atmega_packs"/gcc/dev/atmega328p/ -I "$atmega_packs"/include/ ./src/main.c -o testbox

        Still failed... grrrr.

        $ avr-gcc -mmcu=atmega328p -B "$atmega_packs"/gcc/dev/atmega328p/ -I "$atmega_packs"/include/ ./src/main.c -o testbox
        In file included from c:\program files (x86)\atmel\studio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\avr\interrupt.h:38:0,
                         from ./src/main.c:2:
        c:\program files (x86)\atmel\studio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\avr\io.h:623:6: warning: #warning "device type not defined" [-Wcpp]
         #    warning "device type not defined"
              ^
        c:/program files (x86)/atmel/studio/7.0/toolchain/avr8/avr8-gnu-toolchain/bin/../lib/gcc/avr/5.4.0/../../../../avr/bin/ld.exe: cannot find crtatmega328p.o: No such file or directory
        c:/program files (x86)/atmel/studio/7.0/toolchain/avr8/avr8-gnu-toolchain/bin/../lib/gcc/avr/5.4.0/../../../../avr/bin/ld.exe: cannot find -latmega328p
        collect2.exe: error: ld returned 1 exit status

        Try being more specific: give the path to the folder that has
        crtatmega328p.o.

        This finally worked.
        From the project main.c window, <F2> to open a bash terminal in Vim.
        Go up one level so the build output doesn't end up in the /src folder.
        $ cd ..
        or just navigate there from anywhere:
        $ cd /cygdrive/c/chromation-dropbox/Dropbox/c/LIS-770i/testbox/
        $ atmega328_lib='/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'
        $ atmega_packs='/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203'
        $ avr-gcc -mmcu=atmega328p -B "$atmega328_lib"/ -I "$atmega_packs"/include/ ./src/main.c -o testbox
        That worked! Or at least it did not throw an error.
        The build will get stupid... I'll want to make a recipe in a Makefile.
        I had to remake the tags file one level higher.
        [ ] Try a hello world, turn the LED green/red.
            I wrote the hello world and it builds.
            Next step: do a build and burn in Atmel Studio.
            Look at the resulting Makefile. That informs how I attempt to build
            from atprogram.

    [ ] Refactor:
        Eliminate the function-like macros.
        In-fact, eliminate *all* macros: screaming-CAPS macros become const
        variables.
    [ ] Refactor the .h comments:
        This file contains some wonderful documentation I made that explains the
        FT1248 protocol. This belongs somewhere else, like a .md file for the
        lib, not buried in the .h.


#Environment setup
Debug simBrd with the one red/green LED.

    GUI-style:
        Start Atmel Studio 7.0:
            PS> &$atmelstudio
        Ctrl+Shift+P
        or
        Tools -> Device Programming
            opens the Device Programming dialog window
        at the top of the window:
            Tool: Atmel-ICE
            Device: ATmega328P
            Interface: ISP
        click "Apply"
        Device signature: click "Read"
            status message: "Reading device ID...OK"
        click "Fuses"
            Fuse Register:  Value
                EXTENDED:   0xFF
                HIGH:       0XD9
                LOW:        0xF7
            Full details on fuse settings:
                C:\chromation-dropbox\Dropbox\design files and
                protocols\circuits\mcu\Atmel Studio\LIS-770i_Interface\fuse
                settings.txt

        "Memories" vs "Production Files"
        See Production Files here:
        https://www.microchip.com/webdoc/GUID-ECD8A826-B1DA-44FC-BE0B-5A53418A47BD/index.html?GUID-E9FD4617-290B-4EA2-8C82-B1524094A495
        See Memories here:
        https://www.microchip.com/webdoc/GUID-ECD8A826-B1DA-44FC-BE0B-5A53418A47BD/GUID-B5991BFE-21E0-4AA1-9E69-C78A9BBF599D.html
        I see no difference that would matter to me at this moment on
        2018-02-14, so I'm sticking with "Memories".
        I'm also only programming the Flash, not the EEPROM and Fuses.
        I have nothing to put in EEPROM yet.
        The Fuses are fine just as they are.
        Eventually, we'll want to encode the Fuses in software in a Production
        File.

        click "Memories"
            Under Flash (32 KB):
                set the path to the .elf file for the project:
                e.g.:
                    C:\chromation-dropbox\Dropbox\design files and protocols\circuits\mcu\Atmel Studio\LIS-770i_Interface\archived projects\20160219_SPI_simulant-old\Debug\20160219_SPI_simulant.elf
                e.g.:
                    C:\chromation-dropbox\Dropbox\c\LIS-770i\testbox\AtmelStudio\GenerateMakefile\Debug\testbox.elf
                    
            to check that this is the file programmed on the device, click "Verify".
            To program this file onto the device, click "Program".

    CLI-style:
        Start the 'atprogram' utility:
            PS> &$atprogram
        Start a powershell session to recover powershell functionality.
            PS> powershell
        Read the help:
            PS> atprogram --help
            Better, pipe the help to the clipboard:
            PS> atprogram --help | clip
            (This might throw an error, but it works).

        How do I do the above simple tasks with the 'atprogram' utility?
        Device signature: (atprogram: info)
            Expect: 0x1E950F
            PASS
        Fuses: (atprogram: info)
            Expect: EXTENDED 0xFF, HIGH 0xD9, LOW 0xF7
            PASS

                What COM port is the ATMEL-ICE? It doesn't matter.
                    The ATMEL-ICE shows up in Device Manager under 'Human
                    Interface Devices' as a 'USB Input Device'. There is no COM
                    number associated with it.
                    But I can talk to it without using the -c option to specify
                    COM port.

            Read all of the information about the micontroller:
                PS> atprogram -t atmelice -i isp -d atmega328p info
                PS> atprogram --tool atmelice --interface isp --device atmega328p info
                    Nothing is case-sensitive.
                    -t  --tool              Tool name: avrispmk2, atmelice
                    -i  --interface         Physical interface: debugWIRE, ISP
                    -d  --device            Device part number
                    -v  --verbose           Verbose output (debug)
                    -xr --externalreset     Apply external reset when starting a
                                            session.
                    -V  --version           Display version information.
                    -cf --configfile <arg>  Set the configuration setting file
                                            name with path.
                    [ ] What is an 'atbackend' executable?
                    info    command to request information

                    We get:
                    Device information:
                        voltage on the target
                        atmelice firmware version
                        device signature
                    Fuses:
                        EXTENDED (0b11111111)
                        HIGH (0b11011001)
                        LOW (0b11110111)
                    Converting those to hex:
                        EXTENDED is 0xFF
                        HIGH is 0xD9
                        LOW is 0xF7
                    Great, that matches the expected.
        
        Check the memory:
        Which atprogram command is this?
        verify           Verify content of memory based on a file.

            PS> atprogram help verify | clip
            atprogram [options] verify <arguments>
            PS> atprogram -t atmelice -i isp -d atmega328p verify <arguments>
            [ ] What is the difference between ihex, binary, and elf formats?
            -f (file)               Input file to verify against.
            --format                Input file format, determined from contents
                                    if not specified. Supported values are
                                    'bin', 'elf', and 'ihex'.
            -fl --flash             Verify flash.
            -ee --eeprom            Verify eeprom.
            -us --usersignature     Verify user signature.
            -sp --signatures        Verify signatures.
            -up --userpage          Verify user page.
            -fs --fuses             Verify fuses.
            -lb --lockbits          Verify lockbits.
            -s  --size              Number of bytes to verify.
            -o  --ofset             Start address of verification. Default is 0.
                                    Only valid for binary file format.
            --values                Hex encoded values to write, ex: 0102040A0F

    ---EXAMPLES---
            Verify the flash contents against source.elf:
    atprogram -t atmelice -i isp -d atmega328p verify -fl -f source.elf

            Verify *all* the contents of source.elf in the base address space:
    atprogram -t atmelice -i isp -d atmega328p verify -f source.elf
            [ ] What is the *base* address space?

            Verify *all* the contents of source.elf in the base address space
            with offset 0x400000:
    atprogram -t atmelice -i isp -d atmega328p verify -f source.bin -o 0x400000

            Verify the second and third fuse bytes against 1E1F:
    atprogram -t atmelice -i isp -d atmega328p verify -fs --values 1E1F --offset 1

            [x] Try verifying the flash against the source file:

            Get the full path to the source.elf:
                C:\chromation-dropbox\Dropbox\design files and protocols\circuits\mcu\Atmel Studio\LIS-770i_Interface\archived projects\20160219_SPI_simulant-old\Debug\20160219_SPI_simulant.elf
            Create a variable for the path:
                PS> $source = "C:\chromation-dropbox\Dropbox\design files and
                protocols\circuits\mcu\Atmel Studio\LIS-770i_Interface\archived
                projects\20160219_SPI_simulant-old\Debug\20160219_SPI_simulant.elf"
            Verify the flash:
                atprogram -t atmelice -i isp -d atmega328p verify -fl -f $source
            Result:
                Firmware check OK
                Verification OK

        More commands:
        chiperase        Full erase of chip.
        [ ] Do I need to erase before I can program?
        program          Program device with data from <file>.
        reset            Reset all domains and jump to the reset vector.
        interactive      Run in interactive mode.

        To get help on a command:
        atprogram help {command-name}

## Review all the steps to program the simBrd flash from the CLI

### Change Path instead of adding PowerShell Environment Variables
- Wrong:
```
PS> &$atprogram
```
> `C:\Program Files (x86)\Atmel\Studio\7.0\Extensions\Application\StudioCommandPrompt.exe`
> `/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/Extensions/Application/StudioCommandPrompt.exe`
- See notes in `/cygdrive/c/Users/Mike/Documents/WindowsPowerShell/Microsoft.PowerShell_profile.ps1`
- I copied those notes below and turned them into markdown.

---
- I ended up only needing `atprogram` and I simply added it to the path.
    - `atprogram` is added to the path via the `atbackend` folder.

- I originally created Atmel Studio 7.0 shortcuts.
```
$atmelstudio = "C:\Program Files (x86)\Atmel\Studio\7.0\AtmelStudio.exe"
```
- and ran these with `&` prefix, e.g.:
```
PS> &$atprogram
PS> &$atmelstudio
```
- Then I started automating with Make and found I just needed to invoke
  atprogram.
```
$this_is_NOT_atprogram = 
    "C:\Program Files (x86)\Atmel\Studio\7.0\Extensions\Application\StudioCommandPrompt.exe"
```
- I found the actual `atprogram.exe`:
```
$this_is_atprogram_but_just_add_to_path =
    "C:\Program Files (x86)\Atmel\Studio\7.0\atbackend\atprogram.exe"
```
- but I needed it on my Path, not as a PowerShell Envrionment variable!
```
#Append avr8-gnu-toolchain to PATH.
$avr8_path = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin"
$env:PATH = "$env:PATH;$avr8_path"

#Append atprogram.exe folder to PATH.
$atbackend_path = "C:\Program Files (x86)\Atmel\Studio\7.0\atbackend"
$env:PATH = "$env:PATH;$atbackend_path"
```

### Verify programming communication link
    PS> powershell
    PS> atprogram -t atmelice -i isp -d atmega328p info
        Expect device signature:    0x1E950F
        Expect fuse settings:       EXTENDED 0xFF, HIGH 0xD9, LOW 0xF7
Program flash:

Verify a file is loaded:
    Copy the full path to the source.elf to the clipboard.
    Create a variable: $source = {paste full path}.
    atprogram -t atmelice -i isp -d atmega328p verify -fl -f $source
