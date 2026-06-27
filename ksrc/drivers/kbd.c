#include "kbd.h"
#include "i8042.h"
//#include "8259A.h"
#include "82489DX.h"
#include "i8042.h"
#include "idt.h"

static void kbd_irq_handler(uint32_t vector, idt_registers_t regs, idt_error_t error, idt_cpu_frame_t cpu)
{
	byte_t in = io_port_inb(0x60);
	
	lapic_eio(vector);
    //pic_eio((uint8_t)vector);

    if( in == 0xFA) return; //AN ACK

    printk("KBD %u, IRQ %d |",(uint_t)in,vector);
	
}


static void mouse_irq_handler(uint32_t vector, idt_registers_t regs, idt_error_t error, idt_cpu_frame_t cpu)
{
	byte_t in = io_port_inb(0x60);
	
	lapic_eio(vector);
	//pic_eio((uint8_t)vector);
    
    if( in == 0xFA) return; //AN ACK

    printk("MOUSE %u, IRQ %d |",(uint_t)in,vector);
}

void kbd_init(void)
{

    idt_mask_interrupt(33);

    uint_t timeout = 0;
    // 1. Regista o handler de interrupções (Remova esta linha de i8042_init!)
    idt_set_interrupt_handler(33, &kbd_irq_handler); 

    // 2. Ativa o varrimento - Comando vai para a porta 0x60 (i8042_write)
    i8042_write(I8042_KEYBOARD_COMMAND_ENABLE_SCANNING); // 0xF4
    timeout = 512;
    while (timeout-- && i8042_read() != 0xFA);

    // 3. Testa o Eco - Comando vai para a porta 0x60 (i8042_write)
    i8042_write(I8042_KEYBOARD_COMMAND_ECHO); // 0xEE
    timeout = 512;
    while( timeout-- && i8042_read() != 0xEE );

    // 4. Configura os LEDs - Comando vai para a porta 0x60 (i8042_write)
    i8042_write(I8042_KEYBOARD_COMMAND_SET_LEDS); // 0xED
    timeout = 512;
    while( timeout-- && i8042_read() != 0xFA );        // Teclado responde ACK ao comando
    
    i8042_write(0x00);                             // Envia argumento: todos os LEDs desligados

    //Keyboard, read ACK RESPONSE
    timeout = 512;
    while( timeout-- && i8042_read() != 0xFA ); 
    if( timeout > 0 ) {
        printk("    keyboard ACK !\n");
    }

    idt_unmask_interrupt(33);

}

void mouse_init(void)
{
    idt_mask_interrupt(44);

    // 1. Regista o handler de interrupções (Remova esta linha de i8042_init!)
    idt_set_interrupt_handler(44, &mouse_irq_handler); 

    // 2. Ativa o envio de dados do rato de forma correta (0xD4 na 0x64 e depois 0xF4 na 0x60)
    i8042_command_set(
		I8042_COMMAND_WRITE_MOUSE_DEVICE,
		I8042_MOUSE_COMMAND_ENABLE_DATA_REPORTING
	); 

    //Mouse, read ACK RESPONSE    
    uint_t timeout = 512;
    while( timeout-- && i8042_read() != 0xFA ); 
    if( timeout > 0 ) {
        printk("    mouse ACK !\n");
    }

    idt_unmask_interrupt(44);
}
