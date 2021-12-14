.text
.align 4

.global sys_fgets
.type sys_fgets, %function
sys_fgets:
    ldr     pc, [pc, #-4]
    .word 0x60006424
