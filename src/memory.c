#include "memory.h"

static void zero_pm(int *pm, size_t n) {
    for (size_t i = 0; i < n; i++) pm[i] = 0;
}

static void zero_disk(int d[DISK_BLOCKS][DISK_BLOCK_SIZE]) {
    for (int b = 0; b < DISK_BLOCKS; b++) {
        for (int i = 0; i < DISK_BLOCK_SIZE; i++) d[b][i] = 0;
    }
}

void mem_init(Memory *m) {
    zero_pm(m->PM, PM_SIZE);
    zero_disk(m->D);

    for (int i = 0; i < NUM_FRAMES; i++) m->used_frame[i] = false;

    // Frames 0 and 1 are occupied because the segment table lives in PM[0..1023]
    // (2 words per segment for 512 segments => 1024 ints => frames 0 and 1).
    m->used_frame[0] = true;
    m->used_frame[1] = true;

    m->next_free_scan = 2;
}

void mem_mark_used(Memory *m, int frame) {
    if (frame >= 0 && frame < NUM_FRAMES) {
        m->used_frame[frame] = true;
        if (frame == m->next_free_scan) {
            // advance scan pointer
            while (m->next_free_scan < NUM_FRAMES && m->used_frame[m->next_free_scan]) {
                m->next_free_scan++;
            }
        }
    }
}

int mem_alloc_frame(Memory *m) {
    for (int f = m->next_free_scan; f < NUM_FRAMES; f++) {
        if (!m->used_frame[f]) {
            m->used_frame[f] = true;
            m->next_free_scan = f + 1;
            while (m->next_free_scan < NUM_FRAMES && m->used_frame[m->next_free_scan]) {
                m->next_free_scan++;
            }
            return f;
        }
    }
    return -1;
}