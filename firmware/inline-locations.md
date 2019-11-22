./lib/src/AvrAsmMacros.h:10:// ---2019-11-12 inline for optimal assembly---
./lib/src/BiColorLed.c:3:/* Declare inline functions to define their symbols in *this* translation unit. */
./lib/src/BiColorLed.h:15: * the `inline` call is inlined as a single `sbi` or `cbi` instruction.
./lib/src/BiColorLed.h:34:inline void BiColorLedGreen(uint8_t led_name)
./lib/src/BiColorLed.h:38:inline void BiColorLedRed(uint8_t led_name)
./lib/src/BiColorLed.h:42:inline void BiColorLedToggleColor(uint8_t led_name)
./lib/src/BiColorLed.h:46:inline void BiColorLedOff(uint8_t led_name)
./lib/src/BiColorLed.h:52:inline void BiColorLedOn(uint8_t led_name)
./lib/src/BiColorLed.h:61:inline bool BiColorLedIsOn(uint8_t led_name)
./lib/src/BiColorLed.h:67:inline bool BiColorLedIsRed(uint8_t led_name)
./lib/src/Lis.c:7:inline void OutputCfgByte(uint8_t const cfg_byte, uint8_t const nbits)
./lib/src/Lis.c:76:inline void EnterLisProgrammingMode(void)
./lib/src/Lis.c:84:inline void ExitLisProgrammingMode(void)
./lib/src/Lis.c:169:// ---2019-11-12 correct use of inline for optimized assembly code---
./lib/src/Lis.c:170:inline void LisWaitForClockFallingEdge(void)
./lib/src/Lis.c:174:inline void LisWaitForClockRisingEdge(void)
./lib/src/Lis.c:178:inline void LisStartExposure(void)
./lib/src/Lis.c:182:inline void LisStopExposure(void)
./lib/src/Lis.c:186:inline void LisWaitForReadoutToStart(void)
./lib/src/Lis.h:66:// ---2019-11-12 correct use of inline for optimized assembly code---
./lib/src/Pwm.c:44:// inline function in .c is OK because application does #include <Pwm.c>
./lib/src/Pwm.c:45:inline void WaitForPwmFallingEdge(void)
./lib/src/Pwm.c:51:inline void WaitForPwmRisingEdge(void)
./lib/src/Pwm.h:57:// 2019-11-12 correct use of inline for optimal assembly
./lib/src/ReadWriteBits.c:3:/* Declare inline functions here to emit symbols in this translation unit. */
./lib/src/ReadWriteBits.h:15:/* | 2019-03-04 WIP: inline version of ReadWriteBits | */
./lib/src/ReadWriteBits.h:18:inline void SetBit(uint8_t volatile * const port, uint8_t const bit)
./lib/src/ReadWriteBits.h:22:inline void ClearBit(uint8_t volatile * const port, uint8_t const bit)
./lib/src/ReadWriteBits.h:26:inline void ToggleBit(uint8_t volatile * const port, uint8_t const bit)
./lib/src/ReadWriteBits.h:30:inline bool BitIsSet(uint8_t volatile * const port, uint8_t const bit)
./lib/src/ReadWriteBits.h:34:inline bool BitIsClear(uint8_t volatile * const port, uint8_t const bit)
./lib/src/Spi.c:9:/* | 2019-03-04 WIP: inline version of SpiMasterWrite | */
./lib/src/Spi.h:155:/* | 2019-03-04 WIP: inline version of SpiMasterWrite | */
./lib/src/Spi.h:157:inline void WaitUntilSpiWriteIsDone(void)
./lib/src/Spi.h:163:     * Expect this is inlined. Expect this compiles as:
./lib/src/Spi.h:175:inline void OpenSpiSlave(void)
./lib/src/Spi.h:178:    /* Expect `ClearBit` is compiled inline as a single `cbi` instruction. */
./lib/src/Spi.h:181:inline void CloseSpiSlave(void)
./lib/src/Spi.h:184:    /* Expect `SetBit` is compiled inline as a single `sbi` instruction. */
./lib/src/Spi.h:187:inline void SpiLaunchByte(uint8_t const byte)
./lib/src/Spi.h:193:inline uint8_t SpiMasterReadByte(void)
./lib/src/Spi.h:200:    /* TODO: inline this */
./lib/src/Spi.h:204:inline void SpiMasterWriteByte(uint8_t const byte)
./lib/src/Spi.h:228:inline void SpiMasterWriteN_NoInlineHelpers(uint8_t const * bytes, uint8_t const nbytes)
./lib/src/Spi.h:243:        ClearBit(Spi_port, Spi_Ss); // inline
./lib/src/Spi.h:248:        /* TODO: inline this */
./lib/src/Spi.h:251:        /* TODO: inline this */
./lib/src/Spi.h:254:        SetBit(Spi_port, Spi_Ss); // inline
./lib/src/Spi.h:261:inline bool SpiSlaveShowsDataReady(void)
./lib/src/Spi.h:269:inline bool IsSpiSlaveReadyToSend(void)
./lib/src/Spi.h:274:inline void SpiMasterWaitForSlaveReady(void)
./lib/src/Spi.h:278:inline void SpiMasterWaitForSlaveReadyReset(void)
./lib/src/Spi.h:283:inline void FourWire_SpiMasterWaitForSlaveReady(void)
./lib/src/UartSpi.c:23:inline void StartAdcReadout(void)
./lib/src/UartSpi.c:37:inline void StartAdcConversion(void)
./lib/src/UartSpi.c:92:inline void WaitForEmptyTxBuffer(void)
./lib/src/UartSpi.c:104:inline void WaitForByteFromAdc(void)
./lib/src/UartSpi.c:128:inline void WaitForConversion(void)
./lib/src/UartSpi.c:130:    Delay3ClocksPerTick(16); // temporary inline replacement
./lib/src/UartSpi.c:138:    // inline delay compiles to this:
./lib/src/UartSpi.h:68:// ---2019-11-12 inline for optimal assembly---
./lib/test/fake/AvrAsmMacros.c:2:// ---2019-11-12 fake inline functions that call AVR asm macros ---
./lib/test/test_ReadWriteBits.c:58:/* =====[ Old: tests of Macro versions from before I knew how to inline. ]===== */
./lib/test/test_Spi.h:71:/* 2019-03-04 WIP: inline version of SpiMasterWrite */
./usb-bridge/src/Bridge-old-stuff.c:17:uint16_t BytesComing(BytesComing_s reply_size); // inline defined in .h
./usb-bridge/src/Bridge-old-stuff.h:8:inline uint16_t BytesComing(BytesComing_s reply_size)
./vis-spi-out/src/AvrAsmMacros.c:19:inline void Delay3ClocksPerTick(uint8_t ticks)
./vis-spi-out/src/SensorVis.c:31:// Definition needed in SensorVis.h for inline function.
./vis-spi-out/src/SensorVis.c:47:/* Declare inline functions here to emit symbols in this translation unit. */
./vis-spi-out/src/SensorVis.c:48:// See definition of these inline functions in SensorVis.h
./vis-spi-out/src/SensorVis.c:222:{ // TODO: clean this up using optimized inline functions
./vis-spi-out/src/SensorVis.h:11:#include "ReadWriteBits.h" // make inline functions visible to this translation unit
./vis-spi-out/src/SensorVis.h:26:// value is needed by inline definition in this file
./vis-spi-out/src/SensorVis.h:44:inline uint16_t NumPixelsInFrame(void)
./vis-spi-out/src/SensorVis.h:56:inline void WordToTwoByteArray(uint16_t word, uint8_t * parray)
./vis-spi-out/src/SensorVis.h:65:inline void ExposePhotodiodeArray(void)
./vis-spi-out/src/vis-spi-out.c:115:    // Calling any non-inline functions in an ISR requires stacking many
