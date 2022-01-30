#ifndef INTERRUPT_H
#define INTERRUPT_H

struct idt_entry {
  unsigned short int offset_lower;
  unsigned short int selector;
  unsigned char zero;
  unsigned char attr;
  unsigned short int offset_upper;
};

void init_pic();

#endif
