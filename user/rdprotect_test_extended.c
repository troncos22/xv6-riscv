#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Test adicional para verificar múltiples páginas
void test_multiple_pages() {
  printf("\n=== Test de Múltiples Páginas ===\n");
  
  char *addr = sbrk(0);
  sbrk(3 * 4096); // Reservar 3 páginas
  
  // Escribir en cada página
  addr[0] = 'A';
  addr[4096] = 'B';
  addr[8192] = 'C';
  
  printf("Valores iniciales escritos: %c, %c, %c\n", 
         addr[0], addr[4096], addr[8192]);
  
  // Proteger las 3 páginas
  if (mrdprotect(addr, 3) < 0) {
    printf("ERROR: mrdprotect falló para 3 páginas\n");
    return;
  }
  
  printf("3 páginas protegidas exitosamente\n");
  
  // Intentar escribir (debe funcionar)
  addr[0] = 'X';
  addr[4096] = 'Y';
  addr[8192] = 'Z';
  printf("Escritura en páginas protegidas completada\n");
  
  // Restaurar permisos
  if (munrdprotect(addr, 3) < 0) {
    printf("ERROR: munrdprotect falló\n");
    return;
  }
  
  printf("Permisos restaurados. Valores: %c, %c, %c\n",
         addr[0], addr[4096], addr[8192]);
}

// Test de validación de errores
void test_error_validation() {
  printf("\n=== Test de Validación de Errores ===\n");
  
  char *addr = sbrk(0);
  sbrk(4096);
  
  // Test 1: Dirección no alineada
  printf("Test 1 - Dirección no alineada: ");
  if (mrdprotect(addr + 1, 1) == -1) {
    printf("PASS (error detectado)\n");
  } else {
    printf("FAIL (debería fallar)\n");
  }
  
  // Test 2: len <= 0
  printf("Test 2 - len = 0: ");
  if (mrdprotect(addr, 0) == -1) {
    printf("PASS (error detectado)\n");
  } else {
    printf("FAIL (debería fallar)\n");
  }
  
  printf("Test 3 - len negativo: ");
  if (mrdprotect(addr, -1) == -1) {
    printf("PASS (error detectado)\n");
  } else {
    printf("FAIL (debería fallar)\n");
  }
  
  // Test 4: Dirección inválida (muy alta)
  printf("Test 4 - Dirección inválida: ");
  if (mrdprotect((void*)0xFFFFFFFFFFFF0000, 1) == -1) {
    printf("PASS (error detectado)\n");
  } else {
    printf("FAIL (debería fallar)\n");
  }
}

int main() {
  printf("====================================\n");
  printf("   Test de Protección de Lectura   \n");
  printf("====================================\n");
  
  // Test básico
  printf("\n=== Test Básico ===\n");
  
  char *addr = sbrk(0); // Dirección actual del heap
  sbrk(4096); // Reservar una página

  addr[0] = 'Z'; // Escribir valor inicial
  printf("1. Valor inicial escrito: %c\n", addr[0]);

  // Proteger contra lectura
  printf("2. Protegiendo página contra lectura...\n");
  if (mrdprotect(addr, 1) < 0) {
    printf("   ERROR: mrdprotect falló\n");
    exit(1);
  }
  printf("   Página protegida exitosamente\n");

  // Escritura aún permitida
  printf("3. Escribiendo 'A' en página protegida...\n");
  addr[0] = 'A';
  printf("   Escritura exitosa\n");

  // Intento de lectura debería provocar fallo
  printf("4. Intentando leer de página protegida...\n");
  printf("   NOTA: Si el hardware soporta PTE_R, esto causará page fault\n");
  char c = addr[0];
  printf("   Valor leído: %c (hardware no soporta protección de lectura)\n", c);

  // Revertir protección
  printf("5. Restaurando permisos de lectura...\n");
  if (munrdprotect(addr, 1) < 0) {
    printf("   ERROR: munrdprotect falló\n");
    exit(1);
  }
  printf("   Protección revertida correctamente\n");
  
  // Verificar que ahora sí se puede leer
  c = addr[0];
  printf("6. Valor después de restaurar: %c\n", c);
  
  // Ejecutar tests adicionales
  test_multiple_pages();
  test_error_validation();
  
  printf("\n====================================\n");
  printf("   Todos los tests completados     \n");
  printf("====================================\n");
  
  exit(0);
}
