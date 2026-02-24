#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "memory.h"

typedef struct InitSpec {
    // We’ll fill these out in the next step after we decide how we store ST/PT entries.
    // For now: placeholder.
    int _placeholder;
} InitSpec;

bool parse_init_file(const char *path, InitSpec *out);
bool parse_va_file_and_run(const char *va_path,
                           const char *out_path,
                           const InitSpec *init,
                           Memory *mem);

#endif