#include "slicer/core/status.h"

const char* slicer_status_to_cstr(slicer_status_t status) {
    switch (status) {
        case SLICER_STATUS_OK:
            return "OK";
        case SLICER_STATUS_INVALID_ARG:
            return "Invalid argument";
        case SLICER_STATUS_OUT_OF_MEMORY:
            return "Out of memory";
        case SLICER_STATUS_OUT_OF_RANGE:
            return "Out of range";
        case SLICER_STATUS_PARSE_ERROR:
            return "Parse error";
        case SLICER_STATUS_IO_ERROR:
            return "I/O error";
        case SLICER_STATUS_DEGENERATE_GEOMETRY:
            return "Degenerate geometry";
        case SLICER_STATUS_NUMERIC_ERROR:
            return "Numeric error";
        case SLICER_STATUS_NOT_IMPLEMENTED:
            return "Not implemented";
        case SLICER_STATUS_INTERNAL_ERROR:
            return "Internal error";
        default:
            return "Unknown status";
    }
}

bool slicer_status_is_ok(slicer_status_t status) {
    return status == SLICER_STATUS_OK;
}
