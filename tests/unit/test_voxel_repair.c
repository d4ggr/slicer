#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "slicer/voxel/voxel_grid.h"
#include "slicer/voxel/voxel_repair.h"

static uint32_t voxel_index(uint32_t x, uint32_t y, uint32_t z, uint32_t width, uint32_t height) {
	return x + (y * width) + (z * width * height);
}

static slicer_status_t init_grid(VoxelGrid_t* grid, uint32_t width, uint32_t height, uint32_t depth) {
	if (grid == NULL || width == 0 || height == 0 || depth == 0) {
		return SLICER_STATUS_INVALID_ARG;
	}

	uint32_t total = width * height * depth;
	grid->data = calloc(total, sizeof(Voxel_t));
	if (grid->data == NULL) {
		return SLICER_STATUS_OUT_OF_MEMORY;
	}

	grid->width = width;
	grid->height = height;
	grid->depth = depth;
	grid->origin = (Vec3_t){0.0f, 0.0f, 0.0f};
	grid->cell_size = 1.0f;
	grid->bounds = (AABB_t){{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};

	return SLICER_STATUS_OK;
}

static void free_grid(VoxelGrid_t* grid) {
	if (grid == NULL) {
		return;
	}
	free(grid->data);
	grid->data = NULL;
}

static void fill_shell(VoxelGrid_t* grid) {
	for (uint32_t z = 0; z < grid->depth; z++) {
		for (uint32_t y = 0; y < grid->height; y++) {
			for (uint32_t x = 0; x < grid->width; x++) {
				if (x == 0 || x == grid->width - 1 ||
					y == 0 || y == grid->height - 1 ||
					z == 0 || z == grid->depth - 1) {
					uint32_t index = voxel_index(x, y, z, grid->width, grid->height);
					grid->data[index].state = 1;
				}
			}
		}
	}
}

static uint32_t count_filled(VoxelGrid_t* grid) {
	uint32_t total = grid->width * grid->height * grid->depth;
	uint32_t filled = 0;
	for (uint32_t i = 0; i < total; i++) {
		if (grid->data[i].state != 0) {
			filled++;
		}
	}
	return filled;
}

int main() {
	printf("=== Voxel Repair Test ===\n\n");

	// Test 1: Closed shell should become fully filled
	VoxelGrid_t grid1;
	if (init_grid(&grid1, 5, 5, 5) != SLICER_STATUS_OK) {
		printf("Failed to init grid1\n");
		return 1;
	}
	fill_shell(&grid1);

	uint32_t total1 = grid1.width * grid1.height * grid1.depth;
	uint8_t* exterior1 = calloc(total1, sizeof(uint8_t));
	if (exterior1 == NULL) {
		printf("Failed to allocate exterior1\n");
		free_grid(&grid1);
		return 1;
	}

	if (voxel_repair_flood_fill(&grid1, exterior1) != SLICER_STATUS_OK ||
		voxel_repair_fill_interior(&grid1, exterior1) != SLICER_STATUS_OK) {
		printf("Flood fill or fill interior failed (grid1)\n");
		free(exterior1);
		free_grid(&grid1);
		return 1;
	}

	uint32_t filled1 = count_filled(&grid1);
	printf("Test 1 (closed shell): filled=%u, total=%u\n", filled1, total1);
	int pass1 = (filled1 == total1);

	free(exterior1);
	free_grid(&grid1);

	// Test 2: Shell with a hole should NOT become fully filled
	VoxelGrid_t grid2;
	if (init_grid(&grid2, 5, 5, 5) != SLICER_STATUS_OK) {
		printf("Failed to init grid2\n");
		return 1;
	}
	fill_shell(&grid2);

	uint32_t hole_index = voxel_index(0, 2, 2, grid2.width, grid2.height);
	grid2.data[hole_index].state = 0;

	uint32_t total2 = grid2.width * grid2.height * grid2.depth;
	uint8_t* exterior2 = calloc(total2, sizeof(uint8_t));
	if (exterior2 == NULL) {
		printf("Failed to allocate exterior2\n");
		free_grid(&grid2);
		return 1;
	}

	if (voxel_repair_flood_fill(&grid2, exterior2) != SLICER_STATUS_OK ||
		voxel_repair_fill_interior(&grid2, exterior2) != SLICER_STATUS_OK) {
		printf("Flood fill or fill interior failed (grid2)\n");
		free(exterior2);
		free_grid(&grid2);
		return 1;
	}

	uint32_t filled2 = count_filled(&grid2);
	uint32_t empty2 = total2 - filled2;
	printf("Test 2 (hole): filled=%u, empty=%u, total=%u\n", filled2, empty2, total2);
	int pass2 = (filled2 < total2);

	free(exterior2);
	free_grid(&grid2);

	if (pass1 && pass2) {
		printf("\n✅ Voxel repair tests passed!\n");
		return 0;
	}

	printf("\n❌ Voxel repair tests failed!\n");
	return 1;
}
