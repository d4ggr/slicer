#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "slicer/voxel/voxel_grid.h"
#include "slicer/mesh/mesh.h"
#include "slicer/math/numeric.h"

uint32_t voxel_grid_index_from_coords(VoxelGrid_t* grid, uint32_t x, uint32_t y, uint32_t z){

	if(x >= grid->width || y >= grid->height || z >= grid->depth){
		printf("Error: input x,y,z out of bounds\n");
		return 0;
	}

	return (x + (y * grid->width) + (z * grid->width * grid->height));
}

void voxel_grid_coords_from_index(VoxelGrid_t* grid, uint32_t index, uint32_t* x, uint32_t* y, uint32_t* z){

	uint32_t total_cells = grid->width * grid->height * grid->depth;
	
	if(index >= total_cells){
		printf("Error: index %u out of bounds (max: %u)\n", index, total_cells - 1);
		return;
	}

	uint32_t layer_size = grid->width * grid->height;
	
	*z = index / layer_size;
	uint32_t remainder = index % layer_size;
	
	*y = remainder / grid->width;
	*x = remainder % grid->width;
}

VoxelGrid_t* voxel_grid_create_from_mesh(const Mesh_t* mesh, const Voxelization_Params_t* params){

	if (mesh == NULL || params == NULL) {
		printf("Error: null pointer passed to voxel_grid_create_from_mesh\n");
		return NULL;
	}

	if (mesh_vertex_count(mesh) == 0) {
		printf("Error: mesh has no vertices\n");
		return NULL;
	}

	if (params->cell_size <= 0) {
		printf("Error: cell_size must be positive\n");
		return NULL;
	}

	AABB_t bounds = mesh_get_bounds(mesh);
	bounds.min.x -= params->padding_mm;
	bounds.min.y -= params->padding_mm;
	bounds.min.z -= params->padding_mm;
	bounds.max.x += params->padding_mm;
	bounds.max.y += params->padding_mm;
	bounds.max.z += params->padding_mm;

	uint32_t width, height, depth;
	width = ceil((bounds.max.x - bounds.min.x) / params->cell_size);
	height = ceil((bounds.max.y - bounds.min.y) / params->cell_size);
	depth = ceil((bounds.max.z - bounds.min.z) / params->cell_size);

	VoxelGrid_t* grid = malloc(sizeof(VoxelGrid_t));
	if (grid == NULL) {
		printf("Error: malloc failed for VoxelGrid_t struct\n");
		return NULL;
	}

	uint32_t total_cells = width * height * depth;
	grid->data = malloc(total_cells * sizeof(Voxel_t));
	if (grid->data == NULL) {
		printf("Error: malloc failed for voxel data array\n");
		free(grid);
		return NULL;
	}

	grid->width = width;
	grid->height = height;
	grid->depth = depth;
	grid->origin = bounds.min;
	grid->cell_size = params->cell_size;
	grid->bounds = bounds;

	memset(grid->data, 0, total_cells * sizeof(Voxel_t));

	return grid;
}

slicer_status_t voxel_grid_set_voxel(VoxelGrid_t* grid, uint32_t x, uint32_t y, uint32_t z, uint8_t state){

	if (grid == NULL) {
		return SLICER_STATUS_INVALID_ARG;
	}

	if (x >= grid->width || y >= grid->height || z >= grid->depth) {
		return SLICER_STATUS_OUT_OF_RANGE;
	}

	grid->data[voxel_grid_index_from_coords(grid, x, y, z)].state = state;

	return SLICER_STATUS_OK;
}

Voxel_t voxel_grid_get_voxel(VoxelGrid_t* grid, uint32_t x, uint32_t y, uint32_t z){

	Voxel_t empty_voxel = {0};

	if (grid == NULL) {
		return empty_voxel;
	}

	if (x >= grid->width || y >= grid->height || z >= grid->depth) {
		return empty_voxel;
	}

	return grid->data[voxel_grid_index_from_coords(grid, x, y, z)];
}

bool voxel_grid_is_filled(VoxelGrid_t* grid, uint32_t x, uint32_t y, uint32_t z){

	if (grid == NULL) {
		return false;
	}

	if (x >= grid->width || y >= grid->height || z >= grid->depth) {
		return false;
	}

	return (grid->data[voxel_grid_index_from_coords(grid, x, y, z)].state != 0);
}

