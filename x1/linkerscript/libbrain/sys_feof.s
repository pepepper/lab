.text
.align 4

.global sys_feof
.type sys_feof, %function
sys_feof:
    ldr     pc, [pc, #-4]
    .word 0x60006418
