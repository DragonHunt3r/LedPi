#pragma once

#include <stdio.h>

#include "vector.h"

/*
 * A vector list with a given size and array of vectors.
 */
struct vectors_t {
    int size;
    struct vector_t *array;
};

/*
 * Create a vector list with a given size.
 * Note that this returns NULL if the list could not be created.
 * Note that even though size must be positive, it is a signed int for internal reasons.
 */
struct vectors_t *vectors_create(int size);

/*
 * Destroy a vector list.
 */
void vectors_destroy(struct vectors_t *list);

/*
 * Read and parse a coordinate list from a stream.
 * Note that this returns NULL if a vector list could not be created.
 * Note that the endianness of the stream should match the endianness of the architecture this code runs on.
 * Note that trailing data may be silently ignored depending on the implementation.
 * Stream format:
 *  - Number of vectors (4 bytes)
 *  - Vector0 x (2 bytes)
 *  - Vector0 y (2 bytes)
 *  - Vector0 z (2 bytes)
 *  - ...
 *  - VectorN x (2 bytes)
 *  - VectorN y (2 bytes)
 *  - VectorN z (2 bytes)
 */
struct vectors_t * vectors_read(FILE *file);
