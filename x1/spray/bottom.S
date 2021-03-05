.text
    .align 2
    .global _start

_start:
loop:
    b loop
    ldr r0, =0x00000000
    mov pc, r0
