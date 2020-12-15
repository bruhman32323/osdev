TARGET = i686-elf
ARCH = i386
CC = $(TARGET)-gcc
STRIP = $(TARGET)-strip
NASM = nasm
QEMU = qemu-system-i386
MKDIR_P = mkdir -p

CFLAGS += -std=gnu18 -ffreestanding -O2 -Wall -Wextra -mgeneral-regs-only
LDFLAGS += -ffreestanding -O2 -nostdlib -lgcc
NASMFLAGS += -felf32

BIN = sysroot/boot/hhhos.bin
ISO = build/HhhOS.iso

DIRS = build build/boot build/std build/drivers build/drivers/terminal build/drivers/idt build/drivers/device build/drivers/keyboard build/drivers/pic build/drivers/ahci

OBJS = build/boot/boot.o build/boot/kernel.o build/std/string.o build/drivers/terminal/terminal.o build/drivers/idt/idt.o \
build/drivers/idt/isr.o build/std/util.o build/drivers/device/device.o build/drivers/keyboard/keyboard.o build/drivers/pic/pic.o

ARCHDIR = arch/$(ARCH)/

.PHONY:	all run clean build

dirs:
	${MKDIR_P} ${DIRS}

all:	dirs $(BIN)

run:	$(ISO)
	$(QEMU) -cdrom $(ISO)

$(ISO): dirs $(BIN)
	grub-mkrescue -o $(ISO) sysroot

$(BIN):	$(OBJS)
	$(CC) -T linker.ld $(LDFLAGS) -o $@ $^

build/%.o:	$(ARCHDIR)%.c
	$(CC) -c $(CFLAGS) -Iinclude -o $@ $<

build/%.o:	$(ARCHDIR)%.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

clean:
	rm -rf $(OBJS) $(BIN) $(ISO)
