#pragma once

#include "jade.h"
#include "board.h"

namespace hashkey {
    extern U64 keyPiece[13][120];
    extern U64 keySide;
    extern U64 keyCastle[16];

    void init();
}

U64 generatePosKey(const Board* board);
U64 generatePolyglotKey(const Board* board);

#define HASH_PCE(pce,sq) (pos->posKey ^= (hashkey::keyPiece[(pce)][(sq)]))
#define HASH_CA (pos->posKey ^= (hashkey::keyCastle[(pos->castlePerm)]))
#define HASH_SIDE (pos->posKey ^= (hashkey::keySide))
#define HASH_EP (pos->posKey ^= (hashkey::keyPiece[EMPTY][(pos->enPas)]))
