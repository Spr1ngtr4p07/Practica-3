#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

// Estructura del usuario
struct Usuario {
    string cedula;
    string clave;
    double saldo;
};

void guardarUsuarios(Usuario usuarios[], int cantidad) {
    ofstream archivo("usuarios.txt");
    if (!archivo.is_open()) {
        cout << "Error al guardar usuarios.\n";
        return;
    }
    for (int i = 0; i < cantidad; i++) {
        archivo << usuarios[i].cedula << " "
                << usuarios[i].clave << " "
                << usuarios[i].saldo << endl;
    }
    archivo.close();
}

// Leer los usuarios
int cargarUsuarios(Usuario usuarios[], int maxUsuarios) {
    ifstream archivo("usuarios.txt");
    if (!archivo.is_open()) {
        ofstream crear("usuarios.txt");
        crear.close();
        return 0;
    }
    int cantidad = 0;
    while (cantidad < maxUsuarios &&
           archivo >> usuarios[cantidad].cedula >> usuarios[cantidad].clave >> usuarios[cantidad].saldo) {
        cantidad++;
    }
    archivo.close();
    return cantidad;
}

// transacciones
void registrarTransaccion(string cedula, string tipo, double monto, double saldoFinal) {
    ofstream archivo("transacciones.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "Error al registrar transacción.\n";
        return;
    }

    time_t ahora = time(0);
    string fecha = ctime(&ahora);
    fecha.pop_back();

    archivo << fecha << " | Cedula: " << cedula
            << " | Tipo: " << tipo
            << " | Monto: " << monto
            << " | Saldo final: " << saldoFinal << endl;

    archivo.close();
}

// Buscar usuario
int buscarUsuario(Usuario usuarios[], int cantidad, string cedula, string clave) {
    for (int i = 0; i < cantidad; i++) {
        if (usuarios[i].cedula == cedula && usuarios[i].clave == clave)
            return i;
    }
    return -1;
}

int main() {
    const int MAX_USUARIOS = 100;
    Usuario usuarios[MAX_USUARIOS];
    int cantidadUsuarios = cargarUsuarios(usuarios, MAX_USUARIOS);

    string adminUsuario = "admin";
    string adminClave = "1234";
    int tipoIngreso;

    cout << " SISTEMA DEL BANCARIO \n";
    cout << "1. Ingresar como administrador\n";
    cout << "2. Ingresar como usuario\n";
    cout << "Opcion: ";
    cin >> tipoIngreso;

    // ADMINISTRADOR
    if (tipoIngreso == 1) {
        string inputUsuario, inputClave;
        cout << "\nUsuario administrador: ";
        cin >> inputUsuario;
        cout << "Clave: ";
        cin >> inputClave;

        if (inputUsuario != adminUsuario || inputClave != adminClave) {
            cout << "Acceso denegado.\n";
            return 0;
        }

        cout << "Acceso concedido.\n";
        int opcion;
        do {
            cout << " MENU ADMINISTRADOR ";
            cout << "1. Registrar usuario nuevo\n";
            cout << "2. Ver usuarios\n";
            cout << "3. Guardar y salir\n";
            cout << "Opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1: {
                if (cantidadUsuarios >= MAX_USUARIOS) {
                    cout << "No se pueden registrar más usuarios.\n";
                    break;
                }

                cout << " Registro de nuevo usuario ";
                cout << "Cedula: ";
                cin >> usuarios[cantidadUsuarios].cedula;
                cout << "Clave: ";
                cin >> usuarios[cantidadUsuarios].clave;
                cout << "Saldo inicial (COP): ";
                cin >> usuarios[cantidadUsuarios].saldo;

                cantidadUsuarios++;
                cout << "Usuario registrado exitosamente.\n";
                break;
            }
            case 2: {
                cout << " Lista de usuarios registrados ";
                for (int i = 0; i < cantidadUsuarios; i++) {
                    cout << "Cedula: " << usuarios[i].cedula
                         << " | Saldo: " << usuarios[i].saldo << " COP\n";
                }
                break;
            }
            case 3:
                guardarUsuarios(usuarios, cantidadUsuarios);
                cout << "Datos guardados. Saliendo...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
                break;
            }

        } while (opcion != 3);
    }

    // USUARIO
    else if (tipoIngreso == 2) {
        string cedula, clave;
        cout << "\nCedula: ";
        cin >> cedula;
        cout << "Clave: ";
        cin >> clave;

        int pos = buscarUsuario(usuarios, cantidadUsuarios, cedula, clave);
        if (pos == -1) {
            cout << "Credenciales incorrectas.\n";
            return 0;
        }

        if (usuarios[pos].saldo >= 1000) {
            usuarios[pos].saldo -= 1000;
            registrarTransaccion(cedula, "Cobro ingreso", 1000, usuarios[pos].saldo);
            cout << "\nIngreso exitoso. Se han descontado 1000 COP por acceso.\n";
        } else {
            cout << "Saldo insuficiente para cubrir el costo de acceso.\n";
            return 0;
        }

        int opcion;
        do {
            cout << " MENU USUARIO ";
            cout << "1. Consultar saldo\n";
            cout << "2. Retirar dinero\n";
            cout << "3. Salir\n";
            cout << "Opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1:
                cout << "\nSaldo actual: " << usuarios[pos].saldo << " COP\n";
                registrarTransaccion(cedula, "Consulta saldo", 0, usuarios[pos].saldo);
                break;

            case 2: {
                double retiro;
                cout << "Cantidad a retirar (COP): ";
                cin >> retiro;

                if (retiro <= 0) {
                    cout << "Cantidad invalida.\n";
                } else if (retiro > usuarios[pos].saldo) {
                    cout << "Saldo insuficiente.\n";
                } else {
                    usuarios[pos].saldo -= retiro;
                    cout << "Retiro exitoso. Saldo restante: " << usuarios[pos].saldo << " COP\n";
                    registrarTransaccion(cedula, "Retiro", retiro, usuarios[pos].saldo);
                }
                break;
            }

            case 3:
                guardarUsuarios(usuarios, cantidadUsuarios);
                cout << "Sesion cerrada.\n";
                break;

            default:
                cout << "Opcion invalida.\n";
                break;
            }

        } while (opcion != 3);
    }

    else {
        cout << "Opcion no valida.\n";
    }

    return 0;
}
