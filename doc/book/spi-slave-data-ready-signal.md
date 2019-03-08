% Implement a SPI Slave *Data Ready* Signal On a 4-Wire SPI Bus
% Mike Gazes
% March 4th, 2019

# Pain Point

Signaling when the slave is ready to send data is the pain point of SPI
communication in the general case where the SPI slave does *not* collect
data synchronized to the SPI clock.

An example where the SPI slave *is* synchronized is a SPI-interface SAR
ADC. The ADC uses the SPI clock as the SAR clock. The SPI master knows
*exactly* when the data is ready because the SPI master controls the
generation of the data. Instead of the usual *eight clocks is a byte of
data* protocol, the SPI master clocks the ADC as much as the ADC
requires to do a conversion, ignoring the data that is coming out, and
then it clocks another 16 times to read out the converted value.

But in the general case, when the SPI master asks for data it has no
control over when the data is ready. So how does the SPI master know
when to start readout?

# Solutions

## The 5-wire SPI Bus solution

The usual solution in a single-master SPI system is to use an additional
general purpose I/O pin on the SPI slave that drives an additional
general purpose I/O pin on the SPI master. Thus the four-wire SPI bus
becomes a five-wire SPI bus: MOSI, MISO, SCK, SS, DR (data ready).

But an additional I/O pin is not an option if the SPI master and SPI
slave do not have enough general purpose I/O pins. Even if pin count is
not a problem, using additional I/O pins is undesirable if the SPI
master and SPI slave are on different circuit boards because this
requires an extra dedicated signal wire between the two boards.

## Four-wire solution: if Slave Select pins are tied together, use MISO.

The eval kit hardware *does* tie the Slave select of the SPI slave to the slave
select of the SPI master. Therefore, I cannot use the Slave Select pin on the
SPI slave as my *Data Ready* signal. The SPI master drives this pin. If the SPI
master reconfigures it as an input inbetween transmissions, then when the SPI
slave drives the pin low, it will automatically switch the SPI master to a SPI
slave. This is undesirable because it prevents me from adding other SPI slaves
to the eval kit hardware, such as an LED driver.

Instead, use the MISO pin as the *Data Ready* signal. The SPI hardware
module forces MISO to be an input on the SPI master. But MISO can be an
input or an output on the SPI slave. Be aware that even as an output,
MISO is tri-stated on the SPI slave unless there is a tranmission.

The SPI master transmits a request for data to the SPI slave and then
waits for the slave to send the data ready signal. The SPI master
watches MISO, either by polling it, or by disabling SPI functionality
and enabling the pin-change interrupt on the MISO pin.

The SPI slave signals data is ready by disabling SPI, then spiking MISO
low by driving it low (MISO is already configured as an output), and
then re-enabling SPI. This tri-states MISO and the pin slowly rises back
up. Alternatively, driving MISO high again before re-enabling SPI and
the waveform becomes a clean step.

The SPI master sees MISO go low and knows the data is ready. The SPI
master then watches for MISO to go high again to know for sure that the
SPI slave has re-enabled its SPI module and is ready for a transmission.

## The Slave Select pin in multi-master SPI systems.

Usually the Slave Select pin on the SPI master is configured as an output. It is
disconnected from the SPI hardware module and behaves like a general purpose I/O
pin.

In multi-master systems, the data direction of the SPI master is configured as
an input. This avoids contention on the slave select pin.  As an input, it is
normally high via an external pull-up. Either SPI master can temporarily
configure its slave select as an output and driver the slave select line low.

When one master switches direction to output and drives the pin low, the other
master is automatically changed to a SPI slave. After communication is over, the
SPI slave manually changes itself back to a SPI master. If the other master
wasn't finished communicating, Slave Select is still low (or a synchronizing
pulse pulls it low again) and the SPI master is automatically knocked back into
being a SPI slave.

Multi-master SPI systems are neat. They can be used to implement a
fault-tolerant distributed network of concurrent processes by sharing a common
SPI bus.

## Repurposing the automatic switch from Master-to-Slave

Repurpose the functionality that enables multi-master SPI to give a
single-master SPI network a super power: a way for the SPI slave to
signal to the SPI master that it is ready to send data.

## Four-wire solution: do not connect to Slave Select pin on SPI master.

Rethink this later: when the SPI slave temporarily becomes a SPI master, is
their a possibility of bus contention?

There is another four-wire SPI bus solution besides using MISO.

Do not connect the SPI Slave Select pins of the SPI master and SPI slave. I have
always connected these pins because they have the same name, so connecting them
*seemed* like the intent. It is the intent for a multi-master system. It is not
the intent for a single-master system.

Instead, connect the Slave Select pin on the SPI Slave to a general purpose I/O
pin on the SPI master. Place an external pull-up resistor on this pin so that it
is normally high, independent of what the SPI master does. The SPI master makes
this general purpose pin I/O pin an output and drives it low to transmit to the
SPI slave.

