.align 2
.global _start

_start:
    push {r4, lr}
    bl main
    pop {r4, pc}
