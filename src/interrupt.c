#include "interrupt.h"
#include "io.h"

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
  outb(0x21, 0xFF);
  outb(0xA1, 0xFF);
}
