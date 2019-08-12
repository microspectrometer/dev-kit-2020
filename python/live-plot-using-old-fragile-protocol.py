import serial
import serial.tools.list_ports as usb_ports
import contextlib
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# =====[ Open communication with the spectometer ]=====

def com_port(serial_number):
    with contextlib.suppress(StopIteration):
        return next(usb_ports.grep('CHROMATION'+serial_number)).device

def open_spectrometer(serial_number='012301'):
    usb=serial.Serial()
    usb.baudrate = 115200
    usb.port = com_port(serial_number)
    if usb.port is None:
        print(
            "Cannot `open_spectrometer('{}')`. Make sure it is connected."
            .format(serial_number),
            file=sys.stderr
            )
        return
    usb.open()
    while not usb.is_open: pass
    return usb

# =====[ Configure the spectrometer's linear array ]=====

def cfg_spectrometer(usb):
    """0x07: cfg, 0x06: bin, 0x0B: 1x gain, 0x0C: all rows"""
    usb.write(b'\x07\x06\x0B\x0C')

# =====[ Read a frame of data ]=====

def request_frame(usb, npixels):
    usb.write(b'\x01')
    while (usb.inWaiting()<npixels*2): pass

def pixel_reading(usb):
    return int.from_bytes(usb.read(2), byteorder='big', signed=False)

def read_frame(usb, npixels=392):
    request_frame(usb, npixels)
    return np.array([pixel_reading(usb) for pixel in range(npixels)])

# =====[ Auto-expose and manually set integration time ]=====

def to_ms(tics):
    return tics/50

def auto_expose(usb):
    usb.write(b'\x15')
    while(usb.inWaiting()<2): pass
    return to_ms(int.from_bytes(usb.read(2), byteorder='big', signed=False))

def to_tics(ms):
    return ms*50

def exposure(ms):
    return b''.join((b'\x02',to_tics(ms).to_bytes(2,byteorder='big', signed=False)))

def set_integration_time(usb,ms=1):
    usb.write(exposure(ms))
    while(usb.inWaiting()<2): pass
    return to_ms(int.from_bytes(usb.read(2), byteorder='big', signed=False))

# =====[ Process data ]=====
def is_useful(pixel):
    # TODO: get start,stop from map
    return 300<=pixel<=392

# =====[ Plot data ]=====
def spectra(usb, npixels):
    """FuncAnimation sends this generator's yield as input to `update_plot`."""
    pixels = np.array([p+1 for p in range(npixels)])
    values = read_frame(usb, npixels)
    x = np.array([pixel for pixel in pixels if is_useful(pixel)])
    y = np.array([value for pixel,value in zip(pixels,values) if is_useful(pixel)])
    yield x,y

def update_plot(data, plot_line):
    """Gets `data` from `spectra` via FuncAnimation magic."""
    plot_line.set_data(data) # global spectrum_plot stores `plot`

if __name__ == '__main__':

    # This application works line by line at the REPL,
    # but executed from the script there are problems.

    serial_number = '064301'
    usb = open_spectrometer(serial_number)
    cfg_spectrometer(usb)
    cfg_spectrometer(usb)
    cfg_spectrometer(usb)
    # auto_expose(usb)
    # TODO: these work at the REPL
    # but four LEDs are all red when the code gets here
    # I think this is the fragile protocol problem.
    usb.close()
if 0:
    confirm_time = set_integration_time(usb,ms=50)
    fig = plt.figure()
    ax1 = fig.add_subplot(1,1,1)
    ax1.set_xlabel('Pixel Number')
    npixels = 392 # how do I pass this to `spectra`
    ax1.set_xlim(
        max(n for n in range(npixels+1) if is_useful(n)),
        min(n for n in range(npixels+1) if is_useful(n))
        )
    ax1.set_ylabel('Counts (a.u.)')
    ax1.set_ylim(0,30000)
    ax1.set_title('Spectrometer {} Raw Data'.format(serial_number))
    ax1.grid()
    spectrum_plot, = ax1.plot([],[],'bo--',linewidth=1,markersize=3) #globabl!
    # TODO: understand FuncAnimation better.
    # FuncAnimation forces me to use globals to give `spectra` access to `usb`
    # and to give `update_plot` access to `spectrum_plot`.
    animation.FuncAnimation(
        fig,
        update_plot(data=next(spectra(usb, npixels)),plot_line=spectrum_plot),
        spectra(usb=usb,npixels=npixels),
        interval=100
        )
    # TODO: once you start plotting, how do you control integration time?
    plt.show()
    # When plotting ends, how do I make sure usb.close is called?
    usb.close()


