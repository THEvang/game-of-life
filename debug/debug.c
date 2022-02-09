#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

MemoryInfo g_debug_info = {.allocations = 0, .deallocations = 0};

void *dbg_malloc(size_t n, const char* filename, int line) {

    printf("%s, %d, %ld\n",  __FILE__, __LINE__, n); 
    g_debug_info.allocations++;

    return malloc(n); 
}

void dbg_free(void* obj, const char* filename, int line) {
    
    printf("%s, %d, %p\n", __FILE__, __LINE__, obj); 
    free(obj); 
    g_debug_info.deallocations++; 
}

void dbg_info() {
    printf("Allocation sum: %d\n", g_debug_info.allocations - g_debug_info.deallocations);
}