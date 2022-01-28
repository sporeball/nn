global loader

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER ; magic number + flags + checksum = 0

KERNEL_STACK_SIZE equ 4096

section .bss
align 4
kernel_stack:
  resb KERNEL_STACK_SIZE ; reserve

section .text
align 4

extern main
; these are made available to C:
global load_idt
global kb_handle_asm
; these are made available to us by C:
extern main
extern kb_handle

  dd MAGIC_NUMBER
  dd FLAGS
  dd CHECKSUM

loader:
  call main
  hlt
  cli
  mov esp, kernel_stack + KERNEL_STACK_SIZE
  call main
  hlt

kb_handle_asm:
  call kb_handle
  iretd
load_idt:
  mov edx, [esp + 4]
  lidt [edx]         ; load IDT into idtr
  sti                ; start interrupts
  ret
