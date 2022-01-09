#pragma once

/*
 * A 3-dimensional vector.
 */
struct vector_t {
    float x;
    float y;
    float z;
};

/*
 * (0, 0, 0)
 */
const struct vector_t ORIGIN;

/*
 * (1, 0, 0)
 */
const struct vector_t UNIT_X;

/*
 * (0, 1, 0)
 */
const struct vector_t UNIT_Y;

/*
 * (0, 0, 1)
 */
const struct vector_t UNIT_Z;

/*
 * Add a vector to a given vector.
 */
void vector_add(struct vector_t *v1, const struct vector_t *v2);

/*
 * Subtract a vector from a given vector.
 */
void vector_subtract(struct vector_t *v1, const struct vector_t *v2);

/*
 * Multiply a given vector with a vector.
 */
void vector_multiply(struct vector_t *v1, const struct vector_t *v2);

/*
 * Multiply a given vector with a scalar.
 */
void vector_multiply_scalar(struct vector_t *vector, float scalar);

/*
 * Divide a given vector with a vector.
 */
void vector_divide(struct vector_t *v1, const struct vector_t *v2);

/*
 * Get the minimum components of two vectors.
 */
void vector_min(struct vector_t *v1, const struct vector_t *v2);

/*
 * Get the maximum components of two vectors.
 */
void vector_max(struct vector_t *v1, const struct vector_t *v2);

/*
 * Get the cross product of two vectors.
 */
void vector_cross(struct vector_t *v1, const struct vector_t *v2);

/*
 * Get the dot product of two vectors.
 */
float vector_dot(const struct vector_t *v1, const struct vector_t *v2);

/*
 * Rotate a given vector along an axis.
 */
void vector_rotate(struct vector_t *vector, float angle, const struct vector_t *axis);

/*
 * Rotate a given vector along the x-axis.
 */
void vector_rotate_x(struct vector_t *vector, float angle);

/*
 * Rotate a given vector along the y-axis.
 */
void vector_rotate_y(struct vector_t *vector, float angle);

/*
 * Rotate a given vector along the z-axis.
 */
void vector_rotate_z(struct vector_t *vector, float angle);

/*
 * Get the distance between two points.
 * Note that if possible, the squared variant should be used.
 */
float vector_distance(const struct vector_t *v1, const struct vector_t *v2);

/*
 * Get the distance squared between two points.
 */
float vector_distance_squared(const struct vector_t *v1, const struct vector_t *v2);

/*
 * Get if a given vector is normalized.
 */
int vector_is_normalized(const struct vector_t *vector);

/*
 * Get the magnitude of a given vector.
 * Note that if possible, the squared variant should be used.
 */
float vector_length(const struct vector_t *vector);

/*
 * Get the squared magnitude of a given vector.
 */
float vector_length_squared(const struct vector_t *vector);

/*
 * Normalize a given vector.
 */
void vector_normalize(struct vector_t *vector);
