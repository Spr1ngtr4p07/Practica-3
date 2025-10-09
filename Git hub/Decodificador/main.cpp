#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    string nombreEntrada, nombreSalida;
    int n, metodo;

    cout << "Ingrese el nombre del archivo de entrada (codificado): ";
    cin >> nombreEntrada;
    cout << "Ingrese la semilla de codificacion (entero): ";
    cin >> n;
    cout << "Seleccione el metodo de decodificacion:\n";
    cout << "1. Desde cadena de bits\n";
    cout << "2. Desplazamiento de bits (inverso)\n";
    cout << "Opcion: ";
    cin >> metodo;
    cout << "Ingrese el nombre del archivo de salida (decodificado): ";
    cin >> nombreSalida;

    ifstream entrada(nombreEntrada, ios::binary);
    ofstream salida(nombreSalida, ios::binary);

    if (!entrada.is_open() || !salida.is_open()) {
        cout << "Error al abrir los archivos.\n";
        return 1;
    }

    string bitsTotales = "";
    char c;

    // Leer todo el archivo como texto de bits (0 y 1)
    while (entrada.get(c)) {
        if (c == '0' || c == '1') {
            bitsTotales += c;
        }
    }

    switch (metodo) {
    case 1: {
        // Método 1: reconstruir bytes desde bits
        for (size_t i = 0; i + 7 < bitsTotales.size(); i += 8) {
            unsigned char byte = 0;

            for (int j = 0; j < 8; j++) {
                byte = (byte << 1) | (bitsTotales[i + j] - '0');
            }

            salida.put(static_cast<char>(byte));
        }

        cout << "Archivo decodificado correctamente (metodo 1).\n";
        break;
    }

    case 2: {
        // Método 2: primero, revertir el desplazamiento circular a la izquierda → a la derecha
        string bitsDecodificados = "";
        string grupo = "";

        for (size_t i = 0; i < bitsTotales.size(); i++) {
            grupo += bitsTotales[i];

            if (grupo.size() == n) {
                string grupoOriginal = "";
                // Desplazar una posición a la derecha (inverso)
                grupoOriginal += grupo[n - 1]; // último bit va al inicio
                for (int j = 0; j < n - 1; j++) {
                    grupoOriginal += grupo[j];
                }
                bitsDecodificados += grupoOriginal;
                grupo = "";
            }
        }

        // Si quedaron bits sueltos, se copian igual
        if (!grupo.empty()) {
            bitsDecodificados += grupo;
        }

        // Ahora, reconstruir los caracteres desde bitsDecodificados
        for (size_t i = 0; i + 7 < bitsDecodificados.size(); i += 8) {
            unsigned char byte = 0;
            for (int j = 0; j < 8; j++) {
                byte = (byte << 1) | (bitsDecodificados[i + j] - '0');
            }
            salida.put(static_cast<char>(byte));
        }

        cout << "Archivo decodificado correctamente (metodo 2).\n";
        break;
    }

    default:
        cout << "Metodo no valido.\n";
        return 1;
    }

    entrada.close();
    salida.close();
    cout << "El archivo decodificado se guardo en '" << nombreSalida << "'\n";
    return 0;
}
