# Tarea 2: Lottery Scheduling en xv6-riscv

**Estudiante:** [Tu nombre]  
**Grupo:** [Tu grupo]  
**Fecha:** [Fecha de entrega]

---

## 1. Funcionamiento y Lógica de la Implementación

### 1.1 Concepto de Lottery Scheduling

Lottery Scheduling es un algoritmo de planificación proporcional donde:
- Cada proceso posee un número de "tickets"
- El scheduler realiza una "lotería" en cada ciclo de planificación
- La probabilidad de que un proceso sea seleccionado es proporcional a sus tickets

**Fórmula:**
```
P(proceso i sea seleccionado) = tickets_i / Σ(todos los tickets)
```

### 1.2 Algoritmo Implementado
```
PARA CADA ciclo de scheduler:
  1. Calcular total_tickets = suma de tickets de procesos RUNNABLE
  2. Si total_tickets == 0, continuar al siguiente ciclo
  3. Generar número aleatorio r ∈ [1, total_tickets]
  4. Acumular tickets proceso por proceso hasta que acumulador >= r
  5. Ejecutar el proceso seleccionado
  6. Incrementar run_slices del proceso
```

### 1.3 Componentes Principales

#### Campo `tickets` en `struct proc`
- Almacena el número de tickets de cada proceso
- Valor por defecto: 100
- Valor mínimo: 1

#### Campo `run_slices` en `struct proc`
- Contador de cuántas veces el proceso fue ejecutado
- Permite medir la distribución real de CPU vs. teórica

#### Syscall `settickets(int n)`
- Permite que un proceso modifique sus tickets
- Valida que n >= 1

#### Función `scheduler()`
- Implementa la lotería
- Usa un generador pseudoaleatorio (LCG)

---

## 2. Modificaciones Realizadas

### Archivos Modificados

