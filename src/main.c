#include "interrupt.h"
#include "io.h"
#include "keyboard.h"
#include "memory.h"
#include "multiboot.h"

extern void loader();

void peek(int addr) {
  printn(*(unsigned char*)addr);
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
  init_memory(mb);
  // memory message
  printn(bytes_free);
  print(" bytes free (regions: ");
  printn(num_safe_memory_areas);
  print(")\r\n");

  // enable IRQ1 (keyboard)
  outb(0x21, 0xFD);

  init_keymap();
  init_shiftKeymap();

  // memory stuff
  /* printn((unsigned long) m); // address of m */
  /* printn((unsigned long) *m); // first byte of m */
  /* char *my_string = malloc(16); */
  /* int *my_ints = malloc(sizeof(int) * 16); */
  /* printn((unsigned long) my_string); */
  /* print("\r\n"); */
  /* printn((unsigned long) my_ints); */
  /* print("\r\n"); */
  /* free(my_string); */
  /* char *my_string2 = malloc(16); */
  /* printn((unsigned long) my_string2); */

  while (1);
}
