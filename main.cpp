#include <iostream>
#include <string>
#include <climits>
using namespace std;

typedef unsigned long long ull;

// ---------- FUNCIONES AUXILIARES ----------
ull factorial(int n) {
    ull r = 1;
    for (int i = 2; i <= n; i++) r *= i;
    return r;
}

int leerEntero(const string& mensaje, int minimo, int maximo) {
    int x;
    while (true) {
        cout << mensaje;
        cin >> x;
        if (cin.fail() || x < minimo || x > maximo) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Valor invalido. Debe ser un entero entre "
                 << minimo << " y " << maximo << ".\n";
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
}

// ---------- PERMUTACIONES ----------
void permutacionesSinRepeticion() {
    cout << "\n--- Permutaciones SIN repeticion: P(n,r) = n! / (n-r)! ---\n";
    int n = leerEntero("Cantidad total de elementos (n, maximo 20): ", 1, 20);
    int r = leerEntero("Cuantos elementos se ordenan (r, entre 0 y n): ", 0, n);

    ull resultado = 1;
    for (int i = 0; i < r; i++) resultado *= (n - i);

    cout << "\nP(" << n << "," << r << ") = " << n << "! / (" << n << "-" << r
         << ")! = " << n << "! / " << (n - r) << "!\n";
    cout << "Resultado: " << resultado << "\n";
}

void permutacionesConRepeticionGrupos() {
    cout << "\n--- Permutaciones CON repeticion: n! / (n1! * n2! * ... * nk!) ---\n";
    int n = leerEntero("Total de elementos (n, maximo 20): ", 1, 20);
    int k = leerEntero("Cuantos elementos distintos hay (k): ", 1, n);

    ull denominador = 1;
    int restante = n;
    string formula = "";
    for (int i = 1; i <= k; i++) {
        int ni;
        if (i == k) {
            ni = restante;   // el ultimo toma lo que falta para completar n
            cout << "El elemento " << i << " se repite " << ni
                 << " veces (lo que falta para completar n).\n";
        } else {
            int maxPermitido = restante - (k - i);   // dejar al menos 1 para los demas
            ni = leerEntero("Cuantas veces se repite el elemento " + to_string(i)
                            + " (1 a " + to_string(maxPermitido) + "): ", 1, maxPermitido);
        }
        restante -= ni;
        denominador *= factorial(ni);
        formula += to_string(ni) + "!";
        if (i < k) formula += " * ";
    }

    cout << "\n" << n << "! / (" << formula << ")\n";
    cout << "Resultado: " << factorial(n) / denominador << "\n";
}

void permutacionesConRepeticionPotencia() {
    cout << "\n--- Permutaciones CON repeticion: n^r ---\n";
    int n = leerEntero("Cuantos elementos distintos hay (n): ", 1, 1000);
    int r = leerEntero("Cuantas posiciones hay (r): ", 0, 1000);

    ull resultado = 1;
    for (int i = 0; i < r; i++) {
        if (resultado > ULLONG_MAX / n) {
            cout << "\nEl resultado es demasiado grande para calcularlo.\n";
            return;
        }
        resultado *= n;
    }
    cout << "\n" << n << "^" << r << " = " << resultado << "\n";
}

void permutaciones() {
    int op;
    do {
        cout << "\n===== PERMUTACIONES =====\n";
        cout << "1. Sin repeticion\n";
        cout << "2. Con repeticion (elementos repetidos)\n";
        cout << "3. Con repeticion (n^r)\n";
        cout << "4. Volver al menu principal\n";
        op = leerEntero("Elija una opcion: ", 1, 4);

        switch (op) {
            case 1: permutacionesSinRepeticion(); break;
            case 2: permutacionesConRepeticionGrupos(); break;
            case 3: permutacionesConRepeticionPotencia(); break;
        }
    } while (op != 4);
}

// ---------- COMBINACIONES ----------
// Calcula C(n,r) sin desbordar: multiplica y divide paso a paso
ull calcularCombinacion(int n, int r) {
    if (r > n - r) r = n - r;
    ull resultado = 1;
    for (int i = 1; i <= r; i++) {
        resultado = resultado * (n - r + i) / i;
    }
    return resultado;
}

void combinacionesSinRepeticion() {
    cout << "\n--- Combinaciones SIN repeticion: C(n,r) = n! / (r! * (n-r)!) ---\n";
    int n = leerEntero("Cantidad total de elementos (n, maximo 60): ", 1, 60);
    int r = leerEntero("Cuantos elementos se eligen (r, entre 0 y n): ", 0, n);

    cout << "\nC(" << n << "," << r << ") = " << n << "! / (" << r << "! * "
         << (n - r) << "!)\n";
    cout << "Resultado: " << calcularCombinacion(n, r) << "\n";
}

void combinacionesConRepeticion() {
    cout << "\n--- Combinaciones CON repeticion: C(n+r-1, r) = (n+r-1)! / (r! * (n-1)!) ---\n";
    int n = leerEntero("Cuantos tipos de elementos hay (n, maximo 30): ", 1, 30);
    int r = leerEntero("Cuantos elementos se eligen (r, maximo 30): ", 0, 30);

    cout << "\nC(" << n << "+" << r << "-1," << r << ") = C(" << (n + r - 1)
         << "," << r << ")\n";
    cout << "Resultado: " << calcularCombinacion(n + r - 1, r) << "\n";
}

void combinaciones() {
    int op;
    do {
        cout << "\n===== COMBINACIONES =====\n";
        cout << "1. Sin repeticion\n";
        cout << "2. Con repeticion\n";
        cout << "3. Volver al menu principal\n";
        op = leerEntero("Elija una opcion: ", 1, 3);

        switch (op) {
            case 1: combinacionesSinRepeticion(); break;
            case 2: combinacionesConRepeticion(); break;
        }
    } while (op != 3);
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
        opcion = leerEntero("Elija una opcion: ", 1, 4);

        switch (opcion) {
            case 1: permutaciones(); break;
            case 2: combinaciones(); break;
            case 3: grafos();        break;
            case 4: cout << "\nHasta luego!\n"; break;
        }
    } while (opcion != 4);

    return 0;
}