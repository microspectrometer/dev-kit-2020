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
void old_BridgeGetSensorLED_sends_Sensor_led_number_if_Sensor_responds_ok_to_command(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {ok, 0x00, 0x01, error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n = 6;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SpiWriteByte"));
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, payload));
}
void old_BridgeGetSensorLED_reads_two_bytes_of_reply_size_and_nbytes_of_reply_from_Sensor(void)
{
    /* Inject led_number as one byte of payload for fake UsbReadBytes. */
    uint8_t led_number = led_0+100;
    uint8_t payload[] = {led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {ok, 0x00, 0x01, error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n; uint16_t sensor_reply_size;
    call_n = 7;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 7 is ReadSensor (get Sensor reply size)."
        );
    arg_n = 2; sensor_reply_size=2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_reply_size),
        "Expect first call to ReadSensor reads two bytes of reply_size."
        );
    call_n++;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 8 is ReadSensor."
        );
    arg_n = 2; sensor_reply_size=1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_reply_size),
        "Expect GetSensorLED only reads one byte of data. This test simulates when Sensor responds with error: invalid command parameter."
        );
}
void old_BridgeGetSensorLED_passes_reply_size_and_reply_to_host(void)
{
    /* Inject led_number as one byte of payload for fake UsbReadBytes. */
    uint8_t led_number = led_0;
    uint8_t payload[] = {led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {ok, 0x00, 0x02, ok, led_red};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n; uint16_t byte_index;
    call_n = 9;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 9 is SerialWriteByte."
        );
    arg_n = 1; byte_index = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[byte_index]),
        "Expect MSB of reply_size is sensor_responses[1] (==0)."
        );
    call_n++;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 10 is SerialWriteByte."
        );
    arg_n = 1; byte_index++;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[byte_index]),
        "Expect LSB of reply_size is sensor_responses[2] (==2)."
        );
    call_n++;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 11 is SerialWriteByte."
        );
    arg_n = 1; byte_index++;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[byte_index]),
        "Expect Sensor msg status is sensor_responses[3] (ok)."
        );
    call_n++;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 12 is SerialWriteByte."
        );
    arg_n = 1; byte_index++;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[byte_index]),
        "Expect Sensor led status is sensor_responses[4] (led_red)."
        );
}
void old_BridgeGetSensorLED_sends_msg_status_ok_if_it_receives_a_reply_from_the_Sensor(void)
{
    /* Inject led_number as one byte of payload for fake UsbReadBytes. */
    uint8_t led_number = led_0;
    uint8_t payload[] = {led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {0x00, 0x02, error, led_red};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 5;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 5 is SerialWriteByte."
        );
    arg_n = 1; uint8_t msg_status = ok;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &msg_status),
        "Expect msg_status is ok (==0)."
        );
}
void old_BridgeGetSensorLED_passes_cmd_to_Sensor_and_waits_for_response(void)
{
    /* I'm hanging onto this old test to show future me how to do something like this. */

    /* GetSensorLED calls WriteSensor to pass information to the Sensor. */
    /* The first argument is an array of bytes to send. */
    /* This test spies on the array via `SpyOn_WriteSensor_arg1`. */
    /* `cmd` is first byte in array. */
    /*     This byte is hardcoded in GetSensorLed as `GetSensorLED_key`. */
    /* `led_number` is second byte in array. */
    /*     The LED number is injected by this test. */
    /* Inject led_number as one byte of payload for fake UsbReadBytes. */
    uint8_t led_number = led_0;
    uint8_t payload[] = {led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error, led_red};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint16_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSensor"),
        "Expect call number 2 is WriteSensor."
        );
    arg_n = 2; uint16_t nbytes = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes),
        "Expect WriteSensor writes two bytes."
        );
    /* WriteSensor_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSensor called with write_buffer[0] == %d\n", SpyOn_WriteSensor_arg1[0]);
    printf("WriteSensor called with write_buffer[1] == %d\n", SpyOn_WriteSensor_arg1[1]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        BridgeGetSensorLED_key, SpyOn_WriteSensor_arg1[0],
        "Expect `msg_to_sensor[0]` == 2 (BridgeGetSensorLED_key)."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_number, SpyOn_WriteSensor_arg1[1],
        "Expect `msg_to_sensor[1]` == 0 (led_0)."
        );
}
void old_BridgeGetSensorLED_exits_if_Sensor_responds_error_to_command(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    uint8_t last_call = 5; uint8_t response = error;
    // Test 5th call reports Sensor "invalid cmd error" to the host.
    TEST_ASSERT_TRUE(AssertCall(mock, last_call, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, last_call, 1, &response));
    // Test 5th call is the last call.
    TEST_ASSERT_EQUAL_UINT8(last_call, NumberOfActualCalls(mock));
}
void old_BridgeGetSensorLED_passes_Sensor_command_response_back_to_host(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    uint8_t call_n; uint8_t arg_n;
    call_n = 5;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    arg_n = 1; uint8_t response = error;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &response));
}
void old_BridgeGetSensorLED_reads_two_bytes_of_reply_from_Sensor(void)
{
    /* =====[ Setup ]===== */
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t bad_led_number = led_0+99;
    uint8_t payload[] = {bad_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    uint8_t call_n = 4;
    TEST_ASSERT_TRUE_MESSAGE(
            AssertCall(mock, call_n, "ReadSensor"),
            "Expect call number 5 is ReadSensor."
            )
    uint8_t arg_n = 2; uint16_t num_bytes_to_read = 2;
    TEST_ASSERT_TRUE_MESSAGE(
            AssertArg(mock, call_n, arg_n, &num_bytes_to_read),
            "Expect ReadSensor reads 2 bytes (arg is uint16_t 0x0002)."
            );
}
void old_BridgeGetSensorLED_writes_sensor_reply_to_host(void)
{
    /* =====[ Setup ]===== */
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t good_led_number = led_0;
    uint8_t payload[] = {good_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {ok, led_green};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    PrintAllCalls(mock);
    uint8_t call_n; uint8_t arg_n; uint8_t arg_value;
    /* ---Pass first byte of Sensor reply up to host--- */
    call_n = 5;
    TEST_ASSERT_TRUE_MESSAGE(
            AssertCall(mock, call_n, "SerialWriteByte"),
            "Expect call number 6 is SerialWriteByte."
            )
    arg_n = 1; arg_value = sensor_responses[0];
    TEST_ASSERT_TRUE_MESSAGE(
            AssertArg(mock, call_n, arg_n, &arg_value),
            "Expect first byte of Sensor reply is OK (0x00)."
            );
    /* ---Pass second byte of Sensor reply up to host--- */
    call_n++;
    TEST_ASSERT_TRUE_MESSAGE(
            AssertCall(mock, call_n, "SerialWriteByte"),
            "Expect call number 7 is SerialWriteByte."
            )
    arg_n = 1; arg_value = sensor_responses[1];
    TEST_ASSERT_TRUE_MESSAGE(
            AssertArg(mock, call_n, arg_n, &arg_value),
            "Expect second byte of Sensor reply is LED_GREEN (0x01)."
            );
}
