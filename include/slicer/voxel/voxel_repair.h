#ifndef SLICER_VOXEL_REPAIR_H
#define SLICER_VOXEL_REPAIR_H

#include <stdint.h>
#include <stdbool.h>
#include "slicer/core/status.h"
#include "slicer/voxel/voxel_grid.h"

typedef struct VoxelIndex {
	uint32_t x;
	uint32_t y;
	uint32_t z;
} VoxelIndex_t;

typedef struct VoxelQueue {
	VoxelIndex_t* items;
	uint32_t front;
	uint32_t rear;
	uint32_t size;
	uint32_t capacity;
} VoxelQueue_t;

// Queue operations
slicer_status_t voxel_queue_create(VoxelQueue_t* queue, uint32_t initial_capacity);
slicer_status_t voxel_queue_enqueue(VoxelQueue_t* queue, VoxelIndex_t voxel);
VoxelIndex_t voxel_queue_dequeue(VoxelQueue_t* queue);
bool voxel_queue_is_empty(VoxelQueue_t* queue);
bool voxel_queue_is_full(VoxelQueue_t* queue);
void voxel_queue_free(VoxelQueue_t* queue);

// Repair operations
slicer_status_t voxel_repair_flood_fill(VoxelGrid_t* grid, uint8_t* exterior);
slicer_status_t voxel_repair_fill_interior(VoxelGrid_t* grid, uint8_t* exterior);
slicer_status_t voxel_repair_validate(VoxelGrid_t* grid);
void voxel_repair_statistics(VoxelGrid_t* grid, uint8_t* exterior);

#endif /* SLICER_VOXEL_REPAIR_H */
