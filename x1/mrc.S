.text
    .align 2
    .global _start

_start:
    mrc p15, 0, r10, c1, c0, 0
    mov pc, lr
