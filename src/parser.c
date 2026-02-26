#include "parser.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static bool read_line(FILE *f, char *buf, size_t cap) {
    if (!fgets(buf, (int)cap, f)) return false;
    return true;
}

static int parse_ints_from_line(const char *line, int *out, int cap) {
    int n = 0;
    const char *p = line;

    while (*p) {
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;
        if (n >= cap) break;

        char *end = NULL;
        long v = strtol(p, &end, 10);
        if (end == p) break;

        out[n++] = (int)v;
        p = end;
    }
    return n;
}

static void copy_block_to_frame(Memory *mem, int block, int frame) {
    int base = frame * FRAME_SIZE;
    for (int i = 0; i < FRAME_SIZE; i++) {
        mem->PM[base + i] = mem->D[block][i];
    }
}

bool parse_init_file(const char *path, InitSpec *out, Memory *mem) {
    (void)out;

    FILE *f = fopen(path, "r");
    if (!f) return false;

    char line1[8192], line2[8192];
    if (!read_line(f, line1, sizeof(line1))) { fclose(f); return false; }
    if (!read_line(f, line2, sizeof(line2))) { fclose(f); return false; }
    fclose(f);

    int vals1[2048], vals2[8192];
    int n1 = parse_ints_from_line(line1, vals1, (int)(sizeof(vals1)/sizeof(vals1[0])));
    int n2 = parse_ints_from_line(line2, vals2, (int)(sizeof(vals2)/sizeof(vals2[0])));

    // Line 1: (s, z, f) triples where:
    // PM[2s] = z (segment length)
    // PM[2s+1] = f (PT base frame if positive, disk block if negative)
    for (int i = 0; i + 2 < n1; i += 3) {
        int s = vals1[i];
        int z = vals1[i + 1];
        int pt = vals1[i + 2];

        mem->PM[2 * s] = z;
        mem->PM[2 * s + 1] = pt;

        if (pt > 0) mem_mark_used(mem, pt);
        // if pt < 0 => PT on disk, not occupying a frame yet
    }

    // Line 2: (s, p, f) triples defining PT entries.
    // If PT is in PM (PT base positive): PM[pt_base*512 + p] = f
    // If PT is on disk (PT base negative): D[abs(pt_base)][p] = f
    for (int i = 0; i + 2 < n2; i += 3) {
        int s = vals2[i];
        int p = vals2[i + 1];
        int entry = vals2[i + 2];

        int pt_base = mem->PM[2 * s + 1];

        if (pt_base > 0) {
            int idx = pt_base * FRAME_SIZE + p;
            if (idx >= 0 && idx < PM_SIZE) mem->PM[idx] = entry;

            if (entry > 0) mem_mark_used(mem, entry);
        } else if (pt_base < 0) {
            int block = -pt_base;
            if (block >= 0 && block < DISK_BLOCKS && p >= 0 && p < DISK_BLOCK_SIZE) {
                mem->D[block][p] = entry;
            }
            // If entry > 0 but PT is on disk, it doesn't occupy a frame yet.
        } else {
            // pt_base == 0 -> treat as "segment not initialized"; ignore
        }
    }

    // (Optional but correct) If a PT is on disk (negative pt_base), its block should be "valid"
    // already via line2 entries; when it’s paged in later, we copy that disk block to the new frame.

    return true;
}

bool run_va_file(const char *va_path, const char *out_path,
                 const InitSpec *init, Memory *mem) {
    (void)init;

    FILE *in = fopen(va_path, "r");
    if (!in) return false;

    FILE *out = fopen(out_path, "w");
    if (!out) { fclose(in); return false; }

    int va;
    bool first = true;
    while (fscanf(in, "%d", &va) == 1) {
        int pa = -1;
        (void)vm_translate(va, &pa, init, mem);

        if (!first) fprintf(out, " ");
        fprintf(out, "%d", pa);
        first = false;
    }
    fprintf(out, "\n");

    fclose(in);
    fclose(out);
    return true;
}