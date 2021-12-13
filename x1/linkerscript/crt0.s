.text
.arm
.align 4

.global _start
_start:
    stmdb   sp!, {r4, lr}
    /* store context */
    movs    r4, r0

    /* Clear the .bss section */
    bl      .L_clean_bss

    /* restore context */
    adr     r0, [.L_context]
    str     r4, [r0]
    ldr     r0, [r0]

    /* call user's main */
    bl      main

    ldmia   sp!, {r4, pc}

.L_context:
.long 0

/*
    r0 = start address
    r1 = end address
    tmp: r2
*/
.L_clean_bss:
    ldr     r0, =__bss_start
    ldr     r1, =__bss_end
    sub     r1, r1, #4
    mov     r2, #0
.L_clean_bss_loop:
    cmp     r0, r1
    strlt   r2, [r0], #4
    blt     .L_clean_bss_loop
    bx      lr
