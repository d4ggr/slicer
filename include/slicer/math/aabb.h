#ifndef SLICER_MATH_AABB_H 
#define SLICER_MATH_AABB_H
#include <stdbool.h>
#include "slicer/core/types.h"

AABB_t aabb_create_invalid();
void aabb_init_invalid(AABB_t* box);
void aabb_expand_point(AABB_t* box, Vec3_t point);
void aabb_merge(AABB_t* dest, const AABB_t* src);
bool aabb_contains_point(const AABB_t* box, Vec3_t point);
bool aabb_is_valid(const AABB_t* box);
Vec3_t aabb_center(const AABB_t* box);
Vec3_t aabb_size(const AABB_t* box);

#endif /* SLICER_MATH_AABB_H */
