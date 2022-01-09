#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include "effect.h"

#include "../color.h"
#include "../leds.h"
#include "../random.h"
#include "../vector.h"

#define FLAKES 50
#define SPEED_MIN 0.01F
#define SPEED_MAX 0.05F
#define RADIUS 0.05F
#define DECAY_SHIFT 7.2F
#define DECAY_FADE 0.05F
#define PERIOD_MICROSECONDS(leds) (1000000 / 10)

// TODO: Proper snowfall

struct flake_t {
    float color;
    struct vector_t position;
    float speed;
    //float rad;
};

struct datum_t {
    float shift;
    float fade;
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

    struct flake_t flakes[FLAKES];
    for (int i = 0; i < FLAKES; i++) {
        flakes[i].color = random_next_float_bounded(0.0F, 360.0F);
        flakes[i].position.x = random_next_float();
        flakes[i].position.y = 1.0F + RADIUS;
        flakes[i].position.z = random_next_float();
        flakes[i].speed = (-random_next_float() * (SPEED_MAX - SPEED_MIN)) - SPEED_MIN;
        //flakes[i].rad = 0.0F;
    }
    struct datum_t data[*(leds->size)];
    for (int i = 0; i < *(leds->size); i++) {
        data[i].shift = 0.0F;
        data[i].fade = 0.0F;
    }

    while (*running) {
        for (int i = 0; i < *(leds->size); i++) {
            struct led_t led = leds->leds[i];

            float best = RADIUS * RADIUS;
            float color = -1.0F;
            float brightness = 1.0F;
            for (int j = 0; j < FLAKES; j++) {
                struct flake_t flake = flakes[j];
                float distance = vector_distance_squared(led.position, &(flake.position));
                if (distance <= best) {
                    best = distance;
                    color = flake.color;
                    data[i].shift = color;
                    data[i].fade = 1.0F;
                }
            }

            // Fallback on trail color if no flake is nearby
            if (color == -1.0F) {
                color = data[i].shift;
                brightness = data[i].fade;
            }

            *(led.color) = color_from_hsv(color, 1.0F, brightness);
        }

        // If render fails
        if (!leds_render(leds)) {
            break;
        }

        // Advance flakes
        for (int i = 0; i < FLAKES; i++) {
            //flakes[i].position.x += cosf(flakes[i].rad);
            flakes[i].position.y += flakes[i].speed;

            // Reset to top of the screen
            if (flakes[i].position.y < -RADIUS || flakes[i].position.x < -RADIUS || flakes[i].position.x > 1.0F + RADIUS || flakes[i].position.z < -RADIUS || flakes[i].position.z > 1.0F + RADIUS) {
                flakes[i].color = random_next_float_bounded(0.0F, 360.0F);
                flakes[i].position.x = random_next_float();
                flakes[i].position.y = 1.0F + RADIUS;
                flakes[i].position.z = random_next_float();

                // We do not really need to change speed
            }
        }

        // Advance fade
        for (int i = 0; i < *(leds->size); i++) {
            data[i].shift = fmodf(data[i].shift + DECAY_SHIFT, 360.0F);
            data[i].fade = fmaxf(data[i].fade - DECAY_FADE, 0.0F);
        }

        // Ignore return value
        usleep(PERIOD_MICROSECONDS(*(leds->size)));
    }
}
