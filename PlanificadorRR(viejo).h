#ifndef PLANIFICADOR_RR_H_
#define PLANIFICADOR_RR_H_

#include <iostream>
#include <cassert>
using namespace std;

/**
 * Se puede asumir que el tipo T tiene constructor por copia y operator==
 * No se puede asumir que el tipo T tenga operator=
 */
template<typename T>
class PlanificadorRR {

  public:

	PlanificadorRR();
	PlanificadorRR(const PlanificadorRR<T>&);
	~PlanificadorRR();
	void agregarProceso(const T&);
	void eliminarProceso(const T&);
	const T& procesoEjecutado() const;
	void ejecutarSiguienteProceso();
	void pausarProceso(const T&);
	void reanudarProceso(const T&);
	void detener();
	void reanudar();
	bool detenido() const;
	bool esPlanificado(const T&) const;
	bool estaActivo(const T&) const;
	bool hayProcesos() const;
	bool hayProcesosActivos() const;
	int cantidadDeProcesos() const;
	int cantidadDeProcesosActivos() const;
	bool operator==(const PlanificadorRR<T>&) const;
	ostream& mostrarPlanificadorRR(ostream&) const;

  private:
  
	PlanificadorRR<T>& operator=(const PlanificadorRR<T>& otra) {
		assert(false);
		return *this;
	}

	struct Nodo {
		T pid;
		bool pausado;
		Nodo* siguiente;
		Nodo* anterior;
		Nodo(const T& p): pid(p), pausado(false), siguiente(NULL), anterior(NULL){}
	};

	Nodo* dameProceso(const T&);
	int cantidadProcesosActivos() const;

	Nodo* procesoActual;
	int cantidadProcesos;
	bool planificadorDetenido;
};

/**
 * Crea un nuevo planificador de tipo Round Robin.
 */	
template<class T>
PlanificadorRR<T>::PlanificadorRR(){
	procesoActual = NULL;
	cantidadProcesos = 0;
	planificadorDetenido = false;
}

/**
 * Una vez copiado, ambos planificadores deben ser independientes, 
 * es decir, por ejemplo, que cuando se borra un proceso en uno
 * no debe borrarse en el otro.
 */	
template<class T>
PlanificadorRR<T>::PlanificadorRR(const PlanificadorRR<T>& p){
	if(p.cantidadDeProcesos() == 0){
		procesoActual = NULL;
		cantidadProcesos = 0;
		planificadorDetenido = false;
	}else{
		cantidadProcesos = p.cantidadDeProcesos();
		planificadorDetenido = p.detenido();
		T pid = p.procesoEjecutado();
		procesoActual = new Nodo(pid);
		procesoActual->pausado = !p.estaActivo(pid);
		procesoActual->siguiente = procesoActual;
		procesoActual->anterior = procesoActual;
		int i;
		Nodo* actual = procesoActual;
		Nodo* copia = p.procesoActual;
		for(i=1; i<cantidadProcesos; i++){
			copia = copia->siguiente;
			Nodo* siguiente = new Nodo(copia->pid);
			siguiente->pausado = copia->pausado;
			siguiente->anterior = actual;
			actual->siguiente = siguiente;
			actual = siguiente;
		}
		actual->siguiente = procesoActual;
		procesoActual->anterior = actual;
	}
}

/**
 * Acordarse de liberar toda la memoria!
 */	 
template<class T>
PlanificadorRR<T>::~PlanificadorRR(){
	int i;
	Nodo* borrador = procesoActual;
	for(i = 0; i < cantidadProcesos; i++){
		Nodo* siguiente = borrador->siguiente;
		eliminarProceso(borrador->pid);
		borrador = siguiente;
	}
}

/**
 * Agrega un proceso al planificador. El mismo debe ubicarse,
 * dentro del orden de ejecución, inmediatamente antes del que está
 * siendo ejecutado actualmente. Si no hubiese ningún proceso en ejecución,
 * la posición es arbitraria y el proceso pasa a ser ejecutado automáticamente.
 * PRE: El proceso no está siendo planificado por el planificador.
 */
template<class T>
void PlanificadorRR<T>::agregarProceso(const T& pid){
	assert(!esPlanificado(pid));
	Nodo* nuevoProceso = new Nodo(pid);
	if (cantidadProcesos == 0) {
		procesoActual = nuevoProceso;
		procesoActual->siguiente = procesoActual;
		procesoActual->anterior = procesoActual;
	} else {
		Nodo* ultimo = procesoActual->anterior;
		procesoActual->anterior = nuevoProceso;
		nuevoProceso->anterior = ultimo;
		ultimo->siguiente = nuevoProceso;
		nuevoProceso->siguiente = procesoActual;
	}
	if(cantidadDeProcesosActivos() == 0){
		procesoActual = nuevoProceso;
	}
	cantidadProcesos++;
}

/**
 * Elimina un proceso del planificador. Si el proceso eliminado
 * está actualmente en ejecución, automáticamente pasa a ejecutarse
 * el siguiente (si es que existe).
 * PRE: El proceso está siendo planificado por el planificador.
 */
