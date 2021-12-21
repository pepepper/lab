.text
.align 4

.global sys_remove
.type sys_remove, %function
sys_remove:
    ldr     pc, [pc, #-4]
    .word 0x60006434
