/**
 * @file punto6.cpp
 * @brief Implementación del juego de Dominó en C++ utilizando POO.
 *
 * Este archivo contiene las clases y la lógica necesaria para jugar al dominó
 * en la consola. Incluye las clases Ficha, Jugador y JuegoDomino.
 */

#include <algorithm>
#include <ctime>
#include <deque>
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
  /**
   * @brief Constructor de la clase Ficha.
   * @param izq Valor del lado izquierdo (0-6).
   * @param der Valor del lado derecho (0-6).
   */
  Ficha(int izq, int der) : ladoIzquierdo(izq), ladoDerecho(der) {}

  /**
   * @brief Obtiene el valor del lado izquierdo.
   * @return int Valor izquierdo.
   */
  int getIzquierdo() const { return ladoIzquierdo; }

  /**
   * @brief Obtiene el valor del lado derecho.
   * @return int Valor derecho.
   */
  int getDerecho() const { return ladoDerecho; }

  /**
   * @brief Invierte los valores de la ficha.
   * Útil cuando se necesita colocar la ficha en el tablero por el otro lado.
   */
  void voltear() { swap(ladoIzquierdo, ladoDerecho); }

  /**
   * @brief Devuelve una representación en cadena de la ficha.
   * @return string Formato "[L|R]".
   */
  string toString() const {
    return "[" + to_string(ladoIzquierdo) + "|" + to_string(ladoDerecho) + "]";
  }

  /**
   * @brief Verifica si la ficha es una mula (doble).
   * @return true Si ambos lados son iguales.
   * @return false En caso contrario.
   */
  bool esMula() const { return ladoIzquierdo == ladoDerecho; }

  /**
   * @brief Calcula la suma de los puntos de la ficha.
   * @return int Suma de ladoIzquierdo + ladoDerecho.
   */
  int sumaPuntos() const { return ladoIzquierdo + ladoDerecho; }
};

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
  /**
   * @brief Constructor de la clase Jugador.
   * @param nombre Nombre del jugador.
   */
  Jugador(string nombre) : nombre(nombre), victorias(0) {}

  /**
   * @brief Añade una ficha a la mano del jugador.
   * @param ficha La ficha a añadir.
   */
  void recibirFicha(const Ficha &ficha) { mano.push_back(ficha); }

  /**
   * @brief Muestra las fichas en la mano del jugador.
   */
  void mostrarMano() const {
    cout << "Mano de " << nombre << ":" << endl;
    for (size_t i = 0; i < mano.size(); ++i) {
      cout << i + 1 << ". " << mano[i].toString() << " ";
    }
    cout << endl;
  }

  /**
   * @brief Obtiene una ficha específica de la mano.
   * @param indice Índice de la ficha (0-based).
   * @return Ficha& Referencia a la ficha.
   */
  Ficha &getFicha(int indice) { return mano[indice]; }

  /**
   * @brief Elimina una ficha de la mano (cuando se juega).
   * @param indice Índice de la ficha a eliminar.
   */
  void removerFicha(int indice) {
    if (indice >= 0 && indice < mano.size()) {
      mano.erase(mano.begin() + indice);
    }
  }

  /**
   * @brief Verifica si el jugador tiene algún movimiento válido.
   * @param extremoIzq Valor del extremo izquierdo del tablero.
   * @param extremoDer Valor del extremo derecho del tablero.
   * @return true Si tiene al menos una ficha jugable.
   * @return false Si no tiene fichas jugables.
   */
  bool tieneMovimientoValido(int extremoIzq, int extremoDer) const {
    // Si el tablero está vacío (extremos -1), cualquier ficha es válida
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

  /**
   * @brief Devuelve la cantidad de fichas en la mano.
   * @return int Número de fichas.
   */
  int getCantidadFichas() const { return mano.size(); }

  /**
   * @brief Calcula la suma total de puntos en la mano.
   * @return int Suma de puntos.
   */
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
 * @brief Gestiona la lógica y el flujo del juego.
 */
class JuegoDomino {
private:
  vector<Jugador> jugadores;
  deque<Ficha> tablero;
  vector<Ficha> pozo; // Fichas no repartidas (si aplica) o para repartir
  int turnoActual;

  /**
   * @brief Genera las 28 fichas del dominó (doble 6).
   */
  void generarFichas() {
    pozo.clear();
    for (int i = 0; i <= 6; ++i) {
      for (int j = i; j <= 6; ++j) {
        pozo.emplace_back(i, j);
      }
    }
  }

  /**
   * @brief Baraja las fichas aleatoriamente.
   */
  void barajarFichas() {
    random_device rd;
    mt19937 g(rd());
    shuffle(pozo.begin(), pozo.end(), g);
  }

  /**
   * @brief Reparte fichas a los jugadores.
   * Se reparten 7 fichas a cada jugador (estándar para 2-4 jugadores).
   */
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

public:
  JuegoDomino() : turnoActual(0) {}

  /**
   * @brief Configura el juego solicitando número de jugadores y nombres.
   */
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
    cin.ignore(); // Limpiar buffer

    for (int i = 0; i < numJugadores; ++i) {
      string nombre;
      cout << "Nombre del jugador " << i + 1 << ": ";
      getline(cin, nombre);
      jugadores.emplace_back(nombre);
    }
  }

  /**
   * @brief Muestra el estado actual del tablero.
   */
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

  /**
   * @brief Ejecuta el bucle principal de una partida.
   */
  void jugarPartida() {
    generarFichas();
    barajarFichas();
    repartirFichas();
    tablero.clear();

    // Determinar quién empieza (opcional: el que tenga el doble más alto,
    // simplificado a aleatorio o turno 0)
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

          // Lógica para colocar ficha
          if (tablero.empty()) {
            tablero.push_back(fichaJugada);
            jugadorActual.removerFicha(seleccion - 1);
            movimientoValido = true;
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

            if (!movimientoValido) {
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
  }

  /**
   * @brief Determina el ganador en caso de cierre contando los puntos.
   */
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
      } else if (puntos == minPuntos) {
        // Empate, lógica simple: gana el primero encontrado o se declara empate
        // Aquí mantenemos el primero
      }
    }

    if (indiceGanador != -1) {
      cout << "¡" << jugadores[indiceGanador].getNombre()
           << " gana por tener menos puntos!" << endl;
      jugadores[indiceGanador].agregarVictoria();
    }
  }

  /**
   * @brief Bucle principal para ejecutar múltiples partidas.
   */
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
