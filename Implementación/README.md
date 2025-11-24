# Solución al Problema de Corte y Empaquetamiento 2D (2DSPP) mediante Algoritmos Evolutivos

## 👩‍💻 Autora

**Geraldine Cornejo**   Rol:202173529-1

Proyecto para la asignatura de Inteligencia Artificial.

Este repositorio contiene la implementación de un Algoritmo Evolutivo (AE) diseñado para resolver el problema de optimización combinatoria conocido como *Two-Dimensional Strip Packing Problem* (2DSPP).

El núcleo del proyecto es una implementación eficiente en C++ que utiliza una representación indirecta "ad-hoc" (vectores de orden y rotación) y una decodificación constructiva (heurística de estantes) para minimizar la altura de empaquetado garantizando la factibilidad de las soluciones. Además, se incluyen scripts auxiliares en Bash y Python para la automatización de experimentos y la visualización de resultados.

## 📋 Tabla de Contenidos

1.  Tecnologías Utilizadas y Requisitos
2.  Estructura del Proyecto
3.  Implementación Principal (C++)
      - Compilación
      - Ejecución Individual
4.  Reproducción de Experimentos y Resultados
      - Generación de Datos (Bash)
      - Visualización Gráfica (Python)

-----

## 🛠️ Tecnologías Utilizadas y Requisitos

El proyecto ha sido desarrollado y probado en un entorno **Linux**. Se requieren las siguientes herramientas y librerías para su correcta ejecución:

### Entorno Base

  * **Sistema Operativo:** Linux (o subsistema compatible como WSL en Windows).
  * **Terminal:** Bash.

### Núcleo (Algoritmo Evolutivo)

  * **Compilador C++:** `g++` con soporte para el estándar **C++17** o superior.
  * **Herramienta de construcción:** `make`.

### Análisis y Visualización de Resultados

  * **Python 3.x**
  * **Librerías de Python:** Se deben instalar las siguientes librerías para ejecutar el script de gráficos:
    ```bash
    pip install pandas matplotlib seaborn
    ```

-----

## 📂 Estructura del Proyecto

```
.
├── implementacion.cpp      # CÓDIGO FUENTE PRINCIPAL: Implementación del AE en C++
├── MAKEFILE                # Archivo para automatizar compilación y ejecución
├── CasosDePrueba/          # Carpeta con instancias de prueba (ej: BENG01.txt)
│
├── experimento_completo.sh # Script Bash para ejecutar el benchmark completo (30 corridas x N instancias)
├── generar_boxplot.sh      # Script Bash para generar datos de estabilidad (30 corridas x 1 instancia)
│
├── generarGraficos.py      # Script Python para generar Boxplots y Curvas de Convergencia
├── Datos experimentos/     # Carpeta donde los scripts guardan los CSV resultantes
│   ├── boxplot/
│   └── Convergencia/
│
└── README.md               # Este archivo
```

-----

## 🚀 Implementación Principal (C++)

El archivo `implementacion.cpp` contiene toda la lógica del algoritmo evolutivo: la representación de los individuos, los operadores genéticos (selección por ruleta, cruzamiento OX, mutación swap/bit-flip) y la función de evaluación (decodificador).

### Compilación

Para facilitar la compilación se incluye un `MAKEFILE`. Desde la terminal, en la raíz del proyecto, ejecute:

```bash
make
```

Este comando generará un ejecutable optimizado (`-O3`) llamado `implementacion`.

*Si desea limpiar los archivos compilados anteriormente, puede usar `make clean`.*

### Ejecución Individual

Una vez compilado, puede ejecutar el programa para resolver una instancia específica de la carpeta `CasosDePrueba/`. El programa solicitará el nombre de la instancia por entrada estándar.

Puede ejecutarlo interactivamente usando `make`:

```bash
make run
# El programa pedirá input. Escriba el nombre de la instancia, por ejemplo: BENG01
```

Alternativamente, puede ejecutar el binario directamente y pasarle el nombre de la instancia mediante una tubería (`echo`), lo cual es útil para automatización:

```bash
echo "BENG01" | ./implementacion
```

-----

## 📈 Reproducción de Experimentos y Resultados

Esta sección describe cómo utilizar los scripts auxiliares para reproducir los datos y gráficos presentados en el informe del proyecto. Estos scripts automatizan las ejecuciones múltiples del núcleo en C++.

> **Nota previa:** Asegúrese de dar permisos de ejecución a los scripts de Bash antes de usarlos:
>
> ```bash
> chmod +x experimento_completo.sh generar_boxplot.sh
> ```

### 1\. Generación de Datos (Bash)

Estos scripts ejecutan el binario `implementacion` repetidamente y guardan los resultados en formato CSV en la carpeta `Datos experimentos/`. Asegúrese de haber compilado el programa C++ primero.

  * **Experimento Masivo (Tabla Resumen):**
    Ejecuta 30 veces cada una de las instancias seleccionadas para calcular estadísticas (mejor, promedio, desviación estándar).

    ```bash
    ./experimento_completo.sh
    ```

    *Salida:* Genera un archivo resumen (ej. `resumen_final.csv`).

  * **Datos para Boxplot (Estabilidad):**
    Ejecuta 30 veces una instancia específica y guarda las alturas finales para analizar la dispersión.

    ```bash
    ./generar_boxplot.sh
    ```

    *Salida:* Guarda archivos en `Datos experimentos/boxplot/`.

### 2\. Visualización Gráfica (Python)

Una vez generados los datos CSV en las carpetas correspondientes dentro de `Datos experimentos/`, utilice el script de Python para generar gráficos profesionales (Boxplots comparativos y Curvas de Convergencia).

Asegúrese de tener instaladas las librerías requeridas (`pandas`, `matplotlib`, `seaborn`) y ejecute:

```bash
python3 generarGraficos.py
```

El script leerá los datos CSV y generará imágenes PNG con los gráficos comparativos.

-----