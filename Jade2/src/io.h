#pragma once

#include "board.h"

void printBoard(const Board board);
void startInputLoop();
void printPvLine(Board* board, const int depth);