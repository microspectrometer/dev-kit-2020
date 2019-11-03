Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0004.
status byte: 00
data byte 1: 0x1
data byte 2: 0x1
data byte 3: 0x1f
test/test_runner.c:76:GetSensorConfig_sends_three_bytes_of_data_to_Bridge_after_sending_ok:PASS
test/test_runner.c:84:SetSensorConfig_receives_three_bytes_of_config_from_Bridge:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
WriteSpiMaster sends: 0x01
test/test_runner.c:85:SetSensorConfig_replies_msg_status_error_if_binning_is_invalid:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
WriteSpiMaster sends: 0x01
test/test_runner.c:86:SetSensorConfig_replies_msg_status_error_if_gain_is_invalid:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
WriteSpiMaster sends: 0x01
test/test_runner.c:87:SetSensorConfig_replies_msg_status_error_if_active_rows_is_invalid:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
WriteSpiMaster sends: 0x00
test/test_runner.c:88:SetSensorConfig_replies_msg_status_ok_if_all_config_bytes_are_valid:PASS
Call 2 is named ProgramPhotodiodeArray? Call 2 is ProgramPhotodiodeArray.
Expected config: 00842109
Value passed to call 2, arg 1? (uint32_t)0x842109.
test/test_runner.c:89:SetSensorConfig_converts_three_data_bytes_to_a_28_bit_config:PASS

-----------------------
7 Tests 0 Failures 0 Ignored 
OK
