.text
    .align 2
    .global _start

_start:
    mov r9, #0
    ldr r0, =0x67800000

    mrc p15, 0, r10, c1, c0, 0
    bic r10, r10, #1  @ disable MMU and dcache
    @bic r10, r10, #4096  @ disable icache
    mcr p15, 0, r10, c1, c0, 0 // write ctrl regs
    #mcr p15, 0, r9, c7, c7, 0 // invalidate cache
    #mcr p15, 0, r9, c8, c7, 0 // invalidate TLB
    mov pc, r0

