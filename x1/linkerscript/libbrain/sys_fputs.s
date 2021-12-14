.text
.align 4

.global sys_fputs
.type sys_fputs, %function
sys_fputs:
    ldr     pc, [pc, #-4]
    .word 0x6000642c
