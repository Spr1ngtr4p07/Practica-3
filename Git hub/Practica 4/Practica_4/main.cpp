#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <algorithm>

using namespace std;

/* ===========================================================
   CLASE ROUTER
   Representa un enrutador con sus vecinos, costos y rutas
   =========================================================== */
class Router {
private:
    string nombre;
    map<string, int> vecinos;  // Vecino → costo

public:
    Router() {}
    Router(string _nombre) : nombre(_nombre) {}

    string getNombre() const { return nombre; }

    void agregarVecino(string vecino, int costo) {
        vecinos[vecino] = costo;
    }

    void eliminarVecino(string vecino) {
        vecinos.erase(vecino);
    }

    const map<string, int>& getVecinos() const {
        return vecinos;
    }
};


/* ===========================================================
   CLASE RED
   Administra todos los routers y calcula caminos
   =========================================================== */
class Red {
private:
    map<string, Router> routers; // nombre → objeto Router

public:
    /* ----- Agregar y eliminar routers ----- */
    void agregarRouter(string nombre) {
        if (routers.find(nombre) == routers.end()) {
            routers[nombre] = Router(nombre);
            cout << "Router " << nombre << " agregado.\n";
        } else {
            cout << "El router " << nombre << " ya existe.\n";
        }
    }

    void eliminarRouter(string nombre) {
        if (routers.find(nombre) != routers.end()) {
            routers.erase(nombre);
            // Eliminar también referencias en otros routers
            for (auto& [n, r] : routers) {
                r.eliminarVecino(nombre);
            }
            cout << "Router " << nombre << " eliminado.\n";
        } else {
            cout << "El router no existe.\n";
        }
    }

    /* ----- Conectar routers con un costo ----- */
    void conectarRouters(string origen, string destino, int costo) {
        if (routers.find(origen) == routers.end() || routers.find(destino) == routers.end()) {
            cout << "Uno de los routers no existe.\n";
            return;
        }

        routers[origen].agregarVecino(destino, costo);
        routers[destino].agregarVecino(origen, costo); // conexión bidireccional
    }

    /* ----- Mostrar routers y conexiones ----- */
    void mostrarRed() const {
        cout << "\n--- TOPOLOGÍA DE LA RED ---\n";
        for (auto& [nombre, router] : routers) {
            cout << nombre << " conectado con: ";
            for (auto& [vecino, costo] : router.getVecinos()) {
                cout << "(" << vecino << ", " << costo << ") ";
            }
            cout << "\n";
        }
    }

    /* ----- Cargar red desde archivo ----- */
    void cargarDesdeArchivo(string nombreArchivo) {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo.\n";
            return;
        }

        string origen, destino;
        int costo;
        while (archivo >> origen >> destino >> costo) {
            agregarRouter(origen);
            agregarRouter(destino);
            conectarRouters(origen, destino, costo);
        }

        archivo.close();
        cout << "Topología cargada desde archivo.\n";
    }

    /* ----- Generar red aleatoria ----- */
    void generarAleatoria(int cantidad) {
        for (int i = 0; i < cantidad; ++i) {
            string nombre(1, 'A' + i);
            agregarRouter(nombre);
        }

        srand(time(nullptr));
        for (auto& [nombre, router] : routers) {
            int enlaces = rand() % (cantidad - 1) + 1;
            for (int i = 0; i < enlaces; ++i) {
                string destino(1, 'A' + rand() % cantidad);
                if (destino != nombre) {
                    int costo = rand() % 10 + 1;
                    conectarRouters(nombre, destino, costo);
                }
            }
        }

        cout << "Red generada aleatoriamente.\n";
    }

    /* ----- Dijkstra: camino más corto ----- */
    void caminoMasCorto(string origen, string destino) {
        if (routers.find(origen) == routers.end() || routers.find(destino) == routers.end()) {
            cout << "Uno de los routers no existe.\n";
            return;
        }

        map<string, int> distancia;
        map<string, string> previo;
        for (auto& [nombre, r] : routers)
            distancia[nombre] = numeric_limits<int>::max();

        distancia[origen] = 0;

        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        pq.push({0, origen});

        while (!pq.empty()) {
            auto [dist, actual] = pq.top(); pq.pop();

            for (auto& [vecino, costo] : routers[actual].getVecinos()) {
                int nuevaDist = dist + costo;
                if (nuevaDist < distancia[vecino]) {
                    distancia[vecino] = nuevaDist;
                    previo[vecino] = actual;
                    pq.push({nuevaDist, vecino});
                }
            }
        }

        // Mostrar resultado
        if (distancia[destino] == numeric_limits<int>::max()) {
            cout << "No hay camino entre " << origen << " y " << destino << ".\n";
            return;
        }

        cout << "\nCamino más corto de " << origen << " a " << destino << ": ";

        vector<string> camino;
        for (string at = destino; at != origen; at = previo[at]) {
            camino.push_back(at);
        }
        camino.push_back(origen);
        reverse(camino.begin(), camino.end());

        for (auto& nodo : camino)
            cout << nodo << (nodo == destino ? "" : " → ");
        cout << "\nCosto total: " << distancia[destino] << "\n";
    }
};

int main() {
    Red red;
    int opcion;

    do {
        cout << "\n========== MENÚ ==========\n";
        cout << "1. Agregar router\n";
        cout << "2. Eliminar router\n";
        cout << "3. Conectar routers\n";
        cout << "4. Mostrar red\n";
        cout << "5. Cargar red desde archivo\n";
        cout << "6. Generar red aleatoria\n";
        cout << "7. Camino más corto\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        string a, b;
        int costo, n;

        switch (opcion) {
        case 1:
            cout << "Nombre del router: "; cin >> a;
            red.agregarRouter(a);
            break;
        case 2:
            cout << "Nombre del router a eliminar: "; cin >> a;
            red.eliminarRouter(a);
            break;
        case 3:
            cout << "Router origen: "; cin >> a;
            cout << "Router destino: "; cin >> b;
            cout << "Costo del enlace: "; cin >> costo;
            red.conectarRouters(a, b, costo);
            break;
        case 4:
            red.mostrarRed();
            break;
        case 5:
            cout << "Nombre del archivo (ej: topologia.txt): "; cin >> a;
            red.cargarDesdeArchivo(a);
            break;
        case 6:
            cout << "Cantidad de routers: "; cin >> n;
            red.generarAleatoria(n);
            break;
        case 7:
            cout << "Router origen: "; cin >> a;
            cout << "Router destino: "; cin >> b;
            red.caminoMasCorto(a, b);
            break;
        case 0:
            cout << "Saliendo...\n";
            break;
        default:
            cout << "Opción inválida.\n";
        }

    } while (opcion != 0);
    return 0;
}
