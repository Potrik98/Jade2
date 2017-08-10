#pragma once

#include "board.h"

#define INF 1000000

typedef struct {
    long nodes;
    int fh;
    int fhf;
    int maxDepth;

    bool stop;
} SearchInfo;

void search(Board* board, SearchInfo* info);