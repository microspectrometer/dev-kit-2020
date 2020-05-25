# Tasks
[x] make kitnum works
[x] make progkitnum works (but not from Vim)
[x] make help-flash
[x] make fuses
[ ] clean up common.mk Libs section, explain AvrHeaders and FakeAvrHeaders
[ ] clean up messy conditional in kitnum
    - break this out to its own script?
    - want to use the same input validation in progkitnum
[ ] clean up top-level files used for firmware programming
[ ] should the kits folder be in the repo?


# TODO: progkitnum
progkitnum fails when I run it from the Vim command line. The
scan on line 28 works,

```bash
"$ftdi" scan cycl '0'
```

then the subsequent scans fail

```bash
"$ftdi" scan prog 0 ftcfg.xml cycl 0
cd ../..
"$ftdi" scan
```

The failure message:

```
Scanning for devices...
No devices found!
Device 0 does not exist.
Index was out of range. Must be non-negative and less than the size of the collection.
Parameter name: index
Scanning for devices...
No devices found!
Double-check the serial number on the line above is 0911-21.
```

Why would this fail when I run it from Vim?

