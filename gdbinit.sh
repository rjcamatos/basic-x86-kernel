#!/bin/bash

# 2. Inicia o QEMU em modo suspenso (-S) e abre o servidor GDB na porta 1234 (-s)
qemu-system-i386 -s -S -enable-kvm -rtc base=localtime -kernel './build/ksrc/Kernel.elf' -append 'THE KERNEL ARGUMENTS' &
#qemu-system-x86_64 -s -S -kernel ./build/Kernel.elf -append 'THE KERNEL ARGUMENTS' &


# 3. Aguarda 4 segundos para garantir que o QEMU abriu o socket TCP antes de injetar o GDB
sleep 4

# 4. Inicia o GDB a ler os comandos automáticos do teu ficheiro gdbinit.gdb
gdb -x ./gdbinit.gdb
