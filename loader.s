global loader

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER ; magic number + flags + checksum = 0

KERNEL_STACK_SIZE equ 4096

section .bss
align 4
kernel_stack:
  resb KERNEL_STACK_SIZE

section .text
align 4

extern sum_of_three

  dd MAGIC_NUMBER
  dd FLAGS
  dd CHECKSUM

loader:
  push dword 30
  push dword 20
  push dword 10
  call sum_of_three ; result goes in eax
loop:
  jmp loop
