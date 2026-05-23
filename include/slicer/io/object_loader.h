#ifndef SLICER_IO_OBJECT_LOADER
#define SLICER_IO_OBJECT_LOADER

#include "slicer/mesh/mesh.h"
#include <stdint.h>

Mesh_t obj_load(const char* filename);

#endif /* SLICER_IO_OBJECT_LOADER */
