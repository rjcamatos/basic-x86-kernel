
#include "i8254.h"
#include "io.h"
#include "82489DX.h"



void pit_wait_ms(uint32_t ms) {
    // Carrega o valor correspondente ao tempo solicitado (máximo ~54ms neste modo simples)
    // 11932 ticks = ~10 milissegundos
    uint16_t latch = 11932 * (ms / 10);
    
    io_port_outb(PIT_PORT_COMMAND, PIT_ACCESS_BOTH); // Canal 0, LSB/MSB, Modo 0
    io_port_outb(PIT_SELECT_CHANNEL1, latch & 0xFF);
    io_port_outb(PIT_SELECT_CHANNEL1, (latch >> 8) & 0xFF);

    // Espera ativa até o PIT terminar a contagem decrescente
    // Emulação/Hardware real atualiza o bit de status
    while (1) {
        io_port_outb(PIT_PORT_COMMAND, 0xE2); // Comando Read-Back para o Canal 0
        if ((io_port_inb(PIT_SELECT_CHANNEL1) & 0x80) != 0) {
            break; // O bit Null Count / Output mudou, o tempo passou
        }
    }
}
