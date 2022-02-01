global loader

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER ; magic number + flags + checksum = 0

KERNEL_STACK_SIZE equ 4096

section .bss
align 4
kernel_stack:
  resb KERNEL_STACK_SIZE ; reserve

section .data
align 4

; black magic for GDT
; thanks citrons!
gdt:
  gdt_null:
    dq 0x00
  gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0b11001111
    db 0x00
  gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0b11001111
    db 0x00
  gdt_end:

  gdt_desc:
    dw gdt_end - gdt - 1
    dd gdt

section .text
align 4

; these are made available to C:
global load_idt_asm
global kb_handle_asm
; these are made available to us by C:
extern main
extern kb_handle

  dd MAGIC_NUMBER
  dd FLAGS
  dd CHECKSUM

loader:
  cli
  mov esp, kernel_stack + KERNEL_STACK_SIZE ; point esp to start of stack
  lgdt [gdt_desc]                           ; load GDT into gdtr
  mov ax, 0x10                              ; extra post-GDT black magic...
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  jmp 0x08:continue                         ; move on

continue:
  call main
  hlt

kb_handle_asm:
  call kb_handle
  iret
load_idt_asm:
  mov edx, [esp + 4]
  lidt [edx]         ; load IDT into idtr
  sti                ; start interrupts
  ret

; finish out the sector
times 510-($-$$) db 0
db 0x55
db 0xAA
