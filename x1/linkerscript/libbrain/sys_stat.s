.text
.align 4

.global sys_stat
.type sys_stat, %function
sys_stat:
    ldr     pc, [pc, #-4]
    .word 0x60006444
