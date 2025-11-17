#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;
  int i;
  
  printf("\nLOTTERY SCHEDULING - Demo\n\n");
  printf("Creando 10 procesos con diferentes tickets...\n\n");
  
  // Crear 10 procesos
  for(i = 0; i < 10; i++) {
    pid = fork();
    
    if(pid < 0) {
      printf("Error: fork fallo\n");
      exit(1);
    }
    
    if(pid == 0) {
      // PROCESO HIJO
      int my_tickets = 50 * (i + 1);
      settickets(my_tickets);
      
      printf("[Proceso %d] Tickets: %d\n", i, my_tickets);
      
      // Hacer trabajo
      int sum = 0;
      int j;
      for(j = 0; j < 1000000; j++) {
        sum += j % 13;
      }
      
      printf("[Proceso %d] Completado (sum=%d)\n", i, sum);
      exit(0);
    }
  }
  
  // PROCESO PADRE - esperar a todos
  for(i = 0; i < 10; i++) {
    wait(0);
  }
  
  printf("\nTodos los procesos terminaron.\n");
  printf("Presiona Ctrl-P para ver estadisticas.\n\n");
  
  exit(0);
}