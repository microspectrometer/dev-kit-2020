#include "AutoExpose.h"

uint16_t _MinPeak(uint16_t target, uint16_t target_tolerance);
uint16_t _MaxPeak(uint16_t target, uint16_t target_tolerance);
uint8_t max_tries;
uint16_t start_pixel;
uint16_t stop_pixel;
uint16_t target;
uint16_t target_tolerance;
uint16_t max_dark;
uint16_t min_exposure;
uint16_t max_exposure;
