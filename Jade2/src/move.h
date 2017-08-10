#pragma once

#include "board.h"

#define MAX_MOVES_IN_POSITION 256

typedef struct {
    int move;
    int score;
} Move;

typedef struct {
    Move moves[MAX_MOVES_IN_POSITION];
    int count;
} MoveList;

#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m) >> 7) & 0x7F)
#define CAP(m) (((m) >> 14) & 0xF)
#define PROM(m) (((m) >> 20) & 0xF)

#define MFLAG_EP 0x40000
#define MFLAG_PS 0x80000
#define MFLAG_CA 0x1000000

#define MOVE(fr, to, cap, prom, flags) ((fr) | ((to) << 7) | ((cap) << 14 ) | ((prom) << 20 ) | (flags))

void generateAllMoves(const Board* board, MoveList* list);
bool isSquareAttacked(const int sq, const int side, const Board* board);

bool makeMove(Board* board, const int move);
void takeMove(Board* board);

bool moveExists(Board* board, const int move);
