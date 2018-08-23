uint8_t const cmd_send_dummy_byte       = 0x01;
uint8_t const cmd_send_four_dummy_bytes = 0x02;
uint8_t const cmd_send_dummy_frame      = 0x03;
uint8_t const cmd_send_adc_reading      = 0x04;
uint8_t const cmd_send_fake_adc_reading = 0x05;
uint8_t const cmd_send_adc_frame        = 0x06;
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
