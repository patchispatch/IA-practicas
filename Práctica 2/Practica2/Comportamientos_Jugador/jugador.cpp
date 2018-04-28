//#include "../Comportamientos_Jugador/jugador.hpp"
#include "jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include <algorithm>


bool operator< (const estado &uno, const estado &otro) {

	return uno.coste_f < otro.coste_f;
}

bool operator== (const estado &uno, const estado &otro) {

	return uno.fila == otro.fila && uno.columna == otro.columna;
}

bool operator!= (const estado &uno, const estado &otro) {

	return !(uno == otro);
}

void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}

void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}

void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}


bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {

	/*
	Usamos una cola con prioridad para abiertos y una matriz de booleanos para cerrados.
	Como siempre sacaremos de abiertos la casilla con menor coste, hemos sobrecargado el operator< para que se ordene así.
	Para cerrados, usamos una matriz de booleanos del tamaño del mapa, donde si el valor es true, esa casilla no se debe considerar.
	Cada vez que analicemos un estado, comprobaremos si ya está marcado como cerrado en esta matriz.
	*/
	list<estado> abiertos;
	list<estado> cerrados;
	bool abiertos_index[100][100];
	bool cerrados_index[100][100];

	//Inicializamos la matriz de cerrados_index y abiertos_index:
	for (int i = 0; i < mapaResultado.size(); ++i) {
		for (int j = 0; j < mapaResultado.size(); ++j) {
			cerrados_index[i][j] = false;
			abiertos_index[i][j] = false;
		}
	}


	//Usamos también una lista ruta, para almacenar el path conforme lo calculamos. Después lo usaremos para definir el plan.
	list<estado> ruta;

	/*
	Definimos cuatro estados a partir de uno actual, que en la primera iteración será el origen. Estos estados corresponderán al siguiente esquema:

			e2
		e4	act	 e3
			e1

	*/
	estado e1, e2, e3, e4;
	estado actual = origen;

	//En el inicio, actual tiene que estar en abiertos:
	abiertos.push_back(actual);
	abiertos_index[actual.fila][actual.columna] = true;

	//Para calcular el coste de las casillas, asignamos el coste de la primera posición a 0:
	actual.coste_g = 0;
	actual.coste_f = h(actual);


	//Proceso de búsqueda:
	while (!(cerrados_index[destino.fila][destino.columna]) && !abiertos.empty()) {

		//Ordenamos la lista de abiertos. Como hemos sobrecargado el operator<, se ordenan por el coste f:
		abiertos.sort();

		//Asignamos el menor, es decir, el primero, como actual:
		actual = abiertos.front();

		//Pasamos actual de abiertos a cerrados. Como abiertos está ordenado según f(), actual estará al frente.
		abiertos.pop_front();
		abiertos_index[actual.fila][actual.columna] = false;

		cerrados.push_back(actual);
		cerrados_index[actual.fila][actual.columna] = true;

		//Asignamos los estados "vecinos" de actual:
		e1 = actual; 
		e2 = actual; 
		e3 = actual; 
		e4 = actual;

		++e1.fila;
		--e2.fila;
		++e3.columna;
		--e4.columna;


		//Analizamos dichos estados vecinos uno a uno:
		if(!cerrados_index[e1.fila][e1.columna]) {
			if(!abiertos_index[e1.fila][e1.columna]){
				if(mapaResultado[e1.fila][e1.columna] != 'S' && mapaResultado[e1.fila][e1.columna] != 'T' && mapaResultado[e1.fila][e1.columna] != 'K' 
					&& mapaResultado[e1.fila][e1.columna] != '?') {
					
					cerrados_index[e1.fila][e1.columna] = true;
					cerrados.push_back(e1);

				}

				else {
					abiertos_index[e1.fila][e1.columna] = true;
					abiertos.push_back(e1);

					//Calculamos coste f del estado:
					e1.coste_f = g(e1) + h(e1);
					e1.padre = &actual;

				}
			}

			else {
				auto it = std::find(abiertos.begin(), abiertos.end(), e1);
				it->padre = &actual;
				it->coste_f = g(e1) + h(e1);
			}
		}
				



		if(!cerrados_index[e2.fila][e2.columna]) {
			if(!abiertos_index[e2.fila][e2.columna]){
				if(mapaResultado[e2.fila][e2.columna] != 'S' && mapaResultado[e2.fila][e2.columna] != 'T' && mapaResultado[e2.fila][e2.columna] != 'K' 
					&& mapaResultado[e2.fila][e2.columna] != '?') {
					
					cerrados_index[e2.fila][e2.columna] = true;
					cerrados.push_back(e2);

				}

				else {
					abiertos_index[e2.fila][e2.columna] = true;
					abiertos.push_back(e2);

					//Calculamos coste f del estado:
					e2.coste_f = g(e2) + h(e2);
					e2.padre = &actual;

				}
			}
			
			else {
				auto it = std::find(abiertos.begin(), abiertos.end(), e2);
				it->padre = &actual;
				it->coste_f = g(e2) + h(e2);
			}
		}

		if(!cerrados_index[e3.fila][e3.columna]) {
			if(!abiertos_index[e3.fila][e3.columna]){
				if(mapaResultado[e3.fila][e3.columna] != 'S' && mapaResultado[e3.fila][e3.columna] != 'T' && mapaResultado[e3.fila][e3.columna] != 'K' 
					&& mapaResultado[e3.fila][e3.columna] != '?') {
					
					cerrados_index[e3.fila][e3.columna] = true;
					cerrados.push_back(e3);

				}

				else {
					abiertos_index[e3.fila][e3.columna] = true;
					abiertos.push_back(e3);

					//Calculamos coste f del estado:
					e3.coste_f = g(e3) + h(e3);
					e3.padre = &actual;

				}
			}

			else {
				auto it = std::find(abiertos.begin(), abiertos.end(), e3);
				it->padre = &actual;
				it->coste_f = g(e3) + h(e3);
			}
		}

		if(!cerrados_index[e4.fila][e4.columna]) {
			if(!abiertos_index[e4.fila][e4.columna]){
				if(mapaResultado[e4.fila][e4.columna] != 'S' && mapaResultado[e4.fila][e4.columna] != 'T' && mapaResultado[e4.fila][e4.columna] != 'K' 
					&& mapaResultado[e4.fila][e4.columna] != '?') {
					
					cerrados_index[e4.fila][e4.columna] = true;
					cerrados.push_back(e4);

				}

				else {
					abiertos_index[e4.fila][e4.columna] = true;
					abiertos.push_back(e4);

					//Calculamos coste f del estado:
					e4.coste_f = g(e4) + h(e4);
					e4.padre = &actual;
				}
			}

			else {
				auto it = std::find(abiertos.begin(), abiertos.end(), e4);
				it->padre = &actual;
				it->coste_f = g(e4) + h(e4);
			}
		}
	}

	//Construimos la ruta:
	while (actual != origen) {
		actual = *actual.padre;
		ruta.push_back(actual);
	}

	/*
	for (auto it = ruta.begin(); it != ruta.end(); ++it) {
		cout << "Se ha creado la casilla " << it->fila << ", " << it->columna << endl;
	}
	*/

	//Transformar lista en acciones y guardar en plan
	list<estado>::const_iterator anterior, siguiente;

	for (auto anterior = siguiente = ruta.cbegin(); *siguiente != destino; ++anterior) {
		++siguiente;

		if(anterior->fila < siguiente->fila) {
			switch(anterior->orientacion) {
				case 0:
					plan.push_back(actTURN_L);
					plan.push_back(actTURN_L);
				break;

				case 1:
					plan.push_back(actTURN_R);
				break;

				case 2:
				break;

				case 3:
					plan.push_back(actTURN_L);
				break;
			}
		}

		else if(anterior->fila > siguiente->fila) {
			switch(anterior->orientacion) {
				case 0:
				break;

				case 1:
					plan.push_back(actTURN_L);
				break;

				case 2:
					plan.push_back(actTURN_L);
					plan.push_back(actTURN_L);
				break;

				case 3:
					plan.push_back(actTURN_R);
				break;
			}
		}

		else if(anterior->columna < siguiente->columna) {
			switch(anterior->orientacion) {
				case 0:
					plan.push_back(actTURN_R);
				break;

				case 1:
				break;

				case 2:
					plan.push_back(actTURN_L);
				break;

				case 3:
					plan.push_back(actTURN_L);
					plan.push_back(actTURN_L);
				break;
			}
		}

		else {
			switch(anterior->orientacion) {
				case 0:
					plan.push_back(actTURN_L);
				break;

				case 1:
					plan.push_back(actTURN_L);
					plan.push_back(actTURN_L);
				break;

				case 2:
					plan.push_back(actTURN_R);
				break;

				case 3:
				break;
			}
		}
		
		plan.push_back(actFORWARD);
	}

	return !ruta.empty();
}

