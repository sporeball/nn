#include "interrupt.h"
#include "io.h"

unsigned long kb_addr;    // location of asm-side kbd handler in memory
unsigned long idt_addr;   // location of IDT in memory
unsigned long idt_ptr[2]; // pointer to give to the lidt asm instruction

// the IDT
struct idt_entry idt[256];

// these are made available to us by asm:
extern void kb_handle_asm();
extern void load_idt_asm(unsigned long *idt_ptr);

// handle a keyboard interrupt
void kb_handle() {
  outb(0x20, 0x20);
  unsigned char status = inb(0x64);
  // if the lowest bit is set, a key was just pressed
  if (status & 0x01) {
    char scan = inb(0x60);
    // only respond to the a key for now
    // todo: create a map
    if (scan == 0x1E) {
      fb_write_cell(cursor, 'a', 0, 15);
      cursor += 2;
    }
  }
}

int main() {
  // empty the entire framebuffer
  for (int i = 0; i < 4000; i += 2) {
    fb_write_cell(i, ' ', 0, 0);
  }
  // hi!
  fb_write_cell(0, 'n', 0, 13);
  fb_write_cell(2, 'n', 0, 13);

  // initialize keyboard interrupt handle in IDT
  kb_addr = (unsigned long) kb_handle_asm;
  idt[0x21].offset_lower = kb_addr & 0xFFFF;
  idt[0x21].selector = 0x08;
  idt[0x21].zero = 0;
  idt[0x21].attr = 0x8E;
  idt[0x21].offset_upper = (kb_addr & 0xFFFF0000) >> 16;

  init_pic();

  // send IDT to the asm side
  idt_addr = (unsigned long) idt;
  idt_ptr[0] = (sizeof (struct idt_entry) * 256) + ((idt_addr & 0xFFFF) << 16);
  idt_ptr[1] = idt_addr >> 16;
  load_idt_asm(idt_ptr);

  // enable IRQ1 (keyboard)
  outb(0x21, 0xFD);

  while (1);
}
