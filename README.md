# Slicer

> A voxel-based 3D mesh slicer written in C. Converts triangle mesh models to discrete voxel grids for 3D printing workflows.

## Features

- **Mesh Voxelization**: Convert triangle meshes (OBJ format) to 3D voxel grids
- **Efficient Spatial Partitioning**: Grid-based representation with O(1) voxel access
- **Coordinate Transformations**: Seamless world ↔ grid space conversions
- **Fast Intersection Testing**: AABB-based triangle-voxel overlap detection
- **Modular Architecture**: Clean separation of concerns across 5 stages
- **Production Ready**: Comprehensive error handling and validation

## Getting Started

### Prerequisites

- C compiler (C99 or later)
- Make

### Build

```bash
make build
```

## Development Stages

| # | Stage | Description | Status |
|----|-------|-------------|--------|
| 1 | Core | Types, error handling, math utilities | Complete |
| 2 | Mesh | OBJ parsing, mesh loading, validation | Complete |
| 3 | Voxel | Voxelization engine, grid operations | Complete |
| 4 | Repair | Hole filling, flood fill, connectivity | Complete |
| 5 | Output | Layer slicing, G-code generation | Planned |

## Project Structure

```
.
├── include/slicer/     Public API headers
│   ├── core/           Core types & status codes
│   ├── math/           Vector & geometric utilities
│   ├── io/             File I/O & parsing
│   ├── mesh/           Mesh structures
│   └── voxel/          Voxelization engine
├── src/                Implementation
├── tests/              Test suite
├── Makefile            Build configuration
└── README.md           This file
```