To recap: use Slave Select on the SPI slave but do not use Slave Select on the
SPI Master pin. Or use it as an output pin for some other purpose. But do not
use it as an input (or else a low on this pin causes the SPI module switches
from master to slave). When waiting for a response from the SPI slave, the SPI
master drives the pin high, then makes it an input and watches for a low on this
pin, either by polling the pin or using an interrupt.

The general purpose I/O on the SPI Master that connects to the Slave Select on
he SPI slave is the dedicated *Data Ready* pin for this particular SPI slave.
The SPI slave pulls the pin low when it is ready to send data.

Since the SPI hardware forces the data direction of Slave Select to be an input,
the SPI slave must first change itself into a SPI master, then change the data
direction of this pin. The idea is the same as using MISO to signal *Data
Ready*.

The SPI slave (now temporarily a SPI master) spikes the pin low by
changing its data direction to output, and then back to input.

The SPI slave should hold the pin low for at least a few clock cycles of
the SPI master's CPU clock to make sure the SPI master *hears* it. There
is no danger in holding the pin low for longer.

The SPI slave can change itself back to a slave, or it can leave itself
as a master. If it is still a master, it will be automatically changed
back to a SPI slave when the *true* SPI master pulls Slave Select low.

The low on *Data Ready* is the signal for the SPI master that the SPI
slave is ready. The SPI master then waits for this general purpose I/O
pin to go high again before beginning the SPI transmission.

# SPI Slave Function to signal data is ready

## This function uses the following strategy to overcome this problem.

1. Disable the SPI module to restore general purpose I/O functionality.

2. Pin `Spi_Miso` is driven low because the PORT register has a 0 in this
bit. This is a sudden voltage step from high to low.

3. Enable the SPI module in preparation to receive a transmission from
the SPI master. This is a slow voltage rise from low to high because the
the capacitance on the SPI bus is charged by the small current available
from the power supply via the pull-up resistor. Optionally, drive the
pin high before re-enabling SPI. This is faster, but now the SPI master
must add an extra small delay after seeing the pin go high to be sure
the SPI slave is ready.

## My first attempt to explain why the waveform is this way.

The SpiSlave uses `Spi_Miso` to signal `data_ready` to the SpiMaster.
This requests the SpiMaster to start a SPI transmission.

But the SpiSlave cannot drive pin `Spi_Miso` outside of a transmission
while the ATmega SPI module is enabled. When there is no transmission,
`Spi_Miso` is weakly pulled high via a resistor. When the ATmega SPI
module is enabled in SPI Slave mode, the MISO pin is driven by the SPI
hardware module instead of the general purpose PORT register.

# Protocol

## Intent for the SpiMaster.

Every time the SpiMaster sends a message to the SpiSlave, the SpiSlave
responds. The message from the SpiMaster may be multiple bytes. The
first byte is the `spi_CmdFn_key` byte. The SpiSlave looks up the key in
its jump-table and finds out how many more bytes, if any, to expect from
the SpiMaster. After receiving all bytes, the SpiSlave executes the
command and signals to the SpiMaster on `Spi_Miso` that it is ready to
respond.

The signal is that `Spi_Miso` is driven low. SpiMaster then waits for
`Spi_Miso` to go high before it starts the transmission.

The SpiMaster determines the number of bytes in the response from the
the first two bytes sent by the SpiSlave. These two bytes form a 16-bit
word, most-significant byte first. Therefore, the SpiMaster always knows
how many bytes to read from the SpiSlave.

This is necessary because the SpiSlave might have had an error. For
example, if the SpiMaster sends an invalid command. In this case, the
SpiSlave responds with two bytes: [StatusInvalid, `invalid_cmd`]. If the
SpiMaster thought it was requesting a frame data, it would expect 784
bytes. Instead, the SpiSlave sends [0x00, 0x02] indicating that only two
bytes are coming back: [StatusInvalid, `invalid_cmd`].

Note that even knowing how many bytes to expect, the SpiSlave sends the
*Data-Ready* signal between every byte. This is because the SPI data register is
not double-buffered in the transmit direction. The SpiSlave cannot, therefore,
load the next byte until the transmission is finished. The SpiMaster must wait
to clock the SpiSlave for the start of the next byte until it receives the
*Data-Ready* signal.

The SpiMaster stops checking for *Data-Ready* after it receives all of the bytes
the SpiSlave said it was sending.

After the SpiMaster sends a message, it must wait until the SpiSlave
signals that a response is ready. The SpiMaster polls MISO
continuously until MISO goes low. Alternatively, the SpiMaster can
disable its SPI hardware module and enable the pin change interrupt
PCINT4. In this case, the SpiMaster re-enables SPI in the interrupt
routine.

