ENTRY(loader)

SECTIONS {
  . = 0x00100000;        /* 1MB */

  .text ALIGN (0x1000) : /* 4KB */
  {
    *(.multiboot)
    *(.text)             /* all text sections from all files */
  }

  .rodata ALIGN (0x1000) :
  {
    *(.rodata*)
  }

  /* .data ALIGN (0x1000) : */
  /* { */
  /*   *(.data) */
  /* } */

  .bss ALIGN (0x1000) :
  {
    *(COMMON)
    *(.bss)
  }
}
