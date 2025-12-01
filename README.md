# Trabajo Práctico No. 3 - Estructuras Dinámicas en C++

**Curso:** Programación Avanzada  
**Profesor:** M.Sc. Mauro Baquero-Suárez  
**Estudiantes:** Jhoel David Ospino De La Rosa, Valery Michel Ochoa Lechuga, Andrés Mauricio Amaya Hernandez
**Programa:** Ingeniería Mecatrónica  
**Fecha de entrega:** 4 de Diciembre de 2025

## Descripción

Este trabajo práctico implementa tres ejercicios que demuestran el uso de estructuras de datos dinámicas (listas enlazadas) en C++:

1. **Ejercicio 1 (25%):** Sistema de gestión de estudiantes con lista enlazada simple
2. **Ejercicio 2 (50%):** Procesamiento de señal electrocardiográfica (ECG) con lista doblemente enlazada
3. **Ejercicio 3 (25%):** Juego de dominó con historial de movimientos (basado en punto6.cpp)

## Requisitos del Sistema

- **Compilador:** g++ con soporte para C++17 o superior
- **Sistema Operativo:** Linux, macOS, o Windows con MinGW/WSL
- **Herramientas:** make (opcional, pero recomendado)

## Estructura del Proyecto

```
taller_practico3/
├── ListadoEstudiantil.h       # Header del Ejercicio 1
├── ListadoEstudiantil.cpp     # Implementación del Ejercicio 1
├── testListadoEstudiantil.cpp # Programa de prueba del Ejercicio 1
├── SeñalECG.h                 # Header del Ejercicio 2
├── SeñalECG.cpp               # Implementación del Ejercicio 2
├── testECG.cpp                # Programa de prueba del Ejercicio 2
├── dominoConHistorial.cpp     # Ejercicio 3 (basado en punto6.cpp)
├── punto6.cpp                 # Juego original de dominó  
├── ECG.txt                    # Datos de entrada para Ejercicio 2
├── Makefile                   # Sistema de compilación
└── README.md                  # Este archivo
```

## Compilación

### Usando Makefile (Recomendado)

Para compilar todos los ejercicios:

```bash
make
```

Para compilar ejercicios individuales:

```bash
make ejercicio1    # Compila solo el Ejercicio 1
make ejercicio2    # Compila solo el Ejercicio 2
make ejercicio3    # Compila solo el Ejercicio 3
```

Para ver todas las opciones disponibles:

```bash
make help
```

### Compilación Manual

Si no dispone de `make`, puede compilar manualmente:

**Ejercicio 1:**

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 -o testListadoEstudiantil ListadoEstudiantil.cpp testListadoEstudiantil.cpp
```

**Ejercicio 2:**

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 -o testECG SeñalECG.cpp testECG.cpp
```

