# BiColorLed tests
## BiColorLedOn
test/test_runner.c:53:BiColorLedOn_sets_bit_in_ddr:PASS
## BiColorLedOff
test/test_runner.c:55:BiColorLedOff_clears_bit_in_ddr:PASS
## BiColorLedGreen
test/test_runner.c:57:BiColorLedGreen_clears_bit_in_port:PASS
## BiColorLedRed
test/test_runner.c:59:BiColorLedRed_sets_bit_in_port:PASS
# ReadWriteBits tests
## SetBit
test/test_runner.c:72:SetBit_sets_bit_in_register:PASS
## ClearBit
test/test_runner.c:74:ClearBit_clears_bit_in_register:PASS
## BitIsSet
test/test_runner.c:76:BitIsSet_returns_true_if_bit_is_set:PASS
test/test_runner.c:77:BitIsSet_returns_false_if_bit_is_clear:PASS
## BitIsClear
test/test_runner.c:79:BitIsClear_returns_true_if_bit_is_clear:PASS
test/test_runner.c:80:BitIsClear_returns_false_if_bit_is_set:PASS
# Flag test
test/test_runner.c:497:test_Flag:PASS
# Spi tests
## ClearSpiInterruptFlag
test/test_runner.c:93:ClearSpiInterruptFlag_first_reads_SPI_status_register:PASS
test/test_runner.c:94:ClearSpiInterruptFlag_then_reads_SPI_data_register:PASS
## _SpiTransferIsDone
test/test_runner.c:99:SpiTransferIsDone_returns_true_if_the_SPI_Interrupt_Flag_is_set:PASS
test/test_runner.c:100:SpiTransferIsDone_returns_false_if_the_SPI_Interrupt_Flag_is_clear:PASS
# SpiSlave tests
## _SignalDataReady
test/test_runner.c:113:SignalDataReady_drives_DataReady_LOW:PASS
## DisableSpiInterrupt
test/test_runner.c:118:DisableSpiInterrupt_clears_the_SPI_Interrupt_Enable_bit:PASS
## EnableSpiInterrupt

- Call 1 is named ClearSpiInterruptFlag? Yes.

test/test_runner.c:123:EnableSpiInterrupt_clears_SPI_interrupt_flag:PASS
test/test_runner.c:124:EnableSpiInterrupt_enables_interrupt_SPI_Transfer_Complete:PASS
test/test_runner.c:125:EnableSpiInterrupt_consumes_6_cycles:PASS
## SpiSlaveInit
test/test_runner.c:130:SpiSlaveInit_makes_DataReady_an_output_pin:PASS
test/test_runner.c:131:SpiSlaveInit_idles_DataReady_high:PASS
test/test_runner.c:132:SpiSlaveInit_makes_Miso_an_output_pin:PASS
test/test_runner.c:133:SpiSlaveInit_enables_SPI:PASS

- Call 1 is named EnableSpiInterrupt? Yes.

test/test_runner.c:134:SpiSlaveInit_enables_SPI_interrupt:PASS
## SpiSlaveTxByte
test/test_runner.c:139:SpiSlaveTxByte_loads_SPI_data_register_with_input_byte:PASS

- Call 1 is named DisableSpiInterrupt? Yes.


- Call 2 is named _SignalDataReady? Yes.

test/test_runner.c:140:SpiSlaveTxByte_disables_SPI_ISR_before_signaling_data_ready:PASS

- Call 2 is named _SignalDataReady? Yes.

test/test_runner.c:141:SpiSlaveTxByte_drives_DataReady_LOW_to_signal_data_is_ready:PASS

- Call 3 is named _SpiTransferIsDone? Yes.

test/test_runner.c:142:SpiSlaveTxByte_waits_until_SPI_transfer_is_done:PASS
test/test_runner.c:143:SpiSlaveTxByte_drives_DataReady_HIGH_immediately_after_SPI_transfer_finishes:PASS
test/test_runner.c:144:SpiSlaveTxByte_enables_SPI_ISR_after_transfer:PASS
## SpiSlaveTx
Transmit these bytes: {0xa0, 0xb1, 0xc2}

