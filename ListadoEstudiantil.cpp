#include "ListadoEstudiantil.h"
#include <fstream>
#include <stdexcept>

// Constructor - inicializa una lista vacía
ListadoEstudiantil::ListadoEstudiantil() : cabeza(nullptr), tamanio(0) {}

// Destructor - libera la memoria asignada dinámicamente
ListadoEstudiantil::~ListadoEstudiantil() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->siguiente;
        delete actual->estudiante;
        delete actual;
        actual = siguiente;
    }
}

// Insertar un nuevo nodo al final de la lista
void ListadoEstudiantil::insertar(const Estudiante& estudiante) {
    Estudiante* nuevoEstudiante = new Estudiante(estudiante);
    Nodo* nuevoNodo = new Nodo(nuevoEstudiante);
    
    if (cabeza == nullptr) {
        cabeza = nuevoNodo;
    } else {
        Nodo* actual = cabeza;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }
    tamanio++;
}

// Eliminar un nodo con un valor específico (por número de identificación)
bool ListadoEstudiantil::eliminar(const std::string& numeroIdentificacion) {
    if (cabeza == nullptr) {
        return false;
    }
    
    // Caso especial: el nodo a eliminar es la cabeza
    if (cabeza->estudiante->numeroIdentificacion == numeroIdentificacion) {
        Nodo* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp->estudiante;
        delete temp;
        tamanio--;
        return true;
    }
    
    // Buscar el nodo a eliminar
    Nodo* actual = cabeza;
    while (actual->siguiente != nullptr) {
        if (actual->siguiente->estudiante->numeroIdentificacion == numeroIdentificacion) {
            Nodo* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            delete temp->estudiante;
            delete temp;
            tamanio--;
            return true;
        }
        actual = actual->siguiente;
    }
    
    return false;
}

// Buscar un valor en la lista y devolver su posición
int ListadoEstudiantil::buscar(const std::string& numeroIdentificacion) const {
    Nodo* actual = cabeza;
    int posicion = 0;
    
    while (actual != nullptr) {
        if (actual->estudiante->numeroIdentificacion == numeroIdentificacion) {
            return posicion;
        }
        actual = actual->siguiente;
        posicion++;
    }
    
    return -1; // No encontrado
}

// Imprimir todos los elementos de la lista en un archivo de texto
void ListadoEstudiantil::imprimirEnArchivo(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo para escritura");
    }
    
    archivo << "========================================\n";
    archivo << "     LISTADO DE ESTUDIANTES\n";
    archivo << "========================================\n\n";
    
    Nodo* actual = cabeza;
    int contador = 1;
    
    while (actual != nullptr) {
        archivo << "Estudiante #" << contador << "\n";
        archivo << "----------------------------------------\n";
        archivo << "Nombre Completo: " << actual->estudiante->nombreCompleto << "\n";
        archivo << "Año de Nacimiento: " << actual->estudiante->anioNacimiento << "\n";
        archivo << "Número de Identificación: " << actual->estudiante->numeroIdentificacion << "\n";
        archivo << "Promedio Académico: " << actual->estudiante->promedioAcademico << "\n";
        archivo << "Email: " << actual->estudiante->email << "\n\n";
        
        actual = actual->siguiente;
        contador++;
    }
    
    archivo << "========================================\n";
    archivo << "Total de estudiantes: " << tamanio << "\n";
    archivo << "========================================\n";
    
    archivo.close();
}

// Invertir la lista enlazada
void ListadoEstudiantil::invertir() {
    if (cabeza == nullptr || cabeza->siguiente == nullptr) {
        return; // Lista vacía o con un solo elemento
    }
    
    Nodo* anterior = nullptr;
    Nodo* actual = cabeza;
    Nodo* siguiente = nullptr;
    
    while (actual != nullptr) {
        siguiente = actual->siguiente;
        actual->siguiente = anterior;
        anterior = actual;
        actual = siguiente;
    }
    
    cabeza = anterior;
}

// Sobrecarga del operador [] para acceso por índice
Estudiante& ListadoEstudiantil::operator[](int indice) {
    if (indice < 0 || indice >= tamanio) {
        throw std::out_of_range("Índice fuera de rango");
    }
    
    Nodo* actual = cabeza;
    for (int i = 0; i < indice; i++) {
        actual = actual->siguiente;
    }
    
    return *(actual->estudiante);
}

const Estudiante& ListadoEstudiantil::operator[](int indice) const {
    if (indice < 0 || indice >= tamanio) {
        throw std::out_of_range("Índice fuera de rango");
    }
    
    Nodo* actual = cabeza;
    for (int i = 0; i < indice; i++) {
        actual = actual->siguiente;
    }
    
    return *(actual->estudiante);
}

// Imprimir en consola
void ListadoEstudiantil::imprimir() const {
    std::cout << "\n========================================\n";
    std::cout << "     LISTADO DE ESTUDIANTES\n";
    std::cout << "========================================\n\n";
    
    Nodo* actual = cabeza;
    int contador = 1;
    
    while (actual != nullptr) {
        std::cout << "Estudiante #" << contador << "\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Nombre: " << actual->estudiante->nombreCompleto << "\n";
        std::cout << "Año de Nacimiento: " << actual->estudiante->anioNacimiento << "\n";
        std::cout << "ID: " << actual->estudiante->numeroIdentificacion << "\n";
        std::cout << "Promedio: " << actual->estudiante->promedioAcademico << "\n";
        std::cout << "Email: " << actual->estudiante->email << "\n\n";
        
        actual = actual->siguiente;
        contador++;
    }
    
    std::cout << "========================================\n";
    std::cout << "Total: " << tamanio << " estudiantes\n";
    std::cout << "========================================\n\n";
}
