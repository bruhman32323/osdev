#include <drivers/terminal/terminal.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_clear(void) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	terminal_clear();
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_clearline(size_t line) {
	for (size_t x = 0; x < VGA_WIDTH; x++) {
		const size_t index = line * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
	}
}

void terminal_clearlines(size_t from, size_t to) {
	for (size_t y = from; y <= to; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_updatecursor(void) {
	size_t temp = terminal_row * VGA_WIDTH + terminal_column;
	outportb(0x3D4, 14);
	outportb(0x3D5, temp >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, temp);
}

void terminal_scrollup(void) {
    for (size_t index = 0; index < VGA_WIDTH * (VGA_HEIGHT - 1); index++) {
    	terminal_buffer[index] = terminal_buffer[index + VGA_WIDTH];
    }
    terminal_clearline(VGA_HEIGHT);
    if ((terminal_row - 1) < 0) {
    	terminal_row = 0;
    	terminal_column = 0;
    } else {
    	terminal_row -= 1;
    }
    terminal_updatecursor();
}

void terminal_checknewline(void) {
	if (terminal_row >= VGA_HEIGHT - 1) {
		terminal_scrollup();
	}
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	terminal_buffer[y * VGA_WIDTH + x] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	switch (c) {
		case '\b':
			if (--terminal_column < 0) {
				if (terminal_row-- > 0) {
					terminal_column = VGA_WIDTH - 1;
					terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
				}
			} else {
				terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
			}
			terminal_updatecursor();
			return;
		case '\r':
			terminal_column = 0;
			break;
		case '\n':
			terminal_column = 0;
			terminal_row++;
			break;
		default:
			terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
			terminal_column++;
	}

	if (terminal_column >= VGA_WIDTH) {
		terminal_column = 0;
		terminal_row++;
	}

	terminal_updatecursor();
	terminal_checknewline();
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
