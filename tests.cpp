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

/**
 * Crea un planificador sin porocesos.
 */
void planificadorVacio()
{
  cout << "Inicio test" << endl;
  PlanificadorRR<int> planificador;
  // PlanificadorRR<int> planificador2;
  int pid = 1234;
  
  planificador.agregarProceso(pid);
  ASSERT_EQ(planificador.esPlanificado(pid), true);
  cout << "Proceso numero: " << planificador.procesoEjecutado() << "." << endl;
  ASSERT_EQ(to_s(planificador), "[1234*]");
  planificador.pausarProceso(pid);
  ASSERT_EQ(to_s(planificador), "[1234 (i)]");
  cout << "Pauso proceso" << endl;
  ASSERT_EQ(planificador.estaActivo(pid), false);
  planificador.eliminarProceso(pid);
  cout << "Elimino proceso" << endl;
  
  ASSERT_EQ(planificador.hayProcesos(), false);
  cout << "Planificador vacio" << endl;
  ASSERT_EQ(planificador.hayProcesosActivos(), false);
  cout << "No hay procesos" << endl;
  ASSERT_EQ(planificador.cantidadDeProcesos(), 0);
  cout << "Cantidad de procesos: " << planificador.cantidadDeProcesos() << endl;
  ASSERT_EQ(to_s(planificador), "[]");
}

void planificadorDetenido(){
  PlanificadorRR<int> planificador;
  int pid = 1234;
  planificador.agregarProceso(pid);
  pid = 4321;
  planificador.agregarProceso(pid);
  ASSERT_EQ(planificador.cantidadDeProcesos(), 2);
  ASSERT_EQ(planificador.detenido(), false);
  planificador.detener();
  ASSERT_EQ(planificador.detenido(), true);
}

void borrarProcesos(){
  PlanificadorRR<int> planificador;
  int pid = 1234;
  planificador.agregarProceso(pid);
  pid = 4321;
  planificador.agregarProceso(pid);
  pid = 1432;
  planificador.agregarProceso(pid);
  ASSERT_EQ(planificador.cantidadDeProcesos(), 3);
  planificador.eliminarProceso(1234);
  ASSERT_EQ(planificador.cantidadDeProcesos(), 2);
  planificador.eliminarProceso(4321);
  ASSERT_EQ(planificador.cantidadDeProcesos(), 1);
  planificador.pausarProceso(1432);
  ASSERT_EQ(planificador.esPlanificado(1432), true);
  ASSERT_EQ(planificador.cantidadDeProcesosActivos(), 0);
  planificador.reanudarProceso(1432);
}

void copiaPlanificador(){
  PlanificadorRR<int>* pp = new PlanificadorRR<int>();
    //PlanificadorRR<int> p = *pp;
#define p (*pp)
    p.agregarProceso(0);
    p.pausarProceso(0);
    p.agregarProceso(1);

    ASSERT_EQ(p.hayProcesos(), true);
    ASSERT_EQ(p.hayProcesosActivos(), true);
    ASSERT_EQ(p.cantidadDeProcesos(), 2);
    // cout << p.cantidadDeProcesosActivos() << endl;
    ASSERT_EQ(p.cantidadDeProcesosActivos(), 1);

    ASSERT_EQ(to_s(p), "[1*, 0 (i)]");

    p.agregarProceso(2);
    p.agregarProceso(3);
    p.pausarProceso(2);

    ASSERT_EQ(to_s(p), "[1*, 0 (i), 2 (i), 3]");

    PlanificadorRR<int> q(p);

    ASSERT_EQ(p == q, true);

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

int main()
{
  RUN_TEST( planificadorVacio );
  RUN_TEST( planificadorDetenido );
  RUN_TEST( borrarProcesos );
  RUN_TEST( copiaPlanificador );
  // AGREGAR MAS TESTS...

  return 0;
}
