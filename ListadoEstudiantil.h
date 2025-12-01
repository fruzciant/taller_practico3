#ifndef LISTADO_ESTUDIANTIL_H
#define LISTADO_ESTUDIANTIL_H

#include <string>
#include <iostream>

// Estructura para almacenar información de estudiantes
struct Estudiante {
    std::string nombreCompleto;
    int anioNacimiento;
    std::string numeroIdentificacion;
    double promedioAcademico;
    std::string email;
    
    // Constructor
    Estudiante(std::string nombre = "", int anio = 0, std::string id = "", 
               double promedio = 0.0, std::string correo = "")
        : nombreCompleto(nombre), anioNacimiento(anio), 
          numeroIdentificacion(id), promedioAcademico(promedio), email(correo) {}
};

// Clase ListadoEstudiantil - Lista enlazada simple
class ListadoEstudiantil {
private:
    // Nodo de la lista enlazada
    struct Nodo {
        Estudiante* estudiante;
        Nodo* siguiente;
        
        Nodo(Estudiante* est) : estudiante(est), siguiente(nullptr) {}
    };
    
    Nodo* cabeza;
    int tamanio;
    
public:
    // Constructor
    ListadoEstudiantil();
    
    // Destructor
    ~ListadoEstudiantil();
    
    // Método para insertar un nuevo nodo al final de la lista
    void insertar(const Estudiante& estudiante);
    
    // Método para eliminar un nodo con un valor específico (por ID)
    bool eliminar(const std::string& numeroIdentificacion);
    
    // Método para buscar un valor en la lista y devolver su posición
    int buscar(const std::string& numeroIdentificacion) const;
    
    // Método para imprimir todos los elementos de la lista en archivo
    void imprimirEnArchivo(const std::string& nombreArchivo) const;
    
    // Método para invertir la lista enlazada
    void invertir();
    
    // Sobrecarga del operador de acceso para permitir la indexación
    Estudiante& operator[](int indice);
    const Estudiante& operator[](int indice) const;
    
    // Métodos auxiliares
    int obtenerTamanio() const { return tamanio; }
    bool estaVacia() const { return cabeza == nullptr; }
    
    // Método para imprimir en consola
    void imprimir() const;
};

#endif // LISTADO_ESTUDIANTIL_H
