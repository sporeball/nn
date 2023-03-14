#include "interrupt.h"
#include "io.h"
#include "keyboard.h"
#include "memory.h"
#include "multiboot.h"

extern void loader();

void peek(int addr) {
  printn((unsigned long) *(unsigned char*)addr);
  print("\r\n");
}

int main(multiboot_info_t *mb) {
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

  init_memory(mb);
  printn(bytes_free);
  print(" bytes free\r\n");
  /* for (int i = 0; i < num_safe_memory_areas; i++) { */
  /* } */

  while (1);
}
