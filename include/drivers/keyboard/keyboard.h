#ifndef _KEYBOARD_H
#define _KEYBOARD_H 1

#include <drivers/idt/isr.h>
#include <drivers/terminal/terminal.h>
#include <std/util.h>
#include <std/string.h>

#ifdef __cplusplus
extern "C" {
#endif

int keyboard_init(void);
uint8_t keyboard_enabled(void);

#ifdef __cplusplus
}
#endif

#endif /* _KEYBOARD_H */