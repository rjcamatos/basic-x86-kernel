#!/bin/bash
set -e

# --- CONFIGURAÇÕES DE CAMINHOS ---
KERNEL_SOURCE="../build/ksrc/Kernel.elf"   # Caminho para o seu kernel
LIMINE_DIR="./limine"                       # Caminho para a sua pasta local do Limine
IMAGE_NAME="BasicKernel.img"                # Imagem de disco final
IMAGE_SIZE_MB=64                            # Tamanho do disco em MB

echo "=== Recriando o disco híbrido em MODO TEXTO (Limine v12) ==="

# 1. Validações Iniciais
if [ ! -f "$KERNEL_SOURCE" ]; then
    echo "Erro: O ficheiro do kernel não foi encontrado em: $KERNEL_SOURCE"
    exit 1
fi
if [ ! -d "$LIMINE_DIR" ]; then
    echo "Erro: A pasta local do Limine não foi encontrada em: $LIMINE_DIR"
    exit 1
fi

# 2. Criar e formatar uma partição FAT32 limpa e isolada
echo "-> Formatando o sistema de ficheiros em FAT32 nativo..."
PART_SIZE_MB=$((IMAGE_SIZE_MB - 1))
dd if=/dev/zero of=particao.tmp bs=1M count=$PART_SIZE_MB
mkfs.vfat -F 32 particao.tmp

# 3. Concatenar o bloco inicial reservado para o MBR (1MB) com a partição
echo "-> Estruturando a imagem binária final..."
dd if=/dev/zero of=$IMAGE_NAME bs=1M count=1
dd if=particao.tmp of=$IMAGE_NAME bs=1M seek=1
rm particao.tmp

# 4. Escrever a tabela de partições MBR na imagem
echo "-> Aplicando a tabela de partições MBR..."
printf "o\nn\np\n1\n2048\n\nt\nc\na\nw\n" | fdisk $IMAGE_NAME > /dev/null 2>&1

# 5. Criar a árvore de diretórios em minúsculas
echo "-> Criando a árvore de diretorias..."
mmd -i $IMAGE_NAME@@1M ::/EFI
mmd -i $IMAGE_NAME@@1M ::/EFI/BOOT
mmd -i $IMAGE_NAME@@1M ::/boot
mmd -i $IMAGE_NAME@@1M ::/boot/limine

# 6. Copiar os binários do Limine e o Kernel
echo "-> Copiando os executáveis do ciclo de boot..."
mcopy -i $IMAGE_NAME@@1M "$LIMINE_DIR/BOOTX64.EFI" ::/EFI/BOOT/BOOTX64.EFI
mcopy -i $IMAGE_NAME@@1M "$LIMINE_DIR/BOOTIA32.EFI" ::/EFI/BOOT/BOOTIA32.EFI
mcopy -i $IMAGE_NAME@@1M "$LIMINE_DIR/limine-bios.sys" ::/boot/limine/limine-bios.sys
mcopy -i $IMAGE_NAME@@1M "$KERNEL_SOURCE" ::/boot/kernel.elf

# 7. Gerar o ficheiro limine.conf MODIFICADO PARA MODO TEXTO
echo "-> Criando a configuração limine.conf (Forçando vga: text)..."
cat << 'EOF' > limine.conf
timeout: 5

/O Meu OS (Modo Texto Legacy)
    protocol: multiboot1
    path: boot():/boot/kernel.elf
    textmode: yes
EOF

# 8. Injetar o ficheiro .conf nas localizações do Limine
export MTOOLS_LOWER_CASE=1
mcopy -o -i $IMAGE_NAME@@1M limine.conf ::/limine.conf
mcopy -o -i $IMAGE_NAME@@1M limine.conf ::/boot/limine/limine.conf
rm limine.conf

# 9. Executar o instalador do Limine para selar o código MBR
echo "-> Selando o código MBR na imagem..."
"$LIMINE_DIR/limine" bios-install $IMAGE_NAME

echo "=== Concluído! Imagem $IMAGE_NAME gerada em Modo Texto. ==="
