#ifndef SLICER_MATH_VEC3_H
#define SLICER_MATH_VEC3_H

#include "slicer/core/types.h"

Vec3_t slicer_vec3_create(float x, float y, float z);
Vec3_t slicer_vec3_add(Vec3_t a, Vec3_t b);
Vec3_t slicer_vec3_subtract(Vec3_t a, Vec3_t b);
Vec3_t slicer_vec3_scale(Vec3_t v, float scalar);
float slicer_vec3_dot(Vec3_t a, Vec3_t b);
Vec3_t slicer_vec3_cross(Vec3_t a, Vec3_t b);
float slicer_vec3_length(Vec3_t v);
Vec3_t slicer_vec3_normalize(Vec3_t v);
float slicer_vec3_distance(Vec3_t a, Vec3_t b);

#endif /* SLICER_MATH_VEC3_H */
