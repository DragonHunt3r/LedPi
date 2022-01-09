#pragma once

#include "../leds.h"

/*
 * Run an effect as long as *(running) != 0.
 */
void run(unsigned char *running, struct led_strip_t *leds);
