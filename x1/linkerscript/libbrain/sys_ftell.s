.text
.align 4

.global sys_ftell
.type sys_ftell, %function
sys_ftell:
    ldr     pc, [pc, #-4]
    .word 0x60006408
