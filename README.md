# nn

**nn** is a toy operating system, designed for fun (and from a position of general naïveté). i'm not sure how far it'll go, but one thing's for sure: it'll probably implement **n**early **n**othing.

### building
ensure [gcc](https://gcc.gnu.org), [nasm](https://www.nasm.us), [binutils](https://www.gnu.org/software/binutils), [GRUB](https://www.gnu.org/software/grub), and [xorriso](https://www.gnu.org/software/xorriso) are installed, then run `make`.

if all goes well, an `nn.iso` file should be produced.

### running
to run **nn** in [qemu](https://www.qemu.org), run `make run`. this requires qemu's `i386` architecture.

to run **nn** in [Bochs](https://bochs.sourceforge.io) instead for its useful debugging functionality, run `make debug`.
