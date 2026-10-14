#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdlib>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
using namespace std;

typedef unsigned long long ull;

// ---------- DATOS DEL ESTUDIANTE (editar antes de entregar) ----------
const string ESTUDIANTE  = "VIELMAN ANIBAL VASQUEZ RAMOS";
const string CARNET      = "1790-23-1783";
const string CURSO       = "Matematica Discreta";
const string UNIVERSIDAD = "Universidad Mariano Galvez de Guatemala";

// ==========================================================================
//  UTILIDADES DE CONSOLA (colores, limpiar pantalla, recuadros)
// ==========================================================================
string C_RESET, C_VERDE, C_ROJO, C_CYAN, C_AMAR, C_NEG;
bool ansiActivo = true;

void iniciarConsola() {
#ifdef _WIN32
    ansiActivo = false;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo = 0;
    if (h != INVALID_HANDLE_VALUE && GetConsoleMode(h, &modo)) {
        // 0x0004 = ENABLE_VIRTUAL_TERMINAL_PROCESSING (colores ANSI)
        if (SetConsoleMode(h, modo | 0x0004)) ansiActivo = true;
    }
#endif
    if (ansiActivo) {
        C_RESET = "\033[0m";
        C_VERDE = "\033[32m";
        C_ROJO  = "\033[31m";
        C_CYAN  = "\033[36m";
        C_AMAR  = "\033[33m";
        C_NEG   = "\033[1m";
    }
}

void limpiarPantalla() {
    if (ansiActivo) cout << "\033[2J\033[H";
    else system("cls");
}

void linea(char c = '=', int n = 62) {
    cout << string(n, c) << "\n";
}

void titulo(const string& texto) {
    limpiarPantalla();
    cout << C_CYAN << C_NEG;
    linea('=');
    cout << "  " << texto << "\n";
    linea('=');
    cout << C_RESET << "\n";
}

void pausar() {
    cout << "\n" << C_AMAR << "Presione Enter para continuar..." << C_RESET;
    cin.get();
}

void mostrarResultado(const string& texto) {
    string t = "RESULTADO: " + texto;
    cout << "\n" << C_VERDE << C_NEG;
    cout << "+" << string(t.size() + 2, '-') << "+\n";
    cout << "| " << t << " |\n";
    cout << "+" << string(t.size() + 2, '-') << "+\n";
    cout << C_RESET;
}

