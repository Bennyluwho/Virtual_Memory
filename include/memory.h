#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stddef.h>

#define PM_SIZE 524288
#define FRAME_SIZE 512
#define NUM_FRAMES (PM_SIZE / FRAME_SIZE)

#define DISK_BLOCKS 1024
#define DISK_BLOCK_SIZE 512

typedef struct Memory {
    int PM[PM_SIZE];
    int D[DISK_BLOCKS][DISK_BLOCK_SIZE];

    // Frame allocation (for demand paging)
    bool used_frame[NUM_FRAMES];
    int next_free_scan; // next candidate frame index to try
} Memory;

void mem_init(Memory *m);

// mark frame as used (bounds-checked)
void mem_mark_used(Memory *m, int frame);

// allocate next free frame (ascending search). returns -1 if none.
int mem_alloc_frame(Memory *m);

#endif