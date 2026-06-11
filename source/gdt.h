/*
 * GDT - Global Descriptor Table
 *
 * A Global Descriptor Table (GDT) é uma tabela de dados binária usada pelos 
 * processadores da arquitetura Intel x86 (32-bits) para gerir a memória do 
 * sistema através do mecanismo de segmentação. Ela é a primeira estrutura 
 * que o sistema operativo configura ao iniciar o Modo Protegido. A sua função 
 * principal é dizer ao processador como a memória RAM está dividida, quais as 
 * regras de acesso e qual o nível de privilégio exigido para ler, escrever 
 * ou executar cada bloco.
 *
 * Comentários Por: GEMINI
 *  
 * */

#include "lib/mem.h"

#ifndef GDT_H_
#define GDT_H_

/* Limite máximo de entradas na tabela */
#define GDT_LIMIT 10    // Limite máximo prático para esta implementação específica

/* Tipos de Descritores de Segmento de Dados (Bits 8 a 11 do Access Byte) */
#define GDT_TYPE_DATA_RO    (0<<8)  // Apenas Leitura
#define GDT_TYPE_DATA_ROA   (1<<8)  // Apenas Leitura, Acedido
#define GDT_TYPE_DATA_RW    (2<<8)  // Leitura e Escrita
#define GDT_TYPE_DATA_RWA   (3<<8)  // Leitura e Escrita, Acedido
#define GDT_TYPE_DATA_ROD   (4<<8)  // Apenas Leitura, Expansão para Baixo (Stack)
#define GDT_TYPE_DATA_RODA  (5<<8)  // Apenas Leitura, Expansão para Baixo, Acedido
#define GDT_TYPE_DATA_RWD   (6<<8)  // Leitura e Escrita, Expansão para Baixo (Stack)
#define GDT_TYPE_DATA_RWDA  (7<<8)  // Leitura e Escrita, Expansão para Baixo, Acedido

/* Tipos de Descritores de Segmento de Código (Bits 8 a 11 do Access Byte) */
#define GDT_TYPE_CODE_EO    (8<<8)  // Apenas Execução
#define GDT_TYPE_CODE_EOA   (9<<8)  // Apenas Execução, Acedido
#define GDT_TYPE_CODE_ER    (10<<8) // Execução e Leitura
#define GDT_TYPE_CODE_ERA   (11<<8) // Execução e Leitura, Acedido
#define GDT_TYPE_CODE_EC    (12<<8) // Apenas Execução, Conforme (Conforming)
#define GDT_TYPE_CODE_EOCA  (13<<8) // Apenas Execução, Conforme, Acedido
#define GDT_TYPE_CODE_ERC   (14<<8) // Execução e Leitura, Conforme
#define GDT_TYPE_CODE_ERCA  (15<<8) // Execução e Leitura, Conforme, Acedido

/* Flags de Acesso e Granularidade (Mapeadas para os bits corretos do registo) */
#define GDT_FLAG_S          (1<<12) // Tipo de descritor: 1 = Código/Dados, 0 = Sistema (Ex: TSS)
#define GDT_FLAG_DPL0       (0<<13) // Nível de Privilégio 0: Kernel (Mais alto)
#define GDT_FLAG_DPL1       (1<<13) // Nível de Privilégio 1: Drivers/Serviços do Sistema
#define GDT_FLAG_DPL2       (2<<13) // Nível de Privilégio 2: Drivers/Serviços do Sistema
#define GDT_FLAG_DPL3       (3<<13) // Nível de Privilégio 3: Aplicações Utilizador (Mais baixo)
#define GDT_FLAG_P          (1<<15) // Segmento Presente na memória (Deve ser 1 para segmentos válidos)
#define GDT_FLAG_DB         (1<<22) // Tamanho da operação: 0 = 16-bits; 1 = 32-bits Modo Protegido
#define GDT_FLAG_G          (1<<23) // Granularidade: 0 = 1 Byte; 1 = 4 KiB (Páginas)

/**
 * Estrutura de uma entrada na GDT (8 bytes / 64 bits de tamanho total)
 * Otimizada exclusivamente para o alinhamento binário nativo da arquitetura x86 de 32-bits.
 */
