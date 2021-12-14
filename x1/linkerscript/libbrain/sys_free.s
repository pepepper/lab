.text
.align 4

.global sys_free
.type sys_free, %function
sys_free:
    ldr     pc, [pc, #-4]
    .word 0x600063c8
