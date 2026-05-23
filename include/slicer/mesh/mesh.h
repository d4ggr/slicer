#ifndef SLICER_MESH_MESH_H
#define SLICER_MESH_MESH_H

#include <stdint.h>
#include <stdlib.h>
#include "slicer/core/types.h"
#include "slicer/core/status.h"
#include "slicer/math/aabb.h"

Mesh_t mesh_create();
slicer_status_t mesh_reserve_vertices(Mesh_t* mesh, uint32_t capacity);
slicer_status_t mesh_reserve_triangles(Mesh_t* mesh,uint32_t capacity);
slicer_status_t mesh_add_vertex(Mesh_t* mesh,Vec3_t vertex);
slicer_status_t mesh_add_triangle(Mesh_t* mesh, uint32_t v0, uint32_t v1,uint32_t v2);
void mesh_compute_bounds(Mesh_t* mesh);
AABB_t mesh_get_bounds(const Mesh_t* mesh);
uint32_t mesh_vertex_count(const Mesh_t* mesh);
uint32_t mesh_triangle_count(const Mesh_t* mesh);
slicer_status_t mesh_validate(const Mesh_t* mesh);
void mesh_free(Mesh_t* mesh);

#endif /* SLICER_MESH_MESH_H */
