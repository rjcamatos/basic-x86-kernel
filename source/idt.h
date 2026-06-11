/*
 * IDT - Interrupt Descriptor Table
 *
 * A Interrupt Descriptor Table (IDT) é uma estrutura de dados binária usada
 * pelas arquiteturas x86 e x86-64 para gerir interrupções e exceções. Quando o 
 * processador deteta um evento — como um erro de divisão por zero, uma tecla 
 * pressionada ou um sinal do temporizador —, ele suspende temporariamente o 
 * código atual, consulta a IDT para encontrar a função responsável por lidar 
 * com esse evento (chamada de Interrupt Service Routine - ISR) e executa-a.
 * 
 * Notes By GEMINI
 * 
 */

#ifndef IDT_H_
#define IDT_H_

#include "lib/types.h"

/**
 * @brief Estrutura do registo IDTR (passado para a instrução LIDT).
 * Define os limites e o endereço base onde a tabela IDT está na memória.
 */
typedef struct {
	uint16_t limit; /**< Tamanho total da IDT em bytes menos 1 */
	uint32_t base;  /**< Endereço de memória linear onde a IDT começa */
} __attribute__((packed)) idt_lidt_t;

/**
 * @brief Estrutura de uma entrada (Gate) na IDT (x86 de 32 bits).
 * Cada gate aponta para uma ISR ou uma tarefa específica do sistema.
 */
typedef struct {
	uint32_t offset_low:16;  /**< Bits 0-15 do endereço da função de interrupção (ISR) */
	uint32_t ss:16;          /**< Seletor de Segmento de Código (GDT) */
	uint32_t flags:16;       /**< Atributos e tipo de Gate (DPL, Present, etc.) */
	uint32_t offset_high:16; /**< Bits 16-31 do endereço da função de interrupção (ISR) */
} __attribute__((packed)) idt_gate_t;

/* --- Configurações de Máscaras e Flags para as Gates da IDT --- */
#define IDT_FLAG_GATE_DPL0	(0<<13) /**< Nível de Privilégio 0: Apenas o Kernel pode chamar via software */
#define IDT_FLAG_GATE_DPL1	(1<<13)	/**< Nível de Privilégio 1: Reservado/Drivers */
#define IDT_FLAG_GATE_DPL2	(2<<13)	/**< Nível de Privilégio 2: Reservado/Drivers */
#define IDT_FLAG_GATE_DPL3	(3<<13)	/**< Nível de Privilégio 3: Aplicações do Utilizador podem chamar (ex: Syscalls) */
#define IDT_FLAG_GATE_P		(1<<15)	/**< Bit Presente: Deve ser 1 para a Gate ser considerada válida */
#define IDT_FLAG_GATE_D		(1<<11)	/**< Tamanho: 1 = Gate de 32 bits, 0 = 16 bits (Apenas para Trap/Interrupt) */

/**
 * @brief Estrutura dos argumentos passados para os Handlers de Interrupção.
 * Mapeia o estado dos registos empilhados (geralmente via PUSHAD em Assembly).
 */
typedef struct {
	struct {
		int32_t EDI; /**< Registo Destination Index (geralmente operações de string) */
		int32_t ESI; /**< Registo Source Index (geralmente operações de string) */
		int32_t EBP; /**< Registo Base Pointer (aponta para a base da stack frame) */
		int32_t ESP; /**< Registo Stack Pointer original antes da execução do PUSHAD */
		int32_t EBX; /**< Registo Base (uso geral / indexação de memória) */
		int32_t EDX; /**< Registo Data (uso geral / E/S / divisão e multiplicação) */
		int32_t ECX; /**< Registo Counter (uso geral / contadores de loops) */
		int32_t EAX; /**< Registo Accumulator (uso geral / retorno de funções) */
	} registers;     /**< Cópia dos registos de uso geral salvos pelo software */
} __attribute__((packed)) idt_hargs_t;

/**
 * @brief Estrutura do Código de Erro empilhado pelo CPU em certas exceções (Secção 6.13 do manual Intel).
 * Permite decifrar bit-a-bit os erros de hardware fazendo um cast do argumento 'error' no handler.
 */
typedef struct {
	uint32_t EXT:1;   /**< 1 = Evento externo ao CPU (ex: Interrupção de Hardware / IRQ) */
	uint32_t IDT:1;   /**< 1 = O índice SSI aponta para uma Gate na IDT; 0 = Aponta para GDT/LDT */
	uint32_t TI:1;    /**< Se IDT=0: 1 = Aponta para a LDT, 0 = Aponta para a GDT */
	uint32_t SSI:16;  /**< Segment Selector Index: Índice da tabela associada ao erro */
	uint32_t _r0:13;  /**< Bits reservados pelo CPU */
} __attribute__((packed)) idt_error_t;

/**
 * @brief Estrutura de Contexto Nativo (Frame de Execução) gerada pelo hardware do CPU x86.
 * Útil para fazer o mapeamento da stack quando precisas de inspecionar o ponto de falha num Kernel Panic.
 */
typedef struct {
	uint32_t eip;    /**< Instruction Pointer: Endereço de retorno da instrução interrompida */
	uint32_t cs;     /**< Code Segment: Seletor de segmento de código onde o CPU operava */
	uint32_t eflags; /**< Flags Register: Estado dos registos de controlo e máscaras (ex: IF) */
} __attribute__((packed)) idt_cpu_frame_t;


/* --- Funções de Inicialização e Configuração da IDT --- */

/**
 * @brief Inicializa a IDT configurando as gates padrão e aplicando o LIDT.
 */
extern void idt_init(void);

/**
 * @brief Carrega o registo IDTR do CPU com a estrutura configurada.
 */
extern void idt_set_table(idt_lidt_t *lidt);

/**
 * @brief Configura uma Task Gate na IDT (usada para transições de contexto de hardware TSS).
 */
extern void idt_set_gate_task(idt_gate_t *gate, uint16_t tss, uint16_t flags);

/**
 * @brief Configura uma Interrupt Gate na IDT (Desativa interrupções de hardware automaticamente ao entrar).
 */
extern void idt_set_gate_interrupt(idt_gate_t *gate, uint16_t ss, uint32_t offset, uint16_t flags);

/**
 * @brief Configura uma Trap Gate na IDT (Mantém as interrupções de hardware ativas ao entrar).
 */
extern void idt_set_gate_trap(idt_gate_t *gate, uint16_t ss, uint32_t offset, uint16_t flags);

/**
 * @brief Manipulador genérico que recebe todas as interrupções do Assembly e as distribui.
 */
extern void idt_interrupt_handler(uint32_t vector, idt_hargs_t args, uint32_t error);

/**
 * @brief Regista uma função C específica para tratar um vetor/número de interrupção.
 */
extern void idt_set_interrupt_handler(uint8_t interrupt, voidptr_t handler);

#endif /* IDT_H_ */
