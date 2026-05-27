#ifndef SLICER_VOXEL_GRID_H
#define SLICER_VOXEL_GRID_H

#include <stdint.h>
#include <stdbool.h>
#include "slicer/core/types.h"
#include "slicer/core/status.h"

typedef struct Voxel {
	uint8_t state;
} Voxel_t;

typedef struct VoxelGrid {
	Voxel_t* data;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	Vec3_t origin;
	float cell_size;
	AABB_t bounds;
} VoxelGrid_t;

typedef struct Voxelization_Params {
	float cell_size;
	float padding_mm;
	uint32_t algo;
} Voxelization_Params_t;

// Lifecycle
VoxelGrid_t* voxel_grid_create_from_mesh(const Mesh_t* mesh, const Voxelization_Params_t* params);
void voxel_grid_free(VoxelGrid_t* grid);

// Voxel access
slicer_status_t voxel_grid_set_voxel(VoxelGrid_t* grid, uint32_t x, uint32_t y, uint32_t z, uint8_t state);
Voxel_t voxel_grid_get_voxel(VoxelGrid_t* grid, uint32_t x, uint32_t y, uint32_t z);
bool voxel_grid_is_filled(VoxelGrid_t* grid, uint32_t x, uint32_t y, uint32_t z);

// Index conversions
uint32_t voxel_grid_index_from_coords(VoxelGrid_t* grid, uint32_t x, uint32_t y, uint32_t z);
void voxel_grid_coords_from_index(VoxelGrid_t* grid, uint32_t index, uint32_t* x, uint32_t* y, uint32_t* z);
slicer_status_t voxel_grid_world_to_grid(VoxelGrid_t* grid, Vec3_t pos, uint32_t* x, uint32_t* y, uint32_t* z);
Vec3_t voxel_grid_grid_to_world(VoxelGrid_t* grid, uint32_t x, uint32_t y, uint32_t z);

// Voxelization
slicer_status_t voxel_grid_voxelize_mesh(VoxelGrid_t* grid, const Mesh_t* mesh);

// Helpers
AABB_t voxel_grid_get_triangle_bounds(VoxelGrid_t* grid, const Vec3_t* v0, const Vec3_t* v1, const Vec3_t* v2);
bool voxel_grid_triangle_overlaps_voxel(VoxelGrid_t* grid, const Vec3_t* v0, const Vec3_t* v1, const Vec3_t* v2, uint32_t x, uint32_t y, uint32_t z);

// Utility
float voxel_grid_get_fill_percentage(VoxelGrid_t* grid);
uint32_t voxel_grid_count_filled(VoxelGrid_t* grid);
slicer_status_t voxel_grid_validate(VoxelGrid_t* grid);
slicer_status_t voxel_grid_export_ply(const VoxelGrid_t* grid, const char* path);

#endif /* SLICER_VOXEL_GRID_H */
