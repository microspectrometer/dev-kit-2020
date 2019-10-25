{ // GetSensorLED no longer uses ReadSpiMaster() because SPI reads happen in ISR
    /* =====[ Mock ReadSpiMaster() ]===== */
    static RecordedCall * Record_ReadSpiMaster(uint8_t *arg1, uint16_t arg2)
    { // Define call recorded when func-under-test calls mocked function.
        char const *call_name = "ReadSpiMaster";
        RecordedCall *record_of_this_call = RecordedCall_new(call_name);
        RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
        *((uint8_t **)record_of_arg1->pArg) = arg1;
        RecordedArg *record_of_arg2 = RecordedArg_new(SetupRecord_uint16_t);
        *((uint16_t *)record_of_arg2->pArg) = arg2;
        // Store the arg records in the call record.
        RecordArg(record_of_this_call, record_of_arg1);
        RecordArg(record_of_this_call, record_of_arg2);
        return record_of_this_call;
    }
    uint8_t *FakeByteArray_ForReadSpiMaster;
    static uint16_t ReadSpiMaster_Mocked(uint8_t *read_buffer, uint16_t nbytes)
    { // Define behavior of mocked function: ReadSpiMaster().
        RecordActualCall(mock, Record_ReadSpiMaster(read_buffer, nbytes));
        // Fake reading an array of bytes into the read_buffer.
        // The test is responsible to populate FakeByteArray_ForReadSpiMaster.
        uint16_t num_bytes_read = 0;
        while ( num_bytes_read++ < nbytes )
        {
            *(read_buffer++)  = *(FakeByteArray_ForReadSpiMaster++);
        }
        return nbytes;
    }
    /* =====[ Define how to swap function definitions ]===== */
    static uint16_t (*ReadSpiMaster_Saved)(uint8_t *, uint16_t);
    static void Restore_ReadSpiMaster(void)
    { // how to restore real definition
        ReadSpiMaster = ReadSpiMaster_Saved;
    }
    static void Mock_ReadSpiMaster(void)
    { // how to swap real definition with mocked version
        ReadSpiMaster_Saved = ReadSpiMaster;
        ReadSpiMaster = ReadSpiMaster_Mocked;
    }
}
void old_GetSensorLED_sends_OK_to_Bridge_to_signal_ready_for_payload(void)
{
    // TODO: this test is obsolete!
    // First thing test needs is to stub QueueIsEmpty to return false when it is
    // called.
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Test ]===== */
    PrintAllCalls(mock);
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect 1st call is 'WriteSpiMaster'."
        );
    uint8_t arg_n = 1; uint8_t status = ok; uint8_t *p_status=&status;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArgPointsToValue(mock, call_n, arg_n, &p_status),
        "Expect WriteSpiMaster sends status byte OK."
        );
}
void old_GetSensorLED_receives_led_number(void)
{
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSpiMaster"),
        "Expect call 2 is ReadSpiMaster."
        );
    arg_n = 2; uint16_t num_bytes = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &num_bytes),
        "Expect payload is 1 byte."
        );
}
void old_GetSensorLED_replies_with_three_bytes_if_led_is_non_existent(void)
{
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx+100};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 2 is WriteSpiMaster."
        );
    arg_n = 2; uint16_t num_bytes = 3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &num_bytes),
        "Expect reply is 3 bytes: 2 bytes of size plus 1 byte actual reply."
        );
}
void old_GetSensorLED_replies_with_four_bytes_if_led_number_is_recognized(void)
{
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 2 is WriteSpiMaster."
        );
    arg_n = 2; uint16_t num_bytes = 4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &num_bytes),
        "Expect reply is 4 bytes: 2 bytes of size plus 2 bytes actual reply."
        );
}
void old_GetSensorLED_replies_led_off_if_led_is_off(void)
{
    /* Inject LED state: off */
    BiColorLedOff(led_TxRx);
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Make sure there are four bytes to read before running test. ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    arg_n = 2; uint16_t num_bytes = 4;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &num_bytes));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[3] == %d\n", SpyOn_WriteSpiMaster_arg1[3]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_off, SpyOn_WriteSpiMaster_arg1[3],
        "Expect led_status byte is 0x00 (led_off) because test turns off LED TxRx."
        );
}
void old_GetSensorLED_replies_led_green_if_led_is_green(void)
{
    /* Inject LED state: off */
    BiColorLedOn(led_TxRx); BiColorLedGreen(led_TxRx);
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Make sure there are four bytes to read before running test. ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    arg_n = 2; uint16_t num_bytes = 4;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &num_bytes));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[3] == %d\n", SpyOn_WriteSpiMaster_arg1[3]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_green, SpyOn_WriteSpiMaster_arg1[3],
        "Expect led_status byte is 0x01 (led_green) because test turns LED TxRx green."
        );
}
void old_GetSensorLED_replies_led_red_if_led_is_red(void)
{
    /* Inject LED state: off */
    BiColorLedOn(led_TxRx); BiColorLedRed(led_TxRx);
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Make sure there are four bytes to read before running test. ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    arg_n = 2; uint16_t num_bytes = 4;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &num_bytes));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[3] == %d\n", SpyOn_WriteSpiMaster_arg1[3]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_red, SpyOn_WriteSpiMaster_arg1[3],
        "Expect led_status byte is 0x02 (led_red) because test turns LED TxRx red."
        );
}

