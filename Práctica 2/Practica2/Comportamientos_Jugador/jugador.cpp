#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"
#include <iostream>
#include <cmath>

//Operadores
bool operator== (const estado &uno, const estado &otro) {

	return (uno.fila == otro.fila && uno.columna == otro.columna);
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

bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan) {
	
	//Borramos el plan anterior:
	plan.clear();

	//Lanzamos la búsqueda:
	list<estado> estados = BusquedaAnchura(origen, destino);

	for(auto it=estados.begin(); it != estados.end(); ++it)
		cout << "casilla[" << (*it).fila << "][ " << (*it).columna << "]" << endl;;


	plan = trazadoPlan(estados);

	PintaPlan(plan);
	VisualizaPlan(origen, plan);

	return !plan.empty();
}

list<estado> ComportamientoJugador::BusquedaAnchura(const estado &origen, const estado &destino) {

	bool abiertos_index[mapaResultado.size()][mapaResultado.size()];
	bool cerrados_index[mapaResultado.size()][mapaResultado.size()];
	queue<estado> abiertos;
	list<estado> ruta;

	bool encontrado = false;

	for(int i=0; i<mapaResultado.size(); ++i) {
		for(int j=0; j<mapaResultado.size(); ++j) {
			cerrados_index[i][j] = false;
			abiertos_index[i][j] = false;
		}
	}

	abiertos.push(origen);
	abiertos_index[origen.fila][origen.columna] = true;

	while(!abiertos.empty() && !encontrado) {
		
		estado actual = abiertos.front();
		abiertos.pop();

		if(actual.fila == destino.fila && actual.columna == destino.columna) {
			ruta = actual.recorrido;
			ruta.push_back(actual);
			encontrado = true;
		}

		else {
			cerrados_index[actual.fila][actual.columna] = true;
			
			for(int i=0; i<4; i++) {

				int fil_aux = actual.fila;
				int	col_aux = actual.columna; 
				int	or_aux = i;

				//Definimos la orientación del estado adyacente:
				switch(i) {

					//NORTE:
					case 0: 
						--fil_aux;
					break;

					//ESTE:
					case 1: 
						++col_aux;
					break;

					//SUR:
					case 2:
						++fil_aux;
					break;

					//OESTE:
					case 3:
						--col_aux;
					break;
				}

				//Creamos los estados adyacentes según la iteración:
				estado adyacente;
				adyacente.fila = fil_aux;
				adyacente.columna = col_aux;
				adyacente.orientacion = or_aux;

				//Si el estado no se encuentra en abiertos o en cerrados, lo metemos en abiertos:
				if(!abiertos_index[fil_aux][col_aux] && !cerrados_index[fil_aux][col_aux]
				   && suelo(fil_aux, col_aux)) {

					for (auto it = actual.recorrido.begin(); it != actual.recorrido.end(); ++it) {
						it->recorrido.clear();
						adyacente.recorrido.push_back(*it);
					}

					adyacente.recorrido.push_back(actual);
					abiertos.push(adyacente);
					abiertos_index[adyacente.fila][adyacente.columna] = true;
				}
			}
		}

		actual.recorrido.clear();
	}

	return ruta;
}

