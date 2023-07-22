#ifndef VGA_H
#define VGA_H

extern unsigned char g_640x480x16[61];
void write_regs(unsigned char *regs);
void write_pixel4p(unsigned x, unsigned y, unsigned c);

#endif
