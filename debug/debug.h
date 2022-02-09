#pragma once

#include <stddef.h>

typedef struct MemoryInfo {
    int allocations;
    int deallocations;
} MemoryInfo;

extern MemoryInfo g_debug_info;

void *dbg_malloc(size_t n, const char* filename, int line, const char* function_name);
void dbg_free(void* obj, const char* filename, int line, const char* function_name);
void dbg_info();