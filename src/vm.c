#include "vm.h"

static void copy_block_to_frame(Memory *mem, int block, int frame) {
    int base = frame * FRAME_SIZE;
    for (int i = 0; i < FRAME_SIZE; i++) {
        mem->PM[base + i] = mem->D[block][i];
    }
}

bool vm_translate(int va, int *out_pa, const InitSpec *init, Memory *mem) {
    (void)init;
    *out_pa = -1;

    // s, p, w are 9 bits each
    int s = (va >> 18) & 0x1FF;
    int p = (va >> 9)  & 0x1FF;
    int w = va & 0x1FF;

    int seg_len = mem->PM[2 * s];
    int pt_base = mem->PM[2 * s + 1];

    // Segment not present
    if (seg_len <= 0 || pt_base == 0) return false;

    // Check bounds: offset within segment must be < seg_len
    int offset = p * FRAME_SIZE + w;
    if (offset >= seg_len) return false;

    // If PT is on disk (negative), page it into a free frame
    if (pt_base < 0) {
        int block = -pt_base;
        int new_frame = mem_alloc_frame(mem);
        if (new_frame < 0) return false; // spec says enough frames exist, but be safe

        copy_block_to_frame(mem, block, new_frame);

        pt_base = new_frame;
        mem->PM[2 * s + 1] = pt_base; // update ST entry
    }

    // Read PT entry
    int pte_addr = pt_base * FRAME_SIZE + p;
    if (pte_addr < 0 || pte_addr >= PM_SIZE) return false;

    int page_frame = mem->PM[pte_addr];

    // If page not mapped, fail
    if (page_frame == 0) return false;

    // If page is on disk (negative), page it into a free frame
    if (page_frame < 0) {
        int block = -page_frame;
        int new_frame = mem_alloc_frame(mem);
        if (new_frame < 0) return false;

        copy_block_to_frame(mem, block, new_frame);

        page_frame = new_frame;
        mem->PM[pte_addr] = page_frame; // update PT entry
    }

    // Now translate to physical address
    *out_pa = page_frame * FRAME_SIZE + w;
    return true;
}