.text
.align 4

.global sys_closedir
.type sys_closedir, %function
sys_closedir:
    ldr     pc, [pc, #-4]
    .word 0x6000644c
