CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -Wpedantic -O2 -I include/

.PHONY: all clean build test

all: build

build:
	@echo "Building Stage 1 (Foundation: math, types, mesh)..."
	$(CC) $(CFLAGS) -c src/core/status.c -o build/status.o
	$(CC) $(CFLAGS) -c src/math/numeric.c -o build/numeric.o
	$(CC) $(CFLAGS) -c src/math/vec2.c -o build/vec2.o
	$(CC) $(CFLAGS) -c src/math/vec3.c -o build/vec3.o
	$(CC) $(CFLAGS) -c src/math/aabb.c -o build/aabb.o
	$(CC) $(CFLAGS) -c src/mesh/mesh.c -o build/mesh.o
	@echo "Stage 1 build complete!"
	@echo "Building Stage 2 (I/O: file, parser, obj_loader)..."
	$(CC) $(CFLAGS) -c src/io/file.c -o build/file.o
	$(CC) $(CFLAGS) -c src/io/parser.c -o build/parser.o
	$(CC) $(CFLAGS) -c src/io/object_loader.c -o build/object_loader.o
	@echo "Stage 2 (file.c, parser.c, object_loader.c) build complete! Object files in build/"
	@echo "Building Stage 3-4 (Voxelization + Repair)..."
	$(CC) $(CFLAGS) -c src/voxel/voxel_grid.c -o build/voxel_grid.o
	$(CC) $(CFLAGS) -c src/voxel/voxel_repair.c -o build/voxel_repair.o
	@echo "Stage 3-4 build complete! Object files in build/"

test: build
	@echo ""
	@echo "Building test program..."
	$(CC) $(CFLAGS) tests/unit/test_obj_loader.c build/*.o -o build/test_obj_loader -lm
	$(CC) $(CFLAGS) tests/unit/test_voxel_repair.c build/*.o -o build/test_voxel_repair -lm
	$(CC) $(CFLAGS) tests/integration/test_obj_voxel_repair.c build/*.o -o build/test_obj_voxel_repair -lm
	@echo "Running tests..."
	@./build/test_obj_loader
	@./build/test_voxel_repair
	@./build/test_obj_voxel_repair

clean:
	rm -rf build/*
