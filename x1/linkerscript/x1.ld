OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY {
    RAM (xrw): ORIGIN = 0x90000000, LENGTH = 15M
}

SECTIONS {
    .text : {
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
    } > RAM

    .data : {
        *(.data)
        *(.data*)
        . = ALIGN(4);
    } > RAM

    .bss : {
        __bss_start = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > RAM

    /DISCARD/ : {
        *(.discard)
        *(.discard.*)
        *(.comment)
    }

    .stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
    .ARM.attributes 0 : { *(.ARM.attributes) }
}
