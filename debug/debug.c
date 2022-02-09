#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

MemoryInfo g_debug_info = {.allocations = 0, .deallocations = 0};

const char* format_string = "%-15s\t%-20s\t%5d\t%-p\n";

void *dbg_malloc(size_t n, const char* filename, const char* function_name, int line) {

    void* ptr = malloc(n);
    printf(format_string, filename, function_name, line, ptr); 
    g_debug_info.allocations++;

    return  ptr;
}

void dbg_free(void* obj, const char* filename, const char* function_name, int line) {
    
    printf(format_string, filename, function_name, line, obj); 
    free(obj); 
    g_debug_info.deallocations++; 
}

void dbg_info() {
    printf("Allocation sum: %d\n", g_debug_info.allocations - g_debug_info.deallocations);
}