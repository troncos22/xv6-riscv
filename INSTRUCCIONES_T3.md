# Instrucciones para Compilar y Probar la Tarea 3

## Pasos para Compilar

1. **Abrir una terminal en el directorio del proyecto**
   ```bash
   cd D:\codes\xv6-riscv
   ```

2. **Crear la rama para la tarea** (si no existe)
   ```bash
   git checkout -b jimtroncos_t3
   ```

3. **Limpiar compilación anterior**
   ```bash
   make clean
   ```

4. **Compilar XV6**
   ```bash
   make
   ```

## Pasos para Probar

1. **Ejecutar XV6 en QEMU**
   ```bash
   make qemu
   ```

2. **Una vez dentro de XV6, ejecutar los tests:**

   **Test básico:**
   ```
   $ rdprotect_test
   ```

   **Test extendido (más completo):**
   ```
   $ rdprotect_test_extended
   ```

## Resultados Esperados

### Si el hardware/emulador soporta PTE_R:
- El programa causará un page fault al intentar leer de memoria protegida
- XV6 mostrará un mensaje de error y terminará el proceso

### Si el hardware/emulador NO soporta PTE_R:
- El programa completará todos los tests
- Mostrará que puede leer incluso después de "proteger" la memoria
- Esto es normal en QEMU estándar

## Verificación de la Implementación

Para verificar que la implementación está correcta, revisa:

1. **Que las funciones modifiquen correctamente los PTEs:**
   - `mrdprotect` limpia el bit PTE_R
   - `munrdprotect` establece el bit PTE_R

2. **Que se validen todos los errores:**
   - Direcciones no alineadas
   - Parámetros inválidos
   - Direcciones fuera del espacio de usuario

3. **Que el test extendido muestre:**
   - PASS en todos los tests de validación de errores
   - Correcta protección/desprotección de múltiples páginas

## Troubleshooting

### Error: "undefined reference to `mrdprotect`"
- Verifica que hayas agregado las entradas en usys.pl
- Regenera usys.S con: `rm user/usys.S && make`

### Error al compilar vm.c
- Verifica que las funciones estén correctamente implementadas
- Revisa que no haya errores de sintaxis

### XV6 no arranca
- Ejecuta `make clean` y recompila
- Verifica que no haya errores en los archivos del kernel

## Subir a GitHub

1. **Agregar todos los archivos modificados**
   ```bash
   git add .
   ```

2. **Hacer commit**
   ```bash
   git commit -m "Tarea 3: Implementación de protección de lectura en XV6"
   ```

3. **Subir la rama**
   ```bash
   git push origin jimtroncos_t3
   ```

4. **El enlace para entregar será:**
   ```
   https://github.com/tu_usuario/xv6-riscv/tree/jimtroncos_t3
   ```

## Archivos Modificados

La implementación modifica los siguientes archivos:
- kernel/vm.c (funciones principales)
- kernel/defs.h (declaraciones)
- kernel/sysproc.c (system calls)
- kernel/syscall.c (registro de syscalls)
- kernel/syscall.h (números de syscalls)
- user/user.h (interfaz de usuario)
- user/usys.pl (generación de stubs)
- user/rdprotect_test.c (test básico)
- user/rdprotect_test_extended.c (test completo)
- Makefile (compilación de tests)
- README_T3.md (documentación)
