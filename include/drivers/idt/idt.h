#ifndef _IDT_H
#define _IDT_H 1

#include <stdint.h> 
#include <std/util.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define INTERRUPT_GATE 0x8e

struct idt_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

struct interrupt_frame
{
    uintptr_t ip;
    uintptr_t cs;
    uintptr_t flags;
    uintptr_t sp;
    uintptr_t ss;
};

void idt_init();
void idt_register_handler(uint8_t interrupt, unsigned long address);

#ifdef __cplusplus
}
#endif

#endif /* _IDT_H */