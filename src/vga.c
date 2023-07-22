#include "io.h"
#include "vga.h"

#define	VGA_AC_INDEX 0x3C0
#define	VGA_AC_WRITE 0x3C0
#define	VGA_AC_READ 0x3C1
#define	VGA_MISC_WRITE 0x3C2
#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA 0x3C5
#define	VGA_DAC_READ_INDEX 0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA 0x3C9
#define	VGA_MISC_READ 0x3CC
#define VGA_GC_INDEX 0x3CE
#define VGA_GC_DATA 0x3CF
#define VGA_CRTC_INDEX 0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA 0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ 0x3DA

#define	VGA_NUM_SEQ_REGS 5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS 9
#define	VGA_NUM_AC_REGS 21
#define	VGA_NUM_REGS (1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

#define	peekb(S,O)		_farpeekb(_go32_info_block.selector_for_linear_memory, 16uL * (S) + (O))
#define	pokeb(S,O,V)		_farpokeb(_go32_info_block.selector_for_linear_memory, 16uL * (S) + (O), V)

unsigned char g_640x480x16[61] = {
  // MISC
	0xE3,
  // SEQ
	0x03, 0x01, 0x08, 0x00, 0x06,
  // CRTC
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	0xFF,
  // GC
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,
  // AC
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x01, 0x00, 0x0F, 0x00, 0x00
};

typedef struct {
  unsigned long size_of_this_structure_in_bytes;
  unsigned long linear_address_of_primary_screen;
  unsigned long linear_address_of_secondary_screen;
  unsigned long linear_address_of_transfer_buffer;
  unsigned long size_of_transfer_buffer; /* >= 4k */
  unsigned long pid;
  unsigned char master_interrupt_controller_base;
  unsigned char slave_interrupt_controller_base;
  unsigned short selector_for_linear_memory;
  unsigned long linear_address_of_stub_info_structure;
  unsigned long linear_address_of_original_psp;
  unsigned short run_mode;
  unsigned short run_mode_info;
} Go32_Info_Block;

Go32_Info_Block _go32_info_block;

static unsigned g_wd = 640;
/* static unsigned g_ht = 480; */

extern inline void
_farpokeb(unsigned short selector,
	 unsigned long offset,
	 unsigned char value)
{
  __asm__ __volatile__ ("movw %w0,%%fs\n"
      "	.byte 0x64 \n"
      "	movb %b1,(%k2)"
      :
      : "rm" (selector), "qi" (value), "r" (offset));
}

extern inline unsigned char
_farpeekb(unsigned short selector,
	 unsigned long offset)
{
  unsigned char result;
  __asm__ __volatile__ ("movw %w1,%%fs \n"
      "	.byte 0x64 \n"
      "	movb (%k2),%b0"
      : "=q" (result)
      : "rm" (selector), "r" (offset));
  return result;
}
static unsigned get_fb_seg(void) {
	unsigned seg;

	outb(VGA_GC_INDEX, 6);
	seg = inb(VGA_GC_DATA);
	seg >>= 2;
	seg &= 3;
	switch(seg)
	{
	case 0:
	case 1:
		seg = 0xA000;
		break;
	case 2:
		seg = 0xB000;
		break;
	case 3:
		seg = 0xB800;
		break;
	}
	return seg;
}



static void vpokeb(unsigned off, unsigned val) {
	pokeb(get_fb_seg(), off, val);
}

static unsigned vpeekb(unsigned off) {
	return peekb(get_fb_seg(), off);
}

static void set_plane(unsigned p) {
	unsigned char pmask;

	p &= 3;
	pmask = 1 << p;
/* set read plane */
	outb(VGA_GC_INDEX, 4);
	outb(VGA_GC_DATA, p);
/* set write plane */
	outb(VGA_SEQ_INDEX, 2);
	outb(VGA_SEQ_DATA, pmask);
}

void write_regs(unsigned char *regs) {
	unsigned i;

/* write MISCELLANEOUS reg */
	outb(VGA_MISC_WRITE, *regs);
	regs++;
/* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outb(VGA_SEQ_INDEX, i);
		outb(VGA_SEQ_DATA, *regs);
		regs++;
	}
/* unlock CRTC registers */
	outb(VGA_CRTC_INDEX, 0x03);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
	outb(VGA_CRTC_INDEX, 0x11);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
/* write CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outb(VGA_CRTC_INDEX, i);
		outb(VGA_CRTC_DATA, *regs);
		regs++;
	}
/* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outb(VGA_GC_INDEX, i);
		outb(VGA_GC_DATA, *regs);
		regs++;
	}
/* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		outb(VGA_AC_WRITE, *regs);
		regs++;
	}
/* lock 16-color palette and unblank display */
	(void)inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);
}

void write_pixel4p(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes, off, mask, p, pmask;

	wd_in_bytes = g_wd / 8;
	off = wd_in_bytes * y + x / 8;
	x = (x & 7) * 1;
	mask = 0x80 >> x;
	pmask = 1;
	for(p = 0; p < 4; p++)
	{
		set_plane(p);
		if(pmask & c)
			vpokeb(off, vpeekb(off) | mask);
		else
			vpokeb(off, vpeekb(off) & ~mask);
		pmask <<= 1;
	}
}