- Value passed to call 1, arg 1? 
    - (uint8_t)0xa0 is arg 1 to call SpiSlaveTxByte.


- Value passed to call 2, arg 1? 
    - (uint8_t)0xb1 is arg 1 to call SpiSlaveTxByte.


- Value passed to call 3, arg 1? 
    - (uint8_t)0xc2 is arg 1 to call SpiSlaveTxByte.

test/test_runner.c:149:SpiSlaveTx_sends_nbytes_of_input_buffer_to_SpiMaster:PASS
# SpiMaster tests
## SpiMasterInit
test/test_runner.c:169:SpiMasterInit_idles_SlaveSelect_high:PASS
test/test_runner.c:170:SpiMasterInit_makes_SlaveSelect_an_output:PASS
test/test_runner.c:171:SpiMasterInit_makes_Miso_an_input:PASS
test/test_runner.c:172:SpiMasterInit_enables_pullup_on_Miso:PASS
test/test_runner.c:173:SpiMasterInit_makes_DataReady_an_input:PASS
test/test_runner.c:174:SpiMasterInit_enables_pullup_on_DataReady:PASS
test/test_runner.c:175:SpiMasterInit_makes_Mosi_an_output:PASS
test/test_runner.c:176:SpiMasterInit_makes_Sck_an_output:PASS
test/test_runner.c:177:SpiMasterInit_makes_this_MCU_the_SPI_Master:PASS
test/test_runner.c:178:SpiMasterInit_sets_SPI_Clock_to_10MHz_ext_osc_divided_by_8:PASS
test/test_runner.c:179:SpiMasterInit_enables_the_SPI_hardware_module:PASS

- Call 1 is named ClearSpiInterruptFlag? Yes.

