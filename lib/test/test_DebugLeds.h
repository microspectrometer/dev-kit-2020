#ifndef _TEST_DEBUGLEDS_H
#define _TEST_DEBUGLEDS_H

void SetUp_DebugLeds(void); void TearDown_DebugLeds(void);
void DebugLedsTurnOn_led_N_turns_on_led_N(void);
void DebugLedsTurnRed_led_N_turns_led_N_red(void);
void DebugLedsTurnGreen_led_N_turns_led_N_green(void);
void DebugLedsTurnAllRed_turns_all_4_leds_red(void);
void DebugLedsTurnAllGreen_turns_all_4_leds_green(void);

#endif // _TEST_DEBUGLEDS_H
