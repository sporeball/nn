.SILENT:

# full list of .o files we expect to exist by step 2
OBJECTS = loader.o

ASFLAGS = -f elf
LDFLAGS = -T link.ld -melf_i386

# step 1: assemble all .s files into .o files
%.o: %.s
	nasm $(ASFLAGS) $< -o $@

# step 2: combine all .o files into a single .elf file
kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

# step 3: create an ISO
nn.iso: kernel.elf
	mv kernel.elf iso/boot
	genisoimage -R								\
				-b boot/grub/stage2_eltorito	\
				-no-emul-boot					\
				-boot-load-size 4				\
				-A nn							\
				-input-charset utf8				\
				-quiet							\
				-boot-info-table				\
				-o nn.iso						\
				iso

# boot nn (in qemu)
run: nn.iso
	qemu-system-i386	-accel tcg,thread=single					\
						-cpu n270									\
						-m 128										\
						-no-reboot									\
						-drive format=raw,media=cdrom,file=nn.iso	\
						-serial stdio								\
						-smp 1										\
						-usb										\
						-vga std

# boot nn (in Bochs, for debugging)
debug: nn.iso
	bochs -f .bochsrc -q

clean:
	rm -rf *.o *.log iso/boot/kernel.elf nn.iso