test/test_runner.c:180:SpiMasterInit_clears_SPI_interrupt_flag:PASS
## SpiMasterXfrByte
test/test_runner.c:185:SpiMasterXfrByte_selects_the_SPI_slave:PASS
test/test_runner.c:186:SpiMasterXfrByte_loads_SPI_data_reg_with_the_byte_to_send:PASS
test/test_runner.c:187:SpiMasterXfrByte_waits_until_the_transfer_is_done_by_reading_the_SPI_Interrupt_Flag:PASS
test/test_runner.c:188:SpiMasterXfrByte_unselects_the_SPI_slave:PASS
test/test_runner.c:189:SpiMasterXfrByte_clears_the_SPI_Interrupt_Flag_by_reading_the_SPI_data_reg:PASS
test/test_runner.c:190:SpiMasterXfrByte_returns_the_byte_in_the_SPI_data_reg:PASS
# Queue tests
## QueueInit
test/test_runner.c:203:QueueInit_returns_a_pointer_to_a_Queue_struct:PASS
test/test_runner.c:204:QueueInit_memory_for_Queue_struct_is_allocated_in_Queue_object_file:PASS
test/test_runner.c:205:QueueInit_assigns_input_buffer_as_Queue_buffer:PASS
test/test_runner.c:206:QueueInit_size_input_is_the_maximum_Queue_length:PASS
test/test_runner.c:207:QueueInit_initializes_Queue_with_length_0:PASS
## QueueLength
test/test_runner.c:212:QueueLength_increments_after_a_push:PASS
test/test_runner.c:213:QueueLength_does_not_increase_beyond_max_length:PASS
test/test_runner.c:214:QueueLength_decrements_after_a_pop:PASS
test/test_runner.c:215:QueueLength_does_not_decrease_below_zero:PASS
## QueuePush
test/test_runner.c:220:QueuePush_writes_byte_to_Queue_buffer:PASS
test/test_runner.c:221:QueuePush_writes_next_byte_to_address_after_previous_write:PASS
test/test_runner.c:222:QueuePush_does_not_write_byte_if_Queue_is_full:PASS
test/test_runner.c:223:QueuePush_hits_end_of_buffer_and_wraps_around_if_Queue_is_not_full:PASS
## QueuePop
test/test_runner.c:228:QueuePop_removes_oldest_byte_from_Queue:PASS
test/test_runner.c:229:QueuePop_returns_oldest_byte:PASS
test/test_runner.c:230:QueuePop_does_not_remove_any_bytes_if_Queue_is_empty:PASS
test/test_runner.c:231:QueuePop_returns_0_if_Queue_is_empty:PASS
test/test_runner.c:232:QueuePop_hits_end_of_buffer_and_wraps_around_if_Queue_is_not_empty:PASS
## QueueIsFull
test/test_runner.c:237:QueueIsFull_returns_true_if_Queue_is_full:PASS
test/test_runner.c:238:QueueIsFull_returns_false_if_Queue_is_not_full:PASS
## QueueIsEmpty
test/test_runner.c:243:QueueIsEmpty_returns_true_if_Queue_is_empty:PASS
test/test_runner.c:244:QueueIsEmpty_returns_false_if_Queue_is_not_empty:PASS
# UartSpi tests
## UartSpiInit
test/test_runner.c:257:UartSpiInit_clocks_SPI_bus_at_5MHz:PASS
test/test_runner.c:258:UartSpiInit_sets_Sck_as_an_output:PASS
test/test_runner.c:259:UartSpiInit_sets_AdcConv_to_idle_low:PASS
test/test_runner.c:260:UartSpiInit_sets_AdcConv_as_an_output:PASS
test/test_runner.c:261:UartSpiInit_enables_the_UART_in_Master_SPI_Mode:PASS
test/test_runner.c:262:UartSpiInit_uses_SPI_data_mode_CPOL_1_CPHA_1:PASS
test/test_runner.c:263:UartSpiInit_cfgs_SPI_to_transfer_MSB_first:PASS
test/test_runner.c:264:UartSpiInit_gives_SPI_control_over_Miso_and_Mosi_pin_behavior:PASS
# Lis tests
## MSB
test/test_runner.c:277:MSB_returns_most_significant_bit_of_16bit_input:PASS
## LSB
test/test_runner.c:282:LSB_returns_least_significant_bit_of_16bit_input:PASS
## LisInit
test/test_runner.c:287:LisInit_sets_PixSelect_as_an_output:PASS
test/test_runner.c:288:LisInit_idles_PixSelect_low:PASS
test/test_runner.c:289:LisInit_sets_Rst_as_an_output:PASS
test/test_runner.c:290:LisInit_idles_Rst_low:PASS
test/test_runner.c:291:LisInit_sets_Sync_as_an_input:PASS
test/test_runner.c:292:LisInit_sets_Clk_as_an_output:PASS
test/test_runner.c:293:LisInit_resets_PWM_timer_at_top:PASS
test/test_runner.c:294:LisInit_PWM_timer_top_is_OCR0A:PASS
test/test_runner.c:295:LisInit_PWM_timer_is_clocked_by_CPU_with_no_prescaling:PASS
test/test_runner.c:296:LisInit_sets_PWM_frequency_at_50kHz:PASS
test/test_runner.c:297:LisInit_sets_PWM_duty_cycle_to_50_percent:PASS
test/test_runner.c:298:LisInit_outputs_the_PWM_clock_on_pin_Clk:PASS
## LisConfigIsValid
test/test_runner.c:303:LisConfigIsValid_returns_false_if_binning_is_invalid:PASS
test/test_runner.c:304:LisConfigIsValid_returns_false_if_gain_is_invalid:PASS
test/test_runner.c:305:LisConfigIsValid_returns_false_if_active_rows_is_invalid:PASS
test/test_runner.c:306:LisConfigIsValid_returns_true_if_config_is_valid:PASS
## _ConfigAs28bits
test/test_runner.c:311:ConfigAs28bits_writes_config_as_little_endian_ie_binning_is_config_byte0_bit0:PASS
test/test_runner.c:312:ConfigAs28bits_sets_config_byte0_bit0_if_BINNING_ON:PASS
test/test_runner.c:313:ConfigAs28bits_clears_config_byte0_bit0_if_BINNING_OFF:PASS
test/test_runner.c:314:ConfigAs28bits_byte0_bit1_clear_and_bit2_clear_if_GAIN_1X:PASS
test/test_runner.c:315:ConfigAs28bits_byte0_bit1_clear_and_bit2_set_if_GAIN_2X5:PASS
test/test_runner.c:316:ConfigAs28bits_byte0_bit1_set_and_bit2_clear_if_GAIN_4X:PASS
test/test_runner.c:317:ConfigAs28bits_byte0_bit1_set_and_bit2_set_if_GAIN_5X:PASS
test/test_runner.c:318:ConfigAs28bits_bit3_to_bit27_set_if_ALL_ROWS_ACTIVE:PASS
test/test_runner.c:319:ConfigAs28bits_b3b8b13b18b23_set_if_ROW_1_ACTIVE:PASS
test/test_runner.c:320:ConfigAs28bits_b4b9b14b19b24_set_if_ROW_2_ACTIVE:PASS
test/test_runner.c:321:ConfigAs28bits_b5b10b15b20b25_set_if_ROW_3_ACTIVE:PASS
test/test_runner.c:322:ConfigAs28bits_b6b11b16b21b26_set_if_ROW_4_ACTIVE:PASS
test/test_runner.c:323:ConfigAs28bits_b7b12b17b22b27_set_if_ROW_5_ACTIVE:PASS
## _WaitForLisClkLow
test/test_runner.c:328:WaitForLisClkLow_clears_flag_PwmTimerMatchesOCF0B:PASS
test/test_runner.c:329:WaitForLisClkLow_waits_until_flag_PwmTimerMatchesOCF0B_is_set:PASS
## _WaitForLisClkHigh
test/test_runner.c:334:WaitForLisClkHigh_clears_flag_PwmTimerMatchesOCF0A:PASS
test/test_runner.c:335:WaitForLisClkHigh_waits_until_flag_PwmTimerMatchesOCF0A_is_set:PASS
## _EnterLisProgrammingMode

