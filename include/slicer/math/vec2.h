#ifndef SLICER_MATH_VEC2_H
#define SLICER_MATH_VEC2_H

#include "slicer/core/types.h"

Vec2_t slicer_vec2_create(float x, float y);
Vec2_t slicer_vec2_add(Vec2_t a, Vec2_t b);
Vec2_t slicer_vec2_subtract(Vec2_t a, Vec2_t b);
Vec2_t slicer_vec2_scale(Vec2_t v, float scalar);
float slicer_vec2_dot(Vec2_t a, Vec2_t b);
float slicer_vec2_length(Vec2_t v);
Vec2_t slicer_vec2_normalize(Vec2_t v);
float slicer_vec2_distance(Vec2_t a, Vec2_t b);

#endif /* SLICER_MATH_VEC2_H */