typedef struct {
    uint16_t limit_low;             // [Bits 00-15] Bits baixos do tamanho limite do segmento

    uint16_t base_low;              // [Bits 16-31] Bits baixos do endereço base do segmento

    uint8_t base_middle;            // [Bits 32-39] Bits médios do endereço base do segmento
    
    /* Byte de Acesso (Access Byte) - Dividido em campos de bits */
    uint8_t ac:1;                   // Bit de Acesso (O CPU define para 1 quando o segmento é acedido)
    uint8_t rw:1;                   // Código: Leitura permitida | Dados: Escrita permitida
    uint8_t dc:1;                   // Código: Conforme | Dados: Direção de expansão (1 = para baixo)
    uint8_t ex:1;                   // Executável: 1 = Segmento de Código, 0 = Segmento de Dados
    uint8_t _r1:1;                  // Bit S (Descriptor Type): Deve ser 1 para Código ou Dados
    uint8_t privilege:2;            // Nível de privilégio do segmento (DPL: Anéis de 0 a 3)
    uint8_t present:1;              // Bit de Presença (P): Deve ser 1 para qualquer segmento ativo

    /* Flags e Limite Superior */
    uint8_t limit_high:4;           // [Bits 48-51] Bits altos do tamanho limite do segmento
    uint8_t _r2:1;                  // Bit AVL: Disponível para uso do programador do Sistema Operativo
    uint8_t _r3:1;                  // Bit reservado em 32-bits: Deve ser mantido a 0 (Modo Longo inativo)
    uint8_t size:1;                 // Bit D/B (Tamanho padrão): 0 = 16-bit, 1 = 32-bit Modo Protegido
    uint8_t granularity:1;          // Bit G (Granularidade): 0 = Limite em Bytes, 1 = Limite em blocos de 4 KiB

    uint8_t base_high;              // [Bits 56-63] Bits altos do endereço base do segmento
} __attribute__((packed)) gdt_entry_t;

/**
 * Estrutura do ponteiro da GDT para a instrução nativa LGDT (6 bytes)
 */
typedef struct {
    uint16_t limit;                 // Tamanho total da tabela em bytes menos 1 (GDT_Bytes - 1)
    uint32_t base;                  // Endereço de memória linear de 32-bits onde a GDT está guardada
} __attribute__((packed)) gdt_lgdt_t;

/**
 * Estrutura de um Seletor de Segmento (16 bits de tamanho)
 * Carregado diretamente nos registos de segmento do CPU (CS, DS, SS, ES, FS, GS)
 */
typedef struct {
    uint16_t privilege:2;           // Requested Privilege Level (RPL): Anel de privilégio alvo (0 a 3)
    uint16_t table:1;               // Indicador de Tabela (TI): 0 = Procura na GDT, 1 = Procura na LDT
    uint16_t index:13;              // Índice numérico do descritor pretendido dentro da tabela
} __attribute__((packed)) gdt_segment_t;

/* Protótipos das Funções */

/**
 * @brief Inicializa a GDT de 32-bits do sistema.
 * Configura os seletores padrão planos (Nulo, Kernel Código, Kernel Dados, User Código, User Dados)
 * e aplica a nova tabela ao processador.
 */
extern void gdt_init(void);

/**
 * @brief Carrega o registrador GDTR do processador de 32-bits.
 * Envia o ponteiro com o tamanho e endereço base da tabela para a instrução nativa 'lgdt'.
 * @param lgdt Ponteiro para a estrutura de 6 bytes que contém o limite e a base da GDT.
 */
extern void gdt_set_table(gdt_lgdt_t *lgdt);

/**
 * @brief Preenche uma entrada (gate) específica na GDT de 32-bits.
 * Divide de forma segmentada os valores de base, limite, tipo e flags nos bits correspondentes da estrutura.
 * @param entry Ponteiro para a entrada da GDT que será modificada.
 * @param limit O tamanho máximo do segmento (limite de memória até 4GB).
 * @param base O endereço de memória linear de 32-bits inicial onde o segmento começa.
 * @param type O tipo de acesso do segmento (permissões de código ou dados).
 * @param flags As configurações de granularidade, tamanho de operação de 32-bits e privilégio.
 */
extern void gdt_set_gate(gdt_entry_t *entry, uint32_t limit, uint32_t base, uint32_t type, uint32_t flags);

/**
 * @brief Atualiza e recarrega todos os registadores de segmento de 32-bits do CPU (Far Jump).
 * Implementada em Assembly, esta função força o processador a assumir os novos seletores e privilégios da GDT.
 * @param cs Seletor de 16-bits para o segmento de Código (Code Segment).
 * @param ss Seletor de 16-bits para o segmento de Pilha (Stack Segment).
 * @param ds Seletor de 16-bits para o segmento de Dados (Data Segment).
 * @param es Seletor de 16-bits para o segmento de Dados Extra (Extra Segment).
 * @param fs Seletor de 16-bits para o segmento genérico FS.
 * @param gs Seletor de 16-bits para o segmento genérico GS.
 */
extern void gdt_flush(gdt_segment_t cs, gdt_segment_t ss, gdt_segment_t ds, gdt_segment_t es, gdt_segment_t fs, gdt_segment_t gs);

/**
 * @brief Obtém o endereço de memória virtual (VMA) associado a um seletor de 32-bits.
 * Calcula ou extrai a base linear configurada na GDT para o segmento fornecido.
 * @param s O seletor de segmento que se deseja consultar.
 * @return uint32_t O endereço base de 32-bits do segmento em memória.
 */
extern uint32_t gdt_get_segment_vma(gdt_segment_t s);

#endif /* GDT_H_ */
