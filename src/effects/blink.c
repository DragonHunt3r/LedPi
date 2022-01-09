#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "effect.h"

#include "../color.h"
#include "../leds.h"
#include "../vector.h"

#define SECONDS_PER_BLINK 1
#define PERIOD_MICROSECONDS(leds) (1000000L * SECONDS_PER_BLINK)

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

    int i = 0;
    while (*running && i < *(leds->size)) {
        // Clear previous LED
        if (i != 0) {
            *(leds->leds[i - 1].color) = BLACK;
        }

        struct led_t *led = &(leds->leds[i]);
        struct vector_t *vector = led->position;
        printf("LED: %d/%d @ (%.2f, %.2f, %.2f)\n", i + 1, *(leds->size), vector->x, vector->y, vector->z);
        *(led->color) = WHITE;

        // If render fails
        if (!leds_render(leds)) {
            break;
        }

        // Ignore return value
        // Note that this sleep also happens at the last light
        // This is not optimal Sadge
        usleep(PERIOD_MICROSECONDS(*(leds->size)));

        i++;
    }

    // Cleanup
    *(leds->leds[i - 1].color) = BLACK;
    leds_render(leds);
}
