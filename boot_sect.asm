[org 0x7c00]
KERNEL_OFFSET equ 0x1000 

    mov [BOOT_DRIVE], dl
    mov bp, 0x9000
    mov sp, bp

    call load_kernel
    call switch_to_pm
    jmp $

%include "gdt.asm" ; Define la tabla de segmentos

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET 
    mov dh, 31 ; Carga 31 sectores
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    call KERNEL_OFFSET ; Salta al kernel
    jmp $

BOOT_DRIVE db 0
times 510-($-$$) db 0
dw 0xaa55 ; Firma mágica
