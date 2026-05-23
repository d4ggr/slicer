#ifndef SLICER_IO_FILE_H
#define SLICER_IO_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char* file_read_all(const char* filename,uint32_t* out_size);
void file_free(char* buffer);

#endif /* SLICER_IO_FILE_H */
