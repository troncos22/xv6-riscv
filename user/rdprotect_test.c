#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  char *addr = sbrk(0); // Dirección actual del heap
  sbrk(4096); // Reservar una página

  addr[0] = 'Z'; // Escribir valor inicial
  printf("Valor inicial escrito: %c\n", addr[0]);

  // Proteger contra lectura
  printf("Protegiendo página contra lectura...\n");
  if (mrdprotect(addr, 1) < 0) {
    printf("mrdprotect falló\n");
    exit(1);
  }
  printf("Página protegida exitosamente\n");

  // Escritura aún permitida
  printf("Intentando escribir 'A' en la página protegida...\n");
  addr[0] = 'A';
  printf("Escritura exitosa\n");

  // Intento de lectura debería provocar fallo
  printf("Intentando leer de la página protegida...\n");
  char c = addr[0];
  printf("Valor leído: %c (esto NO debería imprimirse)\n", c);

  // Revertir protección
  printf("Restaurando permisos de lectura...\n");
  if (munrdprotect(addr, 1) < 0) {
    printf("munrdprotect falló\n");
    exit(1);
  }
  
  printf("Protección revertida correctamente.\n");
  
  // Verificar que ahora sí se puede leer
  c = addr[0];
  printf("Valor leído después de restaurar permisos: %c\n", c);
  
  exit(0);
}
