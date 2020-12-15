#ifndef _ISR_H
#define _ISR_H 1

#include <drivers/idt/idt.h>

#ifdef __cplusplus
extern "C" {
#endif

void isr_install(void);

#ifdef __cplusplus
}
#endif

#endif /* _ISR_H */