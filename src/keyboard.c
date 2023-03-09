#include "io.h"
#include "keyboard.h"

static char keymap[256];

void init_keymap() {
  for (int i = 0; i < 256; i++) {
    keymap[i] = '\0';
  }
  keymap[KEY_1] = '1';
  keymap[KEY_2] = '2';
  keymap[KEY_3] = '3';
  keymap[KEY_4] = '4';
  keymap[KEY_5] = '5';
  keymap[KEY_6] = '6';
  keymap[KEY_7] = '7';
  keymap[KEY_8] = '8';
  keymap[KEY_9] = '9';
  keymap[KEY_0] = '0';
  keymap[KEY_DASH] = '-';
  keymap[KEY_EQUALS] = '=';

  keymap[KEY_Q] = 'q';
  keymap[KEY_W] = 'w';
  keymap[KEY_E] = 'e';
  keymap[KEY_R] = 'r';
  keymap[KEY_T] = 't';
  keymap[KEY_Y] = 'y';
  keymap[KEY_U] = 'u';
  keymap[KEY_I] = 'i';
  keymap[KEY_O] = 'o';
  keymap[KEY_P] = 'p';
  keymap[KEY_LSB] = '[';
  keymap[KEY_RSB] = ']';
  keymap[KEY_BACKSLASH] = '\\';

  keymap[KEY_A] = 'a';
  keymap[KEY_S] = 's';
  keymap[KEY_D] = 'd';
  keymap[KEY_F] = 'f';
  keymap[KEY_G] = 'g';
  keymap[KEY_H] = 'h';
  keymap[KEY_J] = 'j';
  keymap[KEY_K] = 'k';
  keymap[KEY_L] = 'l';
  keymap[KEY_SEMICOLON] = ';';
  keymap[KEY_APOSTROPHE] = '\'';
  keymap[KEY_BACKTICK] = '`';

  keymap[KEY_Z] = 'z';
  keymap[KEY_X] = 'x';
  keymap[KEY_C] = 'c';
  keymap[KEY_V] = 'v';
  keymap[KEY_B] = 'b';
  keymap[KEY_N] = 'n';
  keymap[KEY_M] = 'm';
  keymap[KEY_COMMA] = ',';
  keymap[KEY_PERIOD] = '.';
  keymap[KEY_SLASH] = '/';
  keymap[KEY_SPACE] = ' ';
}

// handle keyboard interrupt
void kb_handle() {
  outb(0x20, 0x20);
  unsigned char status = inb(0x64);
  // if the lowest bit is set, a key was just pressed
  if (status & 0x01) {
    unsigned int scan = inb(0x60);
    // backspace
    if (scan == 0xE) {
      if (cursor > 0) {
        cursor -= 2;
        fb_write_cell(cursor, ' ', 0, 0);
      }
    }
    else if (keymap[scan] != '\0') {
      fb_write_cell(cursor, keymap[scan], 0, 15);
      cursor += 2;
    }
  }
}
