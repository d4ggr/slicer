#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <errno.h>
#include "slicer/io/object_loader.h"
#include "slicer/voxel/voxel_grid.h"
#include "slicer/voxel/voxel_repair.h"

static float max3f(float a, float b, float c) {
	float m = a;
	if (b > m) m = b;
	if (c > m) m = c;
	return m;
}

static int ensure_output_dir(const char* path) {
	if (mkdir(path, 0775) == 0) {
		return 0;
	}
	return (errno == EEXIST) ? 0 : -1;
}

int main(int argc, char** argv) {
	const char* path = (argc > 1) ? argv[1] : "tests/data/teapot.obj";
	printf("=== OBJ Voxel Repair Test ===\n\n");
	printf("Loading OBJ: %s\n", path);

	Mesh_t mesh = obj_load(path);
	uint32_t vert_count = mesh_vertex_count(&mesh);
	uint32_t tri_count = mesh_triangle_count(&mesh);
	printf("Vertices: %u\nTriangles: %u\n", vert_count, tri_count);

	if (vert_count == 0 || tri_count == 0) {
		printf("❌ Mesh load failed or empty mesh.\n");
		mesh_free(&mesh);
		return 1;
	}

	mesh_compute_bounds(&mesh);

	AABB_t bounds = mesh_get_bounds(&mesh);
	float dx = bounds.max.x - bounds.min.x;
	float dy = bounds.max.y - bounds.min.y;
	float dz = bounds.max.z - bounds.min.z;
	float max_dim = max3f(dx, dy, dz);
	float cell_size = (max_dim > 0.0f) ? (max_dim / 256.0f) : 1.0f;
	if (cell_size < 1.0f) {
		cell_size = 1.0f;
	}
	if (argc > 2) {
		cell_size = (float)atof(argv[2]);
		if (cell_size <= 0.0f) {
			printf("❌ Invalid cell_size: %s\n", argv[2]);
			mesh_free(&mesh);
			return 1;
		}
	}

	Voxelization_Params_t params = {
		.cell_size = cell_size,
		.padding_mm = cell_size,
		.algo = 0
	};

	printf("Voxelizing with cell_size=%.4f\n", params.cell_size);

	VoxelGrid_t* grid = voxel_grid_create_from_mesh(&mesh, &params);
	if (grid == NULL) {
		printf("❌ Failed to create voxel grid.\n");
		mesh_free(&mesh);
		return 1;
	}
  if (grid != NULL) {
  	printf("grid created from the mesh\n");
  	printf("grid dims: %u x %u x %u\n", grid->width, grid->height, grid->depth);
  }

	slicer_status_t status = voxel_grid_voxelize_mesh(grid, &mesh);
	if (status != SLICER_STATUS_OK) {
		printf("❌ Voxelization failed: %d\n", status);
		voxel_grid_free(grid);
		mesh_free(&mesh);
		return 1;
	}
  if (status == SLICER_STATUS_OK) {
  	printf("voxelization done\n");
  }

	size_t total = (size_t)grid->width * (size_t)grid->height * (size_t)grid->depth;
	uint8_t* exterior = calloc(total, sizeof(uint8_t));
	if (exterior == NULL) {
		printf("❌ Failed to allocate exterior map.\n");
		voxel_grid_free(grid);
		mesh_free(&mesh);
		return 1;
	}

	status = voxel_repair_flood_fill(grid, exterior);
	if (status != SLICER_STATUS_OK) {
		printf("❌ Flood fill failed: %d\n", status);
		free(exterior);
		voxel_grid_free(grid);
		mesh_free(&mesh);
		return 1;
	}
  if(status== SLICER_STATUS_OK){
printf("repair done");
  }

	status = voxel_repair_fill_interior(grid, exterior);
	if (status != SLICER_STATUS_OK) {
		printf("❌ Fill interior failed: %d\n", status);
		free(exterior);
		voxel_grid_free(grid);
		mesh_free(&mesh);
		return 1;
	}
  if(status == SLICER_STATUS_OK){
printf("filled interior");
  }

	float fill_pct = voxel_grid_get_fill_percentage(grid);
	printf("Fill percentage: %.2f%%\n", fill_pct);
	voxel_repair_statistics(grid, exterior);
	
	const char* out_dir = "tests/output";
	const char* out_path = "tests/output/vox_to_ply.ply";
	if (ensure_output_dir(out_dir) != 0) {
		printf("❌ Failed to create output directory: %s\n", out_dir);
		free(exterior);
		voxel_grid_free(grid);
		mesh_free(&mesh);
		return 1;
	}
	
	status = voxel_grid_export_ply(grid, out_path);
	if (status != SLICER_STATUS_OK) {
		printf("❌ PLY export failed: %d\n", status);
		free(exterior);
		voxel_grid_free(grid);
		mesh_free(&mesh);
		return 1;
	}
	
	printf("Voxel point cloud saved: %s\n", out_path);

	free(exterior);
	voxel_grid_free(grid);
	mesh_free(&mesh);

	printf("\n✅ OBJ voxel repair test complete.\n");
	return 0;
}
