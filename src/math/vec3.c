#include "slicer/math/vec3.h"
#include "slicer/math/numeric.h"
#include "slicer/core/constants.h"
#include <math.h>

Vec3_t slicer_vec3_create(float x, float y, float z) {
    Vec3_t v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

Vec3_t slicer_vec3_add(Vec3_t a, Vec3_t b) {
    return slicer_vec3_create(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3_t slicer_vec3_subtract(Vec3_t a, Vec3_t b) {
    return slicer_vec3_create(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3_t slicer_vec3_scale(Vec3_t v, float scalar) {
    return slicer_vec3_create(v.x * scalar, v.y * scalar, v.z * scalar);
}

float slicer_vec3_dot(Vec3_t a, Vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3_t slicer_vec3_cross(Vec3_t a, Vec3_t b) {
    return slicer_vec3_create(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float slicer_vec3_length(Vec3_t v) {
    return sqrtf(slicer_vec3_dot(v, v));
}

Vec3_t slicer_vec3_normalize(Vec3_t v) {
    float len = slicer_vec3_length(v);
    if (len < SLICER_EPS_NORMALIZE) {
        return slicer_vec3_create(0.0f, 0.0f, 0.0f);
    }
    return slicer_vec3_scale(v, 1.0f / len);
}

float slicer_vec3_distance(Vec3_t a, Vec3_t b) {
    return slicer_vec3_length(slicer_vec3_subtract(a, b));
}
