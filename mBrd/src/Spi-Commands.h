uint8_t const cmd_send_dummy_byte       = 0xF1;
uint8_t const cmd_send_four_dummy_bytes = 0xF2;
uint8_t const cmd_send_dummy_frame      = 0xF3;
uint8_t const cmd_send_adc_reading      = 0xF4;
uint8_t const cmd_send_fake_adc_reading = 0xF5;
uint8_t const cmd_send_adc_frame        = 0xF6;
uint8_t const cmd_send_lis_frame        = 0x01;
uint8_t const cmd_set_exposure_time     = 0x02;
uint8_t const cmd_set_gain_5x           = 0x03;
uint8_t const cmd_set_gain_4x           = 0x04;
uint8_t const cmd_set_gain_2pt5x        = 0x05;
uint8_t const cmd_set_gain_1x           = 0x06;
uint8_t const cmd_write_cfg_to_lis      = 0x07;
uint8_t const cmd_cfg_lis_gain_5x       = 0x08;
uint8_t const cmd_cfg_lis_gain_4x       = 0x09;
uint8_t const cmd_cfg_lis_gain_2pt5x    = 0x0A;
uint8_t const cmd_cfg_lis_gain_1x       = 0x0B;

uint16_t const fake_adc_reading = 0x1234;
// When not printing test results, I can send 1540 bytes no problem
#define npixels 784
#define num_bytes_in_a_dummy_frame 2*npixels
/* #define num_bytes_in_a_dummy_frame 1540 // wtf */
// When printing test results:
/* #define num_bytes_in_a_dummy_frame 1385 // last byte will be clobbered */
/* #define num_bytes_in_a_dummy_frame 1384 // largest frame I can send! */
uint8_t dummy_frame[num_bytes_in_a_dummy_frame];
uint16_t const sizeof_dummy_frame = sizeof(dummy_frame); //num_bytes_in_a_dummy_frame;
