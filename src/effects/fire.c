#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include "effect.h"

#include "../color.h"
#include "../leds.h"
#include "../random.h"

#define MAX_BRIGHTNESS (50.0F / 255.0F)
#define SPEED 0.4F
#define RADIUS_SQ 0.05F
#define NUM_PARTICLES 50
#define PERIOD_MICROSECONDS(leds) (1000000 / 10)

struct particle_t {
    float x;
    float y;
    float dx;
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

    struct particle_t particles[NUM_PARTICLES];
    for (int i = 0; i < NUM_PARTICLES; i++) {
        particles[i].x = random_next_float() * M_PI * 2.0F;
        particles[i].y = random_next_float() * 2.0F - 0.5F;
        particles[i].dx = (random_next_float() - 0.5F) * 0.1F;
    }

    while (*running) {
        for (int i = 0; i < *(leds->size); i++) {
            struct led_t *led = &(leds->leds[i]);
            color_t color;
            float theta = atan2f(led->position->x, led->position->z);
            float y = led->position->y;
            float result = y * 0.5F;
            for (int j = 0; j < NUM_PARTICLES; j++) {
                struct particle_t particle = particles[j];
                float scale = particle.y + 1.0F;
                float dx = fmodf(particle.x + M_PI - theta, 2.0F * M_PI) - M_PI;
                float dy = particle.y - y;
                float f = (dx * dx + dy * dy) / (RADIUS_SQ * scale * scale);
                if (f < 1.0F) {
                    result += ((1.0F - f * f) * particle.y);
                }
            }
            float brightness = 1.0F - fminf(fmaxf(result - 0.2F, 0.0F), 1.0F);
            if (brightness > 0.975F) {
                color = color_from_float(MAX_BRIGHTNESS, MAX_BRIGHTNESS, /*0.0F*/ MAX_BRIGHTNESS * ((brightness - 0.975F) / 0.1F));
            }
            else if (brightness > 0.85F) {
                color = color_from_float(MAX_BRIGHTNESS, MAX_BRIGHTNESS * ((brightness - 0.85F) / 0.125F), 0.0F);
            }
            else {
                color = color_from_float(MAX_BRIGHTNESS * (brightness / 0.85F), 0.0F, 0.0F);
            }

            *(led->color) = color;
        }

        // If render fails
        if (!leds_render(leds)) {
            break;
        }

        // Advance particles
        for (int i = 0; i < NUM_PARTICLES; i++) {
            particles[i].y += 0.1F * SPEED;
            particles[i].x += particles[i].dx * SPEED;
            if (particles[i].y > 1.5F) {
                particles[i].y -= 2.0F;
            }
        }

        // Ignore return value
        usleep(PERIOD_MICROSECONDS(*(leds->size)));
    }
}
