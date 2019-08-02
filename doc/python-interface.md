% Python Interface
% Mike Gazes
% August 2, 2019

# USB communication as a COM port in Windows

## Context
The FTDI device on our `usb-bridge` is accessible from the USB Host in two ways:

- FTDI D2XX API
- generic COM port (VCP)

We use the FTDI D2XX API for LabVIEW programs. Moving forward, we are switching
from LabVIEW to Python. We are, therefore, switching from D2XX to the generic
COM port interface:

- I cannot find a stable `d2xx` python library solution
- the extra functionality offered by D2XX is not compelling enough to write our
  own `d2xx` library

### Use `pyserial`

- `pyserial` is the de facto standard Python serial library
- `pyserial` is compatible with different operating systems

### `pyserial` does not need a `COM` string
What about the annoyance of having to open a Windows Device Manager and
plug/unplug the USB device to figure out which device is on which COM port?

- `pyserial` has a `grep` function that takes the serial number and returns a
  `port` object
- `pyserial` can then open/close the `port` object without ever having to use
  the `COM` string explicitly

### VCP sees `Serial Number`
- the essential serial commands are available with VCP
- VCP cannot access *all* the USB Description Strings that D2XX can access:
    - `Product Description` is not visible
    - `Manufacturer` is not visible
- but VCP can access `Serial Number`
- `Serial Number` is all we need for identifying spectrometer dev kits on the
  USB Host

### Leave the default USB identifiers in the FTDI `.inf` file
There is a COM port variable named `manufacturer` but this actually comes from
the `.inf` file installed with the USB driver. FTDI wants you to change this.
This FTDI doc shows where the fields are in `FT_Prog`:

<https://www.ftdichip.com/Support/Documents/AppNotes/AN_124_User_Guide_For_FT_PROG.pdf>

And this doc explains what you're supposed to change:

<https://www.ftdichip.com/Support/Documents/TechnicalNotes/TN_102_OEM_Technical_Support_Requirements_for_FTDI_Products.pdf>

But changing these values voids the security certificate:

> It is important to note that the device drivers in this example are
> shown in Figures 2 and 3 as “Not digitally signed”. Any time there are
> edits performed on any of the device driver files, it will invalidate
> the WHQL signature. Re-certification by the OEM is possible. Refer to
> the `AN_101_WHQL_Certified_Driver_Process(FT_000063)`.

- and that would sketch out customers
- and since no one gives a fuck anyway, just leave it as-is:
    - `manufacturer` shows up as FTDI, not a big deal

This is just `eval kit hardware`. If it *were* a consumer product, then we'd
want to change this and do it right.

Doing it right requires paying for the security certificate mojo:

<https://www.ftdichip.com/Support/Documents/AppNotes/AN_101_Submitting_Modified_FTDI_Drivers_for_Windows_Hardware_Certification.pdf>

## Set the serial number
- I leave the `Product Description` as-is so that my LabVIEW code does not
  have to change: it still gets `ChromationSpect-0643-01`
- but now I put `CHROMATION123456` in FTDI `serial_number`
- when `pyserial` scans the USB ports, it can `grep` (awesome!) for the
  serial number and return a `port` object
- I can tell that `port` object to open up without ever having to use the
  `COM` string explicitly
- this *should* make the find-and-open code cross-platform
- other change in `FT_Prog` is the manufacturer
- change manufacturer from `FTDI` to `CH`
    - does not impact `Load VCP` users, e.g., what you can see from
      `pyserial`
    - does not impact my LabVIEW code since I never look at the manufacturer
      field
    - but it helps with the 44 character limit:
    - Product Description + manufacturer + serial number cannot exceed 44
      characters
    - serial number is supposed to be 16 characters: `CHROMATION123456`

## Make FTDI device visible as a generic COM port
- enable `Load VCP` in Windows Device Manager for FTDI bridge IC:

## Script examples
- serial commands are encapsulated in functions
- functions are all very short thanks to generators and list comprehensions
- even the final plotting code is relatively short, as plotting code goes
- up until 3AM figuring out a snappy live updating matplotlib
- snag here with globals to revisit later

