#!/bin/bash

# --- CONFIGURACIÓN ---
INSTANCIA="BENG10"
OUTPUT="resultados_boxplot.csv"
# En Windows/Git Bash, es mejor ser explícito con el .exe
EJECUTABLE="./implementacion.exe" 

# 1. COMPILAR
echo "🔨 Compilando..."
g++ implementacion.cpp -o implementacion.exe -O3

if [ ! -f "$EJECUTABLE" ]; then
    echo "❌ Error: No se creó el archivo $EJECUTABLE"
    exit 1
fi

# 2. PREPARAR CSV
echo "Altura" > $OUTPUT

echo "🚀 Iniciando 30 ejecuciones para $INSTANCIA..."

# 3. BUCLE
for i in {1..30}
do
    # A. Ejecutar y capturar TODA la salida en una variable
    #    (Usamos 2>&1 para capturar errores también)
    salida_completa=$(echo "$INSTANCIA" | $EJECUTABLE 2>&1)

    # B. Filtrar la altura
    #    grep: Busca la línea
    #    tr -d '\r': BORRA el retorno de carro de Windows (Vital en Git Bash)
    #    awk: Toma el último dato
    altura=$(echo "$salida_completa" | grep "Mejor Altura Encontrada:" | tr -d '\r' | awk '{print $NF}')

    # C. Verificar y Guardar
    if [ -z "$altura" ]; then
        echo "⚠️  Advertencia (Iter $i): No se encontró la altura. Salida del programa:"
        echo "   -> $salida_completa"
    else
        # Imprimir en pantalla para que veas que funciona
        echo "   [$i/30] Altura encontrada: $altura"
        echo "$altura" >> $OUTPUT
    fi
done

echo ""
echo "✅ Proceso terminado. Revisa $OUTPUT"