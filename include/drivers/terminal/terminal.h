#ifndef _TERMINAL_H
#define _TERMINAL_H 1

#include <stddef.h>
#include <stdint.h>
#include <std/string.h>
#include <std/inline.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color const fg, enum vga_color const bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char const uc, uint8_t const color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

extern size_t terminal_row;
extern size_t terminal_column;
extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;

void terminal_clear(void);
void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_clearline(size_t line);
void terminal_clearlines(size_t from, size_t to);
void terminal_updatecursor(void);
void terminal_scrollup(void);
void terminal_checknewline(void);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);

static inline void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

static inline void terminal_writeline(const char* data) {
	terminal_writestring(data);
	terminal_putchar('\n');
}

#ifdef __cplusplus
}
#endif

#endif /* _TERMINAL_H */
