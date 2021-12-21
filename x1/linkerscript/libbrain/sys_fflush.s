.text
.align 4

.global sys_fflush
.type sys_fflush, %function
sys_fflush:
    ldr     pc, [pc, #-4]
    .word 0x6000641c
