//#include "../Comportamientos_Jugador/jugador.hpp"
#include "jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <list>
#include <set>

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
	set<estado> abiertos;
	set<estado> cerrados;
	set<estado> ruta;

	estado e1, e2, e3, e4;
	origen.coste_g = 0;
	estado actual = origen;

	while (!(cerrados.count(destino))) {
		cerrados.push_back(actual);
		abiertos.erase(actual); 

		e1 = actual; 
		e2 = actual; 
		e3 = actual; 
		e4 = actual;

		++e1.fila;
		--e2.fila;
		++e3.columna;
		--e4.columna;

		e1.padre = actual;
		e2,padre = actual;
		e3.padre = actual;
		e4.padre = actual;


		if(mapaResultado[e1.fila][e1.columna] != 'S' && mapaResultado[e1.fila][e1.columna] != 'T') {
			cerrados.push_back(e1);
		}
		else {
			abiertos.push_back(e1);
			e1.coste_f = g(e1) + h(e1);
		}

		if(mapaResultado[e2.fila][e2.columna] != 'S' && mapaResultado[e2.fila][e2.columna] != 'T') {
			cerrados.push_back(e2);
		}
		else {
			abiertos.push_back(e2);
			e1.coste_f = g(e2) + h(e2);
		}

		if(mapaResultado[e3.fila][e3.columna] != 'S' && mapaResultado[e3.fila][e3.columna] != 'T') {
			cerrados.push_back(e3);
		}
		else {
			abiertos.push_back(e3);
			e1.coste_f = g(e3) + h(e3);
		}

		if(mapaResultado[e4.fila][e2.columna] != 'S' && mapaResultado[e2.fila][e2.columna] != 'T') {
			cerrados.push_back(e4);
		}
		else {
			abiertos.push_back(e4);
			e1.coste_f = g(e4) + h(e4);
		}

		abiertos.sort;
		actual = abiertos.front;
		ruta.push_back(actual);
		
	}

	//Transformar lista en acciones y guardar en plan
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
  
