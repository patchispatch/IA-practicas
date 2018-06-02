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
#include <cmath>
using namespace std;

PatrickDotimas::PatrickDotimas() {
	// Inicializar las variables necesarias para ejecutar la partida

	
}

PatrickDotimas::~PatrickDotimas() {
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void PatrickDotimas::initialize() {
	// Inicializar el bot antes de jugar una partida
	patrick = getPlayer();
}

string PatrickDotimas::getName() {
	return "PatrickDotimas"; // Sustituir por el nombre del bot
}

Move PatrickDotimas::nextMove(const vector<Move> &adversary, const GameState &state) {

	Move movimiento= M_NONE;

	//Hacer cosas aquí.



	return movimiento;
}

int PatrickDotimas::h(GameState state, Position pos, Player p) {
	//Valor de la heurística a devolver:
	int h;

	/* Valor numérico que indica la última posición en el tablero.
	Si es positivo: casilla nuestra.
	Si es negativo: casilla del oponente.
	Ej: 1 es nuestra casilla 1, -1 es la del oponente. */
	int ultima_pos;

	/* Para que una ficha "salte" del lado contrario al nuestro dando la vuelta,
    necesitamos tener un número de semillas mayor al que tiene que recorrer contando el granero,
    es decir, 6 + pos. Como pueden dar más de una vuelta, calculamos modulo(semillas, pos+6), y así
    vemos cuántas se introducen en el granero, además de ver cuántas semillas damos al rival en caso de dar
    vueltas. */
    int vueltas = state.getSeedsAt(p, pos) % (pos + 6);

	//Ahora, calculamos el valor de h, teniendo en cuenta varias posibilidades:

	//Si hay semillas, calculamos
	int seeds = state.getSeedsAt(p, pos);

	if (seeds > 0) {
		h = valorJugada(state, pos, p, vueltas);

		//Calculamos última posición:

		//Si hay menos semillas que el número de la casilla, no llegarán al granero, por lo que 
		//el cálculo es simple:
		if (pos > seeds) {
			ultima_pos = pos - state.getSeedsAt(p, pos);
		}

		//Si el número de semillas es igual al de la casilla, la última posición es el granero:
		else if (pos == posRelativa(seeds, vueltas)) {
			ultima_pos = 0;
		}

		//Si posRelativa es mayor a la posición y menor que pos + 6, la última posición está en el otro lado:
		else if (posRelativa(seeds, vueltas) > pos && posRelativa(seeds, vueltas) < (pos + 6)) {
			ultima_pos = enfrente( (Position) (pos - abs(posRelativa(seeds, vueltas))));
		}

		//En otro caso, se queda en nuestro lado:
		else {
			ultima_pos = pos - posRelativa(seeds, vueltas);
		}

		//Comprobamos si la última posición permite el robo:
		if (ultima_pos > 0 && state.getSeedsAt(p, (Position) ultima_pos) && 
		state.getSeedsAt(p, enfrente((Position) ultima_pos)) > 0) {
			h += (int) enfrente((Position) state.getSeedsAt(p, (Position) ultima_pos)) + 1;
		}
	}

	//Si no hay semillas, le asignamos a la casilla un valor que nunca sea viable:
	else {
		h = -1500;
	}

	return h;
}

int PatrickDotimas::alfaBeta(GameState state, Position pos, int alfa, int beta, int profundidad) {

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
		return (h(state, pos, state.getCurrentPlayer()));
	}

	//Si nos toca:
	if(state.getCurrentPlayer() == patrick) {

		//Para cada hijo del estado actual, es decir, para cada casilla:
		for(int i = 1; i <= 6; ++i)  {
			GameState child = state.simulateMove((Move) i);
			alfa = max(alfa, alfaBeta(child, (Position) i, alfa, beta, profundidad - 1));

			if(beta > alfa) {
				//romper
				break;
			}

			return alfa;
		}
	}

	//Si le toca al oponente:
	else {
		
		//Para cada hijo del estado actual, es decir, para cada casilla:
		for(int i = 1; i <= 6; ++i)  {
			GameState child = state.simulateMove((Move) i);
			beta= min(beta, alfaBeta(child, (Position) i, alfa, beta, profundidad - 1));

			if(beta <= alfa) {
				//romper
				break;
			}

			return beta;
		}
	}
}

int PatrickDotimas::valorJugada(GameState state, Position pos, Player p, int vueltas) {
	
	//Valor jugada: semillas metidas en granero menos cedidas al rival.
	if(state.getSeedsAt(p, pos) >= (unsigned char) pos) {
		if (vueltas > 1) {
			return pos - 6 * vueltas + vueltas;
		}

		else {
			return pos - state.getSeedsAt(p, pos) + 1;
		}
	}

	else
		return 0;
}

Position PatrickDotimas::enfrente(Position pos) {
	Position p = (Position) (7 - pos);

	return p;
}

Position PatrickDotimas::posRelativa(int seeds, int vueltas) {
	return (Position) (seeds - vueltas * CAS_VUELTA_COMPLETA);
}