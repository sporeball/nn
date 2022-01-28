# nn

**nn** is a toy operating system, designed for fun (and from a position of general naïveté). i'm not sure how far it'll go, but one thing's for sure: it'll probably implement **n**early **n**othing.

### building
ensure [gcc](https://gcc.gnu.org), [nasm](https://www.nasm.us), [binutils](https://www.gnu.org/software/binutils), and [genisoimage(1)](https://linux.die.net/man/1/genisoimage) are installed, then run `make`.

if all goes well, an `nn.iso` file should be produced.

### running
to run **nn** in [qemu](https://www.qemu.org), run `make run`. this requires qemu's `i386` architecture.

to run **nn** in [Bochs](https://bochs.sourceforge.io) instead for its useful debugging functionality, run `make debug`.

### donate
you can support the development of this project and others via Patreon:

[![Support me on Patreon](https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3Dsporeball%26type%3Dpledges%26suffix%3D%252Fmonth&style=for-the-badge)](https://patreon.com/sporeball)
