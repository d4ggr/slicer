#ifndef SLICER_CORE_CONSTANTS_H
#define SLICER_CORE_CONSTANTS_H

#include <stddef.h>

/*
 * Global coordinate/numeric policy:
 * - Unit system: millimeter
 * - Coordinate system: right-handed
 * - Up axis: +Z
 */

typedef enum slicer_unit_system {
    SLICER_UNIT_MM = 0
} slicer_unit_system_t;

typedef enum slicer_handedness {
    SLICER_HANDEDNESS_RIGHT = 0
} slicer_handedness_t;

typedef enum slicer_axis {
    SLICER_AXIS_X = 0,
    SLICER_AXIS_Y = 1,
    SLICER_AXIS_Z = 2
} slicer_axis_t;

typedef struct slicer_coordinate_policy {
    slicer_unit_system_t unit;
    slicer_handedness_t handedness;
    slicer_axis_t up_axis;
    float world_to_mm_scale;
    float build_plate_z_mm;
    int layer_progress_positive_z;
} slicer_coordinate_policy_t;

typedef struct slicer_numeric_policy {
    float eps;
    float eps_normalize;
    float eps_intersection;
} slicer_numeric_policy_t;

typedef struct slicer_memory_policy {
    size_t mesh_vertex_initial_capacity;
    size_t mesh_triangle_initial_capacity;
    size_t growth_num;
    size_t growth_den;
} slicer_memory_policy_t;

typedef struct slicer_policy {
    slicer_coordinate_policy_t coord;
    slicer_numeric_policy_t numeric;
    slicer_memory_policy_t memory;
} slicer_policy_t;

#define SLICER_WORLD_TO_MM_SCALE             1.0f
#define SLICER_BUILD_PLATE_Z_MM              0.0f
#define SLICER_LAYER_PROGRESS_POSITIVE_Z     1

#define SLICER_EPS                           1.0e-6f
#define SLICER_EPS_NORMALIZE                 1.0e-12f
#define SLICER_EPS_INTERSECTION              1.0e-7f

#define SLICER_MESH_INITIAL_VERTEX_CAP       1024u
#define SLICER_MESH_INITIAL_TRIANGLE_CAP     2048u
#define SLICER_CAPACITY_GROWTH_NUM           2u
#define SLICER_CAPACITY_GROWTH_DEN           1u

static const slicer_policy_t SLICER_POLICY_DEFAULT = {
    .coord = {
        .unit = SLICER_UNIT_MM,
        .handedness = SLICER_HANDEDNESS_RIGHT,
        .up_axis = SLICER_AXIS_Z,
        .world_to_mm_scale = SLICER_WORLD_TO_MM_SCALE,
        .build_plate_z_mm = SLICER_BUILD_PLATE_Z_MM,
        .layer_progress_positive_z = SLICER_LAYER_PROGRESS_POSITIVE_Z
    },
    .numeric = {
        .eps = SLICER_EPS,
        .eps_normalize = SLICER_EPS_NORMALIZE,
        .eps_intersection = SLICER_EPS_INTERSECTION
    },
    .memory = {
        .mesh_vertex_initial_capacity = SLICER_MESH_INITIAL_VERTEX_CAP,
        .mesh_triangle_initial_capacity = SLICER_MESH_INITIAL_TRIANGLE_CAP,
        .growth_num = SLICER_CAPACITY_GROWTH_NUM,
        .growth_den = SLICER_CAPACITY_GROWTH_DEN
    }
};

#endif /* SLICER_CORE_CONSTANTS_H */
