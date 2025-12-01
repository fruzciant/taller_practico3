#include "ListadoEstudiantil.h"
#include <iostream>

int main() {
    std::cout << "\n===========================================\n";
    std::cout << "  PRUEBA DE LISTADO ESTUDIANTIL\n";
    std::cout << "===========================================\n";
    
    // Crear una lista de estudiantes
    ListadoEstudiantil lista;
    
    // Insertar estudiantes
    std::cout << "\n1. Insertando estudiantes...\n";
    lista.insertar(Estudiante("Juan Carlos Pérez García", 2003, "1001", 4.2, "juan.perez@universidad.edu"));
    lista.insertar(Estudiante("María Fernanda López Martínez", 2002, "1002", 4.5, "maria.lopez@universidad.edu"));
    lista.insertar(Estudiante("Carlos Andrés Rodríguez Silva", 2003, "1003", 3.8, "carlos.rodriguez@universidad.edu"));
    lista.insertar(Estudiante("Ana Sofía González Ramírez", 2004, "1004", 4.7, "ana.gonzalez@universidad.edu"));
    lista.insertar(Estudiante("Luis Miguel Torres Herrera", 2002, "1005", 4.0, "luis.torres@universidad.edu"));
    
    std::cout << "✓ Se insertaron " << lista.obtenerTamanio() << " estudiantes\n";
    
    // Imprimir la lista en consola
    std::cout << "\n2. Lista de estudiantes:\n";
    lista.imprimir();
    
    // Buscar un estudiante
    std::cout << "\n3. Búsqueda de estudiantes:\n";
    std::string idBuscado = "1003";
    int posicion = lista.buscar(idBuscado);
    if (posicion != -1) {
        std::cout << "✓ Estudiante con ID " << idBuscado << " encontrado en posición " << posicion << "\n";
    } else {
        std::cout << "✗ Estudiante no encontrado\n";
    }
    
    // Acceder a un estudiante por índice usando operator[]
    std::cout << "\n4. Acceso por índice (operator[]):\n";
    try {
        Estudiante est = lista[2];
        std::cout << "✓ Estudiante en posición 2:\n";
        std::cout << "  Nombre: " << est.nombreCompleto << "\n";
        std::cout << "  Promedio: " << est.promedioAcademico << "\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Error: " << e.what() << "\n";
    }
    
    // Eliminar un estudiante
    std::cout << "\n5. Eliminando estudiante con ID 1003:\n";
    if (lista.eliminar("1003")) {
        std::cout << "✓ Estudiante eliminado exitosamente\n";
        std::cout << "  Tamaño actual de la lista: " << lista.obtenerTamanio() << "\n";
    } else {
        std::cout << "✗ No se pudo eliminar el estudiante\n";
    }
    
    // Imprimir después de eliminar
    std::cout << "\n6. Lista después de eliminar:\n";
    lista.imprimir();
    
    // Invertir la lista
    std::cout << "\n7. Invirtiendo la lista...\n";
    lista.invertir();
    std::cout << "✓ Lista invertida\n";
    
    // Imprimir después de invertir
    std::cout << "\n8. Lista después de invertir:\n";
    lista.imprimir();
    
    // Guardar en archivo
    std::cout << "\n9. Guardando lista en archivo...\n";
    try {
        lista.imprimirEnArchivo("listado_estudiantes.txt");
        std::cout << "✓ Lista guardada en 'listado_estudiantes.txt'\n";
    } catch (const std::exception& e) {
        std::cout << "✗ Error al guardar: " << e.what() << "\n";
    }
    
    std::cout << "\n===========================================\n";
    std::cout << "  PRUEBA COMPLETADA\n";
    std::cout << "===========================================\n\n";
    
    return 0;
}
