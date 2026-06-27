cmake_minimum_required(VERSION 3.16)
project(klib LANGUAGES C)

# 1. Configurações de Cross-Compilation (x86 32-bit)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i386)

# Suporte para seleção dinâmica do Toolchain do sistema
set(TOOLCHAIN_PREFIX "" CACHE STRING "Prefixo do toolchain (ex: i686-elf-)")

set(CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_AR "${TOOLCHAIN_PREFIX}ar")


# 2. Flags do Compilador C (Idênticas ao teu Makefile)
add_compile_options(
    -m32
    -g
    -O0
    -ffreestanding
    -nostdlib
    -nostdinc
    -Wall
    -Wextra
)

# 3. Diretórios de Inclusão de Headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 4. Procura Automática de Ficheiros Fonte (.c)
file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

# 5. Criação da Biblioteca Estática (Gera automaticamente a libklib.a)
add_library(klib STATIC ${SRCS})

# Forçar o nome exato do ficheiro de saída para "libklib.a"
set_target_properties(klib PROPERTIES PREFIX "lib" OUTPUT_NAME "klib")
