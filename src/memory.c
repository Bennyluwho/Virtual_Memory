#include "memory.h"

void mem_init(Memory *m) {
    // Spec implies PM is an integer array; we’ll initialize to 0.
    // Later, you can change default fill if your instructor expects something else.
    for (size_t i = 0; i < PM_SIZE; i++) {
        m->PM[i] = 0;
    }
}