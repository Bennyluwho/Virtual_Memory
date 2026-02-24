#include "parser.h"
#include <stdio.h>

bool parse_init_file(const char *path, InitSpec *out) {
    (void)out;
    FILE *f = fopen(path, "r");
    if (!f) return false;

    // TODO (next step): read line 1 (ST triples) and line 2 (PT triples)
    fclose(f);
    return true;
}

bool parse_va_file_and_run(const char *va_path,
                           const char *out_path,
                           const InitSpec *init,
                           Memory *mem) {
    (void)init;
    (void)mem;

    FILE *in = fopen(va_path, "r");
    if (!in) return false;

    FILE *out = fopen(out_path, "w");
    if (!out) { fclose(in); return false; }

    // TODO (future step): read each VA, call vm_translate, write PA/-1.
    fclose(in);
    fclose(out);
    return true;
}