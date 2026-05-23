#include "slicer/math/numeric.h"
#include <math.h>

float slicer_math_clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float slicer_math_min(float a, float b) {
    return a < b ? a : b;
}

float slicer_math_max(float a, float b) {
    return a > b ? a : b;
}

float slicer_math_abs(float value) {
    return value < 0.0f ? -value : value;
}

bool slicer_math_near_equal(float a, float b, float epsilon) {
    return slicer_math_abs(a - b) < epsilon;
}
