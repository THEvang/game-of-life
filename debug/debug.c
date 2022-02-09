#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

MemoryInfo g_debug_info = {.allocations = 0, .deallocations = 0};

void *dbg_malloc(size_t n, const char* filename, int line, const char* function_name) {

    printf("%-20s\t%-5d\t%-ld\t%s\n",  __FILE__, __LINE__, n, function_name); 
    g_debug_info.allocations++;

    return malloc(n); 
}

void dbg_free(void* obj, const char* filename, int line, const char* function_name) {
    
    printf("%-20s\t%-5d\t%-p\t%s\n", __FILE__, __LINE__, obj, function_name); 
    free(obj); 
    g_debug_info.deallocations++; 
}

void dbg_info() {
    printf("Allocation sum: %d\n", g_debug_info.allocations - g_debug_info.deallocations);
}