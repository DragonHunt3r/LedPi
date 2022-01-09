#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "color.h"
#include "random.h"
#include "vector.h"

static int init = 0;
static unsigned int seed = 0;

void random_init(void) {

    // Random seed
    seed = (unsigned int) time(NULL);
    random_set_seed(seed);

    // Duplicate call
    if (init) {
        fprintf(stderr, "Duplicate random_init() (%d)\n", init);
    }
    else {
        printf("Random seed: %u\n", seed);
        if (RAND_MAX != INT_MAX) {
            printf("RAND_MAX is different from INT_MAX: %d, %d\n", RAND_MAX, INT_MAX);
        }
    }


    // Initialization flag
    init++;
}

void random_set_seed(unsigned int seed) {
    SRAND(seed);
}

int random_next_bool(void) {
    // Preconditions
    assert(init);

    return random_next_float() < 0.5F ? 1 : 0;
}

float random_next_float(void) {
    // Preconditions
    assert(init);

    return (float) RAND / (float) RAND_MAX;
}

float random_next_float_bounded(float min, float max) {
    // Preconditions
    assert(init);
    assert(min < max);

    return random_next_float() * (max - min) + min;
}

int random_next_int(void) {
    // Preconditions
    assert(init);

    return RAND;
}

int random_next_int_bounded(int min, int max) {
    // Preconditions
    assert(init);
    assert(min < max);

    int random = random_next_int();

    int i = max - min;
    int j = i - 1;
    if ((i & j) == 0) {
        return (random & j) + min;
    }
    if (i > 0) {
        for (int k = random >> 1; k + j - (random = k % i) < 0; k = random_next_int() >> 1) {
            // Nothing
        }
        return random + min;
    }
    while (random < min || random >= max) {
        random = random_next_int();
    }
    return random;
}

color_t random_next_color(void) {
    // Preconditions
    assert(init);

    return random_next_color_brightness_set(1.0F);
}

color_t random_next_color_brightness_set(float brightness) {
    // Preconditions
    assert(init);
    assert(brightness >= 0.0F && brightness <= 1.0F);

    return color_from_hsv(random_next_float_bounded(0.0F, 360.0F), 1.0F, brightness);
}

color_t random_next_color_brightness_random(void) {
    // Preconditions
    assert(init);

    return random_next_color_brightness_set(random_next_float());
}

void random_next_vector(struct vector_t *vector) {
    // Preconditions
    assert(init);
    assert(vector);

    vector->x = random_next_float_bounded(-1.0F, 1.0F);
    vector->y = random_next_float_bounded(-1.0F, 1.0F);
    vector->z = random_next_float_bounded(-1.0F, 1.0F);
    vector_normalize(vector);
}
