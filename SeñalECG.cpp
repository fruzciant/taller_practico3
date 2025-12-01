#include "SeñalECG.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

// Constructor - inicializa listas vacías
SeñalECG::SeñalECG() 
    : cabeza(nullptr), cola(nullptr), tamanio(0),
      cabezaFiltrada(nullptr), colaFiltrada(nullptr) {}

// Destructor - libera la memoria asignada dinámicamente
SeñalECG::~SeñalECG() {
    limpiarLista(cabeza, cola);
    limpiarLista(cabezaFiltrada, colaFiltrada);
}

// Método auxiliar para limpiar una lista
void SeñalECG::limpiarLista(Nodo*& cabeza, Nodo*& cola) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    cabeza = nullptr;
    cola = nullptr;
}

// Cargar la señal desde el archivo de texto
bool SeñalECG::cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        return false;
    }
    
    std::string linea;
    
    // Leer y saltar la primera línea (cabecera)
    if (!std::getline(archivo, linea)) {
        std::cerr << "Error: Archivo vacío" << std::endl;
        return false;
    }
    
    // Leer los datos
    while (std::getline(archivo, linea)) {
        // Eliminar el \r si existe (archivos Windows)
        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }
        
        // Saltar líneas vacías
        if (linea.empty()) {
            continue;
        }
        
        std::stringstream ss(linea);
        std::string amplitudStr, tiempoStr;
        
        if (std::getline(ss, amplitudStr, ',') && std::getline(ss, tiempoStr)) {
            try {
                double amplitud = std::stod(amplitudStr);
                double tiempo = std::stod(tiempoStr);
                
                // Crear nuevo nodo e insertar al final
                Nodo* nuevoNodo = new Nodo(amplitud, tiempo);
                
                if (cabeza == nullptr) {
                    cabeza = cola = nuevoNodo;
                } else {
                    cola->siguiente = nuevoNodo;
                    nuevoNodo->anterior = cola;
                    cola = nuevoNodo;
                }
                
                tamanio++;
            } catch (const std::exception& e) {
                std::cerr << "Error al procesar línea: " << linea << std::endl;
            }
        }
    }
    
    archivo.close();
    std::cout << "✓ Cargados " << tamanio << " puntos de la señal ECG" << std::endl;
    return true;
}

// Filtro digital pasa bajos usando ecuaciones de diferencias
// Implementación de un filtro de promedio móvil exponencial (EMA)
// y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
void SeñalECG::aplicarFiltroPasaBajos(double alpha) {
    if (cabeza == nullptr) {
        std::cerr << "Error: No hay señal cargada" << std::endl;
        return;
    }
    
    // Limpiar señal filtrada anterior si existe
    limpiarLista(cabezaFiltrada, colaFiltrada);
    
    Nodo* actual = cabeza;
    double yAnterior = actual->amplitud; // Inicializar con el primer valor
    
    while (actual != nullptr) {
        // Aplicar ecuación de diferencias del filtro
        double yActual = alpha * actual->amplitud + (1.0 - alpha) * yAnterior;
        
        // Crear nuevo nodo para la señal filtrada
        Nodo* nuevoNodo = new Nodo(yActual, actual->tiempo);
        
        if (cabezaFiltrada == nullptr) {
            cabezaFiltrada = colaFiltrada = nuevoNodo;
        } else {
            colaFiltrada->siguiente = nuevoNodo;
            nuevoNodo->anterior = colaFiltrada;
            colaFiltrada = nuevoNodo;
        }
        
        yAnterior = yActual;
        actual = actual->siguiente;
    }
    
    std::cout << "✓ Filtro pasa bajos aplicado (alpha = " << alpha << ")" << std::endl;
}

// Detectar picos en la señal filtrada
std::vector<double> SeñalECG::detectarPicos(double umbral) {
    std::vector<double> picos;
    
    if (cabezaFiltrada == nullptr) {
        std::cerr << "Error: Debe aplicar el filtro antes de detectar picos" << std::endl;
        return picos;
    }
    
    Nodo* actual = cabezaFiltrada->siguiente; // Empezar desde el segundo nodo
    
    while (actual != nullptr && actual->siguiente != nullptr) {
        // Detectar máximo local
        if (actual->amplitud > actual->anterior->amplitud &&
            actual->amplitud > actual->siguiente->amplitud &&
            actual->amplitud > umbral) {
            picos.push_back(actual->tiempo);
        }
        actual = actual->siguiente;
    }
    
    std::cout << "✓ Detectados " << picos.size() << " picos (umbral = " << umbral << ")" << std::endl;
    return picos;
}

