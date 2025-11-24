Resolución del Problema 2DSPP mediante Algoritmo Evolutivo
Este proyecto implementa una solución metaheurística para el problema de Corte y Empaquetamiento en 2D (Two-Dimensional Strip Packing Problem), desarrollado para la asignatura de Inteligencia Artificial.

La solución utiliza un Algoritmo Evolutivo implementado en C++ moderno, apoyado por scripts de automatización en Bash y visualización de datos en Python.

📂 Estructura del Directorio
El proyecto está organizado de la siguiente manera:

Plaintext

.
├── CasosDePrueba/           # Carpeta con las instancias de prueba (BENG01, BENG02, etc.)
├── Datos experimentos/      # Resultados organizados (Boxplots y Convergencia)
├── Latex/                   # Archivos fuente del informe final
├── __pycache__/             # Archivos temporales de Python
│
├── implementacion.cpp       # [NÚCLEO] Código fuente del Algoritmo Evolutivo (C++)
├── MAKEFILE                 # Archivo de automatización para compilar y ejecutar
│
├── generarGraficos.py       # Script Python para generar los gráficos comparativos
├── experimento_completo.sh  # Script Bash para ejecutar el benchmark completo
├── generar_boxplot.sh       # Script Bash para pruebas de estabilidad
│
├── Entrega2_InteligenciaArtificial.pdf  # Informe final del proyecto
├── grafico_boxplot_comparativo.png      # Resultado visual generado
├── grafico_convergencia_comparativo.png # Resultado visual generado
│
└── Archivos de datos sueltos (curva_*.csv, resultados_*.csv) # Datos crudos de experimentos
🛠️ Requisitos Previos
Para ejecutar este proyecto necesitarás un entorno Linux con las siguientes herramientas:

Compilador C++: g++ (con soporte C++17).

Construcción: make.

Python 3: Con las librerías pandas, matplotlib y seaborn.

Bash

pip install pandas matplotlib seaborn
🚀 Instrucciones de Uso
1. Compilación
El proyecto incluye un MAKEFILE para facilitar la compilación. Simplemente ejecuta en la terminal:

Bash

make
Esto generará el ejecutable implementacion optimizado (-O3).

2. Ejecución Individual
Para probar el algoritmo con una sola instancia (por ejemplo, BENG01 ubicada en CasosDePrueba/):

Bash

make run
# Ingresa el nombre de la instancia cuando se solicite (ej: BENG01)
O usando tuberías:

Bash

echo "BENG01" | ./implementacion
3. Reproducción de Experimentos (Scripts)
Se incluyen scripts en Bash para automatizar la recolección de datos estadísticos. Asegúrate de darles permisos de ejecución (chmod +x *.sh).

Experimento Masivo: Ejecuta todas las instancias y genera un resumen.

Bash

./experimento_completo.sh
Generar Datos de Estabilidad: Ejecuta 30 veces una instancia para el análisis de varianza.

Bash

./generar_boxplot.sh
4. Generación de Gráficos
Una vez que tengas los archivos .csv (como curva_1.csv o resultados_boxplot1.csv), ejecuta el script de visualización:

Bash

python3 generarGraficos.py
Esto procesará los datos y generará las imágenes .png correspondientes (como las que ya se observan en el directorio).

👩‍💻 Autora
Geraldine Cornejo Universidad Técnica Federico Santa María Inteligencia Artificial - 2025
