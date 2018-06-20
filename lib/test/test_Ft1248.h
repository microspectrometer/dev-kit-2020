#ifndef _TEST_FT1248_H
#define _TEST_FT1248_H

//void SetUp_NothingForFt1248(void); void TearDown_NothingForFt1248(void);
//
void SetUp_FtSendCommand(void); void TearDown_FtSendCommand(void);
void FtSendCommand_Read_does_entire_command_phase_for_ReadCmd(void);

void SetUp_FtBusTurnaround(void); void TearDown_FtBusTurnaround(void);
void FtBusTurnaround_handles_the_entire_bus_turnaround(void);

void SetUp_FtRead(void); void TearDown_FtRead(void);
void FtRead_reads_bytes_from_MIOSIO(void);

void SetUp_FtPorts(void); void TearDown_FtPorts(void);
//Before initiating read
void FtHasDataToRead_returns_true_if_MISO_is_low(void);
//FtSendCommand
void FtActivateInterface_pulls_SS_low(void);
void FtPushData_pulls_SCK_high(void);
void FtOutputByte_outputs_a_byte_on_port_MIOSIO(void);
void FtLetMasterDriveBus_configures_MIOSIO_port_for_MCU_output(void);
void FtPullData_pulls_SCK_low(void);
//FtBusTuraround
void FtLetSlaveDriveBus_configures_MIOSIO_port_for_MCU_input(void);
void FtIsBusOk_returns_true_if_MISO_is_low(void);
void FtIsBusOk_returns_false_if_MISO_is_high(void);
//FtRead
void FtReadData_returns_the_value_on_MIOSIO_pins(void);
void FtDeactivateInterface_pulls_SS_high(void);
void FtHasDataToRead_returns_false_if_MISO_is_high(void);

#endif // _TEST_FT1248_H