- Call 1 is named _WaitForLisClkLow? Yes.

test/test_runner.c:340:EnterLisProgrammingMode_waits_for_LisClk_LOW:PASS
test/test_runner.c:341:EnterLisProgrammingMode_asserts_LisPixSelect_to_program_Lis:PASS
## _WriteLisConfigBit
test/test_runner.c:346:WriteLisConfigBit_outputs_bit_on_LisRst:PASS

- Call 1 is named _WaitForLisClkHigh? Yes.

test/test_runner.c:347:WriteLisConfigBit_waits_for_LisClk_HIGH:PASS

- Call 2 is named _WaitForLisClkLow? Yes.

test/test_runner.c:348:WriteLisConfigBit_waits_for_LisClk_LOW:PASS
## _Write28bitLisConfig

- Call 1 is named _WriteLisConfigBit? Yes.


- Value passed to call 1, arg 1? 
    - (uint8_t)0xac is arg 1 to call _WriteLisConfigBit.


- Value passed to call 1, arg 2? 
    - (uint8_t)0000 is arg 2 to call _WriteLisConfigBit.


- Call 2 is named _WriteLisConfigBit? Yes.


- Value passed to call 2, arg 1? 
    - (uint8_t)0xac is arg 1 to call _WriteLisConfigBit.


- Value passed to call 2, arg 2? 
    - (uint8_t)0x01 is arg 2 to call _WriteLisConfigBit.


- Call 3 is named _WriteLisConfigBit? Yes.


