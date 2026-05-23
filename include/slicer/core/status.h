#ifndef SLICER_CORE_STATUS_H
#define SLICER_CORE_STATUS_H

#include <stdbool.h>

typedef enum slicer_status {
    SLICER_STATUS_OK = 0,
    SLICER_STATUS_INVALID_ARG,
    SLICER_STATUS_OUT_OF_MEMORY,
    SLICER_STATUS_OUT_OF_RANGE,
    SLICER_STATUS_PARSE_ERROR,
    SLICER_STATUS_IO_ERROR,
    SLICER_STATUS_DEGENERATE_GEOMETRY,
    SLICER_STATUS_NUMERIC_ERROR,
    SLICER_STATUS_NOT_IMPLEMENTED,
    SLICER_STATUS_INTERNAL_ERROR
} slicer_status_t;

const char* slicer_status_to_cstr(slicer_status_t status);
bool slicer_status_is_ok(slicer_status_t status);

#endif /* SLICER_CORE_STATUS_H */
