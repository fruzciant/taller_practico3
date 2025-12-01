/**
 * @file dominoConHistorial.cpp
 * @brief Implementación del juego de Dominó con historial de movimientos (lista enlazada).
 *
 * Modificación del juego punto6.cpp para incluir una lista enlazada que almacena
 * el historial completo de movimientos y lo guarda en un archivo de texto.
 */

#include <algorithm>
#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

using namespace std;

/**
 * @class Ficha
 * @brief Representa una ficha de dominó con dos valores.
 */
class Ficha {
private:
  int ladoIzquierdo;
  int ladoDerecho;

public:
  Ficha(int izq, int der) : ladoIzquierdo(izq), ladoDerecho(der) {}

  int getIzquierdo() const { return ladoIzquierdo; }
  int getDerecho() const { return ladoDerecho; }

  void voltear() { swap(ladoIzquierdo, ladoDerecho); }

  string toString() const {
    return "[" + to_string(ladoIzquierdo) + "|" + to_string(ladoDerecho) + "]";
  }

  bool esMula() const { return ladoIzquierdo == ladoDerecho; }

  int sumaPuntos() const { return ladoIzquierdo + ladoDerecho; }
};

// ========== HISTORIAL DE MOVIMIENTOS (LISTA ENLAZADA) ==========

/**
 * @class HistorialMovimientos
 * @brief Lista enlazada simple para almacenar el historial de movimientos del juego.
 */
class HistorialMovimientos {
private:
  struct Nodo {
    string nombreJugador;
    string fichaJugada;
    string estadoTablero;
    int numeroMovimiento;
    Nodo* siguiente;
    
    Nodo(const string& nombre, const string& ficha, const string& estado, int numMov)
        : nombreJugador(nombre), fichaJugada(ficha), estadoTablero(estado),
          numeroMovimiento(numMov), siguiente(nullptr) {}
  };
  
  Nodo* cabeza;
  Nodo* cola;
  int contador;
  
public:
  HistorialMovimientos() : cabeza(nullptr), cola(nullptr), contador(0) {}
  
  ~HistorialMovimientos() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
      Nodo* siguiente = actual->siguiente;
      delete actual;
      actual = siguiente;
    }
  }
  
  void agregarMovimiento(const string& nombreJugador, const string& fichaJugada, 
                        const string& estadoTablero) {
    contador++;
    Nodo* nuevoNodo = new Nodo(nombreJugador, fichaJugada, estadoTablero, contador);
    
    if (cabeza == nullptr) {
      cabeza = cola = nuevoNodo;
    } else {
      cola->siguiente = nuevoNodo;
      cola = nuevoNodo;
    }
  }
  
  void imprimirEnArchivo(const string& nombreArchivo) const {
    ofstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
      cerr << "Error: No se pudo crear el archivo " << nombreArchivo << endl;
      return;
    }
    
    archivo << "========================================\n";
    archivo << "   HISTORIAL DE PARTIDA DE DOMINÓ\n";
    archivo << "========================================\n\n";
    
    Nodo* actual = cabeza;
    while (actual != nullptr) {
      archivo << "Movimiento #" << actual->numeroMovimiento << "\n";
      archivo << "----------------------------------------\n";
      archivo << "Jugador: " << actual->nombreJugador << "\n";
      archivo << "Ficha jugada: " << actual->fichaJugada << "\n";
      archivo << "Estado del tablero: " << actual->estadoTablero << "\n\n";
      
      actual = actual->siguiente;
    }
    
    archivo << "========================================\n";
    archivo << "Total de movimientos: " << contador << "\n";
    archivo << "========================================\n";
    
    archivo.close();
    cout << "✓ Historial guardado en " << nombreArchivo << endl;
  }
  
  void imprimirEnConsola() const {
    cout << "\n========================================\n";
    cout << "   HISTORIAL DE MOVIMIENTOS\n";
    cout << "========================================\n\n";
    
    Nodo* actual = cabeza;
    int mostrados = 0;
    const int maxMostrar = 10;
    
    while (actual != nullptr && mostrados < maxMostrar) {
      cout << "Mov. #" << actual->numeroMovimiento << ": "
           << actual->nombreJugador << " jugó " << actual->fichaJugada << "\n";
      actual = actual->siguiente;
      mostrados++;
    }
    
    if (contador > maxMostrar) {
      cout << "... y " << (contador - maxMostrar) << " movimientos más\n";
    }
    
    cout << "\nTotal: " << contador << " movimientos\n";
    cout << "========================================\n";
  }
};

// ================================================================

/**
 * @class Jugador
 * @brief Representa a un jugador en el juego de dominó.
 */
