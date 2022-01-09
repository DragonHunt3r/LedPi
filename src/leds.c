#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "leds.h"

struct led_strip_t *leds_wrap(ws2811_t *impl) {
    // Preconditions
    assert(impl);

    struct led_strip_t *leds = malloc(sizeof(*leds));

    // Could not allocate
    if (!leds) {
        return NULL;
    }

    leds->size = &(impl->channel[0].count);
    leds->leds = calloc(*(leds->size), sizeof(*leds->leds));
    leds->impl = impl;
    
    // Could not allocate
    if (!leds->leds) {
        free(leds);
        return NULL;
    }

    for (int i = 0; i < *(leds->size); i++) {
        struct led_t *led = &(leds->leds[i]);
        led->color = &(leds->impl->channel[0].leds[i]);
        led->position = NULL;
        *(led->color) = BLACK;
    }

    return leds;
}

void leds_unwrap(struct led_strip_t *leds) {
    // Nothing to deallocate
    if (!leds) {
        return;
    }

    // Size and LED color/position are not managed by this struct.

    free(leds->leds);
    free(leds);
}

int leds_render(struct led_strip_t *leds) {
    // Preconditions
    assert(leds);
    
    ws2811_return_t result = ws2811_render(leds->impl);

    // Render success
    if (result == WS2811_SUCCESS) {
        return 1;
    }

    fprintf(stderr, "Could not render LEDs: %s\n", ws2811_get_return_t_str(result));
    return 0;
}

void leds_clear(struct led_strip_t *leds) {
    // Preconditinos
    assert(leds);

    for (int i = 0; i < *(leds->size); i++) {
        *(leds->leds[i].color) = BLACK;
    }
}