template<class T>
void PlanificadorRR<T>::eliminarProceso(const T& p){
	assert(esPlanificado(p));
	Nodo* aEliminar = dameProceso(p);
	cout << "Nodo a eliminar: " << aEliminar->pid << endl;
	if (cantidadProcesos > 1) {
		//Reorganizo relaciones;
		if(cantidadProcesos <= 2){
			aEliminar->siguiente->siguiente = aEliminar->siguiente;
			aEliminar->siguiente->anterior = aEliminar->siguiente;
		}else{
			Nodo* anterior = aEliminar->anterior;
			Nodo* siguiente = aEliminar->siguiente;
			anterior->siguiente = aEliminar->siguiente;
			siguiente->anterior = aEliminar->anterior;	
		}
		if(aEliminar->pid == procesoActual->pid){
			//Pasa al que sigue

			procesoActual = procesoActual->siguiente;
			int i = 1;
			if(cantidadProcesos > 2){
				while(i < cantidadProcesos && procesoActual->pausado){
					procesoActual = procesoActual->siguiente;	
				}
			}
		}
	}
	delete aEliminar;
	cantidadProcesos--;
}
/**template<class T>
void PlanificadorRR<T>::eliminarProceso(const T& p){
	assert(esPlanificado(p));
	Nodo* aEliminar = dameProceso(p);
	if(cantidadProcesos > 1){
		
	}
}*/

/**
 * Devuelve el proceso que está actualmente en ejecución.
 * PRE: Hay al menos un proceso activo en el planificador.
 */
template<class T>
const T& PlanificadorRR<T>::procesoEjecutado() const{
	assert(cantidadProcesos > 0);
	return procesoActual->pid;
}

/**
 * Procede a ejecutar el siguiente proceso activo,
 * respetando el orden de planificación.
 * PRE: Hay al menos un proceso activo en el planificador.
 */
template<class T>
void PlanificadorRR<T>::ejecutarSiguienteProceso(){
	assert(cantidadDeProcesosActivos() > 0);
	procesoActual = procesoActual->siguiente;
	while(procesoActual->pausado){
		procesoActual = procesoActual->siguiente;	
	}
}

/**
 * Pausa un proceso por tiempo indefinido. Este proceso pasa
 * a estar inactivo y no debe ser ejecutado por el planificador.
 * Si el proceso pausado está actualmente en ejecución, automáticamente
 * pasa a ejecutarse el siguiente (si es que existe).
 * PRE: El proceso está siendo planificado por el planificador.
 * PRE: El proceso está activo.
 */
template<class T>
void PlanificadorRR<T>::pausarProceso(const T& p){
	assert(esPlanificado(p));
	Nodo* proceso = dameProceso(p);
	if(proceso == procesoActual && cantidadDeProcesosActivos() > 0){
		procesoActual = procesoActual->siguiente;
		while(procesoActual->pausado){
			procesoActual = procesoActual->siguiente;
		}
	}
	assert(!proceso->pausado);
	// Nodo* procesoAPausar = &proceso;
	proceso->pausado = true;
}

/**
 * Reanuda un proceso previamente pausado. Este proceso pasa a estar
 * nuevamente activo dentro del planificador. Si no había ningún proceso
 * en ejecución, el proceso pasa a ser ejecutado automáticamente.
 * PRE: El proceso está siendo planificado por el planificador.
 * PRE: El proceso está inactivo.
 */
template<class T>
void PlanificadorRR<T>::reanudarProceso(const T&p){
	Nodo* proceso = dameProceso(p);
	assert(esPlanificado(p) && proceso->pausado);
	proceso->pausado = true;
}

/**
 * Detiene la ejecución de todos los procesos en el planificador
 * para atender una interrupción del sistema.
 * PRE: El planificador no está detenido.
 */
template<class T>
void PlanificadorRR<T>::detener(){
	assert(!planificadorDetenido);
	planificadorDetenido = true;
}

/**
 * Reanuda la ejecución de los procesos (activos) en el planificador
 * luego de atender una interrupción del sistema.
 * PRE: El planificador está detenido.
 */
template<class T>
void PlanificadorRR<T>::reanudar(){
	assert(planificadorDetenido);
	planificadorDetenido = false;
}

/**
 * Informa si el planificador está detenido por el sistema operativo.
 */
template<class T>
bool PlanificadorRR<T>::detenido() const{
	return planificadorDetenido;
}

/**
 * Informa si un cierto proceso está siendo planificado por el planificador.
 */
template<class T>
bool PlanificadorRR<T>::esPlanificado(const T& p) const{
	if(cantidadProcesos > 0){
		int i;
		Nodo* actual = procesoActual;
		for (i=0; i < cantidadProcesos; i++) {
			if (actual->pid == p) {
				return true;
			} else {
				actual = actual->siguiente;
			}
		}
		return false;
	}else{
		return false;
	}
}

/**
 * Informa si un cierto proceso está activo en el planificador.
 * PRE: El proceso está siendo planificado por el planificador.
 */
