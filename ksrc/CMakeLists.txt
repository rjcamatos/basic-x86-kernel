cmake_minimum_required(VERSION 3.16)
project(Generic-i386-kernel LANGUAGES C ASM)

# ==============================================================================
# 1. CONFIGURAÇÕES DE CROSS-COMPILATION (x86 32-bit)
# ==============================================================================
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i386)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_ASM_COMPILER_WORKS 1)

# Suporte para seleção dinâmica do Toolchain do sistema
set(TOOLCHAIN_PREFIX "" CACHE STRING "Prefixo do toolchain (ex: i686-elf-)")

set(CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}as")
set(LD_LINKER          "${TOOLCHAIN_PREFIX}ld")

# ==============================================================================
# 2. DEFINIÇÃO DAS VARIÁVEIS DO KERNEL (Acedidas por C, ASM e LD)
# ==============================================================================
set(KERNEL_CONFIG_LMA "0x00100000") # Endereço Físico (Low Memory)
set(KERNEL_CONFIG_VMA "0xC0000000") # Endereço Virtual (High Memory)

# Flags específicas para o comando ld puro
set(LINKER_FLAGS "-m elf_i386 -no-pie --no-warn-rwx-segments")

# ==============================================================================
# 3. CONFIGURAÇÃO DO PRÉ-PROCESSADOR DO LINKER SCRIPT
# ==============================================================================
# O ficheiro original deve ser renomeado para "linker.ld.S"
set(LINKER_SCRIPT_IN "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld.S")
set(LINKER_SCRIPT_OUT "${CMAKE_CURRENT_BINARY_DIR}/linker.ld")

# Comando que gera o .ld final substituindo as variáveis antes da linkagem
add_custom_command(
    OUTPUT ${LINKER_SCRIPT_OUT}
    COMMAND ${CMAKE_C_COMPILER} -E -P -x c 
            -DKERNEL_CONFIG_LMA=${KERNEL_CONFIG_LMA} 
            -DKERNEL_CONFIG_VMA=${KERNEL_CONFIG_VMA} 
            ${LINKER_SCRIPT_IN} -o ${LINKER_SCRIPT_OUT}
    DEPENDS ${LINKER_SCRIPT_IN}
    COMMENT "A injetar KERNEL_CONFIG_LMA e KERNEL_CONFIG_VMA no Linker Script..."
)

# ==============================================================================
# 4. FLAGS DO COMPILADOR C E ASSEMBLER (Injeção de Macros Globais)
# ==============================================================================
# Injeta as variáveis no GCC via -D e no AS via --defsym
set(CMAKE_C_FLAGS "-m32 -ffreestanding -fno-pie -fno-stack-protector -mno-red-zone -fno-builtin -nostdlib -Wall -Wextra -Wno-unused-parameter -g -O0 -DKERNEL_CONFIG_LMA=${KERNEL_CONFIG_LMA} -DKERNEL_CONFIG_VMA=${KERNEL_CONFIG_VMA}" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS "--32 -g --defsym KERNEL_CONFIG_LMA=${KERNEL_CONFIG_LMA} --defsym KERNEL_CONFIG_VMA=${KERNEL_CONFIG_VMA}" CACHE STRING "" FORCE)

# ==============================================================================
# 5. DIRETÓRIOS DE INCLUSÃO DE HEADERS E PROCURA DE FONTES
# ==============================================================================
set(KLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../klib")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${KLIB_DIR})

file(GLOB_RECURSE SRCS_C CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB_RECURSE SRCS_ASM CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.s")

# ==============================================================================
# 6. DEFINIÇÃO DO EXECUTÁVEL E LINKAGEM FINAL
# ==============================================================================
# Adiciona o script gerado como dependência direta do executável
add_executable(Kernel.elf ${SRCS_C} ${SRCS_ASM} ${LINKER_SCRIPT_OUT})

# Executa o 'ld' puro apontando para o script processado na pasta de build
set(CMAKE_C_LINK_EXECUTABLE "${LD_LINKER} ${LINKER_FLAGS} -T ${LINKER_SCRIPT_OUT} <OBJECTS> ${CMAKE_CURRENT_SOURCE_DIR}/../build/klib/libklib.a -o <TARGET>")
