
# Conecta o GDB ao QEMU automaticamente
target remote localhost:1234

set architecture i386:x86-64

# Localização da Memória Lógica MAIS o Alinhamento da SECTION .text
set $KERNEL_CONFIG_LMA = 0x00101000

# Descarta símbolos antigos e carrega o ficheiro com o offset 
# Virtual e Lógico correto

add-symbol-file  ./build/ksrc/Kernel.elf
add-symbol-file ./build/ksrc/Kernel.elf $KERNEL_CONFIG_LMA

# Coloca os breakpoints iniciais nos teus símbolos estruturais
# ATENçÃO qemu com -enable-kvm só funciona com hardware breakpoints
#b entry
#b kmain

hbreak entry
hbreak acpi_init




