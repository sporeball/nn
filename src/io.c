#include "io.h"

// these are defined in io.h instead of here:
extern unsigned char inb(unsigned short port);
extern void outb(unsigned short int port, unsigned char val);

char *framebuffer = (char *) 0x000B8000;
unsigned int cursor = 0;

/**
 * write a character to the screen (framebuffer)
 * @param i location in the framebuffer
 * @param c the character
 * @param bg
 * @param fg
 */
void fb_write_cell(unsigned int i, char c, unsigned char bg, unsigned char fg) {
  framebuffer[i] = c;
  framebuffer[i + 1] = ((bg & 0x0F) << 4 | (fg & 0x0F));
}

/**
 * scroll the framebuffer by one row
 */
void fb_scroll() {
  // clear the first row
  for (int i = 0; i < 160; i += 2) {
    fb_write_cell(i, ' ', 0, 0);
  }
  // scroll the existing rows up by 1
  for (int i = 0; i < 3840; i += 2) {
    framebuffer[i] = framebuffer[i + 160];
    framebuffer[i + 1] = framebuffer[i + 161];
  }
  // clear the last row
  for (int i = 3840; i < 4000; i += 2) {
    fb_write_cell(i, ' ', 0, 0);
  }
}

void putchar(char c) {
  switch (c) {
    case '\r':
      cursor -= (cursor % 160);
      break;
    case '\n':
      cursor += 160;
      break;
    default:
      fb_write_cell(cursor, c, 0, 15);
      cursor += 2;
  }
  if (cursor >= 4000) {
    cursor -= 160;
    fb_scroll();
  }
}

void print(const char* str) {
  char c;
  while ((c = *str++) != 0) {
    putchar(c);
  }
}

void printn(unsigned long n) {
  char buffer[16];
  int position = 0;
  if (n == 0) {
    putchar('0');
    return;
  }
  while (n > 0) {
    buffer[position] = (char) ((n % 10) + 48);
    position++;
    n /= 10;
  }
  while (--position >= 0) {
    putchar(buffer[position]);
  }
}
