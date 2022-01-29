char *framebuffer = (char *) 0x000B8000;
unsigned int cur = 4;

unsigned long kb_addr;    // location of asm-side kbd handler in memory
unsigned long idt_addr;   // location of IDT in memory
unsigned long idt_ptr[2]; // pointer to give to the lidt asm instruction

struct idt_entry {
  unsigned short int offset_lower;
  unsigned short int selector;
  unsigned char zero;
  unsigned char attr;
  unsigned short int offset_upper;
};

// the IDT
struct idt_entry idt[256];

// these are made available to us by asm:
extern void kb_handle_asm();
extern void load_idt(unsigned long *idt_ptr);

// todo: figure out how to move these to the asm side
static inline unsigned char inb(unsigned short port) {
  unsigned char ret;
  asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}
static inline void outb(unsigned short int port, unsigned char val) {
  asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/**
 * write a character to the screen (framebuffer)
 * @param i  location in the framebuffer
 * @param c  the character
 * @param bg
 * @param fg
 */
void fb_write_cell(unsigned int i, char c, unsigned char bg, unsigned char fg) {
  framebuffer[i] = c;
  framebuffer[i + 1] = ((bg & 0x0F) << 4 | (fg & 0x0F));
}

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
      fb_write_cell(cur, 'a', 0, 15);
      cur += 2;
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

  // initialize PIC
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

  // send IDT to the asm side
  idt_addr = (unsigned long) idt;
  idt_ptr[0] = (sizeof (struct idt_entry) * 256) + ((idt_addr & 0xFFFF) << 16);
  idt_ptr[1] = idt_addr >> 16;
  load_idt(idt_ptr);

  // enable IRQ1 (keyboard)
  outb(0x21, 0xFD);

  while (1);
}
