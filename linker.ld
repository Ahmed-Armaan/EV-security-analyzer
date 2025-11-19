ENTRY(Reset_Handler)

SECTIONS
{
    . = 0x08000000;

    .text : {
        *(.isr_vector)
        *(.text*)
        *(.rodata*)
    }

    . = 0x20000000;
    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }
}
