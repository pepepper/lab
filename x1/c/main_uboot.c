#include "common.h"
#include "uboot_bin.h"

#define VIRT_START (unsigned char *)0x8c000000

void main() {
    int i;
    u8* ptr = VIRT_START;

    // Turn off interrupts
    asm volatile (
        "cpsid if \n"
    );

    for (i=0; i<UBOOT_SIZE; i++) {
        writeb(uboot_bin[i], ptr);
        ptr++;
    }

    asm volatile (
        "mrc p15, 0, r8, c14, c2, 1 \n"
        "bic r8, r8, #1 \n"
        "mcr p15, 0, r8, c14, c2, 1 \n"
        "mrc p15, 0, r8, c14, c3, 1 \n"
        "bic r8, r8, #1 \n"
        "mcr p15, 0, r8, c14, c3, 1 \n"

        "mrc p15, 0, r10, c1, c0, 0 \n"
        "bic r10, r10, #1 \n"
        "mcr p15, 0, r10, c1, c0, 0 \n"
        "ldr r0, =0x60200000 \n"
        "mov pc, r0"
    );
}
