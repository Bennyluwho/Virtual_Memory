#include "vm.h"

bool vm_translate(int va, int *out_pa, const InitSpec *init, Memory *mem) {
    (void)va;
    (void)init;
    (void)mem;
    *out_pa = -1;
    // TODO (later): implement s/p/w extraction and translation logic.
    return false;
}