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
6 Tests 0 Failures 0 Ignored 
OK
