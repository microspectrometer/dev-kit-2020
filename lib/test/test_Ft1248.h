#ifndef _TEST_FT1248_H
#define _TEST_FT1248_H

void SetUp_NothingForFt1248(void); void TearDown_NothingForFt1248(void);
void FtOutputByte_outputs_a_byte_on_port_MIOSIO(void);

void SetUp_FtSendCommand(void); void TearDown_FtSendCommand(void);
void FtSendCommand_Read_does_entire_command_phase_for_ReadCmd(void);
void FtActivateInterface_pulls_SS_low(void);

#endif // _TEST_FT1248_H
