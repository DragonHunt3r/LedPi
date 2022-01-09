#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "effect.h"

#include "../color.h"
#include "../leds.h"
#include "../random.h"

#define GAP 50
#define LENGTH 25
#define PERIOD_MICROSECONDS(leds) (1000000 / 6)

struct snake_t {
    float color;
    int position;
};

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

    // Start positions
    int numsnakes = *(leds->size) / (LENGTH + GAP) + 1;
    struct snake_t snakes[numsnakes];
    for (int i = 0; i < numsnakes; i++) {
        snakes[i].color = random_next_float_bounded(0.0F, 360.0F);
        snakes[i].position = -i * (LENGTH + GAP);
    }

    while (*running) {
        // Clear LEDs
        // Technically only the last light of each snake needs to be cleared
        // Not optimal Sadge
        leds_clear(leds);

        for (int i = 0; i < numsnakes; i++) {
            for (int j = snakes[i].position; j >= 0 && j > snakes[i].position - LENGTH; j--) {
                // Not visible
                if (j < 0 || j >= *(leds->size)) {
                    continue;
                }

                // Color with brightness ~ distance to snake head
                *(leds->leds[j].color) = color_from_hsv(snakes[i].color, 1.0F, (LENGTH - snakes[i].position + j) / ((float) LENGTH));
            }
        }

        // If render fails
        if (!leds_render(leds)) {
            break;
        }

        // Advance snakes
        for (int i = 0; i < numsnakes; i++) {
            // Wrap around
            if (++(snakes[i].position) == *(leds->size) + LENGTH) {
                struct snake_t previous = snakes[(i == 0 ? numsnakes : i) - 1];
                snakes[i].color = random_next_float_bounded(0.0F, 360.0F);
                snakes[i].position = previous.position - LENGTH - GAP;
            }
        }

        // Ignore return value
        usleep(PERIOD_MICROSECONDS(*(leds->size)));
    }
}
