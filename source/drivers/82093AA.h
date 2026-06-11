/**
 * 
 * 82093AA IOAPIC - I/O Advanced Programmable Interrupt Controller
 * 
 * O Intel 82093AA I/O APIC (Advanced Programmable Interrupt Controller) é um chip 
 * (ou bloco integrado no chipset) periférico cuja função principal é receber interrupções 
 * de hardware dos dispositivos (teclado, PCI, discos) e encaminhá-las para os processadores 
 * em sistemas multiprocessados (SMP/Multi-core). Ele veio substituir o antigo e obsoleto 
 * controlador 8259A PIC.
 * 
 * Direcionado exclusivamente para a plataforma x86 (32-bits Modo Protegido).
 * 
 * Notes By GEMINI
 * 
 */

#ifndef DRIVERS_82093AA_H_
#define DRIVERS_82093AA_H_

#include "../lib/types.h"

/* Registores do IOAPIC (Deslocamentos de Índice) */
#define IOAPIC_IOAPICID             0x00    // Registo de Identificação do IOAPIC
#define IOAPIC_IOAPICVER            0x01    // Registo de Versão do IOAPIC
#define IOAPIC_IOAPICARB            0x02    // Registo de Arbitragem do IOAPIC
#define IOAPIC_IOREDTBL_L(INDEX)    (0x10 + ((INDEX) * 2))     // Parte baixa (Bits 0-31) da entrada da tabela de redirecionamento
#define IOAPIC_IOREDTBL_H(INDEX)    (0x10 + ((INDEX) * 2) + 1) // Parte alta (Bits 32-63) da entrada da tabela de redirecionamento

/* IOREGSEL - Registo de Seleção de Registo de E/S (Usado para indexar o registo interno do IOAPIC) */
typedef struct {
    uint32_t sel:8;     // Bits 0-7: Endereço do Registo Interno (Leitura/Escrita). Especifica o registo a ser acedido via IOWIN.
    uint32_t _res0:24;  // Bits 8-31: Reservado. Deve ser mantido a zero.
} __attribute__((packed)) ioapic_regsel_t;

/* IOWIN - Registo de Janela de E/S (Usado para ler ou escrever no registo interno selecionado) */
typedef struct {
    uint32_t data;      // Bits 0-31: Dados do Registo Interno (Leitura/Escrita). Mapeado para o endereço escolhido no IOREGSEL.
} __attribute__((packed)) ioapic_win_t;

/* IOAPICID - Registo de Identificação do IOAPIC (Necessário para a transmissão de mensagens APIC) */
typedef struct {
    uint32_t _res0:24;  // Bits 0-23: Reservado.
    uint32_t id:4;      // Bits 24-27: Identificação do IOAPIC (Leitura/Escrita). ID único do chip no barramento APIC.
    uint32_t _res1:4;   // Bits 28-31: Reservado.
} __attribute__((packed)) ioapic_id_t;

/* IOAPICVER - Registo de Versão do IOAPIC */
typedef struct {
    uint32_t ver:8;     // Bits 0-7: Versão do IOAPIC (Apenas Leitura). O valor padrão para este chip é 0x11.
    uint32_t _res0:8;   // Bits 8-15: Reservado.
    uint32_t mre:8;     // Bits 16-23: Máximo de Entradas de Redirecionamento (Apenas Leitura). Linha mais alta da tabela (ex: 0x17 = 24 entradas).
    uint32_t _res1:8;   // Bits 24-31: Reservado.
} __attribute__((packed)) ioapic_ver_t;

/* IOAPICARB - Registo de Arbitragem do IOAPIC */
typedef struct {
    uint32_t _res0:24;  // Bits 0-23: Reservado.
    uint32_t id:4;      // Bits 24-27: ID de Arbitragem do IOAPIC (Apenas Leitura/Atualizado por Hardware).
    uint32_t _res1:4;   // Bits 28-31: Reservado.
} __attribute__((packed)) ioapic_arb_t;

/* IOREDTBL[23:0] - Parte Baixa dos Registos da Tabela de Redirecionamento de E/S (Bits 0 a 31) */
typedef struct {
    uint32_t intvec:8;  // Bits 0-7: Vetor de Interrupção (INTVEC) (Leitura/Escrita). Vetor mapeado no IDT (0x20-0xFF).
    uint32_t delmod:3;  // Bits 8-10: Modo de Entrega (DELMOD) (Leitura/Escrita). Ex: 000 (Fixo), 001 (Prioridade Mais Baixa).
    uint32_t destmod:1; // Bit 11: Modo de Destino (DESTMOD) (Leitura/Escrita). 0 = Físico (ID LAPIC), 1 = Lógico (Máscara).
    uint32_t delivs:1;  // Bit 12: Estado de Entrega (DELIVS) (Apenas Leitura). 0 = Ocioso (Idle), 1 = Pendente (Send Pending).
    uint32_t intpol:1;  // Bit 13: Polaridade do Pino de Entrada (INTPOL) (Leitura/Escrita). 0 = Ativo Alto, 1 = Ativo Baixo.
    uint32_t riir:1;    // Bit 14: IRR Remoto (Remote IRR) (Apenas Leitura). Usado para interrupções ativadas por nível.
    uint32_t tm:1;      // Bit 15: Modo de Disparo (Trigger Mode) (Leitura/Escrita). 0 = Por Borda (Edge), 1 = Por Nível (Level).
    uint32_t mask:1;    // Bit 16: Máscara de Interrupção (Leitura/Escrita). 0 = Ativa (Desmascarada), 1 = Inativa (Mascarada).
    uint32_t _res0:15;  // Bits 17-31: Reservado. Completa os 32 bits da metade inferior.
} __attribute__((packed)) ioapic_redtbl_L_t;

/* IOREDTBL[23:0] - Parte Alta dos Registos da Tabela de Redirecionamento de E/S (Bits 32 a 64) */
typedef struct {
    uint32_t _res0:24;  // Bits 32-55: Reservado.
    uint32_t des:8;     // Bits 56-63: Campo de Destino (Leitura/Escrita). ID do APIC Local ou máscara que irá receber o sinal.
} __attribute__((packed)) ioapic_redtbl_H_t;

/* Macro utilitária para conversão de ponteiros (Cast seguro de tipos estruturados para inteiros de 32-bits) */
#define ioapic_cast(VAR) (*((uint32_t*)&(VAR)))

/* Protótipos das Funções */

/**
 * @brief Lê um registo interno do IOAPIC de 32-bits.
 * Escreve o índice desejado na porta IOREGSEL e recolhe o resultado contido na porta IOWIN.
 * @param reg O índice do registo do IOAPIC a ser lido (Ex: IOAPIC_IOAPICVER).
 * @return uint32_t O valor de 32-bits lido do registo interno.
 */
extern uint32_t ioapic_read(uint8_t reg);

/**
 * @brief Escreve um valor de 32-bits num registo interno do IOAPIC.
 * Define o índice alvo na porta IOREGSEL e injeta o valor na porta IOWIN.
 * @param reg O índice do registo do IOAPIC a ser modificado.
 * @param value O valor de 32-bits a ser guardado no registo.
 */
extern void ioapic_write(uint8_t reg, uint32_t value);

/**
 * @brief Inicializa o subsistema IOAPIC de 32-bits.
 * Configura as tabelas de mapeamento e redirecionamento de hardware, mascarando
 * ou atribuindo vetores IDT iniciais às IRQs físicas.
 */
extern void ioapic_init(void);

#endif /* DRIVERS_82093AA_H_ */
