#include <stdio.h>
#include "slicer/io/object_loader.h"

int main() {
	printf("=== OBJ Loader Test ===\n\n");
	
	// Load test model
	printf("Loading test_model.obj...\n");
	Mesh_t mesh = obj_load("tests/data/test_model.obj");
	
	// Check result
	uint32_t vert_count = mesh_vertex_count(&mesh);
	uint32_t tri_count = mesh_triangle_count(&mesh);
	
	printf("Vertices loaded: %u (expected: 4)\n", vert_count);
	printf("Triangles loaded: %u (expected: 4)\n", tri_count);
	
	if (vert_count == 4 && tri_count == 4) {
		printf("\n✅ SUCCESS! OBJ loader working correctly!\n");
	} else {
		printf("\n❌ FAILED! Counts don't match expected values.\n");
	}
	
	// Cleanup
	mesh_free(&mesh);
	
	return 0;
}
