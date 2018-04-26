//#include "../Comportamientos_Jugador/jugador.hpp"
#include "jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <list>


bool ordenar (const estado &uno, const estado &otro) {

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
	list<estado> abiertos;
	vector<vector<bool>> cerrados;

	for (int i = 0; i < mapaResultado.size(); ++i) {
		for (int j = 0; i < mapaResultado.size(); ++j) {
			mapaResultado[i][j] = false;
		}
	}

	list<estado> ruta;

	estado e1, e2, e3, e4;
	estado actual = origen;

	actual.coste_g = 0;

	while (!(cerrados[destino.fila][destino.columna])) {
		cerrados[actual.fila][actual.columna] = true;
		abiertos.erase(actual); 

		e1 = actual; 
		e2 = actual; 
		e3 = actual; 
		e4 = actual;

		++e1.fila;
		--e2.fila;
		++e3.columna;
		--e4.columna;

		e1.padre = &actual;
		e2.padre = &actual;
		e3.padre = &actual;
		e4.padre = &actual;


		if(mapaResultado[e1.fila][e1.columna] != 'S' && mapaResultado[e1.fila][e1.columna] != 'T' && mapaResultado[e1.fila][e1.columna] != 'K') {
			cerrados[e1.fila][e1.columna] = true;
		}
		else {
			abiertos.push_back(e1);
			e1.coste_f = g(e1) + h(e1);
		}

		if(mapaResultado[e2.fila][e2.columna] != 'S' && mapaResultado[e2.fila][e2.columna] != 'T' && mapaResultado[e2.fila][e2.columna] != 'K') {
			cerrados[e2.fila][e2.columna] = true;
		}
		else {
			abiertos.push_back(e2);
			e1.coste_f = g(e2) + h(e2);
		}

		if(mapaResultado[e3.fila][e3.columna] != 'S' && mapaResultado[e3.fila][e3.columna] != 'T' && mapaResultado[e3.fila][e3.columna] != 'K') {
			cerrados[e3.fila][e3.columna] = true;
		}
		else {
			abiertos.push_back(e3);
			e1.coste_f = g(e3) + h(e3);
		}

		if(mapaResultado[e4.fila][e4.columna] != 'S' && mapaResultado[e4.fila][e4.columna] != 'T' && mapaResultado[e4.fila][e4.columna] != 'K') {
			cerrados[e4.fila][e4.columna] = true;
		}
		else {
			abiertos.push_back(e4);
			e1.coste_f = g(e4) + h(e4);
		}

		abiertos.sort(ordenar);
		actual = abiertos.front();
		ruta.push_back(actual);
		
	}

	for (auto it = ruta.begin(); it != ruta.end(); ++it) {
		cout << "Se ha creado la casilla " << it->fila << ", " << it->columna << endl;
	}
	
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
  return actIDLE;
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
  
