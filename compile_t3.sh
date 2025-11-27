#!/bin/bash
# Script para compilar XV6 con la Tarea 3

echo "Compilando XV6 con protección de lectura..."
echo "========================================="

# Limpiar compilación anterior
make clean

# Compilar el kernel y programas de usuario
make

if [ $? -eq 0 ]; then
    echo ""
    echo "Compilación exitosa!"
    echo ""
    echo "Para probar la implementación:"
    echo "1. Ejecuta: make qemu"
    echo "2. En XV6, ejecuta: rdprotect_test"
    echo ""
else
    echo ""
    echo "Error en la compilación. Revisa los mensajes anteriores."
fi
