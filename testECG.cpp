#include "SeñalECG.h"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "\n===========================================\n";
    std::cout << "  PROCESAMIENTO DE SEÑAL ECG\n";
    std::cout << "===========================================\n";
    
    // Crear objeto para procesar la señal ECG
    SeñalECG ecg;
    
    // 1. Cargar señal desde archivo
    std::cout << "\n1. Cargando señal ECG desde archivo...\n";
    if (!ecg.cargarDesdeArchivo("ECG.txt")) {
        std::cerr << "✗ Error al cargar el archivo ECG.txt" << std::endl;
        return 1;
    }
    
    // 2. Mostrar estadísticas de la señal original
    std::cout << "\n2. Estadísticas de la señal original:\n";
    ecg.imprimirEstadisticas();
    
    // 3. Aplicar filtro pasa bajos
    std::cout << "\n3. Aplicando filtro digital pasa bajos...\n";
    ecg.aplicarFiltroPasaBajos(0.1); // alpha = 0.1 para suavizado moderado
    
    // 4. Detectar picos en la señal filtrada
    std::cout << "\n4. Detectando picos en la señal filtrada...\n";
    auto picos = ecg.detectarPicos(50.0); // Umbral de 50 para detectar picos R
    
    if (!picos.empty()) {
        std::cout << "\nPicos detectados en los siguientes tiempos (segundos):\n";
        for (size_t i = 0; i < picos.size() && i < 10; i++) {
            std::cout << "  Pico " << (i+1) << ": t = " << picos[i] << " s\n";
        }
        if (picos.size() > 10) {
            std::cout << "  ... y " << (picos.size() - 10) << " picos más\n";
        }
    }
    
    // 5. Calcular frecuencia cardíaca
    std::cout << "\n5. Calculando frecuencia cardíaca...\n";
    double frecuencia = ecg.calcularFrecuenciaCardiaca();
    
    if (frecuencia > 0) {
        std::cout << "\n┌────────────────────────────────────┐\n";
        std::cout << "│  FRECUENCIA CARDÍACA: " 
                  << std::fixed << std::setprecision(1) 
                  << frecuencia << " BPM  │\n";
        std::cout << "└────────────────────────────────────┘\n";
    }
    
    // 6. Guardar señales en archivo
    std::cout << "\n6. Guardando señales en archivo...\n";
    ecg.imprimirEnArchivo("senales_ECG.txt");
    
    std::cout << "\n===========================================\n";
    std::cout << "  PROCESAMIENTO COMPLETADO\n";
    std::cout << "===========================================\n";
    std::cout << "\nArchivos generados:\n";
    std::cout << "  • senales_ECG.txt - Señal original y filtrada\n";
    std::cout << "\n";
    
    return 0;
}
