#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "memory.h"

typedef struct InitSpec {
    // For now we don’t need extra metadata beyond what’s loaded into PM/D,
    // but this keeps the door open for later extensions.
    int _unused;
} InitSpec;

bool parse_init_file(const char *path, InitSpec *out, Memory *mem);

// Reads VAs from va_path, writes translated PAs (or -1) to out_path.
bool run_va_file(const char *va_path, const char *out_path,
                 const InitSpec *init, Memory *mem);

#endif