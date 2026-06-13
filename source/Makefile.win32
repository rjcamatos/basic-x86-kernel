################################################################################
# Makefile OSDev com Cross-Compiler i686-elf (Linkagem via GCC unificada)
################################################################################

# Força o Make a usar o terminal do Windows, evitando conflitos no VS Code
SHELL := cmd.exe

# Pasta onde todos os ficheiros gerados (.o e .elf) vão ficar
BUILD_DIR := ..\build

# Comando de remoção nativo do Windows
RM := del /q /f

# Ferramentas do Cross-Compiler i686-elf instaladas
CC = i686-elf-gcc
AS = nasm
# Mudança Crítica: Usamos o GCC como Linker para aceitar as flags de OSDev corretamente
LD = i686-elf-gcc

# Opções de compilação sem qualquer interferência do ecossistema Windows
CFLAGS = -nostdlib -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -g

# Busca automaticamente ficheiros .c e .s/.S na raiz e nas subpastas
SRCS_C  := $(wildcard *.c) $(wildcard */*.c)
SRCS_s  := $(wildcard *.s) $(wildcard */*.s)
SRCS_S_CAPS := $(wildcard *.S) $(wildcard */*.S)

# Converte a lista de fontes em objetos, mas direcionando-os para a pasta BUILD_DIR
# O 'notdir' remove o caminho original para não duplicar árvores de pastas dentro da build
OBJS := $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS_C:.c=.o) $(SRCS_s:.s=.o) $(SRCS_S_CAPS:.S=.o)))

# Alvo principal
all: $(BUILD_DIR) $(BUILD_DIR)/Kernel.elf

# Cria a pasta de build caso ela não exista de forma silenciosa
$(BUILD_DIR):
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# Instruções de Linkagem corrigidas - O alvo agora aponta para dentro da pasta build
$(BUILD_DIR)/Kernel.elf: $(OBJS)
	@echo Building target: $@
	@echo Invoking: i686-elf Linker via GCC
	$(LD) -m32 -nostdlib -nostartfiles -nodefaultlibs -Wl,-T,linker.ld -o "$@" $(OBJS)
	@echo Finished building target: $@
	@echo  

# Regras de compilação (procuram o ficheiro fonte onde quer que ele esteja)
# O VPATH diz ao Make onde procurar pelos ficheiros .c e .s originais
VPATH := $(sort $(dir $(SRCS_C) $(SRCS_s) $(SRCS_S_CAPS)))

# Regra genérica para compilar qualquer arquivo .c em .o dentro da pasta build
$(BUILD_DIR)/%.o: %.c
	@echo Compiling: $<
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar ficheiros Assembly (.s) dentro da pasta build
$(BUILD_DIR)/%.o: %.s
	@echo Assembling: $<
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar ficheiros Assembly (.S) dentro da pasta build
$(BUILD_DIR)/%.o: %.S
	@echo Assembling: $<
	$(CC) $(CFLAGS) -c $< -o $@

# Comando de limpeza ajustado para apagar a pasta build inteira no Windows
clean:
	-rmdir /s /q $(BUILD_DIR) 2>nul
	@echo Limpeza concluida com sucesso.

.PHONY: all clean
