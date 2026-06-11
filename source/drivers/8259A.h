/**
 * 8259A PIC - Programmable Interrupt Controller (Controlador de Interrupções Programável)
 * 
 * Um Programmable Interrupt Controller (PIC) é um chip de hardware que 
 * gere os pedidos de interrupção (IRQs) de múltiplos dispositivos periféricos, 
 * encaminhando-os para o processador por ordem de prioridade. Ele liberta 
 * a CPU de monitorizar constantemente os dispositivos, tornando o sistema 
 * muito mais eficiente.
 * 
 * Notes By GEMINI
 * 
 */

#ifndef DRIVERS_8259A_H_
#define DRIVERS_8259A_H_

#include "../lib/types.h"

/* Portas de E/S (I/O) do PIC */
#define PIC_MC 0x0020    // Porta de Comando do PIC Master
#define PIC_MD 0x0021    // Porta de Dados do PIC Master
#define PIC_SC 0x00A0    // Porta de Comando do PIC Slave
#define PIC_SD 0x00A1    // Porta de Dados do PIC Slave

/* ICW1 (Initialization Command Word 1) - Configuração Inicial */
#define PIC_ICW1_IC4    ((1<<0) | (1<<4)) // Necessita da palavra de comando ICW4
#define PIC_ICW1_SNGL   ((1<<1) | (1<<4)) // Modo único (Single); se não definido, usa Cascata (Cascade)
#define PIC_ICW1_ADI    ((1<<2) | (1<<4)) // Intervalo de endereço de chamada de 4 bytes
#define PIC_ICW1_LTIM   ((1<<3) | (1<<4)) // Modo ativado por nível (Level Triggered)

/* ICW2 (Initialization Command Word 2) - Mapeamento de Vetores */
#define PIC_ICW2_VEC(INDEX) (INDEX)       // Endereço base do vetor de interrupção no CPU

/* ICW3 (Initialization Command Word 3) - Configuração de Cascata (Apenas se ~PIC_ICW1_SNGL) */
#define PIC_ICW3_MASTER(IR)  (1<<(IR))    // Indica qual linha IR do Master está ligada ao Slave
#define PIC_ICW3_SLAVE(SID)  ((SID) & 0x07) // Identidade do Slave (ID da linha em binário)

/* ICW4 (Initialization Command Word 4) - Modo de Operação (Apenas se PIC_ICW1_IC4) */
#define PIC_ICW4_PM     (1<<0)           // Modo 8086/8088 (Obrigatório para x86)
#define PIC_ICW4_AEOI   (1<<1)           // Fim de Interrupção Automático (Auto End-Of-Interrupt)
#define PIC_ICW4_SBUF   (1<<3)           // Modo Com Buffer (Buffered Mode) / Configurado como Slave
#define PIC_ICW4_MBUF   (3<<2)           // Modo Com Buffer (Buffered Mode) / Configurado como Master
#define PIC_ICW4_SFNM   (1<<4)           // Modo Especial Totalmente Alinhado (Special Fully Nested Mode)

/* Protótipos das Funções */

/**
 * @brief Inicializa e remapeia os chips PIC Master e Slave.
 * Altera os vetores padrão das IRQs para evitar conflitos com as Exceções do CPU.
 */
extern void pic_init(void);

/**
 * @brief Mascara todas as linhas de interrupção (Desativa todas as IRQs).
 * Desativa o processamento de hardware enviando 0xFF para as portas de dados.
 */
extern void pic_mask_all(void);

/**
 * @brief Remove a máscara de todas as linhas de interrupção (Ativa todas as IRQs).
 * Permite que o hardware envie sinais enviando 0x00 para as portas de dados.
 */
extern void pic_unmask_all(void);

/**
 * @brief Envia o sinal de Fim de Interrupção (EOI - End Of Interrupt).
 * Informa o PIC que a interrupção atual foi processada e que pode receber novas.
 * @param vector O número do vetor de interrupção ou IRQ que terminou.
 */
extern void pic_eio(uint8_t vector);

#endif /* DRIVERS_8259A_H_ */
