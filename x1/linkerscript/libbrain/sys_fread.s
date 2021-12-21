.text
.align 4

.global sys_fread
.type sys_fread, %function
sys_fread:
    ldr     pc, [pc, #-4]
    .word 0x6000640c
