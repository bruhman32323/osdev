#include <stdbool.h>
#include <drivers/pic/pic.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/device/device.h>

unsigned char kbdus[128] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
        '9', '0', '-', '=', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'q', 'w', 'e', 'r',                               /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
        '\'', '`', 0,                                     /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
        'm', ',', '.', '/', 0,                            /* Right shift */
        '*',
        0,   /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,   /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0, /* < ... F10 */
        0, /* 69 - Num lock*/
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow */
        0,
        0, /* Right Arrow */
        '+',
        0, /* 79 - End key*/
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
};

uint8_t lastkey = 0;
uint8_t __kbd_enabled = 0;

unsigned char keyboard_get_key_from_scancode(unsigned char scancode) {
    return kbdus[scancode];
}

__attribute__((interrupt)) static void keyboard_irq(struct interrupt_frame* frame) {
    unsigned char scancode = inportb(0x60);

    bool special = true;

    switch (scancode) {
        case 72:
            terminal_row--;
            break;
        case 75:
            terminal_column--;
            break;
        case 77:
            terminal_column++;
            break;
        case 80:
            terminal_row++;
            break;
        default:
            special = false;
    }

    if (scancode & 128) {
        // This is a release scancode, just ignore it
        pic_send_eoi(1);
        return;
    }

    if (!special) terminal_putchar(keyboard_get_key_from_scancode(scancode));
    terminal_updatecursor();
	pic_send_eoi(1);
}

uint8_t keyboard_enabled(void) {
    return __kbd_enabled;
}

void* keyboard_read(void* data) {
    if (inportb(0x64) & 1) {
        return keyboard_get_key_from_scancode(inportb(0x60));
    } else {
        return -1;
    }
}

void* keyboard_write(void* data) {
    return -1;
}

int keyboard_init(void) {
    pic_irq_enable(1);
    idt_register_handler(33, (uint32_t)keyboard_irq);

    __kbd_enabled = 1;

    struct device keyboard_device = {
        write: &keyboard_write,
        read: &keyboard_read
    };

    int keyboard_descriptor = push_device(keyboard_device);

    return keyboard_descriptor;
}