list<Action> ComportamientoJugador::trazadoPlan(const list<estado> &lista) {
	
	list<Action> resultado;
	list<estado>::const_iterator anterior = lista.begin();
	list<estado>::const_iterator siguiente = lista.begin();

	while(*siguiente != lista.back()) {
		
		++siguiente;

		//Si mira hacia el norte:
		if(anterior->fila > siguiente->fila) {
			
			switch(anterior->orientacion) {
				
				//ESTE:
				case 1:
					resultado.push_back(actTURN_L);
				break;

				//SUR:
				case 2:
					resultado.push_back(actTURN_L);
					resultado.push_back(actTURN_L);
				break;

				//OESTE:
				case 3:
					resultado.push_back(actTURN_R);
				break;
			}

			resultado.push_back(actFORWARD);
		}

		//Si mira hacia el sur:
		else if(anterior->fila < siguiente->fila) {
			
			switch(anterior->orientacion) {

				//NORTE:
				case 0:
					resultado.push_back(actTURN_L);
					resultado.push_back(actTURN_L);
				break;

				//ESTE:
				case 1:
					resultado.push_back(actTURN_R);
				break;
				
				//OESTE:
				case 3:
					resultado.push_back(actTURN_L);
				break;
			}

			resultado.push_back(actFORWARD);
		}

		//Si mira hacia el oeste:
		else if(anterior->columna > siguiente->columna) {
			
			switch(anterior->orientacion) {

				//NORTE:
				case 0: 
					resultado.push_back(actTURN_L);
				break;

				//ESTE
				case 1:
					resultado.push_back(actTURN_L);
					resultado.push_back(actTURN_L);
				break;

				//SUR:
				case 2:
					resultado.push_back(actTURN_R);
				break;
			}

			resultado.push_back(actFORWARD);
		}

		else if(anterior->columna < siguiente->columna) {
			
			switch(anterior->orientacion) {

				//NORTE:
				case 0:
					resultado.push_back(actTURN_R);
				break;

				//SUR:
				case 2:
					resultado.push_back(actTURN_L);
				break;

				//OESTE:
				case 3:
					resultado.push_back(actTURN_L);
					resultado.push_back(actTURN_L);
				break;
			}

			resultado.push_back(actFORWARD);
		}

		++anterior;
	}

	return resultado;
}

bool ComportamientoJugador::suelo(int fila, int columna) {
	
	return (mapaResultado[fila][columna] == 'S' || 
			mapaResultado[fila][columna] == 'T' || 
			mapaResultado[fila][columna] == 'K' ||
			mapaResultado[fila][columna] == '?' ||
			mapaResultado[fila][columna] == 'a');
}

Action ComportamientoJugador::think(Sensores sensores) {
	
	if (sensores.mensajeF != -1 && primera_vez) {
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
		primera_vez = false;
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



	// Determinar si ha cambiado el destino desde la ultima planificacion
	if (hayPlan and (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna)){
		cout << "El destino ha cambiado\n";
		hayPlan = false;
	}

	// Determinar si tengo que construir un plan
	if (!hayPlan) {
		estado origen;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		destino.fila = sensores.destinoF;
		destino.columna = sensores.destinoC;

    	hayPlan = pathFinding(origen,destino,plan);
	}


	// Ejecutar el plan
	Action sigAccion;
	if (hayPlan and plan.size()>0) {
		sigAccion = plan.front();
		plan.erase(plan.begin());

		//Si tenemos un aldeano delante, recalculamos, sustituyendo en el mapa su posición por un muro:
		if(ultimaAccion == actFORWARD && sensores.superficie[2] == 'a') {
			int fil_aux = fil;
			int col_aux = col;

			estado origen;
			origen.fila = fil;
			origen.columna = col;
			origen.orientacion = brujula;

			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;

			//Calculamos nuestra orientación para saber la casilla en la que se encuentra el aldeano:
			switch(brujula)
			{
				//NORTE:
				case 0:
					fil_aux--;
				break;
				
				//ESTE:
				case 1:
					col_aux++;
				break;
				
				//SUR:
				case 2:
					fil_aux++;
				break;
				
				//OESTE:
				case 3:
					col_aux--;
				break;
			}

			//Guardamos el terreno original que pisa el aldeano, y e ponemos un disfraz de precipicio:
			char original = mapaResultado[fil_aux][col_aux];
			mapaResultado[fil_aux][col_aux] = 'P';

			hayPlan = pathFinding(origen,destino,plan);

			//Recuperamos el terreno original:
			mapaResultado[fil_aux][col_aux] = original;

			PintaPlan(plan);

			if(!plan.empty()) {
				sigAccion = plan.front();
				plan.erase(plan.begin());
			}

			else
				sigAccion = actIDLE;
		}
	}

	else
		sigAccion = actIDLE;

	ultimaAccion = sigAccion;
	return sigAccion;
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

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
