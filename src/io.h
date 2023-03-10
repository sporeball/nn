#ifndef IO_H
#define IO_H

extern char *framebuffer;
extern unsigned int cursor;

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_scroll();

// todo: figure out how to move these to the asm side
inline unsigned char inb(unsigned short port) {
  unsigned char ret;
  asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}
inline void outb(unsigned short int port, unsigned char val) {
  asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

#endif
