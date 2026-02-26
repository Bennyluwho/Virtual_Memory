#include <stdio.h>
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

    Memory mem;
    mem_init(&mem);

    InitSpec init;
    if (!parse_init_file(argv[1], &init, &mem)) {
        fprintf(stderr, "Error: failed to read init file\n");
        return 1;
    }

    if (!run_va_file(argv[2], argv[3], &init, &mem)) {
        fprintf(stderr, "Error: failed to process VA file\n");
        return 1;
    }

    return 0;
}