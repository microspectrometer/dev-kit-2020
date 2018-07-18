#ifndef _TEST_FT1248_MOCKUPS_H
#define _TEST_FT1248_MOCKUPS_H

// FtSendCommand
void SetUpMock_FtSendCommand(void);
void TearDownMock_FtSendCommand(void);
// FtBusTurnaround
void SetUpMock_FtBusTurnaround(void);
void TearDownMock_FtBusTurnaround(void);
// FtRead
void SetUpMock_FtRead(void);
void TearDownMock_FtRead(void);
void SetUpMock_DetailsOf_FtRead(void);
void TearDownMock_DetailsOf_FtRead(void);
// FtWrite
void SetUpMock_FtWrite(void);
void TearDownMock_FtWrite(void);
void SetUpMock_DetailsOf_FtWrite(void);
void TearDownMock_DetailsOf_FtWrite(void);

#endif // _TEST_FT1248_MOCKUPS_H
