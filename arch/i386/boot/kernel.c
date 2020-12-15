#include <stdbool.h>
#include <stddef.h>
#include <drivers/idt/isr.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/pic/pic.h>
#include <drivers/device/device.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This operating system will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This operating system needs to be compiled with a ix86-elf compiler"
#endif

uint8_t is_running = 1;

void kmain() {
	terminal_initialize();
    terminal_writeline("Initializing terminal... done");
    terminal_writestring("Preparing interrupts... ");
    pic_init();
    isr_install();
    terminal_writeline("done");
    terminal_writestring("Preparing keyboard... ");
    int keyboard_descriptor = keyboard_init();
    terminal_writestring("done, descriptor: ");
    terminal_writeline(itoa(keyboard_descriptor));
    
    terminal_writestring("Are interrupts enabled? ");
    if (are_interrupts_enabled() == 1) {
        terminal_writeline("yes");
    } else {
        terminal_writeline("no");
    }
    terminal_writestring("Is keyboard enabled? ");
    if (are_interrupts_enabled() == 1) {
        terminal_writeline("yes");
    } else {
        terminal_writeline("no");
    }

    terminal_writestring("kmain memory address: ");
    terminal_writeline(itoa(kmain));

    terminal_writeline("----------");

    terminal_writeline("hello yes");
    terminal_writestring("> ");

    while (is_running);
}