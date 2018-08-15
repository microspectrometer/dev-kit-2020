# =====[ USB Host Application ]=====
# See ../mBrd/src/Spi-Commands.h
# PS> &$python2_os_Windows
import colorama
colorama.init()
import serial
s=serial.Serial()
s.baudrate = 9600
s.port = 'COM12'
s.open()
#
# // After downloading flash, move `SW2` to `SPI`. Press the reset button.
a = s.read(s.inWaiting())
a
# Example output:
# 'Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host:PASS\nSPI slave
# responded with \x00\x01'
print a
# Example output:
# Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host:PASS
# SPI slave responded with  ☺
#
# The raw bytes:
#     The `\x00` is the NULL terminator in the `test_passed` message.
#     The `\x01` is the slave response.
#     "\n" is printed as "\n".
# The print-formatted test result:
#     The `NULL` terminator prints as a single space.
#     The `\x01` shows up as a smiley face.
#     "\n" is interpreted as a newline.
