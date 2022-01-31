#include "interrupt.h"
#include "io.h"

int main() {
  // empty the entire framebuffer
  for (int i = 0; i < 4000; i += 2) {
    fb_write_cell(i, ' ', 0, 0);
  }
  // hi!
  fb_write_cell(0, 'n', 0, 13);
  fb_write_cell(2, 'n', 0, 13);

  // initialization
  init_int_kb();
  init_pic();
  load_idt();

  // enable IRQ1 (keyboard)
  outb(0x21, 0xFD);

  while (1);
}