- Value passed to call 3, arg 1? 
    - (uint8_t)0xac is arg 1 to call _WriteLisConfigBit.


- Value passed to call 3, arg 2? 
    - (uint8_t)0x02 is arg 2 to call _WriteLisConfigBit.


- Call 4 is named _WriteLisConfigBit? Yes.


- Value passed to call 4, arg 1? 
    - (uint8_t)0xac is arg 1 to call _WriteLisConfigBit.


- Value passed to call 4, arg 2? 
    - (uint8_t)0x03 is arg 2 to call _WriteLisConfigBit.


- Call 5 is named _WriteLisConfigBit? Yes.


- Value passed to call 5, arg 1? 
    - (uint8_t)0xac is arg 1 to call _WriteLisConfigBit.


- Value passed to call 5, arg 2? 
    - (uint8_t)0x04 is arg 2 to call _WriteLisConfigBit.


- Call 6 is named _WriteLisConfigBit? Yes.


- Value passed to call 6, arg 1? 
    - (uint8_t)0xac is arg 1 to call _WriteLisConfigBit.


- Value passed to call 6, arg 2? 
    - (uint8_t)0x05 is arg 2 to call _WriteLisConfigBit.


- Call 7 is named _WriteLisConfigBit? Yes.


- Value passed to call 7, arg 1? 
    - (uint8_t)0xac is arg 1 to call _WriteLisConfigBit.


- Value passed to call 7, arg 2? 
    - (uint8_t)0x06 is arg 2 to call _WriteLisConfigBit.


- Call 8 is named _WriteLisConfigBit? Yes.


- Value passed to call 8, arg 1? 
    - (uint8_t)0xac is arg 1 to call _WriteLisConfigBit.


- Value passed to call 8, arg 2? 
    - (uint8_t)0x07 is arg 2 to call _WriteLisConfigBit.


- Call 9 is named _WriteLisConfigBit? Yes.


- Value passed to call 9, arg 1? 
    - (uint8_t)0xbd is arg 1 to call _WriteLisConfigBit.


- Value passed to call 9, arg 2? 
    - (uint8_t)0000 is arg 2 to call _WriteLisConfigBit.


- Call 10 is named _WriteLisConfigBit? Yes.


- Value passed to call 10, arg 1? 
    - (uint8_t)0xbd is arg 1 to call _WriteLisConfigBit.


- Value passed to call 10, arg 2? 
    - (uint8_t)0x01 is arg 2 to call _WriteLisConfigBit.


- Call 11 is named _WriteLisConfigBit? Yes.


- Value passed to call 11, arg 1? 
    - (uint8_t)0xbd is arg 1 to call _WriteLisConfigBit.


- Value passed to call 11, arg 2? 
    - (uint8_t)0x02 is arg 2 to call _WriteLisConfigBit.


- Call 12 is named _WriteLisConfigBit? Yes.


- Value passed to call 12, arg 1? 
    - (uint8_t)0xbd is arg 1 to call _WriteLisConfigBit.


- Value passed to call 12, arg 2? 
    - (uint8_t)0x03 is arg 2 to call _WriteLisConfigBit.


- Call 13 is named _WriteLisConfigBit? Yes.


- Value passed to call 13, arg 1? 
    - (uint8_t)0xbd is arg 1 to call _WriteLisConfigBit.


- Value passed to call 13, arg 2? 
    - (uint8_t)0x04 is arg 2 to call _WriteLisConfigBit.


- Call 14 is named _WriteLisConfigBit? Yes.


- Value passed to call 14, arg 1? 
    - (uint8_t)0xbd is arg 1 to call _WriteLisConfigBit.


- Value passed to call 14, arg 2? 
    - (uint8_t)0x05 is arg 2 to call _WriteLisConfigBit.


- Call 15 is named _WriteLisConfigBit? Yes.


- Value passed to call 15, arg 1? 
    - (uint8_t)0xbd is arg 1 to call _WriteLisConfigBit.