slicer_status_t voxel_grid_world_to_grid(VoxelGrid_t* grid, Vec3_t pos,
                                          uint32_t* x, uint32_t* y, uint32_t* z) {
     
     
     if (grid == NULL || x == NULL || y == NULL || z == NULL) {
         return SLICER_STATUS_INVALID_ARG;
     }
     
     
     float rel_x = pos.x - grid->origin.x;
     float rel_y = pos.y - grid->origin.y;
     float rel_z = pos.z - grid->origin.z;
     
     
     float grid_x_f = rel_x / grid->cell_size;
     float grid_y_f = rel_y / grid->cell_size;
     float grid_z_f = rel_z / grid->cell_size;
     
     
     float floored_x = floor(grid_x_f);
     float floored_y = floor(grid_y_f);
     float floored_z = floor(grid_z_f);
     
     
     if (floored_x < 0 || floored_y < 0 || floored_z < 0) {
         return SLICER_STATUS_OUT_OF_RANGE;
     }
     
     
     uint32_t grid_x = (uint32_t)floored_x;
     uint32_t grid_y = (uint32_t)floored_y;
     uint32_t grid_z = (uint32_t)floored_z;
     
     
     if (grid_x >= grid->width || grid_y >= grid->height || grid_z >= grid->depth) {
         return SLICER_STATUS_OUT_OF_RANGE;
     }
     
     
     *x = grid_x;
     *y = grid_y;
     *z = grid_z;
     
     
     return SLICER_STATUS_OK;
 }

AABB_t voxel_grid_get_triangle_bounds(VoxelGrid_t* grid, const Vec3_t* v0,const Vec3_t* v1,const Vec3_t* v2){
uint32_t x0,x1,x2,y0,y1,y2,z0,z1,z2;
voxel_grid_world_to_grid(grid, *v0, &x0, &y0, &z0);
voxel_grid_world_to_grid(grid, *v1, &x1, &y1, &z1);
voxel_grid_world_to_grid(grid, *v2, &x2, &y2, &z2);

AABB_t box;
box.min.x = (float)(slicer_math_min(slicer_math_min(x0,x1),x2));
box.min.y = (float)(slicer_math_min(slicer_math_min(y0,y1),y2));
box.min.z = (float)(slicer_math_min(slicer_math_min(z0,z1),z2));

box.max.x = (float)(slicer_math_max(slicer_math_max(x0,x1),x2));
box.max.y = (float)(slicer_math_max(slicer_math_max(y0,y1),y2));
box.max.z = (float)(slicer_math_max(slicer_math_max(z0,z1),z2));

return box;
} 

bool voxel_grid_triangle_overlaps_voxel(VoxelGrid_t* grid, const Vec3_t* v0, const Vec3_t* v1, const Vec3_t* v2, uint32_t voxel_x, uint32_t voxel_y, uint32_t voxel_z) {
  
// Get triangle min/max in world space
float tri_min_x = slicer_math_min(slicer_math_min(v0->x, v1->x), v2->x);
float tri_max_x = slicer_math_max(slicer_math_max(v0->x, v1->x), v2->x);
  
float tri_min_y = slicer_math_min(slicer_math_min(v0->y, v1->y), v2->y);
float tri_max_y = slicer_math_max(slicer_math_max(v0->y, v1->y), v2->y);
  
float tri_min_z = slicer_math_min(slicer_math_min(v0->z, v1->z), v2->z);
float tri_max_z = slicer_math_max(slicer_math_max(v0->z, v1->z), v2->z);
  
// Get cube bounds in world space
Vec3_t cube_min = (Vec3_t){
  grid->origin.x + (float)voxel_x * grid->cell_size,
  grid->origin.y + (float)voxel_y * grid->cell_size,
  grid->origin.z + (float)voxel_z * grid->cell_size
};
  
Vec3_t cube_max = (Vec3_t){
  cube_min.x + grid->cell_size,
  cube_min.y + grid->cell_size,
  cube_min.z + grid->cell_size
};
  
// Check AABB overlap on all three axes
if (tri_max_x < cube_min.x || tri_min_x > cube_max.x) return false;
if (tri_max_y < cube_min.y || tri_min_y > cube_max.y) return false;
if (tri_max_z < cube_min.z || tri_min_z > cube_max.z) return false;
  
return true;
}

