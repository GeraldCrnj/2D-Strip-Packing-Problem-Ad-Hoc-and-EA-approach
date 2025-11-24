import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns # Importamos seaborn aunque no se use en el bloque activo por si acaso

# --- CONFIGURACIÓN ---
# Nombres de tus 5 instancias (tal como están en los nombres de archivo)
INSTANCIAS_ORDENADAS = ["1", "3", "7", "8", "10"]

# Patrones de nombres de tus archivos
# Nota: Asegúrate que la ruta sea correcta desde donde ejecutas el script
FILE_PATTERN_BOX = "./Datos experimentos/boxplot/resultados_boxplot{}.csv" 
FILE_PATTERN_CONV = "./Datos experimentos/Convergencia/curva_{}.csv"

# --- FUNCIÓN DE LECTURA ROBUSTA (Para evitar errores de UTF-16/PowerShell) ---
def leer_csv_inteligente(ruta, tiene_encabezado=False):
    """Intenta leer el archivo probando UTF-8 y UTF-16"""
    encodings = ['utf-8', 'utf-16', 'utf-16-le', 'cp1252']
    
    for enc in encodings:
        try:
            header_param = 0 if tiene_encabezado else None
            # sep=None permite que pandas detecte si es coma o espacio
            df = pd.read_csv(ruta, header=header_param, encoding=enc, sep=None, engine='python')
            return df
        except Exception:
            continue
            
    print(f"⚠️ Error: No se pudo leer {ruta}")
    return None

# ---------------------------------------------------------

# 1. GENERAR BOXPLOT COMPARATIVO (Bloque comentado por ahora)
'''
print("Generando Boxplot...")
data_por_instancia = []
etiquetas_finales = []

for inst in INSTANCIAS_ORDENADAS:
    nombre_archivo = FILE_PATTERN_BOX.format(inst)
    
    # Usamos la función robusta
    # Asumimos que el boxplot TIENE encabezado ("Altura")
    df = leer_csv_inteligente(nombre_archivo, tiene_encabezado=True)
    
    if df is not None and not df.empty:
        # Tomamos la última columna por seguridad (si hay índices extra)
        valores = df.iloc[:, -1].values
        data_por_instancia.append(valores)
        etiquetas_finales.append("BENG" + inst) # Agregamos prefijo para que se vea bien
    else:
        print(f"Saltando Boxplot {inst}")

if data_por_instancia:
    plt.figure(figsize=(12, 6))
    plt.boxplot(data_por_instancia, labels=etiquetas_finales, patch_artist=True, widths=0.6,
                boxprops=dict(facecolor="lightblue", color="blue"),
                medianprops=dict(color="red", linewidth=1.5))
                
    plt.title('Estabilidad del Algoritmo por Instancia (30 Ejecuciones)')
    plt.ylabel('Altura Final (H)')
    plt.xlabel('Instancia')
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.savefig('grafico_boxplot_comparativo.png', dpi=300)
    print("✅ Guardado: grafico_boxplot_comparativo.png")
    plt.show()
else:
    print("❌ No hay datos para el Boxplot.")
'''

# 2. GENERAR CURVAS DE CONVERGENCIA (Ordenado)
print("Generando Curvas de Convergencia...")
plt.figure(figsize=(12, 6))
hay_datos = False

for inst in INSTANCIAS_ORDENADAS:
    nombre_archivo = FILE_PATTERN_CONV.format(inst)
    
    # Usamos la función robusta
    # Asumimos que convergencia NO tiene encabezado (o lo limpiamos)
    df = leer_csv_inteligente(nombre_archivo, tiene_encabezado=False)
    
    if df is not None and not df.empty:
        # Limpieza extra: si la primera fila es texto, la quitamos
        if isinstance(df.iloc[0,0], str) and not str(df.iloc[0,0]).replace('.','',1).isdigit():
             df = df.iloc[1:]
             
        # Convertir a numérico por si acaso
        df = df.apply(pd.to_numeric, errors='coerce').dropna()

        # Asumimos Col 0: Generación, Col 1: Altura
        if df.shape[1] >= 2:
            generaciones = df.iloc[:, 0]
            alturas = df.iloc[:, 1]
            
            # Graficar
            plt.plot(generaciones, alturas, label="BENG0" + inst, linewidth=2)
            hay_datos = True
        else:
            print(f"⚠️ El archivo {nombre_archivo} no tiene 2 columnas.")
    else:
        print(f"Saltando Convergencia {inst}")

if hay_datos:
    plt.title('Curvas de Convergencia por Instancia')
    plt.xlabel('Generaciones')
    plt.ylabel('Mejor Altura (H)')
    plt.legend(loc='upper right')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('grafico_convergencia_comparativo.png', dpi=300)
    print("✅ Guardado: grafico_convergencia_comparativo.png")
    plt.show()
else:
    print("❌ No se generó el gráfico de convergencia (sin datos válidos).")