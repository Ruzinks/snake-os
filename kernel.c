/*
 * 🪐 Ruz-OS Kernel v0.1
 * Desarrollado por RuzStudio (Ruzinks)
 * Basado en el motor modular de SnakeOS
 */

#include "display.h"    // Driver VGA 13h
#include "constants.h"  // Definiciones del sistema
#include "keyboard.h"   // Driver de teclado PS/2
#include "font.h"       // Renderizado de texto
#include "idt.h"        // Tabla de Interrupciones

// Variable global para detalles de pantalla[cite: 1]
DisplayDetails globalDisplayDetails;

// --- Callbacks de Teclado (Lógica de Interfaz) ---

void onEnterPressed() {
    // Acción para RuzWriter o ejecutar comando
    display_print_at("Ejecutando...", 10, 180, 0x0F);
}

void onArrowUp() {
    // Navegación de menú
    putpixel(160, 100, 0x09); // Acento azul claro
}

void onArrowDown() {
    putpixel(160, 105, 0x01); // Azul oscuro RuzTech
}

void onKeyboardError() {
    // Error crítico de hardware[cite: 1]
    display_drawErr();
    display_drawErrThree();
}

// --- Funciones de Interfaz Ruz-OS ---

void draw_ruz_interface() {
    // Limpiar pantalla con gris claro (Estilo Microsoft)
    for(int i = 0; i < 320 * 200; i++) {
        // Acceso directo a memoria de video 0xA0000 si está disponible en display.c
        // Usamos la función base por ahora
    }
    
    // Dibujar Barra de Tareas / Navbar Superior (Azul RuzStudio)
    for(int y = 0; y < 15; y++) {
        for(int x = 0; x < 320; x++) {
            putpixel(x, y, 0x01); 
        }
    }

    // Título del Sistema
    display_print_at("Ruz-OS v0.1 - RuzStudio", 5, 4, 0x0F);
}

// --- Punto de Entrada Principal ---

int main() {
    // 1. Inicializar Hardware de Video[cite: 1]
    DisplayDetails details = display_init();
    globalDisplayDetails = details;

    // 2. Inicializar la IDT (Interrupciones)[cite: 1]
    idt_init();

    // 3. Configurar Driver de Teclado con Callbacks[cite: 1]
    keyboard_cbTable callbackTable = keyboard_initiateCbTable();
    
    callbackTable.ENTERKEY = &onEnterPressed;
    callbackTable.UPKEY = &onArrowUp;
    callbackTable.DOWNKEY = &onArrowDown;
    callbackTable.NOKEYBOARD = &onKeyboardError;
    
    // Inyectar tabla de callbacks al driver[cite: 1]
    keyboard_init(callbackTable);

    // 4. Dibujar la interfaz de usuario inicial
    draw_ruz_interface();

    // Mensaje de bienvenida
    display_print_at("Bienvenido, Noctis.", 100, 90, 0x01);
    display_print_at("Listo para programar.", 95, 110, 0x08);

    // 5. El Bucle del Sistema (System Loop)[cite: 1]
    bool running = true;
    while(running) {
        // En un sistema real basado en interrupciones (IDT), 
        // el teclado se procesa vía IRQ1. 
        // keyboard_read() gestiona la lectura del buffer[cite: 1].
        keyboard_read();
        
        // Aquí se pueden añadir gestores de procesos o tareas de fondo
    }

    return 0;
}