- Value passed to call 15, arg 2? 
    - (uint8_t)0x06 is arg 2 to call _WriteLisConfigBit.


- Call 16 is named _WriteLisConfigBit? Yes.


- Value passed to call 16, arg 1? 
    - (uint8_t)0xbd is arg 1 to call _WriteLisConfigBit.


- Value passed to call 16, arg 2? 
    - (uint8_t)0x07 is arg 2 to call _WriteLisConfigBit.


- Call 17 is named _WriteLisConfigBit? Yes.


- Value passed to call 17, arg 1? 
    - (uint8_t)0xce is arg 1 to call _WriteLisConfigBit.


- Value passed to call 17, arg 2? 
    - (uint8_t)0000 is arg 2 to call _WriteLisConfigBit.


- Call 18 is named _WriteLisConfigBit? Yes.


- Value passed to call 18, arg 1? 
    - (uint8_t)0xce is arg 1 to call _WriteLisConfigBit.


- Value passed to call 18, arg 2? 
    - (uint8_t)0x01 is arg 2 to call _WriteLisConfigBit.


- Call 19 is named _WriteLisConfigBit? Yes.


- Value passed to call 19, arg 1? 
    - (uint8_t)0xce is arg 1 to call _WriteLisConfigBit.


- Value passed to call 19, arg 2? 
    - (uint8_t)0x02 is arg 2 to call _WriteLisConfigBit.


- Call 20 is named _WriteLisConfigBit? Yes.


- Value passed to call 20, arg 1? 
    - (uint8_t)0xce is arg 1 to call _WriteLisConfigBit.


- Value passed to call 20, arg 2? 
    - (uint8_t)0x03 is arg 2 to call _WriteLisConfigBit.


- Call 21 is named _WriteLisConfigBit? Yes.


- Value passed to call 21, arg 1? 
    - (uint8_t)0xce is arg 1 to call _WriteLisConfigBit.


- Value passed to call 21, arg 2? 
    - (uint8_t)0x04 is arg 2 to call _WriteLisConfigBit.


- Call 22 is named _WriteLisConfigBit? Yes.


- Value passed to call 22, arg 1? 
    - (uint8_t)0xce is arg 1 to call _WriteLisConfigBit.


- Value passed to call 22, arg 2? 
    - (uint8_t)0x05 is arg 2 to call _WriteLisConfigBit.


- Call 23 is named _WriteLisConfigBit? Yes.


- Value passed to call 23, arg 1? 
    - (uint8_t)0xce is arg 1 to call _WriteLisConfigBit.


- Value passed to call 23, arg 2? 
    - (uint8_t)0x06 is arg 2 to call _WriteLisConfigBit.


- Call 24 is named _WriteLisConfigBit? Yes.


- Value passed to call 24, arg 1? 
    - (uint8_t)0xce is arg 1 to call _WriteLisConfigBit.


- Value passed to call 24, arg 2? 
    - (uint8_t)0x07 is arg 2 to call _WriteLisConfigBit.


- Call 25 is named _WriteLisConfigBit? Yes.


- Value passed to call 25, arg 1? 
    - (uint8_t)0xdf is arg 1 to call _WriteLisConfigBit.


- Value passed to call 25, arg 2? 
    - (uint8_t)0000 is arg 2 to call _WriteLisConfigBit.


- Call 26 is named _WriteLisConfigBit? Yes.


- Value passed to call 26, arg 1? 
    - (uint8_t)0xdf is arg 1 to call _WriteLisConfigBit.


- Value passed to call 26, arg 2? 
    - (uint8_t)0x01 is arg 2 to call _WriteLisConfigBit.


- Call 27 is named _WriteLisConfigBit? Yes.


- Value passed to call 27, arg 1? 
    - (uint8_t)0xdf is arg 1 to call _WriteLisConfigBit.


- Value passed to call 27, arg 2? 
    - (uint8_t)0x02 is arg 2 to call _WriteLisConfigBit.


