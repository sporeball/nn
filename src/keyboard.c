#include "io.h"
#include "keyboard.h"

static char keymap[256];
static char shiftKeymap[256];

int shift = 0;
int rshift = 0;

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

void init_shiftKeymap() {
  for (int i = 0; i < 256; i++) {
    shiftKeymap[i] = '\0';
  }
  shiftKeymap[KEY_1] = '!';
  shiftKeymap[KEY_2] = '@';
  shiftKeymap[KEY_3] = '#';
  shiftKeymap[KEY_4] = '$';
  shiftKeymap[KEY_5] = '%';
  shiftKeymap[KEY_6] = '^';
  shiftKeymap[KEY_7] = '&';
  shiftKeymap[KEY_8] = '*';
  shiftKeymap[KEY_9] = '(';
  shiftKeymap[KEY_0] = ')';
  shiftKeymap[KEY_DASH] = '_';
  shiftKeymap[KEY_EQUALS] = '+';

  shiftKeymap[KEY_Q] = 'Q';
  shiftKeymap[KEY_W] = 'W';
  shiftKeymap[KEY_E] = 'E';
  shiftKeymap[KEY_R] = 'R';
  shiftKeymap[KEY_T] = 'T';
  shiftKeymap[KEY_Y] = 'Y';
  shiftKeymap[KEY_U] = 'U';
  shiftKeymap[KEY_I] = 'I';
  shiftKeymap[KEY_O] = 'O';
  shiftKeymap[KEY_P] = 'P';
  shiftKeymap[KEY_LSB] = '{';
  shiftKeymap[KEY_RSB] = '}';
  shiftKeymap[KEY_BACKSLASH] = '|';

  shiftKeymap[KEY_A] = 'A';
  shiftKeymap[KEY_S] = 'S';
  shiftKeymap[KEY_D] = 'D';
  shiftKeymap[KEY_F] = 'F';
  shiftKeymap[KEY_G] = 'G';
  shiftKeymap[KEY_H] = 'H';
  shiftKeymap[KEY_J] = 'J';
  shiftKeymap[KEY_K] = 'K';
  shiftKeymap[KEY_L] = 'L';
  shiftKeymap[KEY_SEMICOLON] = ':';
  shiftKeymap[KEY_APOSTROPHE] = '"';
  shiftKeymap[KEY_BACKTICK] = '~';

  shiftKeymap[KEY_Z] = 'Z';
  shiftKeymap[KEY_X] = 'X';
  shiftKeymap[KEY_C] = 'C';
  shiftKeymap[KEY_V] = 'V';
  shiftKeymap[KEY_B] = 'B';
  shiftKeymap[KEY_N] = 'N';
  shiftKeymap[KEY_M] = 'M';
  shiftKeymap[KEY_COMMA] = '<';
  shiftKeymap[KEY_PERIOD] = '>';
  shiftKeymap[KEY_SLASH] = '?';
}

void kb_enter() {
  cursor -= (cursor % 160);
  if (cursor == 3840) {
    fb_scroll();
  } else {
    cursor += 160;
  }
}

// handle keyboard interrupt
void kb_handle() {
  outb(0x20, 0x20);
  unsigned char status = inb(0x64);
  // if the lowest bit is set, a key was just pressed
  if (!(status & 0x01)) {
    return;
  }
  unsigned int scan = inb(0x60);
  // backspace
  if (scan == KEY_BACKSPACE) {
    if (cursor > 0) {
      cursor -= 2;
      fb_write_cell(cursor, ' ', 0, 0);
    }
  }
  if (scan == KEY_ENTER) {
    kb_enter();
  }
  // left shift pressed
  else if (scan == 0x2A) {
    shift = 1;
  }
  // left shift released
  else if (scan == 0xAA) {
    shift = 0;
  }
  // right shift pressed
  else if (scan == 0x36) {
    rshift = 1;
  }
  // right shift released
  else if (scan == 0xB6) {
    rshift = 0;
  }
  else if (keymap[scan] != '\0') {
    if (shift | rshift) {
      fb_write_cell(cursor, shiftKeymap[scan], 0, 15);
    }
    else {
      fb_write_cell(cursor, keymap[scan], 0, 15);
    }
    // scroll the screen if about to go off the edge of the screen
    if (cursor == 3998) {
      kb_enter();
    } else {
      cursor += 2;
    }
  }
}
