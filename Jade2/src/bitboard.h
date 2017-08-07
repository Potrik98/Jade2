#pragma once

#include "jade.h"

namespace bitboard {
    extern U64 clrMask[64];
    extern U64 setMask[64];
}

#define CLRBIT(bb,sq) ((bb) &= bitboard::clrMask[(sq)])
#define SETBIT(bb,sq) ((bb) |= bitboard::setMask[(sq)])