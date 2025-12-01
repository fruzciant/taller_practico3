# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

# Ejecutables
EXEC_ESTUDIANTES = testListadoEstudiantil
EXEC_ECG = testECG
EXEC_DOMINO = dominoConHistorial

# Archivos fuente para cada ejercicio
ESTUDIANTES_SOURCES = ListadoEstudiantil.cpp testListadoEstudiantil.cpp
ECG_SOURCES = SeñalECG.cpp testECG.cpp
DOMINO_SOURCE = dominoConHistorial.cpp

# Archivos objeto
ESTUDIANTES_OBJECTS = $(ESTUDIANTES_SOURCES:.cpp=.o)
ECG_OBJECTS = $(ECG_SOURCES:.cpp=.o)

# Regla por defecto: compilar todo
all: $(EXEC_ESTUDIANTES) $(EXEC_ECG) $(EXEC_DOMINO)
	@echo ""
	@echo "========================================="
	@echo "  Compilación completada exitosamente"
	@echo "========================================="
	@echo "Ejecutables generados:"
	@echo "  • $(EXEC_ESTUDIANTES)"
	@echo "  • $(EXEC_ECG)"
	@echo "  • $(EXEC_DOMINO)"
	@echo ""

# Ejercicio 1: ListadoEstudiantil
$(EXEC_ESTUDIANTES): ListadoEstudiantil.o testListadoEstudiantil.o
	@echo "Enlazando $(EXEC_ESTUDIANTES)..."
	$(CXX) $(CXXFLAGS) -o $@ $^

ListadoEstudiantil.o: ListadoEstudiantil.cpp ListadoEstudiantil.h
	@echo "Compilando ListadoEstudiantil.cpp..."
	$(CXX) $(CXXFLAGS) -c ListadoEstudiantil.cpp

testListadoEstudiantil.o: testListadoEstudiantil.cpp ListadoEstudiantil.h
	@echo "Compilando testListadoEstudiantil.cpp..."
	$(CXX) $(CXXFLAGS) -c testListadoEstudiantil.cpp

# Ejercicio 2: SeñalECG
$(EXEC_ECG): SeñalECG.o testECG.o
	@echo "Enlazando $(EXEC_ECG)..."
	$(CXX) $(CXXFLAGS) -o $@ $^

SeñalECG.o: SeñalECG.cpp SeñalECG.h
	@echo "Compilando SeñalECG.cpp..."
	$(CXX) $(CXXFLAGS) -c SeñalECG.cpp

testECG.o: testECG.cpp SeñalECG.h
	@echo "Compilando testECG.cpp..."
	$(CXX) $(CXXFLAGS) -c testECG.cpp

# Ejercicio 3: Dominó con historial
$(EXEC_DOMINO): dominoConHistorial.cpp
	@echo "Compilando $(EXEC_DOMINO)..."
	$(CXX) $(CXXFLAGS) -o $@ $<

# Reglas individuales para compilar cada ejercicio
ejercicio1: $(EXEC_ESTUDIANTES)
	@echo "✓ Ejercicio 1 compilado"

ejercicio2: $(EXEC_ECG)
	@echo "✓ Ejercicio 2 compilado"

ejercicio3: $(EXEC_DOMINO)
	@echo "✓ Ejercicio 3 compilado"

# Ejecutar pruebas
run_ejercicio1: $(EXEC_ESTUDIANTES)
	@echo "\n========== Ejecutando Ejercicio 1 =========="
	./$(EXEC_ESTUDIANTES)

run_ejercicio2: $(EXEC_ECG)
	@echo "\n========== Ejecutando Ejercicio 2 =========="
	./$(EXEC_ECG)

run_ejercicio3: $(EXEC_DOMINO)
	@echo "\n========== Ejecutando Ejercicio 3 (Dominó - INTERACTIVO) =========="
	./$(EXEC_DOMINO)

# Ejecutar todos (nota: ejercicio 3 es interactivo)
run_all: $(EXEC_ESTUDIANTES) $(EXEC_ECG)
	@echo "\n========== Ejecutando Ejercicio 1 =========="
	@./$(EXEC_ESTUDIANTES)
	@echo ""
	@echo "\n========== Ejecutando Ejercicio 2 =========="
	@./$(EXEC_ECG)
	@echo ""
	@echo "Nota: El Ejercicio 3 (Dominó) requiere interacción. Ejecútalo con: make run_ejercicio3"

# Limpiar archivos compilados
clean:
	@echo "Limpiando archivos compilados..."
	rm -f *.o $(EXEC_ESTUDIANTES) $(EXEC_ECG) $(EXEC_DOMINO)
	rm -f listado_estudiantes.txt senales_ECG.txt historial_domino.txt
	@echo "✓ Limpieza completada"

# Limpiar solo archivos objeto
clean_obj:
	@echo "Limpiando archivos objeto..."
	rm -f *.o
	@echo "✓ Archivos objeto eliminados"

# Ayuda
help:
	@echo "Makefile para Trabajo Práctico 3 - Estructuras Dinámicas en C++"
	@echo ""
	@echo "Uso:"
	@echo "  make              - Compilar todos los ejercicios"
	@echo "  make ejercicio1   - Compilar solo el Ejercicio 1"
	@echo "  make ejercicio2   - Compilar solo el Ejercicio 2"
	@echo "  make ejercicio3   - Compilar solo el Ejercicio 3"
	@echo "  make run_all      - Compilar y ejecutar Ejercicios 1 y 2"
	@echo "  make run_ejercicio1 - Ejecutar Ejercicio 1"
	@echo "  make run_ejercicio2 - Ejecutar Ejercicio 2"
	@echo "  make run_ejercicio3 - Ejecutar Ejercicio 3 (interactivo)"
	@echo "  make clean        - Limpiar todo"
	@echo "  make clean_obj    - Limpiar solo archivos objeto"
	@echo "  make help         - Mostrar esta ayuda"

.PHONY: all clean clean_obj help ejercicio1 ejercicio2 ejercicio3 run_ejercicio1 run_ejercicio2 run_ejercicio3 run_all
