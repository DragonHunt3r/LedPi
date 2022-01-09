#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "effect.h"

#include "../color.h"
#include "../leds.h"

#define SECONDS_PER_CYCLE 5
#define PERIOD_MICROSECONDS(leds) (1000000 / leds * SECONDS_PER_CYCLE)

void run(unsigned char *running, struct led_strip_t *leds) {
    // Preconditions
    assert(running);
    assert(leds);

    // No LEDs
    if (*(leds->size) == 0) {
        fprintf(stderr, "No LEDs found\n");
        return;
    }

    float delta = 360.0F / *(leds->size);

    // Initial colors
    for (int i = 0; i < *(leds->size); i++) {
        *(leds->leds[i].color) = color_from_hsv(i * delta, 1.0F, 1.0F);
    }

    // If render fails
    if (!leds_render(leds)) {
        return;
    }

    while (*running) {
        color_t temp = *(leds->leds[0].color);
        for (int i = 1; i < *(leds->size); i++) {
            *(leds->leds[i - 1].color) = *(leds->leds[i].color);
        }
        *(leds->leds[*(leds->size) - 1].color) = temp;

        // If render fails
        if (!leds_render(leds)) {
            break;
        }

        // Ignore return value
        usleep(PERIOD_MICROSECONDS(*(leds->size)));
    }
}
