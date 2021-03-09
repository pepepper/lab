.text
    .align 2
    .global _start

_start:
    ldr r0, =0x67800000

    cpsid if

    mrc p15, 0, r8, c14, c2, 1
    bic r8, r8, #1
    mcr p15, 0, r8, c14, c2, 1
    mrc p15, 0, r8, c14, c3, 1
    bic r8, r8, #1
    mcr p15, 0, r8, c14, c3, 1

    mov r9, #0
    mrc p15, 0, r10, c1, c0, 0
    @bic r10, r10, #5  @ disable MMU and dcache
    bic r10, r10, #1  @ disable MMU
    @bic r10, r10, #4096  @ disable icache
    mcr p15, 0, r10, c1, c0, 0 // write ctrl regs
    #mcr p15, 0, r9, c7, c7, 0 // invalidate cache
    #mcr p15, 0, r9, c8, c7, 0 // invalidate TLB
    mov pc, r0

