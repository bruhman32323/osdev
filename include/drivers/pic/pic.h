#ifndef _PIC_H
#define _PIC_H 1

#include <std/inline.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)
#define PIC_EOI 0x20

#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10
#define ICW4_8086 0x01

void pic_init();
void pic_send_eoi(uint8_t no);
void pic_irq_enable(uint8_t no);
void pic_irq_disable(uint8_t no);

#ifdef __cplusplus
}
#endif

#endif /* _PIC_H */