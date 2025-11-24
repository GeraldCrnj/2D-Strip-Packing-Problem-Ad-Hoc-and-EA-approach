/*
 * IMPLEMENTACIÓN MÍNIMA: 2D STRIP PACKING (2DSPP)
 * TÉCNICA: ALGORITMO EVOLUTIVO (AE)
 * AUTORA: Geraldine Cornejo
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>

using namespace std;

// --- Parámetros del Algoritmo ---
const int POBLACION_SIZE = 50;
const int MAX_GENERACIONES = 1000;
const double PROB_CRUZAMIENTO = 0.8;
const double PROB_MUTACION = 0.1;

// --- GENERADOR ALEATORIO GLOBAL (Para evitar problemas de aleatoriedad) ---
// Se inicializa en el main con una semilla aleatoria real
mt19937 rng; 

// --- 1. Lectura de Instancia ---
struct Rectangulo {
    int id;
    int w;
    int h;
};

struct Instancia {
    int W;
    int n;
    vector<Rectangulo> piezas;
};

Instancia leerInstancia(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        throw runtime_error("Error: No se pudo abrir el archivo " + nombreArchivo);
    }
    
    Instancia inst;
    archivo >> inst.n;   // Primero lee número de piezas
    archivo >> inst.W;   // Segundo lee el ancho del strip
    
    int area = 0;
    for (int i = 0; i < inst.n; ++i) {
        Rectangulo p;
        archivo >> p.id >> p.w >> p.h;
        inst.piezas.push_back(p);
        // cout << "Pieza leida: " << p.w << "x" << p.h << endl; // Debug opcional
        area += p.w * p.h;
    }
    // cout << "Area total de piezas: " << area << endl;
    archivo.close();
    return inst;
}

// --- 2. Generación de Solución Inicial ---
struct Solucion {
    vector<int> orden_piezas;
    vector<bool> rotaciones;
};

Solucion generarSolucionInicial(const Instancia& inst) {
    Solucion sol;
    sol.orden_piezas.resize(inst.n);
    sol.rotaciones.resize(inst.n);

    // 1. Generar orden aleatorio
    iota(sol.orden_piezas.begin(), sol.orden_piezas.end(), 0);
    shuffle(sol.orden_piezas.begin(), sol.orden_piezas.end(), rng); // Usamos el generador global

    // 2. Generar rotaciones aleatorias
    uniform_int_distribution<int> distBool(0, 1);
    for (int i = 0; i < inst.n; ++i) {
        sol.rotaciones[i] = distBool(rng); // 0 o 1 (false o true)
    }
    return sol;
}

// --- 3. Cálculo de Función de Evaluación ---
double calcularAlturaTotal(const Solucion& sol, const Instancia& inst) {
    double altura_total_H = 0.0;
    double altura_estante_actual = 0.0;
    double ancho_estante_actual = 0.0;

    for (int i = 0; i < inst.n; ++i) {
        int indicePieza = sol.orden_piezas[i];
        bool rotar = sol.rotaciones[indicePieza];
        const Rectangulo& piezaOriginal = inst.piezas[indicePieza];

        int w_actual = rotar ? piezaOriginal.h : piezaOriginal.w;
        int h_actual = rotar ? piezaOriginal.w : piezaOriginal.h;

        // Algoritmo Shelf (Next Fit)
        if (ancho_estante_actual + w_actual <= inst.W) {
            // Cabe en el estante actual
            ancho_estante_actual += w_actual;
            altura_estante_actual = max(altura_estante_actual, (double)h_actual);
        } else {
            // No cabe, cerrar estante y abrir uno nuevo
            altura_total_H += altura_estante_actual; // CORREGIDO: faltaba el +=
            
            // Nuevo estante con la pieza actual
            ancho_estante_actual = w_actual;
            altura_estante_actual = h_actual;
        }
    }
    // Sumar la altura del último estante
    altura_total_H += altura_estante_actual;
    return altura_total_H;
}

// --- 4. Operador de Selección (Ruleta) ---
int seleccionRuleta(const vector<double>& fitness_acumulado) {
    uniform_real_distribution<double> dist(0.0, fitness_acumulado.back());
    double r = dist(rng);
    
    auto it = upper_bound(fitness_acumulado.begin(), fitness_acumulado.end(), r);
    return distance(fitness_acumulado.begin(), it);
}

// --- 5. Operador de cruzamiento (OX + Uniforme) ---
Solucion cruzamiento(const Solucion& p1, const Solucion& p2) {
    int n = p1.orden_piezas.size();
    Solucion hijo;
    hijo.orden_piezas.resize(n);
    hijo.rotaciones.resize(n);

    // 1. Order Crossover (OX) para el orden
    uniform_int_distribution<int> distIndex(0, n - 1);
    int corte1 = distIndex(rng);
    int corte2 = distIndex(rng);
    if (corte1 > corte2) swap(corte1, corte2);

    // Copiar segmento del Padre 1
    vector<bool> presente_en_hijo(n, false);
    for (int i = corte1; i <= corte2; ++i) {
        int pieza = p1.orden_piezas[i];
        hijo.orden_piezas[i] = pieza;
        presente_en_hijo[pieza] = true;
    }

    // Rellenar el resto con el orden del Padre 2
    int indice_hijo = (corte2 + 1) % n;
    int indice_p2 = (corte2 + 1) % n;
    
    for (int count = 0; count < n; ++count) {
        int pieza = p2.orden_piezas[indice_p2];
        if (!presente_en_hijo[pieza]) {
            // Buscar posición libre
            while (indice_hijo >= corte1 && indice_hijo <= corte2) {
                indice_hijo = (indice_hijo + 1) % n;
            }
            hijo.orden_piezas[indice_hijo] = pieza;
            indice_hijo = (indice_hijo + 1) % n;
        }
        indice_p2 = (indice_p2 + 1) % n;
    }

    // 2. Cruce Uniforme para rotaciones
    uniform_int_distribution<int> distBool(0, 1);
    for (int i = 0; i < n; ++i) {
        if (distBool(rng) == 0) hijo.rotaciones[i] = p1.rotaciones[i];
        else                    hijo.rotaciones[i] = p2.rotaciones[i];
    }
    return hijo;
}

// --- 6. Operador de Mutación (Swap + Bit Flip) ---
void mutacion(Solucion& sol) {
    int n = sol.orden_piezas.size();
    uniform_int_distribution<int> distIndex(0, n - 1);

    // 1. Mutación Swap en el orden
    int i = distIndex(rng);
    int j = distIndex(rng);
    swap(sol.orden_piezas[i], sol.orden_piezas[j]);

    // 2. Mutación Bit-Flip en rotación
    int k = distIndex(rng);
    sol.rotaciones[k] = !sol.rotaciones[k]; 
}

// --- Programa Principal ---
int main() {
    // Inicializar generador aleatorio una sola vez
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed(seed);

    try {
        // 1. LECTURA DE INSTANCIA
        string nombreInstancia;
        cerr << "Ingrese el nombre de la instancia (ej: BENG01): ";
        cin >> nombreInstancia;

        string rutaCompleta = "./CasosDePrueba/" + nombreInstancia + ".txt";
        Instancia instancia = leerInstancia(rutaCompleta);
        
        cerr << "--- 1. Lectura de Instancia ---" << endl;
        cerr << "Ancho del Strip (W): " << instancia.W << endl;
        cerr << "Numero de piezas (n): " << instancia.n << endl;
        cerr << "----------------------------------" << endl;

        vector<Solucion> poblacion(POBLACION_SIZE);
        vector<double> fitness_poblacion(POBLACION_SIZE);
        
        Solucion mejor_global;
        double mejor_altura_global = 1e9; // Infinito inicial

        // Generar población inicial
        for(int i=0; i<POBLACION_SIZE; ++i) {
            poblacion[i] = generarSolucionInicial(instancia);
            double altura = calcularAlturaTotal(poblacion[i], instancia);
            fitness_poblacion[i] = 1.0 / altura; // Fitness para maximizar

            if(altura < mejor_altura_global) {
                mejor_altura_global = altura;
                mejor_global = poblacion[i];
            }
        }

        // --- Ciclo Evolutivo ---
        uniform_real_distribution<double> distProb(0.0, 1.0);

        for (int generacion = 0; generacion < MAX_GENERACIONES; ++generacion) {
            vector<Solucion> nueva_poblacion;
            
            // Elitismo: el mejor pasa directo
            int idx_mejor = 0;
            for(int i=1; i<POBLACION_SIZE; ++i) {
                if(fitness_poblacion[i] > fitness_poblacion[idx_mejor]) idx_mejor = i;
            }
            nueva_poblacion.push_back(poblacion[idx_mejor]);

            // Preparar Ruleta (Fitness Acumulado)
            vector<double> fitness_acumulado(POBLACION_SIZE);
            fitness_acumulado[0] = fitness_poblacion[0];
            for(int i=1; i<POBLACION_SIZE; ++i) {
                fitness_acumulado[i] = fitness_acumulado[i-1] + fitness_poblacion[i];
            }

            // Llenar nueva población
            while(nueva_poblacion.size() < POBLACION_SIZE) {
                int idx_p1 = seleccionRuleta(fitness_acumulado);
                int idx_p2 = seleccionRuleta(fitness_acumulado);

                Solucion hijo;

                // Cruzamiento
                if (distProb(rng) < PROB_CRUZAMIENTO) {
                    hijo = cruzamiento(poblacion[idx_p1], poblacion[idx_p2]);
                } else {
                    hijo = poblacion[idx_p1];
                }

                // Mutación
                if (distProb(rng) < PROB_MUTACION) {
                    mutacion(hijo);
                }

                nueva_poblacion.push_back(hijo);
            }

            // Reemplazo y Evaluación
            poblacion = nueva_poblacion;
            for(int i=0; i<POBLACION_SIZE; ++i) {
                double altura = calcularAlturaTotal(poblacion[i], instancia);
                fitness_poblacion[i] = 1.0 / altura;

                if(altura < mejor_altura_global) {
                    mejor_altura_global = altura;
                    mejor_global = poblacion[i];
                }
            }
            // Print utilizado para obtener datos, mediante: ./implementacion.exe < curva_x.csv
            //cout << generacion << "," << mejor_altura_global << endl;
            if (generacion % 100 == 0) {
                cerr << "Gen " << generacion << " - Mejor Altura: " << mejor_altura_global << endl;
                
            }
        }

        cerr << "--- FIN ---" << endl;
        cerr << "Mejor Altura Encontrada: " << mejor_altura_global << endl;

    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}