#ifndef VM_H
#define VM_H

#include <stdbool.h>
#include "memory.h"
#include "parser.h"

bool vm_translate(int va, int *out_pa, const InitSpec *init, Memory *mem);

#endif