- Call 28 is named _WriteLisConfigBit? Yes.


- Value passed to call 28, arg 1? 
    - (uint8_t)0xdf is arg 1 to call _WriteLisConfigBit.


- Value passed to call 28, arg 2? 
    - (uint8_t)0x03 is arg 2 to call _WriteLisConfigBit.

test/test_runner.c:353:Write28bitLisConfig_writes_28bits_starting_at_byte0_bit0_and_ending_at_byte3_bit3:PASS
## _ExitLisProgrammingMode
test/test_runner.c:358:ExitLisProgrammingMode_outputs_LOW_on_pin_LisRst:PASS
test/test_runner.c:359:ExitLisProgrammingMode_outputs_LOW_on_pin_LisPixSelect:PASS
## LisWriteConfig

- Call 1 is named _ConfigAs28bits? Yes.

test/test_runner.c:364:LisWriteConfig_converts_config_to_28bit_sequence:PASS

- Call 2 is named _EnterLisProgrammingMode? Yes.

test/test_runner.c:365:LisWriteConfig_enters_LIS_programming_mode:PASS

- Call 3 is named _Write28bitLisConfig? Yes.

test/test_runner.c:366:LisWriteConfig_writes_28bits_to_LIS_setup_register:PASS

- Call 4 is named _ExitLisProgrammingMode? Yes.

test/test_runner.c:367:LisWriteConfig_exits_LIS_programming_mode:PASS
## LisExpose
test/test_runner.c:372:LisExpose_waits_for_the_falling_edge_of_Lis_Clk:PASS
test/test_runner.c:373:LisExpose_starts_exposure_by_driving_Lis_Rst_HIGH:PASS
test/test_runner.c:374:LisExpose_counts_falling_edges_of_Lis_Clk_until_count_equals_exposure_ticks:PASS
test/test_runner.c:375:LisExpose_stops_exposure_by_driving_Lis_Rst_LOW:PASS
# Usb tests
## _FtClockDatabus
test/test_runner.c:388:FtClockDatabus_drives_FtClock_HIGH_if_direction_is_FtDrive:PASS
test/test_runner.c:389:FtClockDatabus_drives_FtClock_LOW_if_direction_is_FtSample:PASS
## _FtReadDatabus
test/test_runner.c:394:FtReadDatabus_copies_databus_pin_values_to_address_pbyte:PASS
## _FtWriteDatabus
test/test_runner.c:399:FtWriteDatabus_outputs_byte_on_databus_pins:PASS
## _FtDatabusPinDirection
test/test_runner.c:404:FtDatabusPinDirection_makes_databus_pins_outputs_if_direction_is_FtOut:PASS
test/test_runner.c:405:FtDatabusPinDirection_makes_databus_pins_inputs_if_direction_is_FtIn:PASS
## FtSelectFT221X
test/test_runner.c:410:FtSelectFT221X_drives_FtChipSelect_LOW:PASS
## FtUnselectFT221X
test/test_runner.c:415:FtUnselectFT221X_drives_FtChipSelect_HIGH:PASS
## FtBusTurnaround
Drive clock to signal Data-Drive then Data-Sample:

- FtDrive: rising edge
- FtSample: falling edge

- Value passed to call 1, arg 1? 
    - (uint8_t)0x01 is arg 1 to call _FtClockDatabus.


- Value passed to call 2, arg 1? 
    - (uint8_t)0000 is arg 1 to call _FtClockDatabus.

test/test_runner.c:420:FtBusTurnaround_clocks_one_cycle_to_signal_data_drive_then_data_sample:PASS
## FtIsOk
test/test_runner.c:425:FtIsOk_returns_true_if_FtMiso_is_LOW:PASS
test/test_runner.c:426:FtIsOk_returns_false_if_FtMiso_is_HIGH:PASS
## FtRead

- Value passed to call 1, arg 1? 
    - (uint8_t)0x01 is arg 1 to call _FtClockDatabus.


