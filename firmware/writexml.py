#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys
serial_number=sys.stdin.read().rstrip()
# example:
# serial_number='1182-02'
fourdig = serial_number.split('-')[0]
twodig = serial_number.split('-')[1]

import xml.etree.ElementTree as et
tree = et.parse('ftcfg.xml')
root = tree.getroot()

# TODO: use ElementTree find() or findall() or findtext() instead
# of hardcoding the tree indices
# example:
# https://stackabuse.com/reading-and-writing-xml-files-in-python/
# example:
# https://stackoverflow.com/questions/6523886/find-and-replace-values-in-xml-using-python

root[3][1].text=f'ChromationSpect-{serial_number}'
root[3][3].text=f'CHROMATION{fourdig}{twodig}'

# Write to new file
tree.write('newftcfg.xml')

print("    ...created new FTDI EEPROM XML to program kit serial number")
print(f"\t\tSerial number for LabVIEW GUI: {root[3][1].text}")
print(f"\t\tSerial number for Python API: {root[3][3].text}")
