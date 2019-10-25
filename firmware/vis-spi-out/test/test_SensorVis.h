#ifndef _TEST_SENSORVIS_H
#define _TEST_SENSORVIS_H

void SetUp_GetSensorLED(void); void TearDown_GetSensorLED(void);
void GetSensorLED_receives_led_number(void);
void GetSensorLED_always_replies_with_two_bytes(void);
void GetSensorLED_replies_msg_status_error_if_led_is_non_existent(void);
void GetSensorLED_replies_msg_status_ok_if_led_number_is_recognized(void);
void GetSensorLED_replies_led_off_if_led_is_off(void);
void GetSensorLED_replies_led_green_if_led_is_green(void);
void GetSensorLED_replies_led_red_if_led_is_red(void);

void test_lib_SensorVis_can_use_lib_Queue_and_sees_SpiFifo(void);
void test_Queue_lib_examples_of_push_and_pop(void);
void test_Queue_lib_example_of_push_wrap_around(void);

#endif // _TEST_SENSORVIS_H
