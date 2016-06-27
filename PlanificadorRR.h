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

}

/**
 * Acordarse de liberar toda la memoria!
 */	 
template<class T>
PlanificadorRR<T>::~PlanificadorRR(){
	
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
	// assert(!esPlanificado(pid))
	Nodo* nuevo = new Nodo(pid);
	if (this->cantidadProcesos == 0) {
		this->procesoActual = nuevo;
		this->procesoActual->siguiente = this->procesoActual;
		this->procesoActual->anterior = this->procesoActual;
	} else {
		Nodo* ultimo = this->procesoActual->anterior;
		this->procesoActual->anterior = nuevo;
		nuevo->anterior = ultimo;
		ultimo->siguiente = nuevo;
		nuevo->siguiente = this->procesoActual;
	}
	if(cantidadDeProcesosActivos() == 0){
		this->procesoActual = nuevo;
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
	
}

/**
 * Devuelve el proceso que está actualmente en ejecución.
 * PRE: Hay al menos un proceso activo en el planificador.
 */
template<class T>
const T& PlanificadorRR<T>::procesoEjecutado() const{
	
}

/**
 * Procede a ejecutar el siguiente proceso activo,
 * respetando el orden de planificación.
 * PRE: Hay al menos un proceso activo en el planificador.
 */
template<class T>
void PlanificadorRR<T>::ejecutarSiguienteProceso(){
	
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
	
}

/**
 * Detiene la ejecución de todos los procesos en el planificador
 * para atender una interrupción del sistema.
 * PRE: El planificador no está detenido.
 */
template<class T>
void PlanificadorRR<T>::detener(){
	
}

/**
 * Reanuda la ejecución de los procesos (activos) en el planificador
 * luego de atender una interrupción del sistema.
 * PRE: El planificador está detenido.
 */
template<class T>
void PlanificadorRR<T>::reanudar(){
	
}

/**
 * Informa si el planificador está detenido por el sistema operativo.
 */
template<class T>
bool PlanificadorRR<T>::detenido() const{
	
}

/**
 * Informa si un cierto proceso está siendo planificado por el planificador.
 */
template<class T>
bool PlanificadorRR<T>::esPlanificado(const T& p) const{
	if (cantidadProcesos > 0) {
		int i;
		Nodo* actual = this->procesoActual;
		for (i=0; i < this.cantidadProcesos; i++) {
			if (actual->pid == p) {
				return true;
			} else {
				actual = actual->siguiente;
			}
		}
		return false;
	} else {
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
	Nodo* actual = this->procesoActual;
	for (i = 0; i < this->cantidadProcesos; i++) {
		if (actual->pid == p) {
			return !(actual->pausado);
		}
		actual = actual->siguiente;
	}
	return false;
}

/**
 * Informa si existen procesos planificados.
 */
template<class T>
bool PlanificadorRR<T>::hayProcesos() const{
	
}

/**
 * Informa si existen procesos activos.
 */
template<class T>
bool PlanificadorRR<T>::hayProcesosActivos() const{
	
}

/**
 * Devuelve la cantidad de procesos planificados.
 */
template<class T>
int PlanificadorRR<T>::cantidadDeProcesos() const{
	
}

/**
 * Devuelve la cantidad de procesos planificados y activos.
 */
template<class T>
int PlanificadorRR<T>::cantidadDeProcesosActivos() const{
	
}

/**
 * Devuelve true si ambos planificadores son iguales.
 */
template<class T>
bool PlanificadorRR<T>::operator==(const PlanificadorRR<T>& p) const{

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
	
}

template<class T>
ostream& operator<<(ostream& out, const PlanificadorRR<T>& a) {
	
}

//Metodos auxiliares
template<class T>
typename PlanificadorRR<T>::Nodo* PlanificadorRR<T>::dameProceso(const T& p){
	
}

#endif // PLANIFICADOR_RR_H_