| Archivo | Cambios |
|---------|---------|
| `kernel/proc.h` | Agregado campos `tickets` y `run_slices` a `struct proc` |
| `kernel/proc.c` | Modificado `allocproc()` para inicializar tickets<br>Reemplazado `scheduler()` con lottery scheduler<br>Agregado `random()` y `random_init()`<br>Modificado `procdump()` para mostrar estadísticas |
| `kernel/sysproc.c` | Implementado `sys_settickets()` |
| `kernel/syscall.h` | Agregado `#define SYS_settickets 22` |
| `kernel/syscall.c` | Agregado declaración y entrada en array para `sys_settickets` |
| `kernel/defs.h` | Agregado prototipos de `random()` y `random_init()` |
| `kernel/main.c` | Agregado llamada a `random_init(1)` |
| `user/user.h` | Agregado prototipo `int settickets(int)` |
| `user/usys.pl` | Agregado `entry("settickets")` |
| `user/lottery_demo.c` | Programa de prueba |
| `Makefile` | Agregado `$U/_lottery_demo\` |

---

## 3. Dificultades Encontradas y Soluciones

### 3.1 Generación de Números Aleatorios
**Problema:** xv6 no tiene soporte para `rand()` o `/dev/random`

**Solución:** Implementé un Linear Congruential Generator (LCG):
```c
random_seed = random_seed * 1103515245 + 12345;
return (random_seed / 65536) % 32768;
```

### 3.2 Condiciones de Carrera
**Problema:** Múltiples CPUs accediendo simultáneamente a estructuras de procesos

**Solución:** Uso correcto de `acquire(&p->lock)` y `release(&p->lock)` antes de acceder/modificar datos del proceso

### 3.3 División por Cero
**Problema:** Si no hay procesos RUNNABLE, `total_tickets` sería 0

**Solución:** Verificación explícita:
```c
if(total_tickets == 0) {
  continue;
}
```

### 3.4 Fairness en la Selección
**Problema:** El primer proceso con tickets suficientes siempre gana si el acumulador es igual

**Solución:** La condición `accumulator >= winner` asegura selección justa

---

## 4. Posibles Problemas del Lottery Scheduling

### 4.1 Problema: Inanición (Starvation)
**Descripción:** Procesos con pocos tickets pueden no ejecutarse por largos períodos

**Ejemplo:**
- Proceso A: 1000 tickets
- Proceso B: 1 ticket
- P(B sea seleccionado) = 1/1001 ≈ 0.1%

**Mitigación posible:**
- Implementar "ticket inflation" dinámica
- Asignar tickets base mínimos que aumentan con el tiempo sin ejecución

### 4.2 Problema: Impredecibilidad
**Descripción:** El comportamiento es estocástico, no determinista

**Implicaciones:**
- Dificil para depuración
- No hay garantías de tiempo real
- Puede violar deadlines en sistemas RT

**Comparación con Round-Robin:**
- RR: tiempo de espera máximo predecible
- Lottery: tiempo de espera es una variable aleatoria

### 4.3 Problema: Overhead del RNG
**Descripción:** Generar números aleatorios consume ciclos de CPU

**Medición:**
- Cada llamada a `random()`: ~10-20 instrucciones
- En sistemas con alta frecuencia de context switch, el overhead se acumula

**Alternativa:**
- Usar stride scheduling (determinista, O(1))

### 4.4 Problema: Manipulación de Tickets
**Descripción:** Procesos maliciosos pueden asignarse muchos tickets

**Escenarios de ataque:**
```c
while(1) {
  settickets(999999);  // Monopolizar CPU
}
```

**Soluciones necesarias:**
- Límite máximo de tickets por proceso
- Requerir privilegios para `settickets()`
- Uso de "currency" y control por usuario

### 4.5 Problema: Granularidad de Tickets
**Descripción:** Difícil expresar relaciones complejas

**Ejemplo:** Si quiero A = 2×B = 3×C:
- A: 6 tickets
- B: 3 tickets
- C: 2 tickets
- Total: 11 tickets

Pero si luego quiero A = 1.5×D:
- Necesito reescalar todos los tickets

**Comparación con prioridades:**
- Prioridades: relaciones absolutas
- Tickets: relaciones proporcionales (requieren reescalado)

### 4.6 Problema: No hay Garantía de Progreso Mínimo
**Descripción:** En teoría, un proceso podría nunca ejecutarse

**Probabilidad:**
Si proceso tiene 1 ticket de 1000 total:
```
P(no ejecutarse en n iteraciones) = (999/1000)^n
Para n=10000: P ≈ 0.0000454
```

Aunque improbable, no es imposible.

### 4.7 Problema: Rendimiento con Muchos Procesos
**Descripción:** El algoritmo es O(n) donde n = número de procesos

**Análisis de complejidad:**
```
Para cada scheduling decision:
  - Calcular total: O(n)
  - Encontrar ganador: O(n) en promedio
  - Total: O(n) por decisión
```

**Comparación:**
- Round-Robin: O(1) con cola FIFO
- Lottery: O(n) por decisión

---

## 5. Pruebas y Resultados

### Ejecución
```bash
$ make qemu
...
$ lottery_demo
```

### Resultados Esperados
La distribución de `run_slices` debe aproximarse a la distribución teórica de tickets.

Con 10 procesos (50, 100, 150, ..., 500 tickets):
- Total tickets: 2750
- Proceso 0 (50 tickets): ~1.8% de CPU
- Proceso 9 (500 tickets): ~18.2% de CPU

### Verificación
Presionar `Ctrl-P` en xv6 muestra la tabla de procesos con:
- Tickets asignados
- Run slices acumulados
- Porcentaje teórico vs. real

---

## 6. Conclusiones

Lottery Scheduling es un algoritmo elegante para compartir CPU proporcionalmente, pero presenta desafíos:

**Ventajas:**
- Simple de implementar
- Proporcional y "fair" en promedio
- Flexible (fácil ajustar prioridades)

**Desventajas:**
- No determinista
- Overhead del RNG
- Susceptible a starvation
- No apto para tiempo real

**Uso apropiado:**
- Sistemas de tiempo compartido (timesharing)
- Cuando la proporcionalidad promedio es suficiente
- Ambientes controlados sin procesos maliciosos

---

## Referencias

1. Waldspurger, C. A., & Weihl, W. E. (1994). "Lottery Scheduling: Flexible Proportional-Share Resource Management"
2. xv6: a simple, Unix-like teaching operating system (MIT)
3. RISC-V Privileged Architecture Specification

---