// Imprimir la señal original y filtrada en un archivo
void SeñalECG::imprimirEnArchivo(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo " << nombreArchivo << std::endl;
        return;
    }
    
    archivo << std::fixed << std::setprecision(6);
    
    // Escribir cabecera
    archivo << "tiempo,amplitud_original,amplitud_filtrada\n";
    
    // Recorrer ambas listas simultáneamente
    Nodo* original = cabeza;
    Nodo* filtrada = cabezaFiltrada;
    
    while (original != nullptr) {
        archivo << original->tiempo << "," << original->amplitud;
        
        if (filtrada != nullptr) {
            archivo << "," << filtrada->amplitud;
            filtrada = filtrada->siguiente;
        } else {
            archivo << ",";
        }
        
        archivo << "\n";
        original = original->siguiente;
    }
    
    archivo.close();
    std::cout << "✓ Señales guardadas en " << nombreArchivo << std::endl;
}

// Calcular la frecuencia cardíaca promedio basada en los picos detectados
double SeñalECG::calcularFrecuenciaCardiaca() {
    std::vector<double> picos = detectarPicos();
    
    if (picos.size() < 2) {
        std::cerr << "Error: Se necesitan al menos 2 picos para calcular la frecuencia cardíaca" << std::endl;
        return 0.0;
    }
    
    // Calcular los intervalos R-R (tiempo entre picos)
    std::vector<double> intervalosRR;
    for (size_t i = 1; i < picos.size(); i++) {
        intervalosRR.push_back(picos[i] - picos[i-1]);
    }
    
    // Calcular el promedio de los intervalos R-R
    double sumaIntervalos = 0.0;
    for (double intervalo : intervalosRR) {
        sumaIntervalos += intervalo;
    }
    double promedioRR = sumaIntervalos / intervalosRR.size();
    
    // Frecuencia cardíaca en latidos por minuto (BPM)
    // FC = 60 / intervalo_promedio_RR
    double frecuenciaCardiaca = 60.0 / promedioRR;
    
    std::cout << "\n=== ANÁLISIS DE FRECUENCIA CARDÍACA ===" << std::endl;
    std::cout << "Picos detectados: " << picos.size() << std::endl;
    std::cout << "Intervalo R-R promedio: " << promedioRR << " segundos" << std::endl;
    std::cout << "Frecuencia cardíaca: " << frecuenciaCardiaca << " BPM" << std::endl;
    std::cout << "======================================" << std::endl;
    
    return frecuenciaCardiaca;
}

// Imprimir estadísticas en consola
void SeñalECG::imprimirEstadisticas() const {
    if (cabeza == nullptr) {
        std::cout << "No hay señal cargada" << std::endl;
        return;
    }
    
    // Calcular estadísticas de la señal original
    double minAmp = cabeza->amplitud;
    double maxAmp = cabeza->amplitud;
    double sumaAmp = 0.0;
    int contador = 0;
    
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->amplitud < minAmp) minAmp = actual->amplitud;
        if (actual->amplitud > maxAmp) maxAmp = actual->amplitud;
        sumaAmp += actual->amplitud;
        contador++;
        actual = actual->siguiente;
    }
    
    double promedioAmp = sumaAmp / contador;
    
    std::cout << "\n=== ESTADÍSTICAS DE LA SEÑAL ===" << std::endl;
    std::cout << "Puntos totales: " << tamanio << std::endl;
    std::cout << "Amplitud mínima: " << minAmp << std::endl;
    std::cout << "Amplitud máxima: " << maxAmp << std::endl;
    std::cout << "Amplitud promedio: " << promedioAmp << std::endl;
    
    if (cabeza != nullptr && cola != nullptr) {
        std::cout << "Duración: " << cola->tiempo << " segundos" << std::endl;
    }
    
    std::cout << "================================" << std::endl;
}
