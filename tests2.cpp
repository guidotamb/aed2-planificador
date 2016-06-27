// g++ -g tests.cpp -o tests
// valgrind --leak-check=full -v ./tests

#include <algorithm>
#include "mini_test.h"
#include "PlanificadorRR.h"

using namespace std;

string remove_spaces(const string& s) {
    string out(s);
    out.erase(remove(out.begin(), out.end(), ' '), out.end());
    out.erase(remove(out.begin(), out.end(), '\n'), out.end());
    return out;
}

template<typename T>
string to_s(const T& m) {
    ostringstream os;
    os << m;
    return os.str();
}

class NoAsignableNiConstruiblePorDefecto {
    int x;

    NoAsignableNiConstruiblePorDefecto& operator=(NoAsignableNiConstruiblePorDefecto);
    NoAsignableNiConstruiblePorDefecto();

public:
    int get() const {return x;};

    NoAsignableNiConstruiblePorDefecto(int y) : x(y) {};

    NoAsignableNiConstruiblePorDefecto(
            const NoAsignableNiConstruiblePorDefecto &other) : x(other.x) {};

    bool operator==(const NoAsignableNiConstruiblePorDefecto& other) {
        return x == other.x;
    }
};

ostream& operator<<(ostream& out, const NoAsignableNiConstruiblePorDefecto& a) {
    return out << a.get();
}

/**
 * Crea un planificador sin porocesos.
 */
void planificadorVacio() {
    PlanificadorRR<int> planificador;
    ASSERT_EQ(planificador.hayProcesos(), false);
    ASSERT_EQ(planificador.hayProcesosActivos(), false);
    ASSERT_EQ(to_s(planificador), "[]");
}

void agregarProcesos() {
    PlanificadorRR<int> planificador;
    planificador.agregarProceso(123);
    ASSERT_EQ(planificador.cantidadDeProcesos(), 1);
    ASSERT_EQ(planificador.cantidadDeProcesosActivos(), 1);
    ASSERT_EQ(planificador.esPlanificado(123), true);
    ASSERT_EQ(planificador.estaActivo(123), true);
    planificador.agregarProceso(321);
    ASSERT_EQ(planificador.cantidadDeProcesos(), 2);
    ASSERT_EQ(planificador.cantidadDeProcesosActivos(), 2);
    ASSERT_EQ(planificador.esPlanificado(321), true);
    ASSERT_EQ(planificador.estaActivo(321), true);
}

/**
 */
void basic() {
    PlanificadorRR<int> p;
    p.agregarProceso(0);
    p.agregarProceso(1);
    p.agregarProceso(2);
    p.agregarProceso(3);
    p.pausarProceso(2);
    p.pausarProceso(0);
    ASSERT_EQ(p.hayProcesos(), true);
    ASSERT_EQ(p.hayProcesosActivos(), true);
    ASSERT_EQ(p.cantidadDeProcesos(), 4);
    ASSERT_EQ(p.cantidadDeProcesosActivos(), 2);
    ASSERT_EQ(to_s(p), "[1*, 2 (i), 3, 0 (i)]");
}

/**
 */
void copy() {
    PlanificadorRR<int>* pp = new PlanificadorRR<int>();
    //PlanificadorRR<int> p = *pp;
#define p (*pp)
    p.agregarProceso(0);
    p.pausarProceso(0);
    p.agregarProceso(1);

    ASSERT_EQ(p.hayProcesos(), true);
    ASSERT_EQ(p.hayProcesosActivos(), true);
    ASSERT_EQ(p.cantidadDeProcesos(), 2);
    ASSERT_EQ(p.cantidadDeProcesosActivos(), 1);
    ASSERT_EQ(to_s(p), "[1*, 0 (i)]");

    p.agregarProceso(2);
    p.agregarProceso(3);
    p.pausarProceso(2);

    ASSERT_EQ(to_s(p), "[1*, 0 (i), 2 (i), 3]");

    PlanificadorRR<int> q(p);

    ASSERT(p == q);

    q.ejecutarSiguienteProceso();
    q.agregarProceso(4);
    p.eliminarProceso(3);
    p.ejecutarSiguienteProceso();

    ASSERT_EQ(to_s(p), "[1*, 0 (i), 2 (i)]");
    ASSERT_EQ(to_s(q), "[3*, 1, 0 (i), 2 (i), 4]");

    ASSERT(!(p == q));

    delete pp;

    ASSERT_EQ(to_s(q), "[3*, 1, 0 (i), 2 (i), 4]");
}

/**
 */
