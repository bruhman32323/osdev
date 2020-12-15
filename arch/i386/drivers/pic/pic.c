#include <drivers/pic/pic.h>

static uint8_t pic1_mask = 0xff;
static uint8_t pic2_mask = 0xff;

void pic_init() {
	outportb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outportb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outportb(PIC1_DATA, 0x20); // master offset 0x20
	io_wait();
	outportb(PIC2_DATA, 0x28); // slave offset 0x28
	io_wait();
	outportb(PIC1_DATA, 4);
	io_wait();
	outportb(PIC2_DATA, 2);
	io_wait();
	outportb(PIC1_DATA, ICW4_8086);
	io_wait();
	outportb(PIC2_DATA, ICW4_8086);
	io_wait();
	outportb(PIC1_DATA, pic1_mask);
	outportb(PIC2_DATA, pic2_mask);
	pic_irq_enable(2);
}

void pic_send_eoi(uint8_t no) {
	if (no >= 8) {
		outportb(PIC2_COMMAND, PIC_EOI);
	}
	outportb(PIC1_COMMAND, PIC_EOI);
}

void pic_irq_enable(uint8_t no) {
	if (no >= 8) {
		no -= 8;
		pic2_mask &= ~(1 << no);
		outportb(PIC2_DATA, pic2_mask);
	} else {
		pic1_mask &= ~(1 << no);
		outportb(PIC1_DATA, pic1_mask);
	}
}

void pic_irq_disable(uint8_t no) {
	if (no >= 8) {
		no -= 8;
		pic2_mask |= (1 << no);
		outportb(PIC2_DATA, pic2_mask);
	} else {
		pic1_mask |= (1 << no);
		outportb(PIC1_DATA, pic1_mask);
	}
}