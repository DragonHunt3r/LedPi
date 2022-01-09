#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "effect.h"

#include "../color.h"
#include "../leds.h"

void run(unsigned char *running, struct led_strip_t *leds) {
    // Preconditions
    assert(running);
    assert(leds);

    // No LEDs
    if (*(leds->size) == 0) {
        fprintf(stderr, "No LEDs found\n");
        return;
    }

    // Color
    color_t color = WHITE;
    for (int i = 0; i < *(leds->size); i++) {
        *(leds->leds[i].color) = color;
    }

    // If render fails
    if (!leds_render(leds)) {
        return;
    }

    while (*running) {
        // Nothing
    }
}
