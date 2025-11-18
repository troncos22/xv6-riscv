# Resumen y Práctica - Sistemas de Archivos (Clase 12)
## Basado en el Control 2 de SSOO

---

## 📚 RESUMEN DE CONCEPTOS CLAVE

### 1. **MEMORIA VIRTUAL Y PAGINACIÓN**

#### **Dirty Bit**
- Es un bit que indica si una página ha sido **modificada** en memoria
- Importante para saber si necesitamos escribir los cambios al disco durante el swap
- NO confundir con:
  - Bit de carga (loaded bit)
  - Bit de uso (use bit)
  - Bit de validez (valid bit)

#### **Fragmentación**
- **Fragmentación INTERNA**: Espacio sin utilizar **dentro** de un elemento de memoria (página o segmento)
  - Ocurre cuando el tamaño del contenido es menor al tamaño de la página
  - Mayor tamaño de página = Mayor fragmentación interna potencial
  
- **Fragmentación EXTERNA**: Espacio sin utilizar **entre** elementos de memoria
  - Ocurre cuando hay huecos entre páginas/segmentos asignados
  - Problema principal en segmentación con tamaños variables

#### **Traducción de Direcciones Virtuales**
Para una dirección virtual, necesitamos:
- **VPN (Virtual Page Number)**: Identifica la página virtual
- **Offset**: Posición dentro de la página

Estructura típica con Page Directory:
```
[Page Directory Index][Page Table Index][Offset]
```

**Cálculo del Offset:**
- Si el tamaño de página = 2^n bytes → Offset usa n bits
- Ejemplo: Páginas de 1KB (2^10) → Offset de 10 bits

---

### 2. **SISTEMAS DE ARCHIVOS**

#### **Componentes del Sistema de Archivos**

1. **SuperBloque**: Metadatos del sistema de archivos
   - Número de bloques para inodos y datos
   - Ubicación de la tabla de inodos
   - Magic number (tipo de sistema de archivos)

2. **Tabla de Inodos**: Almacena los inodos
   - Tamaño típico del inodo: 128 o 256 bytes
   - Un bloque de 4KB puede almacenar:
     - 32 inodos de 128 bytes
     - 16 inodos de 256 bytes

3. **Región de Datos**: Donde se almacenan los datos reales de los archivos

#### **Inodos**
Estructura que contiene la **metadata** de un archivo:
- Permisos (mode)
- Propietario (uid)
- Tamaño del archivo
- Timestamps (atime, ctime, mtime)
- **link-count**: Contador de referencias (hard links)
- Punteros a bloques de datos

#### **Links en el Sistema de Archivos**

**Hard Link**:
- Crea una nueva entrada de directorio con el **mismo inodo**
- Aumenta el contador de referencias (link-count)
- El archivo existe mientras link-count > 0
- `unlink()` disminuye el contador en 1

**Soft Link (Symbolic Link)**:
- Guarda solo la **ruta** al archivo original
- Si se elimina el original, el link queda roto
- Tiene su propio inodo

#### **Operación unlink()**
Cuando se hace unlink sobre un archivo:
1. Se disminuye el contador de referencias
2. Se elimina la entrada del directorio
3. Si link-count llega a 0:
   - Se liberan los bloques de datos
   - Se libera el inodo

#### **Acceso a Archivos - Path Resolution**
Para leer `/usr/a.out`, el SO debe:
1. Leer el inodo raíz (/)
2. Leer los bloques de datos de / para encontrar "usr"
3. Leer el inodo de usr
4. Leer los bloques de usr para encontrar "a.out"
5. Leer el inodo de a.out
6. Leer los bloques de datos de a.out

---

## 💡 EJERCICIOS DE PRÁCTICA

### **PARTE I: Preguntas Conceptuales**

1. **¿Cuál es la diferencia entre el dirty bit y el use bit?**
   
2. **En un sistema con páginas de 8KB, ¿cuál sería la fragmentación interna máxima posible?**

3. **Si tengo un espacio de direcciones de 32 bits con páginas de 4KB:**
   - ¿Cuántos bits necesito para el offset?
   - ¿Cuántos bits para el VPN?

4. **Un archivo tiene un link-count = 5. Si hago 3 operaciones unlink(), ¿qué pasa con:**
   - a) El contador de referencias
   - b) Los bloques de datos
   - c) El inodo

5. **¿Por qué un soft link puede quedar "roto" pero un hard link no?**

---

### **PARTE II: Problemas de Cálculo**

#### **Problema 1: Traducción de Direcciones**
Sistema con:
- Espacio de direcciones: 20 bits
- Tamaño de página: 2KB
- Page Directory Index: 5 bits
- Page Table Index: 4 bits