// ==========================================================================
//  ENTRADA DE DATOS CON VALIDACION
// ==========================================================================
int leerEntero(const string& mensaje, int minimo, int maximo) {
    int x;
    while (true) {
        cout << mensaje;
        cin >> x;
        if (cin.eof() && cin.fail()) {          // se acabo la entrada
            cout << "\nEntrada finalizada.\n";
            exit(0);
        }
        if (cin.fail() || x < minimo || x > maximo) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << C_ROJO << "Valor invalido. Debe ser un entero entre "
                 << minimo << " y " << maximo << "." << C_RESET << "\n";
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
}

// ==========================================================================
//  CALCULOS (sin entrada ni salida: los usa el menu y el modo de pruebas)
// ==========================================================================
ull factorial(int n) {
    ull r = 1;
    for (int i = 2; i <= n; i++) r *= i;
    return r;
}

// P(n,r) = n! / (n-r)!
ull permSinRepeticion(int n, int r) {
    ull res = 1;
    for (int i = 0; i < r; i++) res *= (n - i);
    return res;
}

// n! / (n1! * n2! * ... * nk!)
ull permConRepeticion(const vector<int>& grupos) {
    int n = 0;
    ull den = 1;
    for (int g : grupos) {
        n += g;
        den *= factorial(g);
    }
    return factorial(n) / den;
}

// n^r  (devuelve false si el resultado no cabe en 64 bits)
bool permPotencia(int n, int r, ull& res) {
    res = 1;
    for (int i = 0; i < r; i++) {
        if (res > ULLONG_MAX / (ull)n) return false;
        res *= n;
    }
    return true;
}

// C(n,r) = n! / (r! * (n-r)!)  -> multiplica y divide paso a paso (sin desbordar)
ull combSinRepeticion(int n, int r) {
    if (r > n - r) r = n - r;
    ull res = 1;
    for (int i = 1; i <= r; i++) res = res * (n - r + i) / i;
    return res;
}

// C(n+r-1, r)
ull combConRepeticion(int n, int r) {
    return combSinRepeticion(n + r - 1, r);
}

// ---------- GRAFOS ----------
const int MAXV = 12;
const int INF = INT_MAX;

struct Grafo {
    int n;
    bool dirigido;
    int peso[MAXV + 1][MAXV + 1];   // 0 = no hay arista
};

struct Arista { int o, d, p; };

struct ResultadoRuta {
    bool existe = false;
    int peso = 0;
    vector<int> ruta;
    int evaluadas = 0;
};

Grafo crearGrafo(int n, bool dirigido, const vector<Arista>& aristas) {
    Grafo g;
    g.n = n;
    g.dirigido = dirigido;
    for (int i = 0; i <= MAXV; i++)
        for (int j = 0; j <= MAXV; j++) g.peso[i][j] = 0;
    for (const Arista& a : aristas) {
        g.peso[a.o][a.d] = a.p;
        if (!dirigido) g.peso[a.d][a.o] = a.p;
    }
    return g;
}

string rutaTexto(const vector<int>& ruta, const string& sep) {
    string s;
    for (size_t i = 0; i < ruta.size(); i++) {
        s += to_string(ruta[i]);
        if (i + 1 < ruta.size()) s += sep;
    }
    return s;
}

// Ruta mas corta: algoritmo de Dijkstra
ResultadoRuta rutaMasCorta(const Grafo& g, int origen, int destino) {
    ResultadoRuta res;
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

    if (dist[destino] == INF) return res;
    res.existe = true;
    res.peso = dist[destino];
    for (int v = destino; v != 0; v = previo[v]) res.ruta.push_back(v);
    reverse(res.ruta.begin(), res.ruta.end());
    return res;
}

// Ruta mas larga: busqueda exhaustiva de rutas simples (sin repetir vertices)
struct BusquedaLarga {
    const Grafo* g;
    int destino;
    int mejorPeso;
    vector<int> mejor, actual;
    bool usado[MAXV + 1];
    int evaluadas;
};

void buscarLarga(BusquedaLarga& b, int u, int pesoAct) {
    b.actual.push_back(u);
    if (u == b.destino) {
        b.evaluadas++;
        if (pesoAct > b.mejorPeso) {
            b.mejorPeso = pesoAct;
            b.mejor = b.actual;
        }
        b.actual.pop_back();
        return;
    }
    b.usado[u] = true;
    for (int v = 1; v <= b.g->n; v++)
        if (b.g->peso[u][v] > 0 && !b.usado[v])
            buscarLarga(b, v, pesoAct + b.g->peso[u][v]);
    b.usado[u] = false;
    b.actual.pop_back();
}

ResultadoRuta rutaMasLarga(const Grafo& g, int origen, int destino) {
    ResultadoRuta res;
    BusquedaLarga b;
    b.g = &g;
    b.destino = destino;
    b.mejorPeso = -1;
    b.evaluadas = 0;
    for (int i = 0; i <= MAXV; i++) b.usado[i] = false;
    buscarLarga(b, origen, 0);
    res.evaluadas = b.evaluadas;
    if (b.mejorPeso < 0) return res;
    res.existe = true;
    res.peso = b.mejorPeso;
    res.ruta = b.mejor;
    return res;
}

// ==========================================================================
//  PERMUTACIONES (interfaz)
// ==========================================================================
void uiPermSinRepeticion() {
    titulo("PERMUTACIONES SIN REPETICION");
    cout << "Formula: P(n,r) = n! / (n-r)!\n";
    cout << "Se usa cuando IMPORTA el orden y no se repiten elementos.\n\n";
    int n = leerEntero("Cantidad total de elementos (n, maximo 20): ", 1, 20);
    int r = leerEntero("Cuantos elementos se ordenan (r, entre 0 y n): ", 0, n);

    cout << "\nProcedimiento:\n";
    cout << "  P(" << n << "," << r << ") = " << n << "! / (" << n << "-" << r
         << ")! = " << n << "! / " << (n - r) << "!\n";
    mostrarResultado(to_string(permSinRepeticion(n, r)));
    pausar();
}

void uiPermConRepeticionGrupos() {
    titulo("PERMUTACIONES CON REPETICION (elementos repetidos)");
    cout << "Formula: n! / (n1! * n2! * ... * nk!)\n";
    cout << "Se usa cuando algunos elementos se repiten (ej. letras de una palabra).\n\n";
    int n = leerEntero("Total de elementos (n, maximo 20): ", 1, 20);
    int k = leerEntero("Cuantos elementos distintos hay (k): ", 1, n);

    vector<int> grupos;
    int restante = n;
    for (int i = 1; i <= k; i++) {
        int ni;
        if (i == k) {
            ni = restante;   // el ultimo toma lo que falta para completar n
            cout << "El elemento " << i << " se repite " << ni
                 << " veces (lo que falta para completar n).\n";
        } else {
            int maxPermitido = restante - (k - i);
            ni = leerEntero("Cuantas veces se repite el elemento " + to_string(i)
                            + " (1 a " + to_string(maxPermitido) + "): ", 1, maxPermitido);
        }
        restante -= ni;
        grupos.push_back(ni);
    }

    string formula;
    for (size_t i = 0; i < grupos.size(); i++) {
        formula += to_string(grupos[i]) + "!";
        if (i + 1 < grupos.size()) formula += " * ";
    }
    cout << "\nProcedimiento:\n";
    cout << "  " << n << "! / (" << formula << ")\n";
    mostrarResultado(to_string(permConRepeticion(grupos)));
    pausar();
}

void uiPermPotencia() {
    titulo("PERMUTACIONES CON REPETICION (n^r)");
    cout << "Formula: n^r\n";
    cout << "Se usa cuando cada posicion puede llenarse con cualquiera de los n elementos.\n\n";
    int n = leerEntero("Cuantos elementos distintos hay (n): ", 1, 1000);
    int r = leerEntero("Cuantas posiciones hay (r): ", 0, 1000);

    ull res;
    if (!permPotencia(n, r, res)) {
        cout << C_ROJO << "\nEl resultado es demasiado grande para calcularlo." << C_RESET << "\n";
    } else {
        cout << "\nProcedimiento:\n  " << n << "^" << r << "\n";
        mostrarResultado(to_string(res));
    }
    pausar();
}

void menuPermutaciones() {
    int op;
    do {
        titulo("PERMUTACIONES");
        cout << "  [1] Sin repeticion\n";
        cout << "  [2] Con repeticion (elementos repetidos)\n";
        cout << "  [3] Con repeticion (n^r)\n";
        cout << "  [4] Volver al menu principal\n\n";
        op = leerEntero("  Elija una opcion: ", 1, 4);
        switch (op) {
            case 1: uiPermSinRepeticion(); break;
            case 2: uiPermConRepeticionGrupos(); break;
            case 3: uiPermPotencia(); break;
        }
    } while (op != 4);
}

// ==========================================================================
//  COMBINACIONES (interfaz)
// ==========================================================================
void uiCombSinRepeticion() {
    titulo("COMBINACIONES SIN REPETICION");
    cout << "Formula: C(n,r) = n! / (r! * (n-r)!)\n";
    cout << "Se usa cuando NO importa el orden y no se repiten elementos.\n\n";
    int n = leerEntero("Cantidad total de elementos (n, maximo 60): ", 1, 60);
    int r = leerEntero("Cuantos elementos se eligen (r, entre 0 y n): ", 0, n);

    cout << "\nProcedimiento:\n";
    cout << "  C(" << n << "," << r << ") = " << n << "! / (" << r << "! * "
         << (n - r) << "!)\n";
    mostrarResultado(to_string(combSinRepeticion(n, r)));
    pausar();
}

void uiCombConRepeticion() {
    titulo("COMBINACIONES CON REPETICION");
    cout << "Formula: C(n+r-1, r) = (n+r-1)! / (r! * (n-1)!)\n";
    cout << "Se usa cuando NO importa el orden y los elementos pueden repetirse.\n\n";
    int n = leerEntero("Cuantos tipos de elementos hay (n, maximo 30): ", 1, 30);
    int r = leerEntero("Cuantos elementos se eligen (r, maximo 30): ", 0, 30);

    cout << "\nProcedimiento:\n";
    cout << "  C(" << n << "+" << r << "-1," << r << ") = C(" << (n + r - 1)
         << "," << r << ")\n";
    mostrarResultado(to_string(combConRepeticion(n, r)));
    pausar();
}

void menuCombinaciones() {
    int op;
    do {
        titulo("COMBINACIONES");
        cout << "  [1] Sin repeticion\n";
        cout << "  [2] Con repeticion\n";
        cout << "  [3] Volver al menu principal\n\n";
        op = leerEntero("  Elija una opcion: ", 1, 3);
        switch (op) {
            case 1: uiCombSinRepeticion(); break;
            case 2: uiCombConRepeticion(); break;
        }
    } while (op != 3);
}

// ==========================================================================
//  GRAFOS (interfaz)
// ==========================================================================
void leerGrafo(Grafo& g) {
    int n = leerEntero("Cantidad de vertices (2 a 12): ", 2, MAXV);
    int tipo = leerEntero("Tipo de grafo (1 = No dirigido, 2 = Dirigido): ", 1, 2);
    bool dirigido = (tipo == 2);
    int maxAristas = dirigido ? n * (n - 1) : n * (n - 1) / 2;
    int m = leerEntero("Cantidad de aristas (1 a " + to_string(maxAristas) + "): ", 1, maxAristas);

    g = crearGrafo(n, dirigido, {});
    cout << "\nIngrese cada arista: origen, destino y peso (el peso debe ser mayor a 0).\n";
    int i = 1;
    while (i <= m) {
        cout << "\nArista " << i << " de " << m << ":\n";
        int o = leerEntero("  Vertice origen (1 a " + to_string(n) + "): ", 1, n);
        int d = leerEntero("  Vertice destino (1 a " + to_string(n) + "): ", 1, n);
        if (o == d) {
            cout << C_ROJO << "  Un vertice no puede conectarse consigo mismo. Intente de nuevo."
                 << C_RESET << "\n";
            continue;
        }
        if (g.peso[o][d] != 0) {
            cout << C_ROJO << "  Esa arista ya fue ingresada. Intente de nuevo." << C_RESET << "\n";
            continue;
        }
        int p = leerEntero("  Peso (1 a 1000): ", 1, 1000);
        g.peso[o][d] = p;
        if (!dirigido) g.peso[d][o] = p;
        i++;
    }
}

void mostrarGrafo(const Grafo& g) {
    cout << "\nMatriz de pesos (0 = sin arista):\n\n";
    cout << "      ";
    for (int j = 1; j <= g.n; j++) cout << setw(5) << j;
    cout << "\n";
    for (int i = 1; i <= g.n; i++) {
        cout << "  " << setw(2) << i << " |";
        for (int j = 1; j <= g.n; j++) cout << setw(5) << g.peso[i][j];
        cout << "\n";
    }
}

void uiRuta(bool corta) {
    titulo(corta ? "GRAFOS - RUTA MAS CORTA (algoritmo de Dijkstra)"
                 : "GRAFOS - RUTA MAS LARGA (sin repetir vertices)");
    Grafo g;
    leerGrafo(g);
    mostrarGrafo(g);

    int origen = leerEntero("\nVertice de origen: ", 1, g.n);
    int destino;
    do {
        destino = leerEntero("Vertice de destino: ", 1, g.n);
        if (destino == origen)
            cout << C_ROJO << "El destino debe ser distinto del origen." << C_RESET << "\n";
    } while (destino == origen);

    ResultadoRuta r = corta ? rutaMasCorta(g, origen, destino)
                            : rutaMasLarga(g, origen, destino);
    if (!r.existe) {
        cout << C_ROJO << "\nNo existe ruta entre " << origen << " y " << destino << "."
             << C_RESET << "\n";
    } else {
        if (!corta) cout << "\nSe evaluaron " << r.evaluadas << " rutas posibles.\n";
        mostrarResultado("Peso total = " + to_string(r.peso));
        cout << "\nRuta: " << rutaTexto(r.ruta, " -> ") << "\n";
    }
    pausar();
}

void menuGrafos() {
    int op;
    do {
        titulo("GRAFOS");
        cout << "  [1] Ruta mas corta\n";
        cout << "  [2] Ruta mas larga\n";
        cout << "  [3] Volver al menu principal\n\n";
        op = leerEntero("  Elija una opcion: ", 1, 3);
        switch (op) {
            case 1: uiRuta(true); break;
            case 2: uiRuta(false); break;
        }
    } while (op != 3);
}

// ==========================================================================
//  MODO DE PRUEBAS (verificacion automatica: esperado vs obtenido)
// ==========================================================================
int pruebasOk = 0, pruebasTotal = 0;

void seccionPruebas(const string& nombre) {
    cout << "\n" << C_AMAR << C_NEG << nombre << C_RESET << "\n";
}

void filaPrueba(const string& desc, const string& esperado, const string& obtenido) {
    bool ok = (esperado == obtenido);
    pruebasTotal++;
    if (ok) pruebasOk++;
    cout << left << setw(34) << desc << setw(21) << esperado << setw(21) << obtenido
         << (ok ? C_VERDE : C_ROJO) << (ok ? "[OK]" : "[FALLO]") << C_RESET << "\n";
}

string resumenRuta(const ResultadoRuta& r) {
    if (!r.existe) return "No existe ruta";
    return to_string(r.peso) + " | " + rutaTexto(r.ruta, "-");
}

void modoPruebas() {
    titulo("MODO DE PRUEBAS - RESULTADO ESPERADO vs OBTENIDO");
    pruebasOk = 0;
    pruebasTotal = 0;
    cout << left << setw(34) << "Prueba" << setw(21) << "Esperado" << setw(21) << "Obtenido"
         << "Estado\n";
    linea('-', 82);

    seccionPruebas("Permutaciones sin repeticion  P(n,r) = n!/(n-r)!");
    filaPrueba("P(5,3)", "60", to_string(permSinRepeticion(5, 3)));
    filaPrueba("P(10,4)", "5040", to_string(permSinRepeticion(10, 4)));
    filaPrueba("P(6,6)", "720", to_string(permSinRepeticion(6, 6)));
    filaPrueba("P(8,0)", "1", to_string(permSinRepeticion(8, 0)));
    filaPrueba("P(20,20)", "2432902008176640000", to_string(permSinRepeticion(20, 20)));

    seccionPruebas("Permutaciones con repeticion  n!/(n1! n2! ... nk!)");
    filaPrueba("MATEMATICA (2,3,2,1,1,1)", "151200", to_string(permConRepeticion({2, 3, 2, 1, 1, 1})));
    filaPrueba("BANANA (3,2,1)", "60", to_string(permConRepeticion({3, 2, 1})));
    filaPrueba("MISSISSIPPI (1,4,4,2)", "34650", to_string(permConRepeticion({1, 4, 4, 2})));
    filaPrueba("Grupos (2,1,2)", "30", to_string(permConRepeticion({2, 1, 2})));
    filaPrueba("AAAA (4)", "1", to_string(permConRepeticion({4})));

    seccionPruebas("Permutaciones con repeticion  n^r");
    ull p;
    permPotencia(3, 4, p);   filaPrueba("3^4", "81", to_string(p));
    permPotencia(2, 10, p);  filaPrueba("2^10", "1024", to_string(p));
    permPotencia(10, 3, p);  filaPrueba("10^3", "1000", to_string(p));
    permPotencia(26, 3, p);  filaPrueba("26^3", "17576", to_string(p));
    filaPrueba("10^30 (desborda)", "Desborda", permPotencia(10, 30, p) ? "Calculado" : "Desborda");

    seccionPruebas("Combinaciones sin repeticion  C(n,r) = n!/(r!(n-r)!)");
    filaPrueba("C(5,3)", "10", to_string(combSinRepeticion(5, 3)));
    filaPrueba("C(10,4)", "210", to_string(combSinRepeticion(10, 4)));
    filaPrueba("C(6,6)", "1", to_string(combSinRepeticion(6, 6)));
    filaPrueba("C(52,5)  manos de poker", "2598960", to_string(combSinRepeticion(52, 5)));
    filaPrueba("C(20,10)", "184756", to_string(combSinRepeticion(20, 10)));

    seccionPruebas("Combinaciones con repeticion  C(n+r-1, r)");
    filaPrueba("n=3, r=2", "6", to_string(combConRepeticion(3, 2)));
    filaPrueba("n=5, r=3", "35", to_string(combConRepeticion(5, 3)));
    filaPrueba("n=4, r=10", "286", to_string(combConRepeticion(4, 10)));
    filaPrueba("n=10, r=5", "2002", to_string(combConRepeticion(10, 5)));

    seccionPruebas("Grafos (formato: peso | ruta)");
    Grafo g1 = crearGrafo(5, false, {{1, 2, 4}, {1, 3, 2}, {2, 3, 1}, {2, 4, 5},
                                     {3, 4, 8}, {3, 5, 10}, {4, 5, 2}});
    filaPrueba("G1 no dirigido, corta 1->5", "10 | 1-3-2-4-5", resumenRuta(rutaMasCorta(g1, 1, 5)));
    filaPrueba("G1 no dirigido, larga 1->5", "27 | 1-2-4-3-5", resumenRuta(rutaMasLarga(g1, 1, 5)));

    Grafo g2 = crearGrafo(4, true, {{1, 2, 3}, {1, 3, 1}, {3, 2, 1}, {2, 4, 2}, {3, 4, 6}});
    filaPrueba("G2 dirigido, corta 1->4", "4 | 1-3-2-4", resumenRuta(rutaMasCorta(g2, 1, 4)));
    filaPrueba("G2 dirigido, larga 1->4", "7 | 1-3-4", resumenRuta(rutaMasLarga(g2, 1, 4)));
    filaPrueba("G2 dirigido, corta 4->1", "No existe ruta", resumenRuta(rutaMasCorta(g2, 4, 1)));

    Grafo g3 = crearGrafo(4, false, {{1, 2, 1}, {2, 3, 2}, {3, 4, 3}, {1, 4, 10}});
    filaPrueba("G3 no dirigido, corta 1->4", "6 | 1-2-3-4", resumenRuta(rutaMasCorta(g3, 1, 4)));
    filaPrueba("G3 no dirigido, larga 1->4", "10 | 1-4", resumenRuta(rutaMasLarga(g3, 1, 4)));

    cout << "\n";
    linea('-', 82);
    bool todo = (pruebasOk == pruebasTotal);
    cout << (todo ? C_VERDE : C_ROJO) << C_NEG
         << "Pruebas superadas: " << pruebasOk << " de " << pruebasTotal
         << (todo ? "  -> TODO CORRECTO" : "  -> HAY ERRORES") << C_RESET << "\n";
    pausar();
}

// ==========================================================================
//  MENU PRINCIPAL
// ==========================================================================
int main() {
    iniciarConsola();
    int opcion;

    do {
        titulo("PROYECTO DE CURSO - " + CURSO);
        cout << "  Estudiante  : " << ESTUDIANTE << "\n";
        cout << "  Carnet      : " << CARNET << "\n";
        cout << "  Universidad : " << UNIVERSIDAD << "\n\n";
        linea('-');
        cout << "  [1] Permutaciones\n";
        cout << "  [2] Combinaciones\n";
        cout << "  [3] Grafos (ruta mas corta / ruta mas larga)\n";
        cout << "  [4] Modo de pruebas (verificacion automatica)\n";
        cout << "  [5] Salir\n";
        linea('-');
        opcion = leerEntero("  Elija una opcion: ", 1, 5);

        switch (opcion) {
            case 1: menuPermutaciones(); break;
            case 2: menuCombinaciones(); break;
            case 3: menuGrafos(); break;
            case 4: modoPruebas(); break;
            case 5: cout << "\nHasta luego!\n"; break;
        }
    } while (opcion != 5);

    return 0;
}