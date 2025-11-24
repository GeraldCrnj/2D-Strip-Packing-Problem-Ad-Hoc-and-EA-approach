#!/bin/bash

# Nombre del archivo de salida
OUTPUT="./Datos experimentos/resumen_final.csv"

# Escribir cabecera del CSV
echo "Instancia,Mejor_H_Historico,Promedio_H" > $OUTPUT

# Compilar asegurando optimización
echo "Compilando..."
g++ implementacion.cpp -o implementacion -O3
if [ $? -ne 0 ]; then
    echo "Error de compilación. Abortando."
    exit 1
fi

echo "============================================"
echo " INICIANDO EXPERIMENTO MASIVO"
echo "============================================"

INSTANCIAS=("BENG01") # Agrega aquí el resto: "BENG02" "BENG03" etc.

for caso in "${INSTANCIAS[@]}"
do
    echo "--------------------------------------------"
    echo "Procesando instancia: $caso"
    
    suma=0
    mejor=999999
    validos=0
    
    # Ejecutar 30 veces
    for i in {1..30}
    do
        # 1. Ejecutamos redirigiendo stderr a stdout (2>&1) para capturar TODO
        # 2. Buscamos la línea clave
        # 3. Usamos 'tail' para asegurar que tomamos la última aparición
        # 4. Usamos awk '{print $NF}' para tomar el ÚLTIMO campo (el número), sin importar los espacios antes
        
        resultado=$(echo "$caso" | ./implementacion 2>&1 | grep "Mejor Altura Encontrada" | tail -n 1 | awk '{print $NF}')
        
        # --- SECCIÓN DE DEPURACIÓN (Para ver qué está pasando) ---
        # Si resultado está vacío, avisar
        if [ -z "$resultado" ]; then
            echo "  [ALERTA] Ejecución $i: No se capturó ningún resultado."
            continue
        fi
        # ---------------------------------------------------------

        # Sumar para el promedio
        suma=$((suma + resultado))
        validos=$((validos + 1))
        
        # Verificar si es el mejor histórico
        if [ "$resultado" -lt "$mejor" ]; then
            mejor=$resultado
        fi
    done
    
    # Calcular promedio solo si hubo ejecuciones válidas para evitar división por cero
    if [ "$validos" -gt 0 ]; then
        promedio=$((suma / validos))
    else
        promedio=0
        mejor="N/A"
    fi
    
    # Guardar en el CSV
    echo "$caso,$mejor,$promedio" >> $OUTPUT
    
    echo "Resumen $caso -> Mejor: $mejor, Promedio: $promedio"
done

echo "============================================"
echo " ¡TERMINADO! Revisa el archivo $OUTPUT"
echo "============================================"