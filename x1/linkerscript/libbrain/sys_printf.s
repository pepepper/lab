.text
.align 4

.global sys_printf
.type sys_printf, %function
sys_printf:
    ldr     pc, [pc, #-4]
    .word 0x60092004
