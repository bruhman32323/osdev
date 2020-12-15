#ifndef INLINE_H_
#define INLINE_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline void outportb(uint16_t const port, uint8_t const val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void outportw(uint16_t const port, uint16_t const val) {
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void outportl(uint16_t const port, uint32_t const val) {
    asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inportb(uint16_t const port) {
	uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline uint16_t inportw(uint16_t const port) {
	uint16_t ret;
    asm volatile ( "inw %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline uint32_t inportl(uint16_t const port) {
	uint32_t ret;
    asm volatile ( "inl %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline void io_wait() {
	asm volatile("outb %%al, $0x80" : : "a"(0));
}

static inline bool are_interrupts_enabled() {
	unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

#ifdef __cplusplus
}
#endif

#endif /* INLINE_H_ */
