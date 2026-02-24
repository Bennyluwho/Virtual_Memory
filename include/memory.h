#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define PM_SIZE 524288

typedef struct Memory {
    int PM[PM_SIZE];
    // For demand paging later:
    // int D[1024][512];
} Memory;

void mem_init(Memory *m);

#endif