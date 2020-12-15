#include <drivers/idt/isr.h>
#include <drivers/terminal/terminal.h>

__attribute__((interrupt)) static void isr0(struct interrupt_frame* frame) {
    terminal_writeline("Division By Zero");
    asm("hlt");
}

__attribute__((interrupt)) static void isr1(struct interrupt_frame* frame) {
    terminal_writeline("Debug");
    asm("hlt");
}

__attribute__((interrupt)) static void isr2(struct interrupt_frame* frame) {
    terminal_writeline("Non Maskable Interrupt");
    asm("hlt");
}

__attribute__((interrupt)) static void isr3(struct interrupt_frame* frame) {
    terminal_writeline("Breakpoint");
    asm("hlt");
}

__attribute__((interrupt)) static void isr4(struct interrupt_frame* frame) {
    terminal_writeline("Into Detected Overflow");
    asm("hlt");
}

__attribute__((interrupt)) static void isr5(struct interrupt_frame* frame) {
    terminal_writeline("Out of Bounds");
    asm("hlt");
}

__attribute__((interrupt)) static void isr6(struct interrupt_frame* frame) {
    terminal_writeline("Invalid Opcode");
    asm("hlt");
}

__attribute__((interrupt)) static void isr7(struct interrupt_frame* frame) {
    terminal_writeline("No Coprocessor");
    asm("hlt");
}

__attribute__((interrupt)) static void isr8(struct interrupt_frame* frame) {
    terminal_writeline("Double Fault");
    asm("hlt");
}

__attribute__((interrupt)) static void isr9(struct interrupt_frame* frame) {
    terminal_writeline("Coprocessor Segment Overrun");
    asm("hlt");
}

__attribute__((interrupt)) static void isr10(struct interrupt_frame* frame) {
    terminal_writeline("Bad TSS");
    asm("hlt");
}

__attribute__((interrupt)) static void isr11(struct interrupt_frame* frame) {
    terminal_writeline("Segment Not Present");
    asm("hlt");
}

__attribute__((interrupt)) static void isr12(struct interrupt_frame* frame) {
    terminal_writeline("Stack Fault");
    asm("hlt");
}

__attribute__((interrupt)) static void isr13(struct interrupt_frame* frame) {
    terminal_writeline("General Protection Fault");

    terminal_writeline("heres some info:");

    terminal_writestring("ip: ");
    terminal_writeline(itoa(frame->ip));
    terminal_writestring("sp: ");
    terminal_writeline(itoa(frame->sp));
    terminal_writestring("cs: ");
    terminal_writeline(itoa(frame->cs));

    asm("hlt");
}

__attribute__((interrupt)) static void isr14(struct interrupt_frame* frame) {
    terminal_writeline("Page Fault");
    asm("hlt");
}

__attribute__((interrupt)) static void isr15(struct interrupt_frame* frame) {
    terminal_writeline("Unknown Interrupt");
    asm("hlt");
}

__attribute__((interrupt)) static void isr16(struct interrupt_frame* frame) {
    terminal_writeline("Coprocessor Fault");
    asm("hlt");
}

__attribute__((interrupt)) static void isr17(struct interrupt_frame* frame) {
    terminal_writeline("Alignment Check");
    asm("hlt");
}

__attribute__((interrupt)) static void isr18(struct interrupt_frame* frame) {
    terminal_writeline("Machine Check");
    asm("hlt");
}

__attribute__((interrupt)) static void isr_reserved(struct interrupt_frame* frame) {
    terminal_writeline("Reserved");
    asm("hlt");
}

void isr_install(void) {
    idt_register_handler(0, (unsigned long)isr0);
    idt_register_handler(1, (unsigned long)isr1);
    idt_register_handler(2, (unsigned long)isr2);
    idt_register_handler(3, (unsigned long)isr3);
    idt_register_handler(4, (unsigned long)isr4);
    idt_register_handler(5, (unsigned long)isr5);
    idt_register_handler(6, (unsigned long)isr6);
    idt_register_handler(7, (unsigned long)isr7);
    idt_register_handler(8, (unsigned long)isr8);
    idt_register_handler(9, (unsigned long)isr9);
    idt_register_handler(10, (unsigned long)isr10);
    idt_register_handler(11, (unsigned long)isr11);
    idt_register_handler(12, (unsigned long)isr12);
    idt_register_handler(13, (unsigned long)isr13);
    idt_register_handler(14, (unsigned long)isr14);
    idt_register_handler(15, (unsigned long)isr15);
    idt_register_handler(16, (unsigned long)isr16);
    idt_register_handler(17, (unsigned long)isr17);
    idt_register_handler(18, (unsigned long)isr18);
    idt_register_handler(19, (unsigned long)isr_reserved);
    idt_register_handler(20, (unsigned long)isr_reserved);
    idt_register_handler(21, (unsigned long)isr_reserved);
    idt_register_handler(22, (unsigned long)isr_reserved);
    idt_register_handler(23, (unsigned long)isr_reserved);
    idt_register_handler(24, (unsigned long)isr_reserved);
    idt_register_handler(25, (unsigned long)isr_reserved);
    idt_register_handler(26, (unsigned long)isr_reserved);
    idt_register_handler(27, (unsigned long)isr_reserved);
    idt_register_handler(28, (unsigned long)isr_reserved);
    idt_register_handler(29, (unsigned long)isr_reserved);
    idt_register_handler(30, (unsigned long)isr_reserved);
    idt_register_handler(31, (unsigned long)isr_reserved);

    idt_init();
}