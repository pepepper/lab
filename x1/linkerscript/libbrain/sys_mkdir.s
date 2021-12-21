.text
.align 4

.global sys_mkdir
.type sys_mkdir, %function
sys_mkdir:
    ldr     pc, [pc, #-4]
    .word 0x6000643c
