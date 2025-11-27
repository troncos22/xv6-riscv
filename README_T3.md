# Tarea 3: Protección de Lectura en XV6

## Información del Estudiante
- **Nombre**: Jim Troncos
- **Rama**: jimtroncos_t3
- **Fecha**: 27 de Noviembre, 2024

## Descripción del Proyecto

Esta tarea implementa un mecanismo de protección de memoria "solo escritura" en XV6-RISC-V, útil para manejar datos sensibles como claves criptográficas. El sistema permite remover permisos de lectura de páginas de memoria mientras mantiene los permisos de escritura.

## Implementación

### 1. Funciones Principales en `kernel/vm.c`

#### `mrdprotect(pagetable_t pagetable, uint64 addr, int len)`
Esta función remueve el permiso de lectura de un rango de páginas:
- **Parámetros**:
  - `pagetable`: Tabla de páginas del proceso
  - `addr`: Dirección inicial (debe estar alineada a página)
  - `len`: Número de páginas a proteger
- **Funcionamiento**:
  1. Valida que la dirección esté alineada y len > 0
  2. Itera sobre cada página en el rango
  3. Verifica que cada página sea válida (PTE_V) y de usuario (PTE_U)
  4. Limpia el bit PTE_R manteniendo otros bits
  5. Ejecuta `sfence_vma()` para vaciar el TLB

#### `munrdprotect(pagetable_t pagetable, uint64 addr, int len)`
Esta función restaura el permiso de lectura:
- **Parámetros**: Idénticos a `mrdprotect`
- **Funcionamiento**: Similar pero establece el bit PTE_R en lugar de limpiarlo

### 2. System Calls

#### Implementación en `kernel/sysproc.c`
```c
sys_mrdprotect() - Obtiene argumentos y llama a mrdprotect()
sys_munrdprotect() - Obtiene argumentos y llama a munrdprotect()
```

### 3. Archivos Modificados

1. **kernel/vm.c**: Implementación de las funciones principales
2. **kernel/defs.h**: Declaraciones de las nuevas funciones
3. **kernel/syscall.h**: Números de system call (23 y 24)
4. **kernel/syscall.c**: Registro de las system calls
5. **kernel/sysproc.c**: Implementación de las system calls
6. **user/user.h**: Declaraciones para programas de usuario
7. **user/usys.pl**: Generación de stubs de assembly
8. **Makefile**: Inclusión del programa de prueba

## Programa de Prueba

El programa `rdprotect_test.c` valida la implementación:

1. **Reserva memoria**: Usa `sbrk()` para obtener una página
2. **Escribe valor inicial**: Almacena 'Z' en la página
3. **Protege memoria**: Llama a `mrdprotect()` para remover lectura
4. **Prueba escritura**: Escribe 'A' (debe funcionar)
5. **Prueba lectura**: Intenta leer (debe causar page fault)
6. **Restaura permisos**: Llama a `munrdprotect()`
7. **Verifica restauración**: Lee exitosamente el valor

## Validación de Errores

El sistema valida correctamente:
- Alineación de direcciones a páginas (addr % PGSIZE == 0)
- Parámetro len > 0
- Direcciones dentro del espacio de usuario (< MAXVA)
- Páginas válidas (PTE_V) y de usuario (PTE_U)
- No modifica memoria del kernel

## Consideraciones de Seguridad

### Ventajas
- Permite manejar datos sensibles sin riesgo de lectura accidental
- Útil para claves criptográficas y credenciales
- Mantiene la capacidad de actualizar datos protegidos

### Limitaciones
- La protección es a nivel de página completa (4KB)
- Los procesos pueden revertir la protección con `munrdprotect()`
- No protege contra accesos del kernel

## Testing y Resultados Esperados

Al ejecutar `rdprotect_test`:

1. **Salida normal** (sin protección activada en hardware):
   ```
   Valor inicial escrito: Z
   Protegiendo página contra lectura...
   Página protegida exitosamente
   Intentando escribir 'A' en la página protegida...
   Escritura exitosa
   Intentando leer de la página protegida...
   [Page Fault esperado aquí]
   ```

2. **Si el hardware no respeta PTE_R**:
   - El programa completará sin errores
   - Se imprimirá "Valor leído: A"
   - Esto indica que el hardware/emulador no implementa la protección

## Compilación e Instalación

```bash
# Compilar XV6 con las modificaciones
make clean
make qemu

# En XV6, ejecutar el test
$ rdprotect_test
```

## Conclusiones

Esta implementación demuestra cómo los sistemas operativos pueden proporcionar mecanismos de protección de memoria especializados. Aunque el concepto de memoria "solo escritura" es inusual, tiene aplicaciones prácticas en seguridad, especialmente para el manejo de material criptográfico sensible.

La implementación es eficiente y segura, validando todos los parámetros y manteniendo la integridad del sistema. El uso del bit PTE_R del estándar RISC-V garantiza compatibilidad con el hardware que soporte esta característica.

## Referencias

- RISC-V Privileged Architecture Specification
- XV6 Book - Chapter on Virtual Memory
- Documentation on Page Table Entry bits in RISC-V
