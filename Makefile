.SILENT:

.DEFAULT_GOAL = nn.iso
.RECIPEPREFIX = ~

# full list of .o files we expect to exist by step 2
OBJECTS = loader.o kmain.o

# here be flags
CFLAGS    = -c -m32 -ffreestanding -fno-builtin -fno-stack-protector   \
              -nodefaultlibs -nostartfiles -nostdlib                   \
              -Wall -Wextra -Werror
ASFLAGS   = -f elf
LDFLAGS   = -T link.ld -melf_i386
ISOFLAGS  = -R -b boot/grub/stage2_eltorito -no-emul-boot              \
              -boot-load-size 4 -A nn -input-charset utf8 -quiet       \
              -boot-info-table -o nn.iso iso
QEMUFLAGS = -accel tcg,thread=single -cpu n270 -m 128 -no-reboot       \
              -drive format=raw,media=cdrom,file=nn.iso
BCHSFLAGS = -q

# step 1: assemble all .c files into .o files
%.o: %.c
~ gcc $(CFLAGS) $< -o $@

# step 2: assemble all .s files into .o files
%.o: %.s
~ nasm $(ASFLAGS) $< -o $@

# step 3: combine all .o files into a single .elf file
kernel.elf: $(OBJECTS)
~ ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

# step 4: create an ISO
nn.iso: kernel.elf
~ mv kernel.elf iso/boot
~ genisoimage $(ISOFLAGS)

# boot nn (in qemu)
run: nn.iso
~ qemu-system-i386 $(QEMUFLAGS)

# boot nn (in Bochs, for debugging)
debug: nn.iso
~ bochs $(BCHSFLAGS)

clean:
~ rm -rf *.o *.log iso/boot/kernel.elf nn.iso
