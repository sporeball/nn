char *framebuffer = (char *) 0x000B8000;

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

int main() {
  // empty the entire framebuffer
  for (int i = 0; i < 4000; i += 2) {
    fb_write_cell(i, ' ', 0, 0);
  }
  // hi!
  fb_write_cell(0, 'n', 0, 13);
  fb_write_cell(2, 'n', 0, 13);

  while (1);
}
