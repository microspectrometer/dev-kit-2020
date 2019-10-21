# SPI master and slave
The Sensor board is a SPI slave. The Bridge board is the SPI master.

# SPI signals
## Five wires

| Signal | Name            | Master | Slave |
| ------ | --------------- | ------ | ------|
| `!DR`  | Data Ready      | in     | out   |
| `!SS`  | Slave Select    | out    | in    |
| `SCK`  | SPI Clock       | out    | in    |
| `MOSI` | Master Data Out | out    | in    |
| `MISO` | Slave Data Out  | in     | out   |

Data Ready is special. A SPI interface usually only has four wires. The Bridge
has no way to know when the Sensor is ready to send the requested data. The
Sensor drives Data Ready low to let the Bridge know it is ready to send a byte,
and drives it high again when the tranmission is complete. If the Sensor is
sending multiple bytes, it drives Data Ready low before each byte, and high
after each byte.

## Signaling on wires
- Data Ready is normally high
    - Sensor drives Data Ready wire low to signal to the Master that it has data
      to send
- Slave Select is normally high
    - Bridge uses Slave Select to control when a SPI transfer starts and ends:
    - Bridge drives Slave Select low to activate the SPI module in the Sensor
    - Bridge drives Slave Select high at the end of every byte transfer to
      re-synchronize the Master and Slave SPI modules
- AVR SPI modules have a SPI Data Register
    - Bridge starts the SPI transfer by writing to the SPI Data Register
    - the byte in the Bridge SPI data register is clocked into the Sensor on
      MOSI
    - the byte in the Sensor SPI Data register is clocked out of the Sensor on
      MISO
- Bridge drives SCK (the SPI clock) and therefore controls the speed of the
  transmission

# SPI Data Register

## Circular buffer
- SPI MISO and MOSI wires connect the Master and Slave to form a 16-bit circular
  buffer
- the circular buffer is comprised of the Master's 8-bit SPI Data Register and
  the Slave's 8-bit SPI Data Register

## SPI Data Register is double-bufferred for read access
- the SPI module buffers the SPI Data Register for read access:
    - the received byte is available to read from the SPI Data Register even
      after the next transmission starts
    - but once the next transmission is over, the SPI Data Register is
      over-written with the new received byte
- the SPI module does not buffer the SPI Data Register for write access:
    - wait to write the next byte to the SPI Data Register until the
      transmission is over

## SPI clock polarity and phase
- the phase-relationship between clock rising/falling edge and data
  rising/falling edge is configured in the SPI Control Register
    - Master and Slave must use the same configuration
    - Bridge and Sensor use the default AVR configuration
    - the default AVR configuration is:
        - SPI Mode 0: CPOL=0, CPHA=0
        - SPI Mode 0 means that
        - the rising edge of SCK clocks the MISO/MOSI data into the SPI Data
          Register
        - the falling edge of SCK signals it is OK to load the next bit from the
          SPI Data Register onto the MISO/MOSI data line
    - the MSB (most-signficant bit) of the SPI Data Register is transmitted
      first
    - the sampled data value enters the SPI Data Register at the LSB
      (least-significant bit)

## Example transmission
- if the Sensor is initiating, the Sensor drives Data Ready low
- Bridge drives Slave Select low
- Bridge firmware writes a byte to its SPI Data Register
    - the AVR SPI module handles clocking SCK while driving MOSI and sampling
      MISO
- On each rising edge of SCK:
    - signal on MOSI shifts into the LSB of the Sensor SPI Data Register
    - signal on MISO shifts into the LSB of the Bridge SPI Data Register
- On each falling edge of SCK:
    - drive MOSI with the MSB in the Bridge SPI Data Register
    - drive MISO with the MSB in the Sensor SPI Data Register
- Bridge continues clocking SCK until all eight bits are swapped between Bridge
  and Sensor SPI Data Registers
- Before the transmission, the SPI Data Registers look like this:
    - Master: byte M
    - Slave:  byte S
- After the transmission, the SPI Data Registers are swapped:
    - Master: byte S
    - Slave:  byte M
- At this point the transmission is complete:
    - the Sensor enters a SPI interrupt routine to read the SPI Data Register
    - the Sensor drives Data Ready high
    - the Bridge drives Slave Select high
    - the Bridge reads the SPI Data Register


# SPI summary
The circular buffer means that, from a hardware perspective, both Master and
Slave transmit and receive on every byte transmitted. Who is *actually*
transmitting/receiving depends on who has data to send.

The Bridge transmits to the Sensor when it receives a command from the USB host.
If the Bridge recognizes the command from the USB host, it passes it along to
the Sensor. The Bridge drives Slave Select low and writes the command byte to
the SPI Data Register to start the transmission.

The Sensor transmits data to the Bridge when it responds to a command. The
Sensor responds to every command it receives, even commands that it does not
recognize. The Sensor cannot start a transmission. The Sensor transmits a byte
by writing the byte to the SPI Data Register and driving Data Ready low to tell
the Bridge to start a transmission. The Bridge drives Slave Select low and
writes a garbage byte to the SPI to start the transmission.

In both cases, the device that is sending data writes to the SPI data register
before each transmission starts, and ignores the contents of the SPI data
register at the end of each transmission.
