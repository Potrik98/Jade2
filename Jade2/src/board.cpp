#include "board.h"
#include "boardutils.h"

Board::Board() {
	clear();
}

void Board::clear() {
	for (int i = 0; i < 120; i++) {
		pieces[i] = OFFBOARD;
	}

	for (int i = 0; i < 64; i++) {
		pieces[SQ120(i)] = EMPTY;
	}

	for (int i = 0; i < 2; i++) {
		pieceBig[i] = 0;
		pieceMaj[i] = 0;
		pieceMin[i] = 0;
		material[i] = 0;
	}

	for (int i = 0; i < 3; i++) {
		pawns[i] = 0ULL;
	}

	for (int i = 0; i < 13; i++) {
		pieceNum[i] = 0;
	}

	sideToMove = BOTH;
	enPasSquare = NO_SQ;
	fiftyMove = 0;

	ply = 0;
	hisPly = 0;

	castlePerm = 0;

	posKey = 0ULL;
}

void Board::reset() {

}