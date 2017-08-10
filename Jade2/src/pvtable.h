#pragma once

#include "board.h"

#define HASH_ENTRIES 104729

int getPvMove(const Board* board);
int getPvLine(Board* board, const int depth);
void storePvMove(Board* board, const int move);