.text
.align 4

.global sys_fgetc
.type sys_fgetc, %function
sys_fgetc:
    ldr     pc, [pc, #-4]
    .word 0x60006420
