#include <iostream>
#include <fstream>
using namespace std;

int main() {
    string nombreEntrada, nombreSalida;
    int n, metodo;

    cout << "Ingrese el nombre del archivo de entrada: ";
    cin >> nombreEntrada;
    cout << "Ingrese la semilla de codificacion (entero): ";
    cin >> n;
    cout << "Seleccione el metodo de codificacion:\n";
    cout << "1. Convertir a cadena de bits\n";
    cout << "2. Desplazamiento de bits\n";
    cout << "Opcion: ";
    cin >> metodo;

    cout << "Ingrese el nombre del archivo de salida: ";
    cin >> nombreSalida;

    ifstream entrada(nombreEntrada, ios::binary);
    ofstream salida(nombreSalida, ios::binary);

    if (!entrada.is_open() || !salida.is_open()) {
        cout << "Error al abrir los archivos.\n";
        return 1;
    }

    switch (metodo) {
    case 1: {
        // convertir todo el archivo a bits
        string bitsTotales = "";
        char c;

        while (entrada.get(c)) {                 // Leer un carácter del archivo
            unsigned char byte = c;                // Convertirlo a unsigned char (0-255)

            // Recorrer los 8 bits del carácter, de más significativo (7) a menos (0)
            for (int i = 7; i >= 0; i--) {
                // Desplazar el carácter 'i' bits a la derecha y tomar solo el último bit
                int bit = (byte >> i) & 1;

                // Si el bit es 1, agregar '1' a la cadena; si es 0, agregar '0'
                if (bit == 1) {
                    bitsTotales += '1';
                } else {
                    bitsTotales += '0';
                }
            }
        }

        cout << "Cadena de bits generada:\n" << bitsTotales << "\n";

        // Guardar los bits en el archivo de salida (como texto)
        salida << bitsTotales;
        break;
    }

    case 2: {
        // ---------- MÉTODO 2: desplazamiento de bits en grupos de n ----------

        string bitsTotales = "";
        char c;

        while (entrada.get(c)) {                 // Leer un carácter del archivo
            unsigned char byte = c;                // Convertirlo a unsigned char (0-255)

            // Recorrer los 8 bits del carácter, de más significativo (7) a menos (0)
            for (int i = 7; i >= 0; i--) {
                // Desplazar el carácter 'i' bits a la derecha y tomar solo el último bit
                int bit = (byte >> i) & 1;

                // Si el bit es 1, agregar '1' a la cadena; si es 0, agregar '0'
                if (bit == 1) {
                    bitsTotales += '1';
                } else {
                    bitsTotales += '0';
                }
            }
        }

        // Procesar los bits en grupos de tamaño n
        string bitsCodificados = "";
        string grupo = "";

        for (size_t i = 0; i < bitsTotales.size(); i++) {
            grupo += bitsTotales[i];  // Agregar bit al grupo actual

            // Cuando el grupo alcanza tamaño n, se procesa
            if (grupo.size() == n) {
                string grupoCodificado = "";

                // Desplazar una posición a la izquierda
                for (int j = 1; j < n; j++) {
                    grupoCodificado += grupo[j];
                }

                // Agregar al final el primer bit original
                grupoCodificado += grupo[0];

                // Añadir el grupo codificado al total
                bitsCodificados += grupoCodificado;

                // Vaciar el grupo
                grupo = "";
            }
        }

        // Si quedan bits sueltos al final, se copian sin modificar
        if (!grupo.empty()) {
            bitsCodificados += grupo;
        }

        // Mostrar el resultado en consola
        cout << "Cadena de bits original:\n" << bitsTotales << "\n\n";
        cout << "Cadena de bits codificada (grupos de " << n << "):\n" << bitsCodificados << "\n";

        // Guardar la cadena de bits codificada en el archivo de salida
        salida << bitsCodificados;

        break;
    }

    default:
        cout << "Método no válido.\n";
        return 1;
    }

    entrada.close();
    salida.close();
    cout << "Archivo codificado exitosamente en '" << nombreSalida << "'\n";
    return 0;
}

