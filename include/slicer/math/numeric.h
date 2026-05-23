#ifndef SLICER_MATH_NUMERIC_H
#define SLICER_MATH_NUMERIC_H

#include <math.h>
#include <stdbool.h>

float slicer_math_clamp(float value, float min, float max);
float slicer_math_min(float a, float b);
float slicer_math_max(float a, float b);
float slicer_math_abs(float value);
bool slicer_math_near_equal(float a, float b, float epsilon);

#endif /* SLICER_MATH_NUMERIC_H */
