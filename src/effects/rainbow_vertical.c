#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include "effect.h"

#include "../color.h"
#include "../leds.h"

#define COLOR_DOMAIN 120.0F
#define SPEED 3.0F
#define PERIOD_MICROSECONDS(leds) (1000000 / 10)

void run(unsigned char *running, struct led_strip_t *leds) {
    // Preconditions
    assert(running);
    assert(leds);

    // No LEDs
    if (*(leds->size) == 0) {
        fprintf(stderr, "No LEDs found\n");
        return;
    }

    leds_clear(leds);
    if (!leds_render(leds)) {
        return;
    }

    float offset = 0.0F;
    while (*running) {
        for (int i = 0; i < *(leds->size); i++) {
            struct led_t led = leds->leds[i];
            *(led.color) = color_from_hsv(offset + led.position->y * COLOR_DOMAIN, 1.0F, 1.0F);
        }

        // If render fails
        if (!leds_render(leds)) {
            break;
        }

        offset = fmodf(offset + SPEED, 360.0F);

        // Ignore return value
        usleep(PERIOD_MICROSECONDS(*(leds->size)));
    }
}