Para la dirección `0x3A5C8`:
a) Convierta a binario
b) Identifique el Page Directory Index
c) Identifique el Page Table Index  
d) Calcule el offset

#### **Problema 2: Cálculo de Inodos**
Dado:
- Tamaño del inodo: 256 bytes
- Tamaño del bloque: 4KB
- Necesito almacenar 50,000 archivos

¿Cuántos bloques necesito para la tabla de inodos?

#### **Problema 3: Fragmentación**
Tengo 3 archivos:
- Archivo A: 3.2KB
- Archivo B: 7.8KB
- Archivo C: 1.5KB

Con bloques de 4KB:
a) ¿Cuánta fragmentación interna total hay?
b) ¿Cuántos bloques necesito en total?

#### **Problema 4: Path Resolution**
Para acceder al archivo `/home/user/docs/file.txt`:
a) Liste todas las operaciones de lectura de inodos necesarias
b) ¿Cuántas lecturas de bloques de datos de directorios se necesitan?

---

### **PARTE III: Preguntas de Desarrollo**

1. **Explique por qué la virtualización de memoria mejora la seguridad del sistema operativo.**
   
   *Pistas: Aislamiento de procesos, protección de memoria, permisos por página*

2. **Si debe implementar un sistema de archivos donde TODOS los archivos pesan exactamente 1MB:**
   - ¿Qué tamaño de bloque elegiría y por qué?
   - ¿Qué ventajas y desventajas tendría su elección?
   
   *Considere: fragmentación, número de punteros necesarios, overhead de metadata*

3. **Compare las ventajas y desventajas de usar bloques de 512 bytes vs 64KB para un sistema de archivos general.**

---

### **PARTE IV: Ejercicios Tipo Examen**

#### **Ejercicio 1**
Sistema con:
- RAM: 512 MB
- Espacio virtual: 8 GB  
- Tamaño de página: 2KB

Calcule:
a) Bits para el offset
b) Bits para el VPN
c) Número máximo de páginas virtuales
d) Tamaño de una page table con entradas de 4 bytes

#### **Ejercicio 2**
Un inodo de 128 bytes contiene 12 punteros directos, 1 indirecto simple, 1 indirecto doble y 1 indirecto triple. Con bloques de 4KB y punteros de 4 bytes:

a) ¿Cuál es el tamaño máximo de archivo que se puede representar?
b) ¿Cuántos bloques de indirección se necesitan para un archivo de 100MB?

#### **Ejercicio 3**
En un sistema de archivos:
- Bloques totales: 1,000,000
- Bloques para inodos: 10,000
- Tamaño del bloque: 8KB
- Tamaño del inodo: 256 bytes

a) ¿Cuántos archivos máximo puedo tener?
b) ¿Cuál es la capacidad máxima de almacenamiento de datos?
c) Si cada archivo usa en promedio 3 bloques, ¿cuál es la ocupación máxima?

---

## 📝 RESPUESTAS BREVES A EJERCICIOS CLAVE

### Problema 1 - Traducción:
- 2KB = 2^11 → Offset: 11 bits
- 0x3A5C8 = 0011 1010 0101 1100 1000
- Page Directory: 00111 (7)
- Page Table: 0100 (4)
- Offset: 101 1100 1000 (0x5C8)

### Problema 2 - Inodos:
- Inodos por bloque: 4096/256 = 16
- Bloques necesarios: 50,000/16 = 3,125 bloques

### Problema 3 - Fragmentación:
- A: usa 1 bloque, fragmentación = 0.8KB
- B: usa 2 bloques, fragmentación = 0.2KB
- C: usa 1 bloque, fragmentación = 2.5KB
- Total: 3.5KB de fragmentación interna

---

## 🎯 CONCEPTOS CLAVE PARA EL EXAMEN

1. **Siempre recuerde**: 
   - Dirty bit = modificación
   - Fragmentación interna = dentro del bloque
   - Hard link comparte inodo, soft link no

2. **Para cálculos de direcciones**:
   - Tamaño página = 2^n → offset = n bits
   - VPN = bits totales - offset

3. **Para sistemas de archivos**:
   - Inodos contienen metadata, NO datos
   - Path resolution siempre empieza desde /
   - unlink() no siempre borra el archivo

4. **Seguridad y virtualización**:
   - Cada proceso tiene su espacio de direcciones
   - El SO controla las traducciones
   - Protección a nivel de página

---

*¡Éxito en tu examen! Recuerda practicar los cálculos y entender los conceptos, no solo memorizarlos.*
