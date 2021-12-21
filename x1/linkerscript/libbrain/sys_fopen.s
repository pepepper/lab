.text
.align 4

.global sys_fopen
.type sys_fopen, %function
sys_fopen:
    ldr     pc, [pc, #-4]
    .word 0x60006400
