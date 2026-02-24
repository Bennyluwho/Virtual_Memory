#ifndef VM_H
#define VM_H

#include <stdbool.h>
#include "memory.h"
#include "parser.h"

// Translates one virtual address to physical address.
// Returns true on success, false on fault/error.
// On failure, *out_pa should be set to -1 (basic rules later).
bool vm_translate(int va, int *out_pa, const InitSpec *init, Memory *mem);

#endif