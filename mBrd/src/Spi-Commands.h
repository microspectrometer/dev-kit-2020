uint8_t const cmd_send_dummy_byte       = 0xF1;
uint8_t const cmd_send_four_dummy_bytes = 0xF2;
uint8_t const cmd_send_dummy_frame      = 0xF3;
uint8_t const cmd_send_adc_reading      = 0xF4;
uint8_t const cmd_send_fake_adc_reading = 0xF5;
uint8_t const cmd_send_adc_frame        = 0xF6;
uint8_t const cmd_send_lis_frame        = 0x01;
uint8_t const cmd_set_exposure_time     = 0x02;
uint8_t const cmd_disable_spi_master    = 0x03;
uint8_t const cmd_enable_spi_master     = 0x04;
uint8_t const cmd_cfg_summing_mode_off  = 0x05;
uint8_t const cmd_cfg_summing_mode_on   = 0x06;
uint8_t const cmd_write_cfg_to_lis      = 0x07;
uint8_t const cmd_cfg_lis_gain_5x       = 0x08;
uint8_t const cmd_cfg_lis_gain_4x       = 0x09;
uint8_t const cmd_cfg_lis_gain_2pt5x    = 0x0A;
uint8_t const cmd_cfg_lis_gain_1x       = 0x0B;
uint8_t const cmd_cfg_lis_rowselect_12345   = 0x0C;
uint8_t const cmd_cfg_lis_rowselect_1       = 0x0D;
uint8_t const cmd_cfg_lis_rowselect_2       = 0x0E;
uint8_t const cmd_cfg_lis_rowselect_3       = 0x0F;
uint8_t const cmd_cfg_lis_rowselect_4       = 0x10;
uint8_t const cmd_cfg_lis_rowselect_5       = 0x11;
uint8_t const cmd_cfg_lis_rowselect_12      = 0x12;
uint8_t const cmd_cfg_lis_rowselect_123     = 0x13;
uint8_t const cmd_cfg_lis_rowselect_1234    = 0x14;

uint16_t const fake_adc_reading = 0x1234;
// When not printing test results, I can send 1540 bytes no problem
#define npixels 784
#define num_bytes_in_a_full_frame (2*npixels)
#define npixels_binned (npixels/2)
#define num_bytes_in_a_half_frame (2*npixels_binned)
/* #define num_bytes_in_a_full_frame 1540 // wtf */
// When printing test results:
/* #define num_bytes_in_a_full_frame 1385 // last byte will be clobbered */
/* #define num_bytes_in_a_full_frame 1384 // largest frame I can send! */
uint8_t full_frame[num_bytes_in_a_full_frame];
/* uint16_t const sizeof_full_frame = sizeof(full_frame); //num_bytes_in_a_full_frame; */
uint16_t const sizeof_full_frame = num_bytes_in_a_full_frame;
uint16_t const sizeof_half_frame = num_bytes_in_a_half_frame;
// TODO: set up sizeof_half_frame and add conditionals to use sizeof_half_frame
// or sizeof_full_frame