/* Pull useful stuff from here: */
/* =====[ Jump Table Sandbox ]===== */
/* Functions of type `spi_Cmd` take nothing and return nothing. */
void LookupSensorCmd_returns_Nth_fn_for_Nth_key(void){

    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL(GetSensorLED, LookupSensorCmd(GetSensorLED_key));
}
static sensor_cmd_key const spi_BlackHat_key = 255; // out-of-bounds: return NULL ptr
void LookupSensorCmd_returns_NULL_if_key_is_not_in_jump_table(void){
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_NULL(LookupSensorCmd(spi_BlackHat_key));
}
void LookupSensorCmd_example_calling_the_returned_command(void)
{
    /* TEST_IGNORE_MESSAGE("I do not understand this test."); */
    /* See mocking in SpiSlaveSendBytes_waits_for_master_read_between_each_byte */
    /* Three callers in SpiSlaveSendBytes are mocked up in the test setup. */
    /* 1. Stub_SpiTransferIsDone: check if SPI interrupt flag is set. */
    /*     Fake the SPIF values returned. The test hangs if SPIF is never set! */
    bool SPIFs[] = {true, true, true};
    SpiTransferIsDone_StubbedReturnValue = SPIFs;

    /* 2. WriteSpiDataRegister: *Spi_spdr = byte_to_write; */
    /*     Spy by setting expectations on the values sent. */
    uint8_t Spi_spdr_write_log[sizeof(SPIFs)];
    WriteSpiDataRegister_WriteLog = Spi_spdr_write_log;

    /* 3. SpiSlaveSignalDataIsReady: disable/enable SPI to spike MISO low. */
    /*     Not important for testing. */
    /*     But it is mocked out, so I have to set an expectation that it is called. */

    /* =====[ Mock-up test scenario ]===== */
    /* Context: */
    /*     ---SpiSlave sends StatusOk byte after turning the LEDs red.--- */
    /* SpiSlaveWrite_StatusOK calls SpiSlaveSendBytes to send three bytes. */
    uint8_t const StatusOk[] = {
        0x00, 0x01, // nybtes [msb lsb]
        0x00        // data (0x00 means StatusOk)
    };
    uint8_t const * bytes = StatusOk; uint8_t nbytes = 3;
    /* Make sure the test fakes one SPIF value for each byte sent. */
    TEST_ASSERT_EQUAL(sizeof(SPIFs), nbytes);

    /* =====[ Record the expectations ]===== */
    while(nbytes-- != 0)
    {
        /* Set expectation for value written to the SPI data register. */
        Expect_WriteSpiDataRegister(*(bytes++));
        /* Set expectation that SpiSlave signals *Data-Ready* to the SpiMaster. */
        Expect_SpiSlaveSignalDataIsReady();
        /* Set expectation SpiSlave checks the SPIF. */
        Expect_SpiTransferIsDone();
    }

    /* =====[ Setup ]===== */
    /* Fake that led_TxRx pin is red. */
    *BiColorLed_port = 0xFF; // HIGH is red
    /* =====[ Operate ]===== */
    /* Note the parentheses to make it a function call */
    LookupSensorCmd(GetSensorLED_key)(); // call the function returned by lookup
    /* ------------------------------- */
    //=====[ Test ]=====
    // --- 2019-09-30: What am I trying to test here? Does the LED change?
    TEST_ASSERT_BIT_LOW(led_TxRx, *BiColorLed_port); // LOW is green
    // ---
    /* =====[ Spy on values sent from SpiSlave to SpiMaster ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void SpiSlaveWrite_StatusOk_sends_0x00_0x02_0x00_valid_cmd(void)
{
    TEST_IGNORE_MESSAGE("Move test to lib `Sensor`.");
    /* Use SetUp_SpiSlaveSendBytes */
    /* Three functions in SpiSlaveSendBytes are mocked out in SetUp. */
    /* 1. Stub_SpiTransferIsDone: check if SPI interrupt flag is set. */
    /*     Fake the SPIF values returned. The test hangs if SPIF is never set! */
    bool SPIFs[] = {true, true, true};
    SpiTransferIsDone_StubbedReturnValue = SPIFs;

    /* 2. WriteSpiDataRegister: *Spi_spdr = byte_to_write; */
    /*     Spy by setting expectations on the values sent. */
    uint8_t Spi_spdr_write_log[sizeof(SPIFs)];
    WriteSpiDataRegister_WriteLog = Spi_spdr_write_log;

    /* 3. SpiSlaveSignalDataIsReady: disable/enable SPI to spike MISO low. */
    /*     Not important for testing. */
    /*     But it is mocked out, so I have to set an expectation that it is called. */

    /* =====[ Mock-up test scenario ]===== */
    /* Context: */
    /*     ---SpiSlave sends StatusOk byte after turning the LEDs red.--- */
    /* SpiSlaveWrite_StatusOK calls SpiSlaveSendBytes to send three bytes. */
    sensor_cmd_key valid_cmd = 0x00;
    uint8_t const StatusOk[] = {
        0x00, 0x02, // nybtes [msb lsb]
        0x00, valid_cmd  // data (0x00 means StatusOk)
    };
    uint8_t const * bytes = StatusOk; uint8_t nbytes = 4;
    /* Make sure the test fakes one SPIF value for each byte sent. */
    TEST_ASSERT_EQUAL(sizeof(SPIFs), nbytes);

    /* =====[ Record the expectations ]===== */
    while(nbytes-- != 0)
    {
        /* Set expectation for value written to the SPI data register. */
        Expect_WriteSpiDataRegister(*(bytes++));
        /* Set expectation that SpiSlave signals *Data-Ready* to the SpiMaster. */
        Expect_SpiSlaveSignalDataIsReady();
        /* Set expectation SpiSlave checks the SPIF. */
        Expect_SpiTransferIsDone();
    }

    /* =====[ Operate ]===== */
    SpiSlaveWrite_StatusOk(valid_cmd);

    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

void SpiSlaveWrite_StatusInvalid_sends_0x00_0x02_0xFF_invalid_cmd_name(void)
{
    /* Use SetUp_SpiSlaveSendBytes */
    /* Three functions in SpiSlaveSendBytes are mocked out in SetUp. */
    /* 1. Stub_SpiTransferIsDone: check if SPI interrupt flag is set. */
    /*     Fake the SPIF values returned. The test hangs if SPIF is never set! */
    bool SPIFs[] = {true, true, true, true};
    SpiTransferIsDone_StubbedReturnValue = SPIFs;

    /* 2. WriteSpiDataRegister: *Spi_spdr = byte_to_write; */
    /*     Spy by setting expectations on the values sent. */
    uint8_t Spi_spdr_write_log[sizeof(SPIFs)];
    WriteSpiDataRegister_WriteLog = Spi_spdr_write_log;

    /* 3. SpiSlaveSignalDataIsReady: disable/enable SPI to spike MISO low. */
    /*     Not important for testing. */
    /*     But it is mocked out, so I have to set an expectation that it is called. */

    /* =====[ Mock-up test scenario ]===== */
    /* Context: */
    /*     ---SpiSlave sends StatusOk byte after turning the LEDs red.--- */
    /* SpiSlaveWrite_StatusOK calls SpiSlaveSendBytes to send three bytes. */
    uint8_t const StatusInvalid[] = {
        0x00, 0x02,             // nybtes [msb lsb]
        0xFF, spi_BlackHat_key  // data (0xFF means StatusInvalid)
    };
    uint8_t const * bytes = StatusInvalid; uint8_t nbytes = sizeof(StatusInvalid);
    TEST_ASSERT_EQUAL_MESSAGE(
        sizeof(SPIFs),
        nbytes,
        "Make sure the test fakes one SPIF value as true for each byte sent."
        );

    /* =====[ Record the expectations ]===== */
    while(nbytes-- != 0)
    {
        /* Set expectation for value written to the SPI data register. */
        Expect_WriteSpiDataRegister(*(bytes++));
        /* Set expectation that SpiSlave signals *Data-Ready* to the SpiMaster. */
        Expect_SpiSlaveSignalDataIsReady();
        /* Set expectation SpiSlave checks the SPIF. */
        Expect_SpiTransferIsDone();
    }

    /* =====[ Operate ]===== */
    SpiSlaveWrite_StatusInvalid(spi_BlackHat_key);

    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}



