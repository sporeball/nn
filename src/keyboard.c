#include "io.h"

// handle keyboard interrupt
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
