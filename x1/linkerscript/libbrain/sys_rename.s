.text
.align 4

.global sys_rename
.type sys_rename, %function
sys_rename:
    ldr     pc, [pc, #-4]
    .word 0x60006438
