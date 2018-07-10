#ifndef _MOCK_DEBUGLED_H
#define _MOCK_DEBUGLED_H

void Expect_DebugLedTurnOn(void);
void Stub_DebugLedTurnOn(void);     void Unstub_DebugLedTurnOn(void);
void Expect_DebugLedTurnGreen(void);
void Stub_DebugLedTurnGreen(void);  void Unstub_DebugLedTurnGreen(void);
void Expect_DebugLedTurnRedToShowError(void);
void DebugLedTurnRedToShowError_Stubbed(void);

#endif // _MOCK_DEBUGLED_H
