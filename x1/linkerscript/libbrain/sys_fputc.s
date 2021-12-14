.text
.align 4

.global sys_fputc
.type sys_fputc, %function
sys_fputc:
    ldr     pc, [pc, #-4]
    .word 0x60006428