template<class T>
bool PlanificadorRR<T>::estaActivo(const T& p) const{
	assert(esPlanificado(p));
	int i;
	Nodo* actual = procesoActual;
	for (i = 0; i < cantidadProcesos; i++) {
		if (actual->pid == p) {
			return !(actual->pausado);
		}
		actual = actual->siguiente;
	}
	// Nodo* procesoActivo = &proceso;
	return false;
}

/**
 * Informa si existen procesos planificados.
 */
template<class T>
bool PlanificadorRR<T>::hayProcesos() const{
	return cantidadProcesos > 0;
}

/**
 * Informa si existen procesos activos.
 */
template<class T>
bool PlanificadorRR<T>::hayProcesosActivos() const{
	return cantidadDeProcesosActivos() > 0;
}

/**
 * Devuelve la cantidad de procesos planificados.
 */
template<class T>
int PlanificadorRR<T>::cantidadDeProcesos() const{
	return cantidadProcesos;
}

/**
 * Devuelve la cantidad de procesos planificados y activos.
 */
template<class T>
int PlanificadorRR<T>::cantidadDeProcesosActivos() const{
	if(cantidadProcesos > 0){
		int procesosActivos = 0;
		int i = 0;
		Nodo* actual = procesoActual;
		for (i=0; i < cantidadProcesos; i++) {
			if (!actual->pausado) {
				procesosActivos++;
			}
			actual = actual->siguiente;
		}
		return procesosActivos;
	}
	return 0;
}

/**
 * Devuelve true si ambos planificadores son iguales.
 */
template<class T>
bool PlanificadorRR<T>::operator==(const PlanificadorRR<T>& p) const{
	if (cantidadProcesos == 0 && p.cantidadDeProcesos() == 0){
		return true;
	}
	bool result = (cantidadProcesos == p.cantidadDeProcesos()) && p.detenido() == planificadorDetenido;
	if (!result){
		return false;
	}
	int i = 1;
	T pid = p.procesoEjecutado();
	result = procesoActual->pid == pid && procesoActual->pausado == !p.estaActivo(pid);
	Nodo* it1 = procesoActual->siguiente;
	Nodo* it2 = p.procesoActual->siguiente;
	while (result && i<cantidadProcesos){
		result = result && it1->pid == it2->pid && it1->pausado == !p.estaActivo(it2->pid);
		it1 = it1->siguiente;
		it2 = it2->siguiente;
		i++;
	}
	return result; //result;

}

/**
 * Debe mostrar los procesos planificados por el ostream (y retornar el mismo).
 * Los procesos deben aparecer en el mismo orden en el que son ejecutados
 * por el planificador. Como la lista es circular, se decidió que el primer
 * proceso que se muestra debe ser el que está siendo ejecutado en ese momento.
 * En el caso particular donde exista al menos un proceso planificado,
 * pero estén todos pausados, se puede comenzar por cualquier proceso.
 * Un proceso inactivo debe ser identificado con el sufijo ' (i)'
 * y el proceso que está siendo ejecutado, de existir, debe ser identificado
 * con el sufijo '*'.
 * PlanificadorRR vacio: []
 * PlanificadorRR con 1 elemento activo: [p0*]
 * PlanificadorRR con 2 elementos inactivos: [p0 (i), p1 (i)]
 * PlanificadorRR con 3 elementos (p0 inactivo, p2 siendo ejecutado: [p2*, p0 (i), p1]
 *
 * OJO: con pX (p0, p1, p2) nos referimos a lo que devuelve el operador <<
 * para cada proceso, es decir, cómo cada proceso decide mostrarse en el sistema.
 * El sufijo 'X' indica el orden relativo de cada proceso en el planificador.
 */
template<class T>
ostream& PlanificadorRR<T>::mostrarPlanificadorRR(ostream& os) const{
	if(cantidadProcesos == 0){
			os << "[]";
	}else{
		os <<"[";
		Nodo* mostrado = procesoActual;
		int i=0;
		while(i<cantidadProcesos){
			os<<mostrado->pid;
			if(mostrado->pausado){
				os<<" (i)";
			}else if(procesoActual == mostrado){
				os<<"*";
			}
			i++;
			if (i==cantidadProcesos){
				os<<"]";
			}else{
				os<<", ";
			}
			mostrado = mostrado->siguiente;
		}

	}
	// return os;
}

template<class T>
ostream& operator<<(ostream& out, const PlanificadorRR<T>& a) {
	return a.mostrarPlanificadorRR(out);
}

//Metodos auxiliares
template<class T>
typename PlanificadorRR<T>::Nodo* PlanificadorRR<T>::dameProceso(const T& p){
	int i;
	Nodo* actual = procesoActual;
	for (i=0; i < cantidadProcesos; i++) {
		if (actual->pid == p) {
			return actual;
		} else {
			actual = actual->siguiente;
		}
	}
	// cout << "No existe";
	return NULL;
}

#endif // PLANIFICADOR_RR_H_
