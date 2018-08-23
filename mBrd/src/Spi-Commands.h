uint8_t const cmd_send_dummy_byte       = 0xF1;
uint8_t const cmd_send_four_dummy_bytes = 0xF2;
uint8_t const cmd_send_dummy_frame      = 0xF3;
uint8_t const cmd_send_adc_reading      = 0xF4;
uint8_t const cmd_send_fake_adc_reading = 0xF5;
uint8_t const cmd_send_adc_frame        = 0xF6;
uint8_t const cmd_send_lis_frame        = 0x01;
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
