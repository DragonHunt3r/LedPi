#include <assert.h>
#include <math.h>
#include <stdint.h>

#include "color.h"

const color_t BLACK = 0x000000;
const color_t RED = 0xFF0000;
const color_t GREEN = 0x00FF00;
const color_t BLUE = 0x0000FF;
const color_t YELLOW = 0xFFFF00;
const color_t AQUA = 0x00FFFF;
const color_t PURPLE = 0xFF00FF;
const color_t WHITE = 0xFFFFFF;

static float _clamp(float f, float min, float max) {
    // Preconditions
    assert(isfinite(f));
    assert(isfinite(min));
    assert(isfinite(max));
    assert(min <= max);

    return fminf(max, fmaxf(min, f));
}

static float _clamp_unit(float f) {
    return _clamp(f, 0.0F, 1.0F);
}

color_t color_from_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return color_from_value(r << 16 | g << 8 | b);
}

color_t color_from_value(uint32_t rgb) {
    // Preconditions
    assert(rgb <= 0xFFFFFF);

    return (color_t) rgb;
}

color_t color_from_float(float r, float g, float b) {
    // Preconditions
    assert(r >= 0.0F && r <= 1.0F);
    assert(g >= 0.0F && g <= 1.0F);
    assert(b >= 0.0F && b <= 1.0F);

    return color_from_rgb((uint8_t) round(r * 255.0F), (uint8_t) round(g * 255.0F), (uint8_t) round(b * 255.0F));
}

color_t color_from_hsl(float hue, float saturation, float lightness) {
    // Preconditions
    assert(isfinite(hue));
    assert(saturation >= 0.0F && saturation <= 1.0F);
    assert(lightness >= 0.0F && lightness <= 1.0F);

    // Ensure range [0, 360]
    hue = fmodf(hue, 360.0F);
    if (hue < 0.0F) {
        hue += 360.0F;
    }

    // Convert to hsv
    float value = lightness + saturation * fminf(lightness, 1 - lightness);

    return color_from_hsv(hue, value == 0.0F ? 0.0F : (2.0F * (1 - lightness / value)), value);
}

color_t color_from_hsv(float hue, float saturation, float value) {
    // Preconditions
    assert(isfinite(hue));
    assert(saturation >= 0.0F && saturation <= 1.0F);
    assert(value >= 0.0F && value <= 1.0F);

    // Ensure range [0, 360]
    hue = fmodf(hue, 360.0F);
    if (hue < 0.0F) {
        hue += 360.0F;
    }

    float chroma = saturation * value;

    float f0 = hue / 60.0F;
    float f1 = chroma * (1.0F - fabs(fmod(f0, 2.0F) - 1.0F));
    float f2 = value - chroma;
    
    float r;
    float g;
    float b;
    if (f0 < 1.0F) {
        r = chroma;
	g = f1;
	b = 0.0F;
    }
    else if (f0 < 2.0F) {
        r = f1;
	g = chroma;
	b = 0.0F;
    }
    else if (f0 < 3.0F) {
        r = 0.0F;
	g = chroma;
	b = f1;
    }
    else if (f0 < 4.0F) {
        r = 0.0F;
	g = f1;
	b = chroma;
    }
    else if (f0 < 5.0F) {
        r = f1;
	g = 0.0F;
	b = chroma;
    }
    else {
        r = chroma;
	g = 0.0F;
	b = f1;
    }

    return color_from_float(r + f2, g + f2, b + f2);
}

color_t color_average(color_t c1, color_t c2) {
    // Preconditions
    assert(c1 <= 0xFFFFFF);
    assert(c2 <= 0xFFFFFF);

    float r1 = ((c1 & 0xFF0000) >> 16) / 255.0F;
    float g1 = ((c1 & 0x00FF00) >> 8) / 255.0F;
    float b1 = (c1 & 0x0000FF) / 255.0F;
    float r2 = ((c2 & 0xFF0000) >> 16) / 255.0F;
    float g2 = ((c2 & 0x00FF00) >> 8) / 255.0F;
    float b2 = (c2 & 0x0000FF) / 255.0F;

    // Clamp is necessary for floating point errors
    float r = _clamp_unit(sqrtf(r1 * r1 + r2 * r2 / 2.0F));
    float g = _clamp_unit(sqrtf(g1 * g1 + g2 * g2 / 2.0F));
    float b = _clamp_unit(sqrtf(b1 * b1 + b2 * b2 / 2.0F));

    return color_from_float(r, g, b);
}

color_t color_multiply(color_t color, float factor) {
    // Preconditions
    assert(color <= 0xFFFFFF);
    assert(factor >= 0.0F);

    float r = ((color & 0xFF0000) >> 16) / 255.0F;
    float g = ((color & 0x00FF00) >> 8) / 255.0F;
    float b = (color & 0x0000FF) / 255.0F;

    // Clamp
    r = _clamp_unit(sqrtf(r * r * factor));
    g = _clamp_unit(sqrtf(g * g * factor));
    b = _clamp_unit(sqrtf(b * b * factor));

    return color_from_float(r, g, b);
}
