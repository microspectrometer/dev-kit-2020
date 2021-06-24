#!/usr/bin/bash

# Generate first four digits of kit serial number as an example.
fourdig=$(python3 kitnum.py)
# Ask user for kit serial number.
echo Enter ENTIRE kit serial number, for example: $fourdig-01
read -p 'XXXX-XX: ' dashed_number
kit_xml="./kits/$dashed_number/ftcfg.xml"
echo
echo Looking for $kit_xml...

# Stop if folder and XML file do not exist.
if [ -e $kit_xml ]
then
    echo OK: $kit_xml exists.
else
    echo FAIL: $kit_xml does not exist.
    echo
    echo Run \'make kitnum\' to create $kit_xml.
    echo Then do \'make progkitnum\' again.
    return -1
fi

# Program Device 0 EEPROM using XML file.
echo
echo Programming EEPROM...
cd kits/$dashed_number/
#Do not cycle power before scanning 
#"$ftdi" scan cycl '0'
"$ftdi" scan prog 0 ftcfg.xml cycl 0
cd ../..
# "$ftdi" scan
echo \'make scan\' to check the serial number is $dashed_number.
echo
echo \'make troubleshoot-ftdi\' if you run into trouble.
return 0
