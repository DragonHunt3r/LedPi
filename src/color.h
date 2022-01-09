#pragma once

#include <stdint.h>

#include <ws2811/ws2811.h>

/*
 * A color in sRGB color space, 0xFFRRGGBB.
 */
typedef ws2811_led_t color_t;

/*
 * A black color.
 */
const color_t BLACK;

/*
 * A red color.
 */
const color_t RED;

/*
 * A green color.
 */
const color_t GREEN;

/*
 * A blue color.
 */
const color_t BLUE;

/*
 * A yellow color.
 */
const color_t YELLOW;

/*
 * An aqua color.
 */
const color_t AQUA;

/*
 * A purple color.
 */
const color_t PURPLE;

/*
 * A white color.
 */
const color_t WHITE;

/*
 * Create a color from rgb components.
 * Note that the components should range [0, 255].
 */
color_t color_from_rgb(uint8_t r, uint8_t g, uint8_t b);

/*
 * Create a color from rgb.
 * Note that the value should range [0, 16777215].
 */
color_t color_from_value(uint32_t rgb);

/*
 * Create a color from rgb components.
 * Note that the components should range [0, 1].
 */
color_t color_from_float(float r, float g, float b);

/*
 * Create a color from hsl components.
 * Note that the saturation and lightness should range [0, 1].
 *
 */
color_t color_from_hsl(float hue, float saturation, float lightness);

/*
 * Create a color from hsv components.
 * Note that the saturation and value should range [0, 1].
 */
color_t color_from_hsv(float hue, float saturation, float value);

/*
 * Get the mathematical average color from two colors.
 */
color_t color_average(color_t c1, color_t c2);

/*
 * Multiply a color by some scalar.
 * Note that values are automatically clamped.
 */
color_t color_multiply(color_t color, float factor);