Action ComportamientoJugador::think(Sensores sensores) {
  if (sensores.mensajeF != -1){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
	}

	// Actualizar el efecto de la ultima accion
	switch (ultimaAccion){
		case actTURN_R: brujula = (brujula+1)%4; break;
		case actTURN_L: brujula = (brujula+3)%4; break;
		case actFORWARD:
			switch (brujula){
				case 0: fil--; break;
				case 1: col++; break;
				case 2: fil++; break;
				case 3: col--; break;
			}
			cout << "fil: " << fil << "  col: " << col << " Or: " << brujula << endl;
	}
	cout << "Destino fil: " << sensores.destinoF << "  col: " << sensores.destinoC << endl;

	if (sensores.reset){
		hayPlan = false;
	}

	// Determinar si ha cambiado el destino desde la ultima planificacion
	if (hayPlan and (sensores.destinoF != ultPosF or sensores.destinoC != ultPosC)){
		cout << "El destino ha cambiado\n";
		hayPlan = false;
	}

	// Determinar si tengo que construir un plan
	if (!hayPlan){
		estado origen, destino;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;

    	hayPlan = pathFinding(origen,destino,plan);

		ultPosF = sensores.destinoF;
		ultPosC = sensores.destinoC;
	}


	// Ejecutar el plan
	Action sigAccion;
	if (hayPlan and plan.size()>0){
		sigAccion = plan.front();
		plan.erase(plan.begin());
	}
	else {
		sigAccion = actIDLE;
	}

	ultimaAccion = sigAccion;
	return sigAccion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

int ComportamientoJugador::h(estado e) {
	int h = (int) abs(destino.fila - e.fila) + (int) abs(destino.columna - e.columna);

	return h;
}

int ComportamientoJugador::g(estado e) {
	int g = e.padre->coste_g + 1;
	e.coste_g = g;

	return g; 
}

  
