.text
.align 4

.global sys_opendir
.type sys_opendir, %function
sys_opendir:
    ldr     pc, [pc, #-4]
    .word 0x60006454