class Jugador {
private:
  string nombre;
  vector<Ficha> mano;
  int victorias;

public:
  Jugador(string nombre) : nombre(nombre), victorias(0) {}

  void recibirFicha(const Ficha &ficha) { mano.push_back(ficha); }

  void mostrarMano() const {
    cout << "Mano de " << nombre << ":" << endl;
    for (size_t i = 0; i < mano.size(); ++i) {
      cout << i + 1 << ". " << mano[i].toString() << " ";
    }
    cout << endl;
  }

  Ficha &getFicha(int indice) { return mano[indice]; }

  void removerFicha(int indice) {
    if (indice >= 0 && indice < mano.size()) {
      mano.erase(mano.begin() + indice);
    }
  }

  bool tieneMovimientoValido(int extremoIzq, int extremoDer) const {
    if (extremoIzq == -1)
      return true;

    for (const auto &ficha : mano) {
      if (ficha.getIzquierdo() == extremoIzq ||
          ficha.getIzquierdo() == extremoDer ||
          ficha.getDerecho() == extremoIzq ||
          ficha.getDerecho() == extremoDer) {
        return true;
      }
    }
    return false;
  }

  int getCantidadFichas() const { return mano.size(); }

  int sumaPuntosMano() const {
    int suma = 0;
    for (const auto &ficha : mano) {
      suma += ficha.sumaPuntos();
    }
    return suma;
  }

  string getNombre() const { return nombre; }

  void agregarVictoria() { victorias++; }
  int getVictorias() const { return victorias; }

  void limpiarMano() { mano.clear(); }
};

/**
 * @class JuegoDomino
 * @brief Gestiona la lógica y el flujo del juego con historial de movimientos.
 */
class JuegoDomino {
private:
  vector<Jugador> jugadores;
  deque<Ficha> tablero;
  vector<Ficha> pozo;
  int turnoActual;
  HistorialMovimientos historial; // Lista enlazada para el historial

  void generarFichas() {
    pozo.clear();
    for (int i = 0; i <= 6; ++i) {
      for (int j = i; j <= 6; ++j) {
        pozo.emplace_back(i, j);
      }
    }
  }

  void barajarFichas() {
    random_device rd;
    mt19937 g(rd());
    shuffle(pozo.begin(), pozo.end(), g);
  }

  void repartirFichas() {
    int fichasPorJugador = 7;
    int fichaIndex = 0;

    for (auto &jugador : jugadores) {
      jugador.limpiarMano();
      for (int i = 0; i < fichasPorJugador && fichaIndex < pozo.size(); ++i) {
        jugador.recibirFicha(pozo[fichaIndex++]);
      }
    }
  }
  
  string obtenerEstadoTablero() const {
    if (tablero.empty()) {
      return "[VACÍO]";
    }
    
    string resultado;
    for (const auto& ficha : tablero) {
      resultado += ficha.toString();
    }
    return resultado;
  }

public:
  JuegoDomino() : turnoActual(0) {}

  void inicializarJuego() {
    int numJugadores;
    do {
      cout << "Ingrese el número de jugadores (2-4): ";
      if (!(cin >> numJugadores)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        numJugadores = 0;
      }
    } while (numJugadores < 2 || numJugadores > 4);
    cin.ignore();

    for (int i = 0; i < numJugadores; ++i) {
      string nombre;
      cout << "Nombre del jugador " << i + 1 << ": ";
      getline(cin, nombre);
      jugadores.emplace_back(nombre);
    }
  }

  void mostrarTablero() const {
    cout << "\n--- Tablero ---" << endl;
    if (tablero.empty()) {
      cout << "(Vacío)" << endl;
    } else {
      for (const auto &ficha : tablero) {
        cout << ficha.toString();
      }
      cout << endl;
    }
    cout << "---------------" << endl;
  }

