# See ../mBrd/src/Spi-Commands.h
import serial
s=serial.Serial()
s.baudrate = 9600
s.port = 'COM12'
s.open()
#
a = s.read(s.inWaiting())
a
# Example output:
# 'Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host:PASS\nSPI slave
# responded with \x00\x01'
print a
# Example output:
# Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host:PASS
# SPI slave responded with  ☺
