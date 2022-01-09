#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "effect.h"

#include "../color.h"
#include "../leds.h"
#include "../random.h"

#define SPEED 0.01F
#define GRADIENT 0.25F
#define PERIOD_MICROSECONDS(leds) (1000000 / 10)

static color_t _color_average_weighted(color_t c0, float w0, color_t c1, float w1) {
    // Preconditions
    assert(isfinite(w0) && w0 >= 0.0F);
    assert(isfinite(w1) && w1 >= 0.0F);

    float f = w0 + w1;

    color_t r0 = color_multiply(c0, f == 0.0F ? 0.0F : (w0 / f));
    color_t r1 = color_multiply(c1, f == 0.0F ? 0.0F : (w1 / f));
    return color_average(r0, r1);
}

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
    
    float gradient_d2 = GRADIENT / 2.0F;
    float height = 0.0F - gradient_d2;
    int direction = 1;
    color_t c0 = random_next_color_brightness_random();
    color_t c1 = BLACK;

    while (*running) {
        for (int i = 0; i < *(leds->size); i++) {
            struct led_t *led = &(leds->leds[i]);

            float delta = led->position->y - height;

            color_t color;
            if (delta < -gradient_d2) {
                color = c0;
            }
            else if (delta > gradient_d2) {
                color = c1;
            }
            else {
                delta += gradient_d2;
                color = _color_average_weighted(c0, GRADIENT - delta, c1, delta);
            }
 
            *(led->color) = color;
        }

        // Advance visor
        // This is jank
        height += direction ? SPEED : (-SPEED);
        visor_advance:
        if (height > 1.0F + gradient_d2) {
            height = 2.0F + GRADIENT - height;
            direction = 0;
            c1 = random_next_color_brightness_random();
            goto visor_advance;
        }
        if (height < -gradient_d2) {
            height = fabsf(height) - gradient_d2;
            direction = 1;
            c0 = random_next_color_brightness_random();
            goto visor_advance;
        }

        // If render fails
        if (!leds_render(leds)) {
            break;
        }

        // Ignore return value
        usleep(PERIOD_MICROSECONDS(*(leds->size)));
    }
}
