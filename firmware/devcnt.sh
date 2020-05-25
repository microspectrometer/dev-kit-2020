#!/usr/bin/bash

# Count the number of USB-connected 'FT X Series' devices.
devcnt=$(grep 'FT X Series' devscan -c)

# FAIL if the number of devices is not 1.
if [ $devcnt -eq 0 ]
then
    echo FAIL: No devices found!
    return -1
else
    grep 'FT X Series' devscan | cat
    if [ $devcnt -gt 1 ]
    then
        echo FAIL: More than one device found.
        return $devcnt
    else
        echo OK: One device found.
        return 0
    fi
fi

