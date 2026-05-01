#!/bin/bash

# Configuración de carpetas
OBJ_DIR="obj"
BIN_DIR="bin"
KERNEL_BIN="$BIN_DIR/kernel.bin"
OS_IMAGE="$BIN_DIR/os.bin"

# Crear directorios si no existen
mkdir -p $OBJ_DIR $BIN_DIR

echo "--- Checking Dependencies ---"

check_dep() {
    if ! command -v "$1" &> /dev/null; then
        printf "\e[31m\e[1mDependency $1 not found.\e[0m\n"
        echo "Try: $2"
        exit 1
    fi
}

check_dep "gcc" "sudo apt install build-essential"
check_dep "ld" "sudo apt install build-essential"
check_dep "nasm" "sudo apt-get install nasm"

echo "--- Compiling Kernel ---"

# 1. Compilar el kernel (C -> Objeto)
# -ffreestanding: Indica que no tenemos biblioteca estándar de C (no hay printf, etc.)
if ! gcc -m32 -fno-pie -ffreestanding -g -c kernel.c -o $OBJ_DIR/main.o; then
    echo "Error en la compilación de kernel.c"
    exit 1
fi

# 2. Ensamblar punto de entrada y sector de arranque
nasm -f elf kernel_entry.asm -o $OBJ_DIR/entry.o
nasm -f bin boot_sect.asm -o $OBJ_DIR/boot.bin

echo "--- Linking ---"

# 3. Enlazar (Linker)
# -Ttext 0x1000: Alinea el código con la dirección de memoria donde el bootloader lo carga
ld -m elf_i386 -o $KERNEL_BIN -Ttext 0x1000 $OBJ_DIR/entry.o $OBJ_DIR/main.o -e main --oformat binary

if [ ! -f $KERNEL_BIN ]; then
    echo "Linking Failed."
    exit 1
fi

# 4. Crear imagen final (Pegar archivos)
cat $OBJ_DIR/boot.bin $KERNEL_BIN > $OS_IMAGE

echo "--- Process Finished ---"

# Ejecutar en QEMU
if [ "$1" != "-nvm" ]; then
    check_dep "qemu-system-x86_64" "sudo apt-get install qemu-system"
    echo "Booting QEMU..."
    qemu-system-x86_64 -drive format=raw,file=$OS_IMAGE -display sdl -monitor stdio
else
    echo "Image created at $OS_IMAGE"
fi
