.align 2
.global _start

_start:
    bl main
    ldr r0, =0x00000000
    mov pc, r0
