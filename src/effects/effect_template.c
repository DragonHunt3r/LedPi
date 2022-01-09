#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "effect.h"

#include "../leds.h"

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

    while (*running) {
        // TODO: Effect

        // If render fails
        if (!leds_render(leds)) {
            break;
        }

        // Ignore return value
        usleep(PERIOD_MICROSECONDS(*(leds->size)));
    }
}
