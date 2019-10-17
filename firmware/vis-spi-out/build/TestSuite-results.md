
- Expected 0 calls:
- Received 3 calls:
    - "WriteSpiMaster", (uint8_t *)0000, (uint16_t)0x0001
    - "ReadSpiMaster", (uint8_t *)0x80, (uint16_t)0x0001
    - "WriteSpiMaster", (uint8_t *)0000, (uint16_t)0x0004
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value at address passed to call 1, arg 1? (uint8_t *)0000.
test/test_runner.c:51:GetSensorLED_sends_OK_to_Bridge_to_signal_ready_for_payload:PASS
Call 2 is named ReadSpiMaster? Call 2 is ReadSpiMaster.
Value passed to call 2, arg 2? (uint16_t)0x0001.
test/test_runner.c:52:GetSensorLED_reads_one_byte_of_payload:PASS

-----------------------
2 Tests 0 Failures 0 Ignored 
OK
