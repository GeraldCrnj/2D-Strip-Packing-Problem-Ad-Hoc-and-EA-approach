/*
 * IMPLEMENTACIÓN MÍNIMA: 2D STRIP PACKING (2DSPP)
 * TÉCNICA: ALGORITMO EVOLUTIVO (AE)
 * AUTORA: Geraldine Cornejo
 * * Este archivo contiene los 3 requisitos de la implementación mínima:
 * 1. Lectura de Instancia
 * 2. Generación de Solución Inicial (el "cromosoma" ad-hoc)
 * 3. Cálculo de Función de Evaluación (el "decodificador" de fitness)
 */

#include <iostream>  
#include <fstream>   
#include <vector>   
#include <string>   
#include <stdexcept>
#include <numeric>  
#include <algorithm>
#include <random>   

using namespace std;

// --- 1. LECTURA DE INSTANCIA ---

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
    archivo >> inst.n;   
    archivo >> inst.W;
    for (int i = 0; i < inst.n; ++i) {
        Rectangulo p;
        archivo >> p.id >> p.w >> p.h;
        inst.piezas.push_back(p);
    }
    archivo.close();
    return inst;
}

// --- 2. GENERACIÓN DE SOLUCIÓN INICIAL ---

struct Solucion {
    vector<int> orden_piezas;
    vector<bool> rotaciones;
};

Solucion generarSolucionInicial(const Instancia& inst) {
    Solucion sol;
    sol.orden_piezas.resize(inst.n);
    sol.rotaciones.resize(inst.n);
    iota(sol.orden_piezas.begin(), sol.orden_piezas.end(), 0);
    random_device rd;
    mt19937 g(rd());
    shuffle(sol.orden_piezas.begin(), sol.orden_piezas.end(), g);
    for (int i = 0; i < inst.n; ++i) {
        sol.rotaciones[i] = (rand() % 2 == 0); 
    }
    return sol;
}

// --- 3. CÁLCULO DE FUNCIÓN DE EVALUACIÓN ---

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
        if (ancho_estante_actual + w_actual <= inst.W) {
            ancho_estante_actual += w_actual;
            altura_estante_actual = max(altura_estante_actual, (double)h_actual);
        } else {
            altura_total_H += altura_estante_actual;
            ancho_estante_actual = w_actual;
            altura_estante_actual = h_actual;
        }
    }
    altura_total_H += altura_estante_actual;
    return altura_total_H;
}


// --- Programa Principal ---

int main() {
    try {
        // 1. LECTURA DE INSTANCIA
        Instancia instancia = leerInstancia("./CasosDePrueba/BENG10.txt");
        cout << "--- 1. Lectura de Instancia ---" << endl;
        cout << "Instancia 'instancia.txt' leida con exito." << endl;
        cout << "Ancho del Strip (W): " << instancia.W << endl;
        cout << "Numero de piezas (n): " << instancia.n << endl;
        cout << "----------------------------------" << endl;

        // 2. GENERACIÓN DE SOLUCIÓN INICIAL
        Solucion solInicial = generarSolucionInicial(instancia);
        cout << "--- 2. Solucion Inicial Generada ---" << endl;
        cout << "Orden (indices): ";
        for(int idx : solInicial.orden_piezas) cout << idx << " ";
        cout << endl;
        cout << "Rotaciones (1=SI): ";
        for(bool r : solInicial.rotaciones) cout << r << " ";
        cout << endl;
        cout << "----------------------------------" << endl;

        // 3. CÁLCULO DE FUNCIÓN DE EVALUACIÓN
        double alturaObtenida = calcularAlturaTotal(solInicial, instancia);
        cout << "--- 3. Funcion de Evaluacion ---" << endl;
        cout << "La altura (H) de la solucion inicial es: " << alturaObtenida << endl;
        cout << "----------------------------------" << endl;

    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
