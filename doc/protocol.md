% Communication Protocol
% Mike Gazes
% August 1st, 2019

# Summary
- **USB Host** communicates with the **sensor-interface** via the **usb-bridge**
- all action starts with the USB Host
- USB Host sends a command to the usb-bridge
- usb-bridge passes the command to the sensor-interface
- the number of bytes in the command depends on the command
- the first byte of the command is *always* the `spi_CmdFn_key`
- given the command key, the sensor-interface knows how many bytes are in
  the message
    - this is hard-coded into whichever function the `key` jumps to
- after it receives all bytes, the sensor interface:
    - executes the command (if it is a command to do something)
    - sends a response to the usb-bridge
- sensor-interface responds to *all* messages, even if the command does not
  require the sensor-interface to return sensor data
- sensor-interface is a SpiSlave, so it cannot directly respond
- sensor-interface signals to usb-bridge when it is ready to respond
- then the usb-bridge sends dummy bytes to read the data from the
  sensor-interface
- there are two ways to send the **data-ready** signal
- the old **4-wire** way:
    - `Spi_Miso` is driven low
    - usb-bridge (SpiMaster) then waits for `Spi_Miso` to go high before it
      starts the transmission
- new **5-wire** way:
    - signal is on `Spi_DataReady` (net `DR` on EAGLE schematic)
    - I have no idea yet exactly what this signal will do
- usb-bridge starts reading the response, but does not know beforehand how
  long the response will be
- usb-bridge uses the **first two bytes** of this response to determine the
  number of bytes in the response
    - these two bytes form a 16-bit word, MSB first
- these two bytes are necessary because the sensor-interface might have had an
  error
    - example: the usb-bridge thinks it is requesting frame data, but actually
      sends an invalid command
    - sensor-interface responds to an invalid command with two bytes:
      [StatusInvalid, `invalid_cmd`]
    - usb-bridge expects 784 bytes
    - instead, sensor-interface first sends [0x00, 0x02] indicating that only
      two bytes are coming back
    - then it sends the two bytes: [StatusInvalid, `invalid_cmd`]
    - so a total of four bytes are sent:
        - two bytes of message length
        - two bytes of actual message
- between every byte, the sensor-interface sends a *Data-Ready* signal:
    - SPI data register is not double-buffered
    - SPI slave cannot load next byte until this transfer is finished
    - SpiMaster has to wait for SpiSlave to load next byte before it starts
      the next transmission
- in 4-wire SPI:
    - the usb-bridge must not attempt any other SPI communication while it waits
      for a SpiSlave response because *Data-Ready* is a SPI pin
- in 5-wire SPI:
    - the usb-bridge can do whatever it wants because *Data-Ready* is not a SPI
      bus wire, and the sensor-interface will sit and wait with its data ready
      until the usb-bridge reads it
- after receiving the message, the usb-bridge passes it to the USB Host
- usb-bridge pushes all data from sensor-interface to USB Host, including
  the 2-byte header from the sensor-interface stating how many bytes it is
  sending
- see the rest of section `Protocol` in
  `LIS-770i/doc/book/spi-slave-data-ready-signal.md` to see how I intend to
  handle a timeout


