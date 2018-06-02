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
	//Guardaremos qué jugador es Patrick:
	Player patrick;

	PatrickDotimas();
	~PatrickDotimas();

	int h(GameState state, Position pos, Player p);

	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);
	int valorJugada(GameState state, Position pos, Player p, int vueltas);
	Position enfrente(Position pos);
	Position posRelativa(int seeds, int vueltas);
	int alfaBeta(GameState state, Position pos, int alfa, int beta, int profundidad);

};

#endif /* PATCHISPATCH_H_ */

