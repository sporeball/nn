#include "io.h"

// these are defined in io.h instead of here:
extern unsigned char inb(unsigned short port);
extern void outb(unsigned short int port, unsigned char val);

char *framebuffer = (char *) 0x000B8000;
unsigned int cursor = 4;

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
