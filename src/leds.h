#pragma once

#include <ws2811/ws2811.h>

#include "color.h"
#include "vector.h"

/*
 * A LED with color and positional data.
 */
struct led_t {
    color_t *color;
    struct vector_t *position;
};

/*
 * A LED strip.
 * Note that the implementation struct is not intended to be used.
 */
struct led_strip_t {
    int *size;
    struct led_t *leds;
    ws2811_t *impl;
};

/*
 * Create a LED strip for a given implementation.
 * Note that this returns NULL if no space could be allocated.
 */
struct led_strip_t *leds_wrap(ws2811_t *impl);

/*
 * Destroy a LED strip.
 * Note that the implementation struct is not destroyed.
 */
void leds_unwrap(struct led_strip_t *leds);

/*
 * Render a LED strip.
 * Note that this returns 0 if the render fails.
 */
int leds_render(struct led_strip_t *leds);

/*
 * Turn of all LEDs in a LED strip.
 * Note that this does not render the strip.
 */
void leds_clear(struct led_strip_t *leds);
