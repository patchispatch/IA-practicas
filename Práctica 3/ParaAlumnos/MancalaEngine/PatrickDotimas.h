/*
 * PatrickDotimas.h
 *
 *  Created on: 31 may. 2018
 *      Author: patchispatch
 */

#include "Bot.h"

#ifndef PATCHISPATCH_H_
#define PATCHISPATCH_H_

class PatrickDotimas:Bot {
public:
	//Constantes:
	//Número de casillas para completar una vuelta completa al tablero:
	const int CAS_VUELTA_COMPLETA = 13;

	//Variables:
	//Guardaremos qué jugador es Patrick, y el oponente:
	bool primeraVez;
	Player patrick;
	Player oponente;

	PatrickDotimas();
	~PatrickDotimas();

	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);

	//Métodos nuevos:

	int h(GameState state);
	int alfaBeta(GameState state, int alfa, int beta, int profundidad);

};

#endif /* PATCHISPATCH_H_ */

