#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "parser.h"

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <init_file> <va_file> <out_file>\n", prog);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        usage(argv[0]);
        return 1;
    }

    const char *init_path = argv[1];
    const char *va_path   = argv[2];
    const char *out_path  = argv[3];

    Memory mem;
    mem_init(&mem);

    InitSpec init;
    if (!parse_init_file(init_path, &init)) {
        fprintf(stderr, "Error: failed to read init file: %s\n", init_path);
        return 1;
    }

    if (!parse_va_file_and_run(va_path, out_path, &init, &mem)) {
        fprintf(stderr, "Error: failed to process VA file: %s\n", va_path);
        return 1;
    }

    return 0;
}