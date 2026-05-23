#include "slicer/math/vec2.h"
#include "slicer/math/numeric.h"
#include "slicer/core/constants.h"
#include <math.h>

Vec2_t slicer_vec2_create(float x, float y) {
    Vec2_t v;
    v.x = x;
    v.y = y;
    return v;
}

Vec2_t slicer_vec2_add(Vec2_t a, Vec2_t b) {
    return slicer_vec2_create(a.x + b.x, a.y + b.y);
}

Vec2_t slicer_vec2_subtract(Vec2_t a, Vec2_t b) {
    return slicer_vec2_create(a.x - b.x, a.y - b.y);
}

Vec2_t slicer_vec2_scale(Vec2_t v, float scalar) {
    return slicer_vec2_create(v.x * scalar, v.y * scalar);
}

float slicer_vec2_dot(Vec2_t a, Vec2_t b) {
    return a.x * b.x + a.y * b.y;
}

float slicer_vec2_length(Vec2_t v) {
    return sqrtf(slicer_vec2_dot(v, v));
}

Vec2_t slicer_vec2_normalize(Vec2_t v) {
    float len = slicer_vec2_length(v);
    if (len < SLICER_EPS_NORMALIZE) {
        return slicer_vec2_create(0.0f, 0.0f);
    }
    return slicer_vec2_scale(v, 1.0f / len);
}

float slicer_vec2_distance(Vec2_t a, Vec2_t b) {
    return slicer_vec2_length(slicer_vec2_subtract(a, b));
}