**Ejercicio 3:**

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 -o dominoConHistorial dominoConHistorial.cpp
```

## Ejecución

### Ejercicio 1: ListadoEstudiantil

```bash
./testListadoEstudiantil
```

**Salida generada:** `listado_estudiantes.txt`

Este programa demuestra todas las funcionalidades de la lista enlazada simple:

- Inserción de estudiantes
- Búsqueda por ID
- Eliminación de nodos
- Acceso por índice (operator[])
- Inversión de la lista
- Exportación a archivo de texto

### Ejercicio 2: SeñalECG

```bash
./testECG
```

**Entrada requerida:** `ECG.txt` (incluido en el proyecto)  
**Salida generada:** `senales_ECG.txt`

Este programa procesa la señal ECG:

- Carga 600 puntos de la señal desde archivo CSV
- Aplica filtro digital pasa bajos (ecuaciones de diferencias)
- Detecta picos R en la señal
- Calcula frecuencia cardíaca promedio (BPM)
- Exporta señal original y filtrada a archivo

### Ejercicio 3: Dominó con Historial

```bash
./dominoConHistorial
```

**Salida generada:** `historial_domino.txt`

**Instrucciones de juego:**

1. Ingrese el número de jugadores (2-4)
2. Ingrese el nombre de cada jugador
3. Durante el juego:
   - Seleccione la ficha a jugar por su número
   - Elija dónde colocarla (izquierda o derecha del tablero)
   - El juego continúa hasta que alguien gane o se tranque

Al finalizar la partida, se muestra el historial de movimientos en consola y se guarda en `historial_domino.txt`.

## Limpieza

Para eliminar archivos compilados y generados:

```bash
make clean
```

Para eliminar solo archivos objeto (.o):

```bash
make clean_obj
```

---

## Metodología de Implementación

### Ejercicio 1: ListadoEstudiantil (Lista Enlazada Simple)

#### Estructura de Datos

- **Nodo:** Contiene un puntero a `Estudiante` y un puntero al siguiente nodo
- **Estudiante:** Almacena nombre completo, año de nacimiento, número de identificación, promedio académico y email

#### Métodos Implementados

**1. Inserción al Final**

- Complejidad: O(n)
- Recorre la lista hasta encontrar el último nodo
- Enlaza el nuevo nodo al final

**2. Eliminación por ID**

- Complejidad: O(n)
- Busca el nodo con el ID especificado
- Reenlaza los punteros omitiendo el nodo a eliminar
- Libera la memoria del nodo y del estudiante

**3. Búsqueda**

- Complejidad: O(n)
- Recorre la lista comparando IDs
- Retorna la posición (índice) o -1 si no existe

**4. Inversión de Lista**

- Complejidad: O(n)
- Utiliza técnica de tres punteros (anterior, actual, siguiente)
- Invierte los enlaces en un solo recorrido

**5. Operador de Indexación**

- Complejidad: O(n)
- Permite acceso estilo array: `lista[i]`
- Valida el rango del índice

#### Gestión de Memoria

El destructor recorre toda la lista liberando cada nodo y su estudiante para prevenir fugas de memoria.

---

### Ejercicio 2: SeñalECG (Lista Doblemente Enlazada)

#### Estructura de Datos

- **Nodo:** Contiene amplitud, tiempo, puntero anterior y puntero siguiente
- **Dos listas:** Una para la señal original y otra para la filtrada

#### Algoritmos Implementados

**1. Carga de Datos**

- Complejidad: O(n)
- Lee archivo CSV con formato: `amplitud,tiempo`
- Construye lista doblemente enlazada
- Maneja errores de formato

**2. Filtro Pasa Bajos**

- Complejidad: O(n)
- Implementa Promedio Móvil Exponencial (EMA)
- Ecuación de diferencias: `y[n] = α·x[n] + (1-α)·y[n-1]`
- α = 0.1 para suavizado moderado

**Características:**

- Tipo: IIR (Infinite Impulse Response)
- Orden: 1
- Efectivo para eliminar ruido de alta frecuencia

**3. Detección de Picos**

- Complejidad: O(n)
- Busca máximos locales en la señal filtrada
- Condiciones para un pico en posición n:
  - `amplitud[n] > amplitud[n-1]`
  - `amplitud[n] > amplitud[n+1]`
  - `amplitud[n] > umbral` (50.0 por defecto)

**4. Cálculo de Frecuencia Cardíaca**

- Calcula intervalos R-R (tiempo entre picos)
- Promedio de intervalos: `RR_avg = Σ(RR_i) / N`
- Frecuencia cardíaca: `FC (BPM) = 60 / RR_avg`

#### Ventajas de la Lista Doblemente Enlazada

- Navegación bidireccional para comparar con nodos adyacentes
- Eficiente para detección de máximos locales
- Facilita algoritmos que requieren contexto temporal

---

### Ejercicio 3: Dominó con Historial

#### Modificaciones sobre punto6.cpp

Se agregó la clase `HistorialMovimientos` que implementa una **lista enlazada simple** para rastrear todos los movimientos del juego.

#### Estructura del Historial

**Nodo contiene:**

- Nombre del jugador
- Ficha jugada (como string)
- Estado completo del tablero después del movimiento
- Número de movimiento

**Operaciones:**

- Inserción al final: O(1) con puntero a cola
- Impresión secuencial del historial: O(n)

#### Integración con el Juego

El historial se actualiza automáticamente cada vez que:

1. Un jugador coloca una ficha exitosamente
2. Se captura el estado actual del tablero
3. Se incrementa el contador de movimientos

Al finalizar la partida:

- Se muestra un resumen en consola
- Se guarda el historial completo en `historial_domino.txt`

#### Formato del Archivo de Historial

```
========================================
   HISTORIAL DE PARTIDA DE DOMINÓ
========================================

Movimiento #1
----------------------------------------
Jugador: Alice
Ficha jugada: [3|5]
Estado del tablero: [3|5]

...

========================================
Total de movimientos: N
========================================
```

---

## Archivos de Salida

### `listado_estudiantes.txt`

Contiene todos los estudiantes registrados con sus datos completos, formateado para fácil lectura.

### `senales_ECG.txt`

Archivo CSV con tres columnas:

- `tiempo`: Tiempo en segundos
- `amplitud_original`: Señal ECG original
- `amplitud_filtrada`: Señal después del filtro pasa bajos

Puede ser importado en herramientas como Excel, Python (pandas), o MATLAB para visualización.

### `historial_domino.txt`

Historial completo de la partida incluyendo cada movimiento con jugador, ficha y estado del tablero.

---

## Conceptos Clave de Programación

### Gestión Dinámica de Memoria

- Uso de `new` y `delete`
- Destructores garantizan liberación completa
- Prevención de fugas de memoria (memory leaks)

### Encapsulamiento

- Separación de interfaz pública e implementación privada
- Ocultación de detalles internos (nodos de las listas)

### Sobrecarga de Operadores

- Implementación de `operator[]` en Ejercicio 1
- Acceso tipo array a listas enlazadas

### Procesamiento de Señales

- Filtro digital IIR
- Detección de picos basada en máximos locales
- Cálculo de frecuencia cardíaca

### Reutilización de Código

- Ejercicio 3 extiende `punto6.cpp` sin modificar su lógica original
- Separación de responsabilidades (juego vs. historial)

---

## Notas Adicionales

- El código utiliza C++17 para mejor legibilidad y eficiencia
- Se incluye manejo robusto de errores y validación de entrada
- Los programas proporcionan salida detallada para seguimiento de ejecución
- El diseño es modular para facilitar mantenimiento y extensión
- El Ejercicio 3 es un juego interactivo que requiere participación del usuario

---

Pa' la clase de Programación Avanzada.
