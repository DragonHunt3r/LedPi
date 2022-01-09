#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "effect.h"

#include "../color.h"
#include "../leds.h"
#include "../vector.h"

#define COLORS 2
#define ROTATION_PER_TICK (2.0F * M_PI / 50.0F)
#define ROTATION_VERTICAL (1.5F * M_PI)
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

    // Color table
    color_t colors[COLORS] = {0};
    colors[0] = AQUA;
    colors[1] = PURPLE;

    // Tick rotation
    float rotation = 0.0F;

    while (*running) {
        for (int i = 0; i < *(leds->size); i++) {
            struct led_t *led = &(leds->leds[i]);
            struct vector_t vector = {
                .x = led->position->x - 0.5F,
                .y = 0.0F,
                .z = led->position->z - 0.5F
            };
            vector_rotate_y(&vector, -rotation - (led->position->y * ROTATION_VERTICAL));
            vector_normalize(&vector);
            float angle = M_PI - atan2f(vector.z, vector.x);
            uint8_t color = (uint8_t) (angle / ((M_PI * 2.0F) / COLORS));
            *(led->color) = colors[color % COLORS];
        }

        // If render fails
        if (!leds_render(leds)) {
            break;
        }

        // Advance rotation
        rotation = fmodf(rotation + ROTATION_PER_TICK, M_PI * 2.0F);

        // Ignore return value
        usleep(PERIOD_MICROSECONDS(*(leds->size)));
    }
}
