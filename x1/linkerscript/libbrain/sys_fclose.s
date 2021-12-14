.text
.align 4

.global sys_fclose
.type sys_fclose, %function
sys_fclose:
    ldr     pc, [pc, #-4]
    .word 0x60006404
