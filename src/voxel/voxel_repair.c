#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slicer/voxel/voxel_repair.h"

slicer_status_t voxel_queue_create(VoxelQueue_t* queue, uint32_t initial_capacity) {
  if(queue == NULL || initial_capacity == 0){
    return SLICER_STATUS_INVALID_ARG ;
  }
  queue->items = NULL;
  queue->items = malloc(initial_capacity*sizeof(VoxelIndex_t));
  if(queue->items == NULL){
    return SLICER_STATUS_OUT_OF_MEMORY;
  }
  queue->front = 0;
  queue->rear = 0 ;
  queue->size = 0 ;
  queue->capacity = initial_capacity;
  return SLICER_STATUS_OK ; 
}


slicer_status_t voxel_queue_enqueue(VoxelQueue_t* queue, VoxelIndex_t voxel) {
	if (queue == NULL || queue->items == NULL || queue->capacity == 0) {
		return SLICER_STATUS_INVALID_ARG;
	}

	if (queue->size == queue->capacity) {
		uint32_t new_capacity = queue->capacity * 2;
		VoxelIndex_t* new_items = malloc(new_capacity * sizeof(VoxelIndex_t));
		if (new_items == NULL) {
			return SLICER_STATUS_OUT_OF_MEMORY;
		}

		for (uint32_t i = 0; i < queue->size; i++) {
			uint32_t idx = (queue->front + i) % queue->capacity;
			new_items[i] = queue->items[idx];
		}

		free(queue->items);
		queue->items = new_items;
		queue->capacity = new_capacity;
		queue->front = 0;
		queue->rear = queue->size;
	}

	queue->items[queue->rear] = voxel;
	queue->rear = (queue->rear + 1) % queue->capacity;
	queue->size += 1;

	return SLICER_STATUS_OK;
}

VoxelIndex_t voxel_queue_dequeue(VoxelQueue_t* queue) {
  if(queue == NULL || queue->items == NULL || queue->size == 0){
    return (VoxelIndex_t){0,0,0} ;
  }
  VoxelIndex_t result = queue->items[queue->front];
  queue->front = (queue->front+1)% queue->capacity;
  queue->size--;
  return result;
}

bool voxel_queue_is_empty(VoxelQueue_t* queue) {
  if(queue == NULL){
    return true;
  }
  return (queue->size == 0);
}

bool voxel_queue_is_full(VoxelQueue_t* queue) {
  if(queue != NULL && (queue->size == queue->capacity)){
    return true;
  }
  return false;
}

void voxel_queue_free(VoxelQueue_t* queue) {
  if(queue == NULL){
    return ;
  }
  if(queue->items != NULL){
    free(queue->items);
  }
  queue->items = NULL ;
  queue->front =0 ;
  queue->rear = 0;
  queue->size = 0;
  queue->capacity = 0;
}

