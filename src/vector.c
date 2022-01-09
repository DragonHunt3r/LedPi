#include <assert.h>
#include <math.h>

#include "vector.h"

#ifndef EPSILON
#define EPSILON 0.000001F
#endif

const struct vector_t ORIGIN = {
    .x = 0.0F,
    .y = 0.0F,
    .z = 0.0F
};

const struct vector_t UNIT_X = {
    .x = 1.0F,
    .y = 0.0F,
    .z = 0.0F
};

const struct vector_t UNIT_Y = {
    .x = 0.0F,
    .y = 1.0F,
    .z = 0.0F
};

const struct vector_t UNIT_Z = {
    .x = 0.0F,
    .y = 0.0F,
    .z = 1.0F
};

void vector_add(struct vector_t *v1, const struct vector_t *v2) {
    // Preconditions
    assert(v1);
    assert(v2);

    v1->x += v2->x;
    v1->y += v2->y;
    v1->z += v2->z;
}

void vector_subtract(struct vector_t *v1, const struct vector_t *v2) {
    // Preconditions
    assert(v1);
    assert(v2);

    v1->x -= v2->x;
    v1->y -= v2->y;
    v1->z -= v2->z;
}

void vector_multiply(struct vector_t *v1, const struct vector_t *v2) {
    // Preconditions
    assert(v1);
    assert(v2);

    v1->x *= v2->x;
    v1->y *= v2->y;
    v1->z *= v2->z;
}

void vector_multiply_scalar(struct vector_t *vector, float scalar) {
    // Preconditions
    assert(vector);
    assert(isfinite(scalar));

    struct vector_t v = {
        .x = scalar,
        .y = scalar,
        .z = scalar
    };

    vector_multiply(vector, &v);
}

void vector_divide(struct vector_t *v1, const struct vector_t *v2) {
    // Preconditions
    assert(v1);
    assert(v2);

    v1->x /= v2->x;
    v1->y /= v2->y;
    v1->z /= v2->z;
}

void vector_min(struct vector_t *v1, const struct vector_t *v2) {
    // Preconditions
    assert(v1);
    assert(v2);

    v1->x = fmin(v1->x, v2->x);
    v1->y = fmin(v1->y, v2->y);
    v1->z = fmin(v1->z, v2->z);
}

void vector_max(struct vector_t *v1, const struct vector_t *v2) {
    // Preconditions
    assert(v1);
    assert(v2);

    v1->x = fmax(v1->x, v2->x);
    v1->y = fmax(v1->y, v2->y);
    v1->z = fmax(v1->z, v2->z);
}

void vector_cross(struct vector_t *v1, const struct vector_t *v2) {
    // Preconditions
    assert(v1);
    assert(v2);

    float x = v1->x;
    float y = v1->y;
    float z = v1->z;

    v1->x = y * v2->z - z * v2->y;
    v1->y = z * v2->x - x * v2->z;
    v1->z = x * v2->y - y * v2->x;
}

float vector_dot(const struct vector_t *v1, const struct vector_t *v2) {
    // Preconditions
    assert(v1);
    assert(v2);

    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

void vector_rotate(struct vector_t *vector, float angle, const struct vector_t *axis) {
    // Preconditions
    assert(vector);
    assert(isfinite(angle));
    assert(axis);

    float x = vector->x;
    float y = vector->y;
    float z = vector->z;
    float xa = axis->x;
    float ya = axis->y;
    float za = axis->z;

    float cos = cosf(angle);
    float sin = sinf(angle);
    float dot = vector_dot(vector, axis);

    float xp = xa * dot * (1.0F - cos) + x * cos + (-za * y + ya * z) * sin;
    float yp = ya * dot * (1.0F - cos) + y * cos + (za * x - xa * z) * sin;
    float zp = za * dot * (1.0F - cos) + z * cos + (-ya * x + xa * y) * sin;

    vector->x = xp;
    vector->y = yp;
    vector->z = zp;
}

void vector_rotate_x(struct vector_t *vector, float angle) {
    vector_rotate(vector, angle, &UNIT_X);
}

void vector_rotate_y(struct vector_t *vector, float angle) {
    vector_rotate(vector, angle, &UNIT_Y);
}

void vector_rotate_z(struct vector_t *vector, float angle) {
    vector_rotate(vector, angle, &UNIT_Z);
}

float vector_distance(const struct vector_t *v1, const struct vector_t *v2) {
    return sqrtf(vector_distance_squared(v1, v2));
}

float vector_distance_squared(const struct vector_t *v1, const struct vector_t *v2) {
    // Preconditions
    assert(v1);
    assert(v2);

    float dx = v1->x - v2->x;
    float dy = v1->y - v2->y;
    float dz = v1->z - v2->z;

    return dx * dx + dy * dy + dz * dz;
}

int vector_is_normalized(const struct vector_t *vector) {
    return fabsf(vector_length_squared(vector) - 1.0F) <= EPSILON;
}

float vector_length(const struct vector_t *vector) {
    return sqrtf(vector_length_squared(vector));
}

float vector_length_squared(const struct vector_t *vector) {
    return vector_distance_squared(&ORIGIN, vector);
}

void vector_normalize(struct vector_t *vector) {
    // Preconditions
    assert(vector);

    float length = vector_length(vector);

    assert(length != 0.0F);

    struct vector_t v = {
        .x = length,
        .y = length,
        .z = length
    };

    vector_divide(vector, &v);
}
