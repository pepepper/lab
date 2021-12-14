.text
.align 4

.global sys_ferror
.type sys_ferror, %function
sys_ferror:
    ldr     pc, [pc, #-4]
    .word 0x60006430