slicer_status_t voxel_repair_flood_fill(VoxelGrid_t* grid, uint8_t* exterior) {
	if (grid == NULL || grid->data == NULL || exterior == NULL) {
		return SLICER_STATUS_INVALID_ARG;
	}
	uint32_t width = grid->width;
	uint32_t height = grid->height;
	uint32_t depth = grid->depth;
	if (width == 0 || height == 0 || depth == 0) {
		return SLICER_STATUS_INVALID_ARG;
	}

	size_t total_cells = (size_t)width * (size_t)height * (size_t)depth;
	memset(exterior, 0, total_cells * sizeof(uint8_t));

	uint32_t initial_capacity = width * height;
	if (initial_capacity == 0) {
		initial_capacity = 1;
	}

	VoxelQueue_t queue;
	slicer_status_t status = voxel_queue_create(&queue, initial_capacity);
	if (status != SLICER_STATUS_OK) {
		return status;
	}

	for (uint32_t z = 0; z < depth; z++) {
		for (uint32_t y = 0; y < height; y++) {
			uint32_t index = voxel_grid_index_from_coords(grid, 0, y, z);
			if (grid->data[index].state == 0 && exterior[index] == 0) {
				exterior[index] = 1;
				status = voxel_queue_enqueue(&queue, (VoxelIndex_t){0, y, z});
				if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
			}
			if (width > 1) {
				index = voxel_grid_index_from_coords(grid, width - 1, y, z);
				if (grid->data[index].state == 0 && exterior[index] == 0) {
					exterior[index] = 1;
					status = voxel_queue_enqueue(&queue, (VoxelIndex_t){width - 1, y, z});
					if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
				}
			}
		}
	}

	for (uint32_t z = 0; z < depth; z++) {
		for (uint32_t x = 0; x < width; x++) {
			uint32_t index = voxel_grid_index_from_coords(grid, x, 0, z);
			if (grid->data[index].state == 0 && exterior[index] == 0) {
				exterior[index] = 1;
				status = voxel_queue_enqueue(&queue, (VoxelIndex_t){x, 0, z});
				if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
			}
			if (height > 1) {
				index = voxel_grid_index_from_coords(grid, x, height - 1, z);
				if (grid->data[index].state == 0 && exterior[index] == 0) {
					exterior[index] = 1;
					status = voxel_queue_enqueue(&queue, (VoxelIndex_t){x, height - 1, z});
					if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
				}
			}
		}
	}

	for (uint32_t y = 0; y < height; y++) {
		for (uint32_t x = 0; x < width; x++) {
			uint32_t index = voxel_grid_index_from_coords(grid, x, y, 0);
			if (grid->data[index].state == 0 && exterior[index] == 0) {
				exterior[index] = 1;
				status = voxel_queue_enqueue(&queue, (VoxelIndex_t){x, y, 0});
				if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
			}
			if (depth > 1) {
				index = voxel_grid_index_from_coords(grid, x, y, depth - 1);
				if (grid->data[index].state == 0 && exterior[index] == 0) {
					exterior[index] = 1;
					status = voxel_queue_enqueue(&queue, (VoxelIndex_t){x, y, depth - 1});
					if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
				}
			}
		}
	}

	while (!voxel_queue_is_empty(&queue)) {
		VoxelIndex_t current = voxel_queue_dequeue(&queue);

		if (current.x + 1 < width) {
			uint32_t index = voxel_grid_index_from_coords(grid, current.x + 1, current.y, current.z);
			if (grid->data[index].state == 0 && exterior[index] == 0) {
				exterior[index] = 1;
				status = voxel_queue_enqueue(&queue, (VoxelIndex_t){current.x + 1, current.y, current.z});
				if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
			}
		}
		if (current.x > 0) {
			uint32_t index = voxel_grid_index_from_coords(grid, current.x - 1, current.y, current.z);
			if (grid->data[index].state == 0 && exterior[index] == 0) {
				exterior[index] = 1;
				status = voxel_queue_enqueue(&queue, (VoxelIndex_t){current.x - 1, current.y, current.z});
				if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
			}
		}
		if (current.y + 1 < height) {
			uint32_t index = voxel_grid_index_from_coords(grid, current.x, current.y + 1, current.z);
			if (grid->data[index].state == 0 && exterior[index] == 0) {
				exterior[index] = 1;
				status = voxel_queue_enqueue(&queue, (VoxelIndex_t){current.x, current.y + 1, current.z});
				if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
			}
		}
		if (current.y > 0) {
			uint32_t index = voxel_grid_index_from_coords(grid, current.x, current.y - 1, current.z);
			if (grid->data[index].state == 0 && exterior[index] == 0) {
				exterior[index] = 1;
				status = voxel_queue_enqueue(&queue, (VoxelIndex_t){current.x, current.y - 1, current.z});
				if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
			}
		}
		if (current.z + 1 < depth) {
			uint32_t index = voxel_grid_index_from_coords(grid, current.x, current.y, current.z + 1);
			if (grid->data[index].state == 0 && exterior[index] == 0) {
				exterior[index] = 1;
				status = voxel_queue_enqueue(&queue, (VoxelIndex_t){current.x, current.y, current.z + 1});
				if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
			}
		}
		if (current.z > 0) {
			uint32_t index = voxel_grid_index_from_coords(grid, current.x, current.y, current.z - 1);
			if (grid->data[index].state == 0 && exterior[index] == 0) {
				exterior[index] = 1;
				status = voxel_queue_enqueue(&queue, (VoxelIndex_t){current.x, current.y, current.z - 1});
				if (status != SLICER_STATUS_OK) { voxel_queue_free(&queue); return status; }
			}
		}
	}

	voxel_queue_free(&queue);
	return SLICER_STATUS_OK;
}

slicer_status_t voxel_repair_fill_interior(VoxelGrid_t* grid, uint8_t* exterior) {
  if(grid == NULL || grid->data == NULL || exterior == NULL){
    return SLICER_STATUS_INVALID_ARG ;
  }
  uint32_t total = (grid->width*grid->height*grid->depth);
  for(uint32_t i=0;i<total;i++){
    if(grid->data[i].state == 0 && exterior[i] == 0 ){
    grid->data[i].state = 1 ;
    }
  }
  return SLICER_STATUS_OK;
}

slicer_status_t voxel_repair_validate(VoxelGrid_t* grid) {
	if (grid == NULL || grid->data == NULL) {
		return SLICER_STATUS_INVALID_ARG;
	}

	uint32_t total = grid->width * grid->height * grid->depth;
	for (uint32_t i = 0; i < total; i++) {
		if (grid->data[i].state != 0 && grid->data[i].state != 1) {
			return SLICER_STATUS_INVALID_ARG;
		}
	}

	return SLICER_STATUS_OK;
}

void voxel_repair_statistics(VoxelGrid_t* grid, uint8_t* exterior) {
	if (grid == NULL || grid->data == NULL) {
		return;
	}

	uint32_t total = grid->width * grid->height * grid->depth;
	uint32_t filled = 0;
	uint32_t exterior_count = 0;

	for (uint32_t i = 0; i < total; i++) {
		if (grid->data[i].state != 0) {
			filled++;
		}
		if (exterior != NULL && exterior[i] != 0) {
			exterior_count++;
		}
	}

	uint32_t empty = total - filled;
	printf("Voxel repair stats: total=%u, filled=%u, empty=%u", total, filled, empty);
	if (exterior != NULL) {
		uint32_t interior = empty - exterior_count;
		printf(", exterior=%u, interior=%u", exterior_count, interior);
	}
	printf("\n");
}
