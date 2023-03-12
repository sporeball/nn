#include "interrupt.h"
#include "io.h"
#include "keyboard.h"

int main() {
  // empty the entire framebuffer
  for (int i = 0; i < 4000; i += 2) {
    fb_write_cell(i, ' ', 0, 0);
  }
  // hi!
  print("nn\r\n");

  // initialization
  init_int_kb();
  init_pic();
  load_idt();

  // enable IRQ1 (keyboard)
  outb(0x21, 0xFD);

  init_keymap();
  init_shiftKeymap();

  while (1);
}
