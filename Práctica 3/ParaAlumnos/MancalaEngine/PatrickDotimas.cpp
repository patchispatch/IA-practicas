/*
 * PatrickDotimas.cpp
 *
 *  Created on: 31 may. 2018
 *      Author: patchispatch
 */

#include "PatrickDotimas.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;

PatrickDotimas::PatrickDotimas() {
	// Inicializar las variables necesarias para ejecutar la partida
	primeraVez = true;
	
}

PatrickDotimas::~PatrickDotimas() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void PatrickDotimas::initialize() {
	// Inicializar el bot antes de jugar una partida
	
}

string PatrickDotimas::getName() {
	return "PatrickDotimas"; 
}

Move PatrickDotimas::nextMove(const vector<Move> &adversary, const GameState &state) {
	
	if(primeraVez) {
		patrick = state.getCurrentPlayer();

		if(patrick == J1) {
			oponente = J2;
		}

		else {
			oponente = J1;
		}

		primeraVez = false;
	}

	//Guardaremos aquí la casilla que elegiremos como movimiento.
	Move movimiento= M_NONE;

	//Primero fijamos la profundidad a la que va a explorar Patrick por cada opción.
	int profundidad = 10;

	//Acto seguido, marcamos el valor inicial de alfa y beta:
	//Alfa será un valor muy bajo (idealmente, -infinito).
	int alfa = numeric_limits<int>::min();
	//Lo mismo ocurre para beta, pero con un valor muy grande.
	int beta = numeric_limits<int>::max();

	//Utilizamos estas variables para localizar el movimiento de valor más alto:
	int valor;

	//Para cada casilla, calculamos su valor:
	for(int i = 1; i < 7; i++) {	
		
		//Si no tiene semillas, no miramos.
		if(state.getSeedsAt(patrick, (Position) i) > 0) {
			
			GameState child = state.simulateMove ((Move) i);
			valor = alfaBeta(child, alfa, beta, profundidad);
			
			if (valor > alfa) {
				alfa = valor;
				movimiento = ((Move) i);
			}
		}
	}
	return movimiento;
}

int PatrickDotimas::h(GameState state) {

	//Contamos las semillas de nuestro granero menos las del del contrario:
	int h = state.getScore(patrick) - state.getScore(oponente);

	return h;
}

int PatrickDotimas::alfaBeta(GameState state, int alfa, int beta, int profundidad) {

	/* Utilizaremos el algoritmo minimax con poda alfa-beta, junto con nuestra heurística,
	para valorar cada movimiento.

	Este algoritmo realizará una búsqueda recursiva en forma de árbol, el cual estará
	compuesto por nodos MAX (nosotros) y nodos MIN (el adversario), simulando estados 
	hasta llegar a las hojas del árbol. El valor de un nodo vendrá determinado por 
	los valores de sus hijos de la siguiente forma:

	- Nodo MAX: tendrá el valor del máximo de sus hijos.
	- Nodo MIN: tendrá el valor del mínimo de sus hijos.

	Esto es así ya que debemos suponer que el oponente (MIN) siempre va a intentar minimizar
	nuestro beneficio, y nosotros lo maximizaremos.

	Sin embargo, esto generará un árbol inmenso (en nuestro caso, para cada estado generará seis hijos
	de forma recursiva), por lo que recurriremos a la técnica de poda alfa-beta para evitar recorrer 
	gran parte del árbol.

	- alfa representa el máximo valor de lo recorrido para MAX. Se reemplazará con el valor más alto 
	  que encontremos.

	-beta es el valor de la mejor opción encontrada para MIN. Se reemplazará con el valor más bajo.

	En cada casilla, evaluaremos tantos turnos como profundidad indiquemos en la llamada al método. */


	//Si encontramos un nodo terminal o es el primer nodo, devolvemos el valor heurístico.
	if (state.isFinalState() || profundidad == 0) {
		return h(state);
	}

	//Si nos toca:
	if(state.getCurrentPlayer() == patrick) {

		//Para cada hijo del estado actual, es decir, para cada casilla:
		for(int i = 1; i <= 6; i++)  {

			if(state.getSeedsAt(patrick, (Position) i) != 0) {
				GameState child = state.simulateMove((Move) i);
				alfa = max(alfa, alfaBeta(child, alfa, beta, profundidad - 1));

				if(beta <= alfa) {
					//romper
					break;
				}
			}
		}

		//Como es nuestro turno, devolvemos alfa (nuestra mejor opción).
		return alfa;
	}

	//Si le toca al oponente:
	else {
		
		//Para cada hijo del estado actual, es decir, para cada casilla:
		for(int i = 1; i <= 6; i++)  {

			if(state.getSeedsAt(oponente, (Position) i) != 0) {
				GameState child = state.simulateMove((Move) i);
				beta = min(beta, alfaBeta(child, alfa, beta, profundidad - 1));
			

				if(beta <= alfa) {
					//romper
					break;
				}
			}
		}

		//Como es el turno del oponente, devolvemos beta, su mejor opción.
		return beta;
	}
}

