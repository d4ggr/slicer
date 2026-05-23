#ifndef SLICER_CORE_TYPES_H
#define SLICER_CORE_TYPES_H

#include <stdint.h>

typedef struct Vec2{
	float x;
	float y;
} Vec2_t;

typedef struct Vec3{
	float x;
	float y;
	float z;
} Vec3_t;

typedef struct AABB{
	Vec3_t min;
	Vec3_t max;
}AABB_t;

typedef struct Triangle{
	uint32_t  indices[3];
} Triangle_t;

typedef struct Mesh{
	Vec3_t* vertices;
	uint32_t num_vertices;
	uint32_t vertex_capacity;

	Triangle_t* triangles;
	uint32_t num_triangles;
	uint32_t triangle_capacity;

	AABB_t bounds;	
} Mesh_t;






#endif /* SLICER_CORE_TYPES_H */
