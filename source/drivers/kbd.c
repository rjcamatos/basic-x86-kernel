#include "kbd.h"
#include "i8042.h"
//#include "8259A.h"
#include "82489DX.h"
#include "i8042.h"
#include "../idt.h"

static void kbd_irq_handler(uint32_t vector, idt_hargs_t args, uint32_t error)
{
	byte_t in = i8042_read();
	printk("KBD: %u|",(uint_t)in);
	lapic_eio(vector);
	//pic_eio((uint8_t)vector);
}


static void mouse_irq_handler(uint32_t vector, idt_hargs_t args, uint32_t error)
{
	byte_t in = i8042_read();
	printk("MOUSE: %u|",(uint_t)in);
	lapic_eio(vector);
	//pic_eio((uint8_t)vector);
}

void kbd_init(void)
{
	idt_set_interrupt_handler(33,&kbd_irq_handler); //IDT 33 para o teclado

	i8042_write(KBD_OUT_COMMAND_ENABLE);
	while( i8042_read() != 0xFA ) continue;

	i8042_write(KBD_OUT_COMMAND_ECHO);
	while( i8042_read() != 0xEE ) continue;

	i8042_write(KBD_OUT_COMMAND_SET_INDICATOR);
	while( i8042_read() != 0xFA ) continue;

	i8042_write(0b00000111);
	while( i8042_read() != 0xFA ) continue;

}

void mouse_init(void)
{

	idt_set_interrupt_handler(44,&mouse_irq_handler); //IDT 44 para o mouse

	//BY GEMINI:
		//(EM MODO STANDBY, TEM QUE SER FEITO ISTO)
		// --- O QUE FALTAVA ADICIONAR ENQUANTO COMANDO DO RATO ---
			//printk("    enabling mouse data reporting...\n");
			// 1. Envia comando 0xD4 ao i8042 a dizer: "O próximo byte é para o rato"
			i8042_command(I8042_COMMAND_WRITE_MOUSE_DEVICE); 
			// 2. Envia o comando 0xF4 (Enable Data Reporting) para a porta de dados (0x60)
			i8042_write(0xF4);
			// 3. Opcional: Consumir o ACK (0xFA) que o rato vai devolver
			uint8_t ack = i8042_read();
			printk("    mouse ack: 0x%x\n", ack);
}
