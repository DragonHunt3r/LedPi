#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include "effect.h"

#include "../leds.h"
#include "../random.h"
#include "../vector.h"

#define RADIUS 0.15F
#define SPEED 0.05F
#define PERIOD_MICROSECONDS(leds) (1000000 / 10)

static color_t _random_next_color(void) {
    return color_from_hsv(random_next_float_bounded(0.0F, 360.0F), random_next_float(), random_next_float_bounded(0.25F, 1.0F));
}

static int _plane_in_box(const struct vector_t *p, const struct vector_t *n) {
    // Preconditions
    assert(p);
    assert(n);

    struct vector_t center = {
        .x = 0.5F,
        .y = 0.5F,
        .z = 0.5F
    };

    float pir = fabsf(n->x) * 0.5F + fabsf(n->y) * 0.5F + fabsf(n->z) * 0.5F;
    float s = vector_dot(n, &center) - vector_dot(n, p);
    return fabsf(s) <= pir;
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

    color_t color = _random_next_color();
    struct vector_t dir;
    random_next_vector(&dir);
    struct vector_t pos = {
        .x = -1.0F,
        .y = -1.0F,
        .z = -1.0F
    };

    while (*running) {
        struct vector_t v0 = {
            .x = 0.0F,
            .y = 0.0F,
            .z = 0.0F
        };
        vector_add(&v0, &dir);
        vector_multiply_scalar(&v0, RADIUS);
        vector_add(&v0, &pos);
        int i0 = _plane_in_box(&v0, &dir);
        vector_multiply(&v0, &ORIGIN);
        vector_subtract(&v0, &dir);
        vector_multiply_scalar(&v0, RADIUS);
        vector_add(&v0, &pos);
        int i1 = _plane_in_box(&v0, &dir);

        // If not in box generate new
        if (!i0 && !i1) {
            color = _random_next_color();
            random_next_vector(&dir);
            pos.x = dir.x < 0.0F ? 1.0F : 0.0F;
            pos.y = dir.y < 0.0F ? 1.0F : 0.0F;
            pos.z = dir.z < 0.0F ? 1.0F : 0.0F;
            struct vector_t v1 = {
                .x = 0.0F,
                .y = 0.0F,
                .z = 0.0F,
            };
            vector_subtract(&v1, &dir);
            vector_multiply_scalar(&v1, RADIUS);
            vector_add(&pos, &v1);
        }

        for (int i = 0; i < *(leds->size); i++) {
            struct led_t led = leds->leds[i];
            *(led.color) = (fabsf(vector_dot(&dir, led.position) - vector_dot(&dir, &pos)) <= RADIUS) ? color : BLACK;
        }

        // If render fails
        if (!leds_render(leds)) {
            break;
        }

        // Advance plane
        v0.x = dir.x * SPEED;
        v0.y = dir.y * SPEED;
        v0.z = dir.z * SPEED;
        vector_add(&pos, &v0);

        // Ignore return value
        usleep(PERIOD_MICROSECONDS(*(leds->size)));
    }
}
