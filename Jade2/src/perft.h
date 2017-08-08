#pragma once

#include "jade.h"
#include "board.h"

U64 perft(const int depth, Board* board);
void runPerftTests(const std::string file);