- Value passed to call 2, arg 1? 
    - (uint8_t)0000 is arg 1 to call _FtClockDatabus.

test/test_runner.c:431:FtRead_clocks_one_byte_out_of_the_FT221X:PASS

- Value passed to call 3, arg 1? 
    - (uint8_t)0xcd is arg 1 to call _FtReadDatabus.

test/test_runner.c:432:FtRead_stores_the_byte_at_address_pbyte:PASS
## FtWrite

- Value passed to call 1, arg 1? 
    - (uint8_t)0x01 is arg 1 to call _FtClockDatabus.

test/test_runner.c:437:FtWrite_signals_to_drive_data_onto_the_databus:PASS

- Value passed to call 2, arg 1? 
    - (uint8_t)0xff is arg 1 to call _FtDatabusPinDirection.

test/test_runner.c:438:FtWrite_sets_microcontroller_databus_pins_as_outputs:PASS

- Value passed to call 3, arg 1? 
    - (uint8_t)0xab is arg 1 to call _FtWriteDatabus.

test/test_runner.c:439:FtWrite_outputs_byte_on_databus_pins:PASS

- Value passed to call 4, arg 1? 
    - (uint8_t)0000 is arg 1 to call _FtClockDatabus.

test/test_runner.c:440:FtWrite_signals_FT221X_to_sample_the_databus:PASS

- Value passed to call 5, arg 1? 
    - (uint8_t)0000 is arg 1 to call _FtDatabusPinDirection.

test/test_runner.c:441:FtWrite_sets_microcontroller_databus_pins_as_inputs:PASS
## UsbRxbufferIsEmpty
test/test_runner.c:446:UsbRxbufferIsEmpty_returns_true_if_pin_FT1248_MISO_is_HIGH:PASS
test/test_runner.c:447:UsbRxbufferIsEmpty_returns_false_if_pin_FT1248_MISO_is_LOW:PASS
## UsbTxbufferIsFull
test/test_runner.c:452:UsbTxbufferIsFull_returns_true_if_pin_MIOSIO0_is_HIGH:PASS
test/test_runner.c:453:UsbTxbufferIsFull_returns_false_if_pin_MIOSIO0_is_LOW:PASS
## UsbReadByte

- Call 1 is named FtSelectFT221X? Yes.

test/test_runner.c:458:UsbReadByte_selects_the_FT221X:PASS

*Read command is* `0xc6`

- Value passed to call 2, arg 1? 
    - (uint8_t)0xc6 is arg 1 to call FtWrite.

test/test_runner.c:459:UsbReadByte_drives_databus_with_read_command:PASS

- Call 3 is named FtBusTurnaround? Yes.

test/test_runner.c:460:UsbReadByte_signals_FT221X_to_sample_the_databus:PASS

- Call 4 is named FtIsOk? Yes.

test/test_runner.c:445:UsbReadByte_reads_the_byte_if_data_transfer_status_is_OK:FAIL: Implement Test

- Call 5 is named FtUnselectFT221X? Yes.

test/test_runner.c:462:UsbReadByte_unselects_the_FT221X:PASS
test/test_runner.c:463:UsbReadByte_returns_either_FtOK_if_pbyte_has_the_read_data_or_FtError_if_Usb_receive_buffer_was_empty:FAIL: Implement Test
## UsbWriteByte
FAIL: Implement Test
# StatusCode tests
## led_setting_is_valid
test/test_runner.c:481:led_setting_is_valid_returns_TRUE_if_setting_is_OFF:PASS
test/test_runner.c:482:led_setting_is_valid_returns_TRUE_if_setting_is_GREEN:PASS
test/test_runner.c:483:led_setting_is_valid_returns_TRUE_if_setting_is_RED:PASS
test/test_runner.c:484:led_setting_is_valid_returns_FALSE_if_setting_is_any_other_value:PASS

-----------------------
163 Tests 2 Failures 0 Ignored 
FAIL
