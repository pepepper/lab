.text
.align 4

.global sys_readdir
.type sys_readdir, %function
sys_readdir:
    ldr     pc, [pc, #-4]
    .word 0x60006450
