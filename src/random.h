#pragma once

#include "color.h"
#include "vector.h"

// Overridable definitions
#ifndef SRAND
#define SRAND(seed) srand(seed)
#endif
#ifndef RAND
#define RAND rand()
#endif

/*
 * Initialise random number generator.
 */
void random_init(void);

/*
 * Set random number generator seed.
 */
void random_set_seed(unsigned int seed);

/*
 * Generate a boolean.
 */
int random_next_bool(void);

/*
 * Generate a float in range [0, 1].
 */
float random_next_float(void);

/*
 * Generate a float in range [min, max].
 * Note that min must be smaller than max.
 */
float random_next_float_bounded(float min, float max);

/*
 * Generate an int in range [0, RAND_MAX].
 */
int random_next_int(void);

/*
 * Generate an int in range [min, max].
 */
int random_next_int_bounded(int min, int max);

/*
 * Generate a random full bright color.
 */
color_t random_next_color(void);

/*
 * Generate a random color with given brightness.
 * Note that brightness must be in range [0, 1].
 */
color_t random_next_color_brightness_set(float brightness);

/*
 * Generate a random color with random brightness.
 */
color_t random_next_color_brightness_random(void);

/*
 * Generate a random normalized vector.
 */
void random_next_vector(struct vector_t *vector);