slicer_status_t voxel_grid_voxelize_mesh(VoxelGrid_t* grid, const Mesh_t* mesh) {
  
  if (grid == NULL || mesh == NULL) {
    return SLICER_STATUS_INVALID_ARG;
  }
  
  uint32_t tri_count = mesh_triangle_count(mesh);
  if (tri_count == 0) {
    return SLICER_STATUS_INVALID_ARG;
  }
  
  // Process each triangle
  for (uint32_t tri_idx = 0; tri_idx < tri_count; tri_idx++) {
    
    // Get triangle
    Triangle_t* triangle = &mesh->triangles[tri_idx];
    
    // Get the 3 vertices from mesh
    Vec3_t v0 = mesh->vertices[triangle->indices[0]];
    Vec3_t v1 = mesh->vertices[triangle->indices[1]];
    Vec3_t v2 = mesh->vertices[triangle->indices[2]];
    
    // Get bounding box in grid space
    AABB_t bounds = voxel_grid_get_triangle_bounds(grid, &v0, &v1, &v2);
    
    // Clamp bounds to grid dimensions
    uint32_t x_min = (bounds.min.x < 0) ? 0 : (uint32_t)bounds.min.x;
    uint32_t x_max = (bounds.max.x >= (float)grid->width) ? grid->width - 1 : (uint32_t)bounds.max.x;
    
    uint32_t y_min = (bounds.min.y < 0) ? 0 : (uint32_t)bounds.min.y;
    uint32_t y_max = (bounds.max.y >= (float)grid->height) ? grid->height - 1 : (uint32_t)bounds.max.y;
    
    uint32_t z_min = (bounds.min.z < 0) ? 0 : (uint32_t)bounds.min.z;
    uint32_t z_max = (bounds.max.z >= (float)grid->depth) ? grid->depth - 1 : (uint32_t)bounds.max.z;
    
    // Check if bounding box is entirely outside grid
    if (x_min > x_max || y_min > y_max || z_min > z_max) {
      continue;
    }
    
    // Loop through all voxels in bounding box
    for (uint32_t x = x_min; x <= x_max; x++) {
      for (uint32_t y = y_min; y <= y_max; y++) {
        for (uint32_t z = z_min; z <= z_max; z++) {
          
          // Test if triangle overlaps this voxel
          if (voxel_grid_triangle_overlaps_voxel(grid, &v0, &v1, &v2, x, y, z)) {
            voxel_grid_set_voxel(grid, x, y, z, 1);
          }
        }
      }
    }
  }
  
  return SLICER_STATUS_OK;
}

void voxel_grid_free(VoxelGrid_t* grid){

if(grid == NULL || grid->data == NULL){
return;
}

free(grid->data);
free(grid);
}

Vec3_t voxel_grid_grid_to_world(VoxelGrid_t* grid, uint32_t x, uint32_t y, uint32_t z){

if(grid == NULL){
return (Vec3_t){0.0, 0.0, 0.0};
}

Vec3_t World;

World.x = grid->origin.x + (float)x * grid->cell_size;
World.y = grid->origin.y + (float)y * grid->cell_size;
World.z = grid->origin.z + (float)z * grid->cell_size;

return World;
}

uint32_t voxel_grid_count_filled(VoxelGrid_t* grid) {
  
  if (grid == NULL || grid->data == NULL) {
    return 0;
  }
  
  uint32_t count = 0;
  uint32_t total_cells = grid->width * grid->height * grid->depth;
  
  for (uint32_t i = 0; i < total_cells; i++) {
    if (grid->data[i].state != 0) {
      count++;
    }
  }
  
  return count;
}

slicer_status_t voxel_grid_validate(VoxelGrid_t* grid) {
  
  if (grid == NULL) {
    return SLICER_STATUS_INVALID_ARG;
  }
  
  if (grid->width == 0 || grid->height == 0 || grid->depth == 0) {
    return SLICER_STATUS_INVALID_ARG;
  }
  
  if (grid->data == NULL) {
    return SLICER_STATUS_INVALID_ARG;
  }
  
  if (grid->cell_size <= 0) {
    return SLICER_STATUS_INVALID_ARG;
  }
  
  return SLICER_STATUS_OK;
}

float voxel_grid_get_fill_percentage(VoxelGrid_t* grid) {
  
  if (grid == NULL) {
    return 0.0f;
  }
  
  uint32_t total = grid->width * grid->height * grid->depth;
  if (total == 0) {
    return 0.0f;
  }
  
  uint32_t filled = voxel_grid_count_filled(grid);
  float percentage = ((float)filled / (float)total) * 100.0f;
  
  return percentage;
}


