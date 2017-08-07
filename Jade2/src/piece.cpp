#include "piece.h"
#include "board.h"

namespace piece {
    const bool isBig[14] = { false, false, true, true, true, true, true, false, true, true, true, true, true, false };
    const bool isMaj[14] = { false, false, false, false, true, true, true, false, false, false, true, true, true, false };
    const bool isMin[14] = { false, false, true, true, false, false, false, false, true, true, false, false, false, false };

    const bool isKnight[14] = { false, false, true, false, false, false, false, false, true, false, false, false, false, false };
    const bool isKing[14] = { false, false, false, false, false, false, true, false, false, false, false, false, true, false };
    const bool isRookQueen[14] = { false, false, false, false, true, true, false, false, false, false, true, true, false, false };
    const bool isBishopQueen[14] = { false, false, false, true, false, true, false, false, false, true, false, true, false, false };
    const bool isPawn[14] = { false, true, false, false, false, false, false, true, false, false, false, false, false, false };

    const bool isSliding[13] = { false, false, false, true, true, true, false, false, false, true, true, true, false };

    const int getVal[13] = { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000 };
    const int getCol[14] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BOTH };

    namespace direction {
        const int dirKn[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };
        const int dirRk[4] = { -1, -10, 1, 10 };
        const int dirBi[4] = { -9, -11, 11, 9 };
        const int dirKi[8] = { -1, -10, 1, 10, -9, -11, 11, 9 };

        const int dirLength[13] = { 0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8 };
        const int getDir[13][8] = {
            {},
            {},
            { -8, -19, -21, -12, 8, 19, 21, 12 },
            { -9, -11, 11, 9 },
            { -1, -10, 1, 10 },
            { -1, -10, 1, 10, -9, -11, 11, 9 },
            { -1, -10, 1, 10, -9, -11, 11, 9 },
            {},
            { -8, -19, -21, -12, 8, 19, 21, 12 },
            { -9, -11, 11, 9 },
            { -1, -10, 1, 10 },
            { -1, -10, 1, 10, -9, -11, 11, 9 },
            { -1, -10, 1, 10, -9, -11, 11, 9 },
        };
    }
}