.text
.align 4

.global sys_malloc
.type sys_malloc, %function
sys_malloc:
    ldr     pc, [pc, #-4]
    .word 0x600063c4
