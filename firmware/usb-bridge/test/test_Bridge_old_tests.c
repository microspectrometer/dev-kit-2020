void GetBridgeLED_replies_msg_status_error_if_host_queries_nonexistent_led(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0+100};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Test case: msg_status is error because led number is not recognized. */
    status_byte msg_status = error;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t *)&msg_status));
}
void GetBridgeLED_replies_with_one_byte_if_led_number_is_not_recognized(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0+100};
    FakeByteArray_ForUsbReadBytes = payload;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* PrintAllCalls(mock); */
    /* =====[ Test ]===== */
    // Assert there are only two calls total.
    TEST_ASSERT_EQUAL_UINT8(2, NumberOfActualCalls(mock));
    // And assert that only the last call sends a byte.
    uint8_t call_n;
    call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadBytes"));
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
}
void GetBridgeLED_replies_msg_status_ok_if_host_queries_status_led(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Test case: LED is recognized, expect msg_status is ok */
    status_byte msg_status = ok;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t *)&msg_status));
}
void GetBridgeLED_replies_with_two_bytes_if_led_number_is_recognized(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* PrintAllCalls(mock); */
    /* =====[ Test ]===== */
    // Assert there are only three calls total.
    TEST_ASSERT_EQUAL_UINT8(3, NumberOfActualCalls(mock));
    // And assert that only the last two calls send bytes.
    uint8_t call_n;
    call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadBytes"));
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    call_n = 3;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
}
void GetBridgeLED_replies_with_msg_status_byte_and_led_status_byte(void)
{
    /* =====[ Test Case: BridgeLED is green ]===== */
    status_byte msg_status = ok; status_byte led_status = led_green;
    /* Inject value in fake hardware registers to set led state for test: */
    /*  - (BiColorLed_ddr,status_led) set: on, clear: off */
    /*  - (BiColorLed_port,status_led) set: red, clear: green */
    SetBit(BiColorLed_ddr,status_led); // led is on
    ClearBit(BiColorLed_port,status_led); // led is green
    /* Inject one byte of payload for fake UsbReadBytes. */
     /* - inject led_0 to indicate payload from host */
     /* - any other value is an error because status_led is the only Bridge LED */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2; arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock,call_n,"SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock,call_n,arg_n,(uint8_t*)&msg_status));
    call_n = 3; arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock,call_n,"SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock,call_n,arg_n,(uint8_t*)&led_status));
}
void GetBridgeLED_replies_led_off_if_status_led_is_off(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* =====[ Inject led_state ]===== */
    BiColorLedOff(status_led);
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 3; arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    status_byte led_status = led_off;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t*)&led_status));
}
void GetBridgeLED_replies_led_green_if_status_led_is_green(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* =====[ Inject led_state ]===== */
    BiColorLedOn(status_led); BiColorLedGreen(status_led);
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 3; arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    status_byte led_status = led_green;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t*)&led_status));
}
void GetBridgeLED_replies_led_red_if_status_led_is_red(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* =====[ Inject led_state ]===== */
    BiColorLedOn(status_led); BiColorLedRed(status_led);
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 3; arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    status_byte led_status = led_red;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t*)&led_status));
}

void BytesComing_returns_16bit_word_from_struct_spi_NBytesToExpect(void)
{
    /* =====[ Setup ]===== */
    BytesComing_s response_size;
    response_size.msb = 0x03;
    response_size.lsb = 0x10;
    /* =====[ Operate and Test ]===== */
    uint16_t expect = 0x0310;
    TEST_ASSERT_EQUAL_UINT16(expect, BytesComing(response_size));
}

void old_GetBridgeLED_always_replies_with_two_bytes(void)
{
    /* ---Test reply is two bytes when led_number is valid--- */
    /* =====[ Setup ]===== */
    // Inject one byte of payload for fake UsbReadBytes.
    uint8_t good_led_number = led_0;
    uint8_t payload[] = {good_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject LED state */
    BiColorLedOn(status_led);
    BiColorLedGreen(status_led);
    /* uint8_t led_state = led_green; */
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test: assert UsbReadBytes called to read 1 byte ]===== */
    printf("Valid LED number:\n");
    /* PrintAllCalls(mock); // view entire call history */
    uint8_t call_n;
    /* uint16_t arg_n; uint8_t arg_value; */
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    /* arg_n = 1; arg_value = ok; */
    /* TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value)); */
    call_n = 3;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    /* arg_n = 1; arg_value = led_state; */
    /* TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value)); */

    /* ---Test reply is two bytes when led_number is invalid--- */
    TearDown_Mock();
    SetUp_Mock();
    /* =====[ Setup ]===== */
    // Inject one byte of payload for fake UsbReadBytes.
    uint8_t bad_led_number = led_0+99;
    payload[0] = bad_led_number;
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject LED state */
    BiColorLedOn(status_led);
    BiColorLedGreen(status_led);
    /* uint8_t led_state = led_green; */
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test: assert UsbReadBytes called to read 1 byte ]===== */
    printf("Invalid LED number:\n");
    /* PrintAllCalls(mock); // view entire call history */
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    /* arg_n = 1; arg_value = ok; */
    /* TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value)); */
    call_n = 3;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    /* arg_n = 1; arg_value = led_state; */
    /* TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value)); */
}
