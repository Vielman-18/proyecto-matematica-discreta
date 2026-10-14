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
const int MAXV = 12;          // maximo de vertices
const int INF = INT_MAX;

struct Grafo {
    int n;                        // cantidad de vertices (numerados de 1 a n)
    bool dirigido;
    int peso[MAXV + 1][MAXV + 1]; // 0 = no hay arista
};

void leerGrafo(Grafo& g) {
    g.n = leerEntero("Cantidad de vertices (2 a 12): ", 2, MAXV);
    int tipo = leerEntero("Tipo de grafo (1 = No dirigido, 2 = Dirigido): ", 1, 2);
    g.dirigido = (tipo == 2);

    for (int i = 0; i <= MAXV; i++)
        for (int j = 0; j <= MAXV; j++)
            g.peso[i][j] = 0;

    int maxAristas = g.dirigido ? g.n * (g.n - 1) : g.n * (g.n - 1) / 2;
    int m = leerEntero("Cantidad de aristas (1 a " + to_string(maxAristas) + "): ", 1, maxAristas);

    cout << "\nIngrese cada arista: origen, destino y peso (el peso debe ser mayor a 0).\n";
    int i = 1;
    while (i <= m) {
        cout << "\nArista " << i << " de " << m << ":\n";
        int o = leerEntero("  Vertice origen (1 a " + to_string(g.n) + "): ", 1, g.n);
        int d = leerEntero("  Vertice destino (1 a " + to_string(g.n) + "): ", 1, g.n);
        if (o == d) {
            cout << "  Un vertice no puede conectarse consigo mismo. Intente de nuevo.\n";
            continue;
        }
        if (g.peso[o][d] != 0) {
            cout << "  Esa arista ya fue ingresada. Intente de nuevo.\n";
            continue;
        }
        int p = leerEntero("  Peso (1 a 1000): ", 1, 1000);
        g.peso[o][d] = p;
        if (!g.dirigido) g.peso[d][o] = p;
        i++;
    }
}

void mostrarGrafo(const Grafo& g) {
    cout << "\nMatriz de pesos (0 = sin arista):\n      ";
    for (int j = 1; j <= g.n; j++) {
        cout << j << "\t";
    }
    cout << "\n";
    for (int i = 1; i <= g.n; i++) {
        cout << "  " << i << " | ";
        for (int j = 1; j <= g.n; j++) cout << g.peso[i][j] << "\t";
        cout << "\n";
    }
}

void mostrarRuta(const int ruta[], int largo, int pesoTotal) {
    cout << "Ruta: ";
    for (int i = 0; i < largo; i++) {
        cout << ruta[i];
        if (i < largo - 1) cout << " -> ";
    }
    cout << "\nPeso total: " << pesoTotal << "\n";
}

// ----- Ruta mas corta (algoritmo de Dijkstra) -----
void rutaMasCorta() {
    cout << "\n--- RUTA MAS CORTA (algoritmo de Dijkstra) ---\n";
    Grafo g;
    leerGrafo(g);
    mostrarGrafo(g);

    int origen = leerEntero("\nVertice de origen: ", 1, g.n);
    int destino;
    do {
        destino = leerEntero("Vertice de destino: ", 1, g.n);
        if (destino == origen) cout << "El destino debe ser distinto del origen.\n";
    } while (destino == origen);

    int dist[MAXV + 1], previo[MAXV + 1];
    bool visitado[MAXV + 1];
    for (int i = 1; i <= g.n; i++) {
        dist[i] = INF;
        previo[i] = 0;
        visitado[i] = false;
    }
    dist[origen] = 0;

    for (int paso = 1; paso <= g.n; paso++) {
        int u = 0;
        for (int v = 1; v <= g.n; v++)
            if (!visitado[v] && dist[v] != INF && (u == 0 || dist[v] < dist[u])) u = v;
        if (u == 0) break;
        visitado[u] = true;

        for (int v = 1; v <= g.n; v++) {
            if (g.peso[u][v] > 0 && dist[u] + g.peso[u][v] < dist[v]) {
                dist[v] = dist[u] + g.peso[u][v];
                previo[v] = u;
            }
        }
    }

    if (dist[destino] == INF) {
        cout << "\nNo existe ruta entre " << origen << " y " << destino << ".\n";
        return;
    }

    // reconstruir la ruta de atras hacia adelante
    int ruta[MAXV + 1], largo = 0;
    for (int v = destino; v != 0; v = previo[v]) ruta[largo++] = v;
    for (int i = 0; i < largo / 2; i++) {
        int tmp = ruta[i];
        ruta[i] = ruta[largo - 1 - i];
        ruta[largo - 1 - i] = tmp;
    }

    cout << "\nRESULTADO - Ruta mas corta de " << origen << " a " << destino << "\n";
    mostrarRuta(ruta, largo, dist[destino]);
}

// ----- Ruta mas larga (busqueda exhaustiva de rutas simples) -----
struct BusquedaLarga {
    int mejorPeso;
    int mejorLargo;
    int mejor[MAXV + 1];
    int actual[MAXV + 1];
    bool usado[MAXV + 1];
    int rutasEvaluadas;
};

void buscarLarga(const Grafo& g, BusquedaLarga& b, int u, int destino, int pesoAct, int largo) {
    b.actual[largo - 1] = u;
    if (u == destino) {
        b.rutasEvaluadas++;
        if (pesoAct > b.mejorPeso) {
            b.mejorPeso = pesoAct;
            b.mejorLargo = largo;
            for (int i = 0; i < largo; i++) b.mejor[i] = b.actual[i];
        }
        return;
    }
    b.usado[u] = true;
    for (int v = 1; v <= g.n; v++)
        if (g.peso[u][v] > 0 && !b.usado[v])
            buscarLarga(g, b, v, destino, pesoAct + g.peso[u][v], largo + 1);
    b.usado[u] = false;
}

void rutaMasLarga() {
    cout << "\n--- RUTA MAS LARGA (sin repetir vertices) ---\n";
    Grafo g;
    leerGrafo(g);
    mostrarGrafo(g);

    int origen = leerEntero("\nVertice de origen: ", 1, g.n);
    int destino;
    do {
        destino = leerEntero("Vertice de destino: ", 1, g.n);
        if (destino == origen) cout << "El destino debe ser distinto del origen.\n";
    } while (destino == origen);

    BusquedaLarga b;
    b.mejorPeso = -1;
    b.mejorLargo = 0;
    b.rutasEvaluadas = 0;
    for (int i = 0; i <= MAXV; i++) b.usado[i] = false;

    buscarLarga(g, b, origen, destino, 0, 1);

    if (b.mejorPeso < 0) {
        cout << "\nNo existe ruta entre " << origen << " y " << destino << ".\n";
        return;
    }

    cout << "\nSe evaluaron " << b.rutasEvaluadas << " rutas posibles.\n";
    cout << "\nRESULTADO - Ruta mas larga de " << origen << " a " << destino << "\n";
    mostrarRuta(b.mejor, b.mejorLargo, b.mejorPeso);
}

void grafos() {
    int op;
    do {
        cout << "\n===== GRAFOS =====\n";
        cout << "1. Ruta mas corta\n";
        cout << "2. Ruta mas larga\n";
        cout << "3. Volver al menu principal\n";
        op = leerEntero("Elija una opcion: ", 1, 3);

        switch (op) {
            case 1: rutaMasCorta(); break;
            case 2: rutaMasLarga(); break;
        }
    } while (op != 3);
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