% Load VCP

# Internal Use
This document is for internal use. *I* need to remember how to enable `Load VCP`
because, by default, the `FT221XA` does not show up in `Device Manager` with
`Load VCP` enabled.

Customers no longer need to know how to enable `Load VCP` to communicate with
the eval kit as a `COM` port.

# Configure FTDI Driver for VCP
## Context
The Chromation eval software uses the FTDI `D2XX` driver. On Windows, this
is the `ftd2xx.dll` library. The `ftd2xx` API provides functions for
identifying the Chromation eval kit by its `Product Description`.

The Python code snippets in this doc use Python package `pyserial`. On Windows,
this package only accesses serial devices listed as COM ports.

Before I can communicate with the eval kit using Python package `pyserial`,
I need to Load `VCP`.

- the eval hardware uses an `FTDI FT221X` for USB communication
- the `FT221X` ships configured to communicate using the `D2XX` driver
- the Python example in this doc uses `VCP` instead of `D2XX`
- I need to enable `VCP`
- I can switch back to `D2XX` by unselecting `Load VCP` anytime and toggling
  power, but why would I?

## Instructions
### Change the driver
- connect Chromation eval kit to my computer via USB
- run `Device Manager`:
    - `WinKey+x`
    - press `m` to select `Device Manager`
- expand `Universal Serial Bus Controllers`
- find `Usb Serial Converter`
- right-click, select `Properties`
- click on the `Advanced` tab
- check the box `Load VCP`
- this uses `VCP` instead of `D2XX`
- disconnect and reconnect the Chromation eval kit USB cable

# Select `Load VCP`
The `D2XX` API works even with `Load VCP` enabled. The hardware eval kits,
therefore, are sent to customers with `Load VCP` enabled. This gives customers
the flexibility to use the Windows Virtual COM to establish communication.

## Details
`Load VCP` simplifies the USB Host application. With `Load VCP` enabled,
the `pyserial` API is sufficient to do all necessary tasks: *find* the attached
eval kit, and *communicate* with it. This saves customers from having to write
`D2XX` wrappers, i.e., a customer can talk to our device without ever consulting
the *FTDI D2XX Programmer's Guide*. Yay.

Without `Load VCP`, `pyserial` needs an additional set of wrappers to make the
`D2XX` .dll calls described in the *FTDI D2XX Programmer's Guide*. Boo.

A `D2XX` call is still necessary for the USB Host application to send a hard
reset. The eval kit uses `CBUS`. This is very hardware-specific, so `pyserial`
has no API for manipulating the `CBUS` pin. I provide this function in our
pyspect.py package. So customer's writing a USB Host application can `import
pyspect` and call `pyspect.reset()` without ever consulting the *FTDI D2XX
Programmer's Guide*. Yay.

