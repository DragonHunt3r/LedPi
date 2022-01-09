#include <assert.h>
#include <stdio.h>

#include "effect.h"

#include "../leds.h"

void run(unsigned char *running, struct led_strip_t *leds) {
    // Preconditions
    assert(running);
    assert(leds);

    fprintf(stderr, "No effect set\n");
}
