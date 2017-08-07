#pragma once

#include "board.h"

#define FR2SQ(f,r) ( (21 + (f) ) + ( (r) * 10 ) ) 
#define SQ64(sq120) (sq120to64[(sq120)])
#define SQ120(sq64) (sq64to120[(sq64)])
#define OFFBOARD(sq) (getRank[(sq)] == OFFBOARD)

extern int sq120to64[120];
extern int sq64to120[64];
extern int getRank[120];
extern int getFile[120];