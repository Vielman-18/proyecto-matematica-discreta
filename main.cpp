#include <iostream>
using namespace std;

// ---------- PERMUTACIONES ----------
void permutaciones() {
    cout << "\n[Permutaciones] En construccion...\n";
}

// ---------- COMBINACIONES ----------
void combinaciones() {
    cout << "\n[Combinaciones] En construccion...\n";
}

// ---------- GRAFOS ----------
void grafos() {
    cout << "\n[Grafos] En construccion...\n";
}

int main() {
    int opcion;

    do {
        cout << "\n=====================================\n";
        cout << "     PROYECTO MATEMATICA DISCRETA\n";
        cout << "=====================================\n";
        cout << "1. Permutaciones\n";
        cout << "2. Combinaciones\n";
        cout << "3. Grafos (ruta mas corta / mas larga)\n";
        cout << "4. Salir\n";
        cout << "-------------------------------------\n";
        cout << "Elija una opcion: ";
        cin >> opcion;

        if (cin.fail()) {          // por si escriben letras
            cin.clear();
            cin.ignore(1000, '\n');
            opcion = 0;
        }

        switch (opcion) {
            case 1: permutaciones(); break;
            case 2: combinaciones(); break;
            case 3: grafos();        break;
            case 4: cout << "\nHasta luego!\n"; break;
            default: cout << "\nOpcion invalida, intente de nuevo.\n";
        }
    } while (opcion != 4);

    return 0;
}