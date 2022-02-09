#pragma once
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG_MEMORY

    #include "debug/debug.h"

    #define free(obj) dbg_free(obj, __FILE__, __LINE__); 
    #define malloc(n) dbg_malloc(n, __FILE__, __LINE__);
    #define PRINT_DEBUG_INFO() dbg_info();

#else
    #define free(obj) free(obj)
    #define malloc(n) malloc(n);
    #define PRINT_DEBUG_INFO() {};
#endif
