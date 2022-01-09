#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coords.h"
#include "vector.h"

static void _normalize_vectors(struct vectors_t *list) {
    // Preconditions
    assert(list);

    // Bounding box
    struct vector_t min = {
        .x = INFINITY,
        .y = INFINITY,
        .z = INFINITY
    };
    struct vector_t max = {
        .x = -INFINITY,
        .y = -INFINITY,
        .z = -INFINITY
    };

    // Add all vectors to the bounding box
    for (int i = 0; i < list->size; i++) {
        struct vector_t *vector = &(list->array[i]);
        vector_min(&min, vector);
        vector_max(&max, vector);
    }

    struct vector_t diff = {
        .x = max.x - min.x,
        .y = max.y - min.y,
        .z = max.z - min.z
    };

    // Fix divide by zero
    if (diff.x == 0.0F) {
        diff.x = 1.0F;
    }
    if (diff.y == 0.0F) {
        diff.y = 1.0F;
    }
    if (diff.z == 0.0F) {
        diff.z = 1.0F;
    }

    for (int i = 0; i < list->size; i++) {
        struct vector_t *vector = &(list->array[i]);
        vector_subtract(vector, &min);
        vector_divide(vector, &diff);

        // In pictures (0, 0) correlates to the top left corner
        // The vectors parsed are based on screenshot coordinates
        // This means y needs to be inverted
        vector->y = 1.0F - vector->y;
    }
}

struct vectors_t *vectors_create(int size) {
    // Preconditions
    assert(size >= 0);

    struct vectors_t *list = malloc(sizeof(*list));

    // Could not allocate
    if (!list) {
        return NULL;
    }

    list->size = size;
    list->array = calloc(size, sizeof(*(list->array)));

    // Could not allocate
    if (!list->array) {
        free(list);
        return NULL;
    }

    return list;
}

void vectors_destroy(struct vectors_t *list) {
    // Nothing to deallocate
    if (!list) {
        return;
    }

    free(list->array);
    free(list);
}

struct vectors_t *vectors_read(FILE *file) {
    // Preconditions
    assert(file);

    // Read size
    uint32_t size0;
    if (fread(&size0, sizeof(uint32_t), 1, file) != 1) {
        fprintf(stderr, "Could not read coord list size: %s\n", strerror(errno));
        return NULL;
    }
    int size = (int) size0;
    if (size < 0) {
        fprintf(stderr, "Invalid coordinate data: Negative size\n");
        return NULL;
    }

    struct vectors_t *list = vectors_create(size);
    if (!list) {
        fprintf(stderr, "Could not allocate space for vector list\n");
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        // Read xyz as int (pixel coordinates)
        uint16_t coords[3];
        if (fread(&coords, sizeof(uint16_t), 3, file) != 3) {
            fprintf(stderr, "Could not read vector for LED %d: %s\n", i, strerror(errno));
            vectors_destroy(list);
            return NULL;
        }
 
        struct vector_t *vector = &(list->array[i]);
        vector->x = (float) coords[0];
        vector->y = (float) coords[1];
        vector->z = (float) coords[2];
    }

    // Note that trailing data is ignored alltogether

    // Normalize list to range [0, 1]
    _normalize_vectors(list);

    return list;
}
