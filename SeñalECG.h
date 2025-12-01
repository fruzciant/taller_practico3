#ifndef SENAL_ECG_H
#define SENAL_ECG_H

#include <string>
#include <vector>

// Clase SeñalECG - Lista doblemente enlazada para procesamiento de señales ECG
class SeñalECG {
private:
    // Nodo de la lista doblemente enlazada
    struct Nodo {
        double amplitud;
        double tiempo;
        Nodo* anterior;
        Nodo* siguiente;
        
        Nodo(double amp, double t) 
            : amplitud(amp), tiempo(t), anterior(nullptr), siguiente(nullptr) {}
    };
    
    Nodo* cabeza;
    Nodo* cola;
    int tamanio;
    
    // Lista enlazada para la señal filtrada
    Nodo* cabezaFiltrada;
    Nodo* colaFiltrada;
    
    // Métodos auxiliares privados
    void limpiarLista(Nodo*& cabeza, Nodo*& cola);
    
public:
    // Constructor
    SeñalECG();
    
    // Destructor
    ~SeñalECG();
    
    // Método para cargar la señal desde el archivo de texto
    bool cargarDesdeArchivo(const std::string& nombreArchivo);
    
    // Filtro digital pasa bajos en ecuaciones de diferencias
    void aplicarFiltroPasaBajos(double alpha = 0.1);
    
    // Método para detectar picos en la señal filtrada
    std::vector<double> detectarPicos(double umbral = 50.0);
    
    // Método para imprimir la señal original y filtrada en un archivo
    void imprimirEnArchivo(const std::string& nombreArchivo) const;
    
    // Método para calcular la frecuencia cardíaca promedio
    double calcularFrecuenciaCardiaca();
    
    // Métodos auxiliares
    int obtenerTamanio() const { return tamanio; }
    bool estaVacia() const { return cabeza == nullptr; }
    
    // Método para imprimir estadísticas en consola
    void imprimirEstadisticas() const;
};

#endif // SENAL_ECG_H