void restricted() {
    NoAsignableNiConstruiblePorDefecto p0 = NoAsignableNiConstruiblePorDefecto(0);
    NoAsignableNiConstruiblePorDefecto p1 = NoAsignableNiConstruiblePorDefecto(1);
    NoAsignableNiConstruiblePorDefecto p2 = NoAsignableNiConstruiblePorDefecto(2);
    NoAsignableNiConstruiblePorDefecto p3 = NoAsignableNiConstruiblePorDefecto(3);
    NoAsignableNiConstruiblePorDefecto p4 = NoAsignableNiConstruiblePorDefecto(4);

    PlanificadorRR<NoAsignableNiConstruiblePorDefecto>* pp =
      new PlanificadorRR<NoAsignableNiConstruiblePorDefecto>();
    //PlanificadorRR<NoAsignableNiConstruiblePorDefecto> p = *pp;
# define p (*pp)

    PlanificadorRR<NoAsignableNiConstruiblePorDefecto> r(p);

    
    ASSERT_EQ(p == r, true);

    p.agregarProceso(p4);
    p.ejecutarSiguienteProceso();

    cout << to_s(p);
    ASSERT_EQ(to_s(p), "[4*]");

    p.pausarProceso(p4);

    ASSERT_EQ(to_s(p), "[4 (i)]");

    p.eliminarProceso(p4);

    p.agregarProceso(p0);
    p.pausarProceso(p0);
    p.agregarProceso(p1);

    ASSERT_EQ(p.hayProcesos(), true);
    ASSERT_EQ(p.hayProcesosActivos(), true);
    ASSERT_EQ(p.cantidadDeProcesos(), 2);
    ASSERT_EQ(p.cantidadDeProcesosActivos(), 1);
    ASSERT_EQ(p.esPlanificado(p1), true);
    ASSERT_EQ(p.esPlanificado(p4), false);
    ASSERT_EQ(p.estaActivo(p1), true);
    ASSERT_EQ(p.estaActivo(p0), false);
    ASSERT_EQ(to_s(p), "[1*, 0 (i)]");

    p.agregarProceso(p2);
    p.agregarProceso(p3);
    p.pausarProceso(p2);

    ASSERT_EQ(to_s(p), "[1*, 0 (i), 2 (i), 3]");

    PlanificadorRR<NoAsignableNiConstruiblePorDefecto> q(p);

    ASSERT(p == q);

    p.detener();
    ASSERT(!(p == q));
    ASSERT((p.detenido()));
    ASSERT(!(q.detenido()));
    p.reanudar();

    q.ejecutarSiguienteProceso();
    q.agregarProceso(p4);
    p.eliminarProceso(p3);
    p.ejecutarSiguienteProceso();

    ASSERT_EQ(to_s(p), "[1*, 0 (i), 2 (i)]");
    ASSERT_EQ(to_s(q), "[3*, 1, 0 (i), 2 (i), 4]");

    ASSERT(!(p == q));

    delete pp;

    ASSERT_EQ(to_s(q), "[3*, 1, 0 (i), 2 (i), 4]");

    q.eliminarProceso(p0);

    ASSERT_EQ(to_s(q), "[3*, 1, 2 (i), 4]");
    //ASSERT((q.procesoEjecutado() == p3));
    ASSERT_EQ(q.hayProcesos(), true);
    ASSERT_EQ(q.hayProcesosActivos(), true);
    ASSERT_EQ(q.cantidadDeProcesos(), 4);
    ASSERT_EQ(q.cantidadDeProcesosActivos(), 3);

    q.eliminarProceso(p4);
    q.eliminarProceso(p3);

    q.eliminarProceso(p1);
    cout << to_s(q);
    ASSERT_EQ(to_s(q), "[2 (i)]");
    ASSERT_EQ(q.hayProcesos(), true);
    ASSERT_EQ(q.hayProcesosActivos(), false);
    ASSERT_EQ(q.cantidadDeProcesos(), 1);
    ASSERT_EQ(q.cantidadDeProcesosActivos(), 0);

    q.reanudarProceso(p2);
    q.pausarProceso(p2);
    q.eliminarProceso(p2);

    ASSERT_EQ(to_s(q), "[]");
    ASSERT_EQ(q.hayProcesos(), false);
    ASSERT_EQ(q.hayProcesosActivos(), false);
    ASSERT_EQ(q.cantidadDeProcesos(), 0);
    ASSERT_EQ(q.cantidadDeProcesosActivos(), 0);

}

int main() {
    // RUN_TEST( planificadorVacio );
    RUN_TEST(agregarProcesos);
    // RUN_TEST( basic );
    // RUN_TEST( copy );
    // RUN_TEST( restricted );

    return 0;
}