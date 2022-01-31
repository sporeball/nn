#include "interrupt.h"
#include "io.h"

unsigned long kb_addr;    // location of asm-side kbd handler in memory
unsigned long idt_addr;   // location of IDT in memory
unsigned long idt_ptr[2]; // pointer to give to the lidt asm instruction

struct idt_entry idt[256];

// these are made available to us by asm:
extern void kb_handle_asm();
extern void load_idt_asm(unsigned long *idt_ptr);

// create IDT entry for keyboard handling
// todo: generalize so i don't have to do this 20 times,
//       and in only this file
// cf. levex/osdev
void init_int_kb() {
  kb_addr = (unsigned long) kb_handle_asm;
  idt[0x21].offset_lower = kb_addr & 0xFFFF;
  idt[0x21].selector = 0x08;
  idt[0x21].zero = 0;
  idt[0x21].attr = 0x8E;
  idt[0x21].offset_upper = (kb_addr & 0xFFFF0000) >> 16;
}

void init_pic() {
  // ICW1
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  // ICW2
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  // ICW3
  outb(0x21, 4);
  outb(0xA1, 2);
  // ICW4
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  // disable all interrupts for a little bit
  // main.c will turn them on later
  outb(0x21, 0xFF);
  outb(0xA1, 0xFF);
}

// load IDT into idtr register
void load_idt() {
  idt_addr = (unsigned long) idt;
  idt_ptr[0] = (sizeof (struct idt_entry) * 256) + ((idt_addr & 0xFFFF) << 16);
  idt_ptr[1] = idt_addr >> 16;
  // go off and use the lidt instruction on the asm side
  load_idt_asm(idt_ptr);
}

