.text
.align 4

.global sys_fwrite
.type sys_fwrite, %function
sys_fwrite:
    ldr     pc, [pc, #-4]
    .word 0x60006410
