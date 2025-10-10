#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string textoABits(const string &texto) {
    string bits = "";
    for (unsigned char c : texto)
        for (int i = 7; i >= 0; i--)
            bits += ((c >> i) & 1) ? '1' : '0';
    return bits;
}

string bitsATexto(const string &bits) {
    string texto = "";
    for (size_t i = 0; i + 7 < bits.size(); i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8; j++)
            byte = (byte << 1) | (bits[i + j] - '0');
        texto += byte;
    }
    return texto;
}

string codificarBits(const string &bits, int n) {
    string resultado = "", grupo = "";
    for (char bit : bits) {
        grupo += bit;
        if (grupo.size() == n) {
            string g = grupo.substr(1) + grupo[0];
            resultado += g;
            grupo = "";
        }
    }
    if (!grupo.empty()) resultado += grupo;
    return resultado;
}

string decodificarBits(const string &bits, int n) {
    string resultado = "", grupo = "";
    for (char bit : bits) {
        grupo += bit;
        if (grupo.size() == n) {
            string g = grupo.back() + grupo.substr(0, n - 1);
            resultado += g;
            grupo = "";
        }
    }
    if (!grupo.empty()) resultado += grupo;
    return resultado;
}

int main() {
    int n, opcion;
    cout << "   SISTEMA BANCARIO - MENU PRINCIPAL \n";
    cout << "Ingrese la semilla de codificacion: ";
    cin >> n;

    do {
        cout << " MENU PRINCIPAL ";
        cout << "1. Acceder como administrador\n";
        cout << "2. Acceder como usuario\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            if (validarAdmin(n)) {
                cout << "Acceso concedido al administrador.\n";
                registrarUsuario(n);
            } else {
                cout << "Acceso denegado.\n";
            }
            break;

        case 2: {
            vector<Usuario> usuarios = cargarUsuarios(n);
            menuUsuario(usuarios, n);
            break;
        }

        case 3:
            cout << "Saliendo del sistema...\n";
            break;

        default:
            cout << "Opcion no valida.\n";
        }
    } while (opcion != 3);

    return 0;
}