The SpiMaster can carry out any other task while the SpiSlave
is busy and then read from the SpiSlave at its leisure. The only
exception is that the SpiMaster *should not* communicate with other SPI
slaves while the SpiSlave is busy because this opens the possibility of
bus contention. The SpiSlave can share the SPI bus with other SPI slaves
as long as the SpiMaster does not attempt communication with the other
SPI slaves after it sends a message to the SpiSlave.

The low on MISO signals that the SpiSlave is ready. The SpiMaster
proceeds with readout. Based on the first two bytes received, the
SpiMaster knows how many bytes remain for readout and continues SPI
transmissions until all bytes are collected. The SpiMaster waits for
MISO low on every byte.

## Control over the timeout for when SpiSlave hangs.

There is one flaw in this strategy. If the SpiSlave hangs after
receiving the message from the SpiMaster, the SpiMaster also hangs
because it is waiting for the SpiSlave to respond.

Less likely, but also a possibility, is getting out of sync on the
number of bytes sent during a long transmission:

1. If the SpiMaster thinks there are still more bytes coming, but the
SpiSlave has sent all the bytes, the SpiSlave will stop signaling
*Data-Ready* and the SpiMaster will hang waiting for *Data-Ready*.

2. If the SpiMaster thinks it already received all the bytes, but the
SpiSlave still has bytes to send, the SpiSlave will still be in a state
where it is waiting for the SpiMaster to do another SPI transmission.
The SpiSlave will not be listening for new communications from the
SpiMaster.

For these two unlikely cases described above, the simple solution is to
wait a more-than-reasonable amount of time and then give up on the
transmission.

In case 1, the SpiMaster tells the UsbHost that the request timed out.
It is up to the UsbHost to resend the command.

In case 2, the SpiSlave just returns to the main loop and does not try
to notify the SpiMaster of an error. For a nasty *silent* error like
this, turn three out of the four Debug LEDs red. This indicates
a silent error occurred and the system just went on about its business.

Back to the more likely scenario that the SpiSlave is taking too long in
attempting to carry out a task requested by the SpiMaster.

The SpiMaster cannot simply timeout and give up on the SpiSlave because
the response may still come at some future time, in which case the
SpiSlave will pull MISO low when the master does not expect it. This is
OK, unless it happens while the SpiMaster is communicating with another
SpiSlave. In this case, there is the possibility for bus contention.

Even without other SPI slaves, this case needs to be dealt with anyway
because now the SpiSlave is hung and the SpiMaster cannot talk to it!
The SpiSlave has a timer (to be implemented), but its timeout is
relatively long. The following solution gives the SpiMaster control over
the timeout period. The SpiMaster needs the power to decide when to
timeout on the unresponsive SpiSlave.

The solution is that after timeout, the SpiMaster sends a new message to
the SpiSlave telling it to abandon whatever it's doing. For this to
work, the SpiSlave must be checking for this message while it works.

Therefore, if a `spi_CmdFn` in the SpiSlave jump-table has a while loop,
it must poll the SPI interrupt flag on every iteration. If the SpiMaster
sent 0xFF, the SpiSlave abandons its task by doing the following:

1. Perform any necessary clean-up prior to abandoning its task. For
example, if it is in the middle of a frame readout, put the LIS-770i
back in its idle state.

2. Abandon the task by returning an error all the way up the call chain
to get back to the `spi_CmdFn`.

To return to the `spi_CmdFn`, each function call in the path returns an
error status. When the hung task abandons, it returns an error. It's
caller uses that error to return as an error, and so on. Eventually,
execution returns to the function in the jump table.

The `spi_CmdFn` sees the task was abandoned and prepares to send [0x00,
0x02] indicating that two more bytes are coming. The two bytes are
[StatusTaskKilled, cmd]. The SpiMaster is expecting this since it killed
the task. The SpiMaster has to read out this status signal for execution
in the SpiSlave to return to its main loop.

3. Drive MISO low as usual to signal data-ready. In this context, the
SpiMaster already knows what is waiting for it, but it has to read it
out anyway. The SpiSlave reports that the task was killed, and it
reports which `spi_CmdFn` it was executing when the command was killed.

As with the usual data-ready signal, disable SPI, output a low on MISO,
re-enable SPI. This results in a brief spike low, followed by a slow
rise via the pull-up.


## Causes of SpiSlave timeout and the expected response by the UsbHost.

The most likely cause for the timeout is that the SpiSlave takes too
long to AutoExpose. In this case, after sending 0xFF, when the SpiMaster
receives the signal that the SpiSlave has abandoned task, the SpiMaster
sends a StatusTimedOut error to the UsbHost. Normally, auto-expose
results in the SpiMaster sending back the updated integration time. In
this case, the task was abandoned, so the integration time on the
SpiSlave is unknown.

The UsbHost reponds by sending a new command to set the integration
time. The SpiMaster responds with the updated integration time on the
SpiSlave, and the system is once again in sync.


