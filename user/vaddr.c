#include "kernel/types.h"  // Tipos de datos del kernel (int, uint, etc.)
#include "kernel/stat.h"   // Estructuras para información de archivos
#include "user/user.h"     // Funciones de usuario (printf, malloc, exit, etc.)

// ============================================================================
// SEGMENTO DE DATOS (.data)
// ============================================================================
// Variables globales INICIALIZADAS se almacenan en el segmento .data
// Este segmento está en direcciones BAJAS de memoria (después del código)
int global = 1;

// ============================================================================
// SEGMENTO BSS (Block Started by Symbol)
// ============================================================================
// Variables globales NO INICIALIZADAS se almacenan en el segmento .bss
// Este segmento viene después del .data y se inicializa automáticamente a 0
int bss_var;

// ============================================================================
// VARIABLE ESTÁTICA
// ============================================================================
// Las variables estáticas (static) también van al segmento .data
// Tienen alcance limitado pero duración toda la vida del programa
static int sttc = 2;

int main(int argc, char* argv[]) {
    // ========================================================================
    // SEGMENTO DE STACK (PILA)
    // ========================================================================
    // Las variables LOCALES se almacenan en el stack
    // El stack crece HACIA ABAJO (de direcciones altas a bajas)
    // Estas variables se destruyen cuando termina la función
    int x = 3;
    
    // ========================================================================
    // SEGMENTO DE HEAP (MONTÍCULO)
    // ========================================================================
    // malloc() reserva memoria dinámicamente en el heap
    // El heap crece HACIA ARRIBA (de direcciones bajas a altas)
    // Esta memoria persiste hasta que se libere con free()
    char* heap_ptr = (char*)malloc(1);
    
    // Verificar que malloc tuvo éxito
    if (heap_ptr == 0) {
        printf("Error: malloc failed\n");
        exit(1);
    }
    
    printf("=== Virtual Memory Layout in xv6-riscv ===\n\n");
    
    // ========================================================================
    // SEGMENTO DE CÓDIGO (.text)
    // ========================================================================
    // El código ejecutable (instrucciones) está en las direcciones MÁS BAJAS
    // Es de solo lectura para prevenir modificaciones accidentales
    printf("Code segment (.text):\n");
    printf("  location of main()     : %p\n", (void*)main);
    
    // ========================================================================
    // SEGMENTO DE DATOS (.data)
    // ========================================================================
    // Contiene variables globales y estáticas INICIALIZADAS
    // Se carga desde el archivo ejecutable con sus valores iniciales
    printf("\nData segment (.data):\n");
    printf("  location of global     : %p\n", (void*)&global);
    printf("  location of sttc       : %p\n", (void*)&sttc);
    
    // ========================================================================
    // SEGMENTO BSS
    // ========================================================================
    // Variables globales NO inicializadas (se inicializan a 0 automáticamente)
    // No ocupa espacio en el archivo ejecutable (se crea al cargar)
    printf("\nBSS segment (uninitialized data):\n");
    printf("  location of bss_var    : %p\n", (void*)&bss_var);
    
    // ========================================================================
    // SEGMENTO DE HEAP
    // ========================================================================
    // Memoria dinámica asignada en tiempo de ejecución
    // Crece hacia direcciones ALTAS (hacia el stack)
    printf("\nHeap segment (dynamic memory):\n");
    printf("  location of malloc(1)  : %p\n", (void*)heap_ptr);
    
    // ========================================================================
    // SEGMENTO DE STACK
    // ========================================================================
    // Variables locales, parámetros de función, direcciones de retorno
    // Crece hacia direcciones BAJAS (hacia el heap)
    // Está en las direcciones MÁS ALTAS del espacio de usuario
    printf("\nStack segment (local variables):\n");
    printf("  location of x          : %p\n", (void*)&x);
    printf("  location of argc       : %p\n", (void*)&argc);
    
    // ========================================================================
    // RESUMEN DEL LAYOUT DE MEMORIA
    // ========================================================================
    printf("\n=== Memory Layout (low to high addresses) ===\n");
    printf("┌─────────────────────────────────────────┐\n");
    printf("│ Code (.text)    - Instrucciones        │ <- Dirección más baja\n");
    printf("│ Data (.data)    - Globales inicializ.  │\n");
    printf("│ BSS             - Globales sin inicial. │\n");
    printf("│ Heap            - malloc() crece ↑      │\n");
    printf("│        ...      - Espacio libre         │\n");
    printf("│ Stack           - Variables locales ↓   │ <- Dirección más alta\n");
    printf("└─────────────────────────────────────────┘\n");
    
    printf("\nNote: En xv6, el heap crece hacia arriba (↑)\n");
    printf("      y el stack crece hacia abajo (↓)\n");
    
    // ========================================================================
    // LIMPIEZA
    // ========================================================================
    // Siempre liberar la memoria asignada con malloc()
    // para evitar memory leaks (fugas de memoria)
    free(heap_ptr);
    
    // Terminar el programa con código de salida 0 (éxito)
    exit(0);
}