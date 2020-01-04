// ---Global Flags---
/** Clear flag before writing a frame.
 *  Set flag again after writing a frame.
 *
 *  *Purpose:* do not push bytes on the Queue while writing a
 *  frame to the SPI Master.
 *  - writing bytes over SPI triggers the SPI ISR
 *  - the interrupt flag synchronizes transmissions
 *  - running the interrupt clears the flag
 *  - but the value received in SPDR is garbage on a write
 *  - clear flag so that ISR does not push garbage byte onto
 *  the Queue
 *  - two benefits:
 *    - ISR execution time is cut in half by skipping
 *    QueuePush
 *    - eliminate QueuePop (to toss garbage byte) after each
 *    write
 * */
volatile bool listening_for_SPIM = true;

ISR(SPI_STC_vect)
{
    // Push data onto Queue when listening for SPI Master.
    if (listening_for_SPIM) QueuePush(SpiFifo, *Spi_SPDR);
}