  void jugarPartida() {
    generarFichas();
    barajarFichas();
    repartirFichas();
    tablero.clear();

    turnoActual = 0;
    bool juegoTerminado = false;
    int pasesConsecutivos = 0;

    cout << "\n--- Nueva Partida ---" << endl;

    while (!juegoTerminado) {
      Jugador &jugadorActual = jugadores[turnoActual];
      mostrarTablero();
      cout << "\nTurno de: " << jugadorActual.getNombre() << endl;

      int extremoIzq = tablero.empty() ? -1 : tablero.front().getIzquierdo();
      int extremoDer = tablero.empty() ? -1 : tablero.back().getDerecho();

      if (jugadorActual.tieneMovimientoValido(extremoIzq, extremoDer)) {
        pasesConsecutivos = 0;
        jugadorActual.mostrarMano();

        int seleccion = -1;
        bool movimientoValido = false;

        while (!movimientoValido) {
          cout << "Seleccione el número de la ficha a jugar (0 para "
                  "cancelar/revisar): ";
          if (!(cin >> seleccion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
          }

          if (seleccion < 1 || seleccion > jugadorActual.getCantidadFichas()) {
            cout << "Selección inválida." << endl;
            continue;
          }

          Ficha &fichaJugada = jugadorActual.getFicha(seleccion - 1);
          string fichaStr = fichaJugada.toString(); // Guardar antes de modificar

          if (tablero.empty()) {
            tablero.push_back(fichaJugada);
            jugadorActual.removerFicha(seleccion - 1);
            movimientoValido = true;
            
            // Registrar movimiento en el historial
            historial.agregarMovimiento(jugadorActual.getNombre(), 
                                       fichaStr, obtenerEstadoTablero());
            
          } else {
            cout << "Donde desea colocarla? (1: Izquierda, 2: Derecha): ";
            int lado;
            cin >> lado;

            if (lado == 1) { // Izquierda
              if (fichaJugada.getDerecho() == extremoIzq) {
                tablero.push_front(fichaJugada);
                jugadorActual.removerFicha(seleccion - 1);
                movimientoValido = true;
              } else if (fichaJugada.getIzquierdo() == extremoIzq) {
                fichaJugada.voltear();
                tablero.push_front(fichaJugada);
                jugadorActual.removerFicha(seleccion - 1);
                movimientoValido = true;
              }
            } else if (lado == 2) { // Derecha
              if (fichaJugada.getIzquierdo() == extremoDer) {
                tablero.push_back(fichaJugada);
                jugadorActual.removerFicha(seleccion - 1);
                movimientoValido = true;
              } else if (fichaJugada.getDerecho() == extremoDer) {
                fichaJugada.voltear();
                tablero.push_back(fichaJugada);
                jugadorActual.removerFicha(seleccion - 1);
                movimientoValido = true;
              }
            }

            if (movimientoValido) {
              // Registrar movimiento en el historial
              historial.agregarMovimiento(jugadorActual.getNombre(), 
                                         fichaStr, obtenerEstadoTablero());
            } else {
              cout << "Movimiento inválido. La ficha no coincide con el lado "
                      "seleccionado."
                   << endl;
            }
          }
        }

        // Verificar si ganó
        if (jugadorActual.getCantidadFichas() == 0) {
          cout << "\n¡" << jugadorActual.getNombre() << " ha ganado la partida!"
               << endl;
          jugadorActual.agregarVictoria();
          juegoTerminado = true;
        }

      } else {
        cout << "No tienes movimientos válidos. Pasas turno." << endl;
        pasesConsecutivos++;
      }

      // Verificar cierre (tranca)
      if (pasesConsecutivos >= jugadores.size()) {
        cout << "\n¡Juego cerrado (Tranca)! Nadie puede mover." << endl;
        determinarGanadorPorPuntos();
        juegoTerminado = true;
      }

      turnoActual = (turnoActual + 1) % jugadores.size();
    }
    
    // Mostrar historial y guardar en archivo
    historial.imprimirEnConsola();
    historial.imprimirEnArchivo("historial_domino.txt");
  }

  void determinarGanadorPorPuntos() {
    int minPuntos = numeric_limits<int>::max();
    int indiceGanador = -1;

    cout << "\nPuntajes finales:" << endl;
    for (int i = 0; i < jugadores.size(); ++i) {
      int puntos = jugadores[i].sumaPuntosMano();
      cout << jugadores[i].getNombre() << ": " << puntos << " puntos." << endl;

      if (puntos < minPuntos) {
        minPuntos = puntos;
        indiceGanador = i;
      }
    }

    if (indiceGanador != -1) {
      cout << "¡" << jugadores[indiceGanador].getNombre()
           << " gana por tener menos puntos!" << endl;
      jugadores[indiceGanador].agregarVictoria();
    }
  }

  void run() {
    inicializarJuego();
    bool seguirJugando = true;

    while (seguirJugando) {
      jugarPartida();

      cout << "\n--- Marcador Global ---" << endl;
      for (const auto &j : jugadores) {
        cout << j.getNombre() << ": " << j.getVictorias() << " victorias"
             << endl;
      }

      cout << "\n¿Desea jugar otra partida? (s/n): ";
      char respuesta;
      cin >> respuesta;
      if (respuesta == 'n' || respuesta == 'N') {
        seguirJugando = false;
      }
    }

    cout << "¡Gracias por jugar!" << endl;
  }
};

int main() {
  JuegoDomino juego;
  juego.run();
  return 0;
}
